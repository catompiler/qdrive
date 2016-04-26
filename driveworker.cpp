#include "driveworker.h"
#include <QTimer>
#include <QVector>
#include "settings.h"
#include <modbus/modbus-rtu.h>
#include <sys/time.h>
#include <errno.h>
#include <QDebug>


//! Число попыток чтения данных.
#define DRIVE_RETRIES_COUNT 10

#define DRIVE_ID_NAME_MAX 20

#pragma pack(push, 1)
typedef struct _DriveModbusId {
    uint16_t magic;
    uint8_t major;
    uint8_t minor;
    char name[DRIVE_ID_NAME_MAX];
} drive_modbus_id_t;
#pragma pack(pop)

#define DRIVE_ID_MAGIC 0x4702

//! Начало адресов регистров хранения.
#define DRIVE_MODBUS_HOLD_REGS_START 2000
//! Начало адресов регистров ввода.
#define DRIVE_MODBUS_INPUT_REGS_START 2000
//! Начало адресов цифровых входов.
#define DRIVE_MODBUS_DINS_START 2000
//! Начало адресов регистров флагов.
#define DRIVE_MODBUS_COIS_START 2000

// Константы адресов.
// Регистры ввода.
// Регистры хранения.
#define DRIVE_MODBUS_HOLD_REG_REFERENCE (DRIVE_MODBUS_HOLD_REGS_START + 0)
// Цифровые входа.
// Регистры флагов.
#define DRIVE_MODBUS_COIL_RUN (DRIVE_MODBUS_COIS_START + 0)


DriveWorker::DriveWorker() : QThread()
{
    moveToThread(this);

    timer = new QTimer();
    timer->moveToThread(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &DriveWorker::update);

    modbus = nullptr;

    connected_to_device = false;

    dev_reference = 0;
    dev_running = false;
}

DriveWorker::~DriveWorker()
{
    cleanup_modbus();
    delete timer;
}

bool DriveWorker::setup()
{
    if(isRunning()){
        emit errorOccured(tr("Попытка изменения настроек во время выполнения потока."));
        return false;
    }

    cleanup_modbus();

    Settings& settings = Settings::get();

    const char parity_names[] = {'N', 'E', 'O'};

    unsigned int parity_index = static_cast<unsigned int>(settings.parity());
    if(parity_index >= sizeof(parity_names)){
        cleanup_modbus();
        emit errorOccured(tr("Неправильно задана чётность. Проверьте настройки."));
        return false;
    }

    unsigned int stop_bits = static_cast<unsigned int>(settings.stopBits()) + 1;
    if(stop_bits > 2){
        cleanup_modbus();
        emit errorOccured(tr("Неправильно заданы стоповые биты. Проверьте настройки."));
        return false;
    }

    modbus = modbus_new_rtu(settings.ioDeviceName().toLocal8Bit().data(),
                            static_cast<int>(settings.baud()),
                            parity_names[parity_index], 8, stop_bits);

    if(modbus == nullptr){
        cleanup_modbus();
        emit errorOccured(tr("Невозможно создать контекст протокола Modbus RTU."));
        return false;
    }

    modbus_set_debug(modbus, false);

    modbus_set_error_recovery(modbus, static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));

    modbus_rtu_set_serial_mode(modbus, MODBUS_RTU_RS232);

    modbus_set_slave(modbus, settings.deviceAddress());

    if(modbus_connect(modbus) == -1){
        cleanup_modbus();
        emit errorOccured(tr("Невозможно открыть порт для связи с устройством."));
        return false;
    }

    struct timeval tv_timeout;
    tv_timeout.tv_sec = settings.deviceTimeout() / 1000;
    tv_timeout.tv_usec = (settings.deviceTimeout() % 1000) * 1000;

    modbus_set_response_timeout(modbus, &tv_timeout);

    timer->setInterval(settings.devicePeriod());

    return true;
}

bool DriveWorker::good() const
{
    return modbus != nullptr;
}

bool DriveWorker::connectedToDevice() const
{
    return connected_to_device;
}

unsigned int DriveWorker::reference() const
{
    return dev_reference;
}

bool DriveWorker::running() const
{
    return dev_running;
}

void DriveWorker::connectToDevice()
{
    if(connected_to_device) return;
    if(!good()) return;

    size_t retries = 0;
    int res = 0;

    const int max_id_size = 252;

    QVector<quint8> response(max_id_size);

    for(retries = 0; retries < DRIVE_RETRIES_COUNT; retries ++){
        res = modbus_report_slave_id(modbus, &response[0]);
        if(res != -1) break;
    }

    if(res == -1){
        emit errorOccured(tr("Нет ответа от устройства.(%1)").arg(modbus_strerror(errno)));
        return;
    }

    drive_modbus_id_t* drive_id = reinterpret_cast<drive_modbus_id_t*>(&response[0]);

    if(drive_id->magic != DRIVE_ID_MAGIC){
        emit errorOccured(tr("Неправильный идентификатор устройства."));
        return;
    }

    QString dev_name = tr("%1 ver %2.%3").arg(drive_id->name).arg(drive_id->major).arg(drive_id->minor);

    emit information(tr("Соединено с устройством: %1").arg(dev_name));

    connected_to_device = true;

    emit connected();

    timer->start();
}

void DriveWorker::disconnectFromDevice()
{
    if(!connected_to_device) return;

    timer->stop();

    connected_to_device = false;

    emit disconnected();
}

void DriveWorker::setRunning(bool run)
{
    size_t retries = 0;
    int res = 0;

    for(retries = 0; retries < DRIVE_RETRIES_COUNT; retries ++){
        res = modbus_write_bit(modbus, DRIVE_MODBUS_COIL_RUN, run);
        if(res != -1) break;
    }
    if(res == -1){
        emit errorOccured(tr("Невозможно записать данные о запуске.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_running = run;
}

void DriveWorker::startDrive()
{
    setRunning(true);
}

void DriveWorker::stopDrive()
{
    setRunning(false);
}

void DriveWorker::setReference(unsigned int reference)
{
    size_t retries = 0;
    int res = 0;

    for(retries = 0; retries < DRIVE_RETRIES_COUNT; retries ++){
        res = modbus_write_register(modbus, DRIVE_MODBUS_HOLD_REG_REFERENCE, static_cast<int>(reference));
        if(res != -1) break;
    }
    if(res == -1){
        emit errorOccured(tr("Невозможно записать данные о задании.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_reference = reference;
}

void DriveWorker::update()
{
    size_t retries = 0;
    int res = 0;
    uint16_t udata = 0;
    uint8_t bits_data = 0;

    for(retries = 0; retries < DRIVE_RETRIES_COUNT; retries ++){
        res = modbus_read_registers(modbus, DRIVE_MODBUS_HOLD_REG_REFERENCE, 1, &udata);
        if(res != -1) break;
    }
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать данные о задании.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_reference = udata;

    for(retries = 0; retries < DRIVE_RETRIES_COUNT; retries ++){
        res = modbus_read_bits(modbus, DRIVE_MODBUS_COIL_RUN, 1, &bits_data);
        if(res != -1) break;
    }
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать данные о выполнении.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_running = bits_data;

    emit updated();

    timer->start();
}

void DriveWorker::run()
{
    exec();

    timer->stop();
}

void DriveWorker::cleanup_modbus()
{
    if(modbus){
        modbus_free(modbus);
        modbus = nullptr;
        connected_to_device = false;
        dev_running = false;
    }
}

float DriveWorker::unpack_fxd_10_6(int16_t value)
{
    return (float)value / 64;
}
