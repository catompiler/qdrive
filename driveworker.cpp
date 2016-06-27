#include "driveworker.h"
#include <QTimer>
#include <QMutex>
#include <QVector>
#include "settings.h"
#include "future.h"
#include "parameter.h"
#include <modbus/modbus-rtu.h>
#include <sys/time.h>
#include <errno.h>
#include <QDebug>


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
//! Задание.
#define DRIVE_MODBUS_HOLD_REG_REFERENCE (DRIVE_MODBUS_HOLD_REGS_START + 0)
// Цифровые входа.
// Регистры флагов.
//! Запуск/останов.
#define DRIVE_MODBUS_COIL_RUN (DRIVE_MODBUS_COIS_START + 0)
//! Сброс ошибок.
#define DRIVE_MODBUS_COIL_CLEAR_ERRORS (DRIVE_MODBUS_COIS_START + 1)
//! Применение настроек.
#define DRIVE_MODBUS_COIL_APPLY_PARAMS (DRIVE_MODBUS_COIS_START + 2)
//! Сохранение настроек.
#define DRIVE_MODBUS_COIL_SAVE_PARAMS (DRIVE_MODBUS_COIS_START + 3)


DriveWorker::DriveWorker() : QThread()
{
    moveToThread(this);

    timer = new QTimer();
    timer->moveToThread(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &DriveWorker::update);

    upd_mutex = new QMutex();
    upd_params = new UpdateParamsList();

    read_mutex = new QMutex();
    read_params = new ReadParamsList();

    write_mutex = new QMutex();
    write_params = new WriteParamsList();

    modbus = nullptr;

    connected_to_device = false;

    dev_reference = 0;
    dev_running = false;
}

DriveWorker::~DriveWorker()
{
    cleanup_modbus();
    delete write_params;
    delete write_mutex;

    delete read_params;
    delete read_mutex;

    delete upd_params;
    delete upd_mutex;
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

void DriveWorker::addUpdParam(Parameter *param)
{
    upd_mutex->lock();

    UpdateParamsList::iterator it = upd_params->find(param->id());

    if(it == upd_params->end()){
        upd_params->insert(param->id(), qMakePair(param, static_cast<size_t>(1)));
    }else{
        it->second ++;
    }

    upd_mutex->unlock();
}

void DriveWorker::removeUpdParam(Parameter *param)
{
    upd_mutex->lock();

    UpdateParamsList::iterator it = upd_params->find(param->id());

    if(it != upd_params->end()){
        if(-- it->second == 0){
            upd_params->erase(it);
        }
    }

    upd_mutex->unlock();
}

void DriveWorker::addReadParams(QList<Parameter *> &params, Future *future)
{
    read_mutex->lock();

    read_params->append(qMakePair(params, future));

    read_mutex->unlock();
}

void DriveWorker::addWriteParams(QList<Parameter *> &params, Future *future)
{
    write_mutex->lock();

    write_params->append(qMakePair(params, future));

    write_mutex->unlock();
}

void DriveWorker::connectToDevice()
{
    if(connected_to_device) return;
    if(!good()) return;

    int res = 0;

    const int max_id_size = 252;

    QVector<quint8> response(max_id_size);

    res = modbusTry(modbus_report_slave_id, &response[0]);
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

    cleanup_rw_params();

    emit disconnected();
}

void DriveWorker::setRunning(bool run)
{
    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_RUN, run);
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
    if(dev_running) emit information(tr("Запуск привода."));
}

void DriveWorker::stopDrive()
{
    setRunning(false);
    if(!dev_running) emit information(tr("Останов привода."));
}

void DriveWorker::setReference(unsigned int reference)
{
    int res = 0;

    res = modbusTry(modbus_write_register, DRIVE_MODBUS_HOLD_REG_REFERENCE, static_cast<int>(reference));
    if(res == -1){
        emit errorOccured(tr("Невозможно записать данные о задании.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_reference = reference;

    emit information(tr("Задание установлено."));
}

void DriveWorker::clearErrors()
{
    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_CLEAR_ERRORS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно очистить ошибки привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Ошибки очищены."));
}

void DriveWorker::saveParams()
{
    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_SAVE_PARAMS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно сохранить параметры привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Параметры сохранены."));
}

void DriveWorker::readNextParams()
{
    int res = 0;
    uint16_t udata = 0;
    int cur_progress = 0;

    for(;;){
        read_mutex->lock();

        if(read_params->isEmpty()){
            read_mutex->unlock();
            break;
        }

        QPair<QList<Parameter*>, Future*> curParams = read_params->takeFirst();

        read_mutex->unlock();

        curParams.second->setProgressRange(0, curParams.first.size());
        curParams.second->setProgress(0);

        for(QList<Parameter*>::iterator it = curParams.first.begin(); it != curParams.first.end(); ++ it){
            if(curParams.second->needCancel()){
                emit information(tr("Чтение отменено."));
                break;
            }
            res = modbusTry(modbus_read_registers, (*it)->id(), 1, &udata);
            if(res == -1){
                emit errorOccured(tr("Невозможно прочитать параметр с id %1.(%2)")
                                  .arg((*it)->id()).arg(modbus_strerror(errno)));
                curParams.second->finish();
                disconnectFromDevice();
                return;
            }
            (*it)->setRaw(udata);
            curParams.second->setProgress(++ cur_progress);
        }
        curParams.second->finish();
    }
}

void DriveWorker::writeNextParams()
{
    int res = 0;
    uint16_t udata = 0;
    int cur_progress = 0;

    for(;;){
        write_mutex->lock();

        if(write_params->isEmpty()){
            write_mutex->unlock();
            break;
        }

        QPair<QList<Parameter*>, Future*> curParams = write_params->takeFirst();

        write_mutex->unlock();

        curParams.second->setProgressRange(0, curParams.first.size());
        curParams.second->setProgress(0);

        for(QList<Parameter*>::iterator it = curParams.first.begin(); it != curParams.first.end(); ++ it){
            if(curParams.second->needCancel()){
                emit information(tr("Запись отменена."));
                break;
            }
            udata = (*it)->toRaw();
            res = modbusTry(modbus_write_registers, (*it)->id(), 1, &udata);
            if(res == -1){
                emit errorOccured(tr("Невозможно записать параметр с id %1.(%2)")
                                  .arg((*it)->id()).arg(modbus_strerror(errno)));
                curParams.second->finish();
                disconnectFromDevice();
                return;
            }
            curParams.second->setProgress(++ cur_progress);
        }
        curParams.second->finish();
    }

    // Применим параметры.
    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_APPLY_PARAMS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно применить параметры привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
}


void DriveWorker::update()
{
    int res = 0;
    uint16_t udata = 0;
    uint8_t bits_data = 0;

    res = modbusTry(modbus_read_registers, DRIVE_MODBUS_HOLD_REG_REFERENCE, 1, &udata);
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать данные о задании.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_reference = udata;

    res = modbusTry(modbus_read_bits, DRIVE_MODBUS_COIL_RUN, 1, &bits_data);
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать данные о выполнении.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_running = bits_data;
    upd_mutex->lock();

    for(UpdateParamsList::iterator it = upd_params->begin(); it != upd_params->end(); ++ it){
        res = modbusTry(modbus_read_input_registers, it->first->id(), 1, &udata);
        if(res == -1){
            emit errorOccured(tr("Невозможно прочитать параметр с id %1.(%2)")
                              .arg(it->first->id()).arg(modbus_strerror(errno)));
            disconnectFromDevice();
            return;
        }
        it->first->setRaw(udata);
    }

    upd_mutex->unlock();

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
        modbus_close(modbus);
        modbus_free(modbus);
        modbus = nullptr;
        connected_to_device = false;
        dev_running = false;
    }
}

void DriveWorker::cleanup_rw_params()
{
    read_mutex->lock();
    for(auto it = read_params->begin(); it != read_params->end(); ++it){
        it->second->finish();
    }
    read_params->clear();
    read_mutex->unlock();


    write_mutex->lock();
    for(auto it = write_params->begin(); it != write_params->end(); ++it){
        it->second->finish();
    }
    write_params->clear();
    write_mutex->unlock();
}

float DriveWorker::unpack_parameter(int16_t value, param_type_t type)
{
    switch(type){
    default:
    case PARAM_TYPE_INT:
        return static_cast<float>(value);
    case PARAM_TYPE_UINT:
        return static_cast<float>(static_cast<unsigned int>(value));
    case PARAM_TYPE_FRACT_10:
        return static_cast<float>(value) / 10;
    case PARAM_TYPE_FRACT_100:
        return static_cast<float>(value) / 100;
    case PARAM_TYPE_FRACT_1000:
        return static_cast<float>(value) / 1000;
    }
    return 0.0f;
}
