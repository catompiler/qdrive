#include "driveworker.h"
#include <QTimer>
#include <QMutex>
#include <QVector>
#include <QByteArray>
#include <QDataStream>
#include <algorithm>
#include <iterator>
#include "settings.h"
#include "future.h"
#include "parameter.h"
#include <modbus/modbus-rtu.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <QDate>
#include <QTime>
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

#define DRIVE_ID 0xAA
#define DRIVE_ID_MAGIC 0x4702

//! Начало адресов регистров хранения.
#define DRIVE_MODBUS_HOLD_REGS_START 2000
//! Начало адресов регистров ввода.
#define DRIVE_MODBUS_INPUT_REGS_START 2000
//! Начало адресов цифровых входов.
#define DRIVE_MODBUS_DINS_START 2000
//! Начало адресов регистров флагов.
#define DRIVE_MODBUS_COILS_START 2000

// Константы адресов.
// Регистры ввода.
//! Полуслова состояния.
#define DRIVE_MODBUS_INPUT_REG_STATE (DRIVE_MODBUS_INPUT_REGS_START + 0)
//! Полуслова ошибок.
#define DRIVE_MODBUS_INPUT_REG_ERRORS0 (DRIVE_MODBUS_INPUT_REGS_START + 10)
#define DRIVE_MODBUS_INPUT_REG_ERRORS1 (DRIVE_MODBUS_INPUT_REGS_START + 11)
//! Полуслова предупреждений
#define DRIVE_MODBUS_INPUT_REG_WARNINGS0 (DRIVE_MODBUS_INPUT_REGS_START + 12)
#define DRIVE_MODBUS_INPUT_REG_WARNINGS1 (DRIVE_MODBUS_INPUT_REGS_START + 13)
//! Полуслова ошибок питания.
#define DRIVE_MODBUS_INPUT_REG_PWR_ERRORS0 (DRIVE_MODBUS_INPUT_REGS_START + 14)
#define DRIVE_MODBUS_INPUT_REG_PWR_ERRORS1 (DRIVE_MODBUS_INPUT_REGS_START + 15)
//! Полуслова предупреждений питания.
#define DRIVE_MODBUS_INPUT_REG_PWR_WARNINGS0 (DRIVE_MODBUS_INPUT_REGS_START + 16)
#define DRIVE_MODBUS_INPUT_REG_PWR_WARNINGS1 (DRIVE_MODBUS_INPUT_REGS_START + 17)
//! Полуслова ошибок фаз.
#define DRIVE_MODBUS_INPUT_REG_PHASE_ERRORS0 (DRIVE_MODBUS_INPUT_REGS_START + 18)
#define DRIVE_MODBUS_INPUT_REG_PHASE_ERRORS1 (DRIVE_MODBUS_INPUT_REGS_START + 19)
// Регистры хранения.
//! Задание.
#define DRIVE_MODBUS_HOLD_REG_REFERENCE (DRIVE_MODBUS_HOLD_REGS_START + 0)
// Дата и время.
//! Год.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_YEAR (DRIVE_MODBUS_HOLD_REGS_START + 1)
//! Месяц.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_MONTH (DRIVE_MODBUS_HOLD_REGS_START + 2)
//! День.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_DAY (DRIVE_MODBUS_HOLD_REGS_START + 3)
//! Час.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_HOUR (DRIVE_MODBUS_HOLD_REGS_START + 4)
//! Минута.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_MIN (DRIVE_MODBUS_HOLD_REGS_START + 5)
//! Секунда.
#define DRIVE_MODBUS_HOLD_REG_DATETIME_SEC (DRIVE_MODBUS_HOLD_REGS_START + 6)
// Цифровые входа.
// Регистры флагов.
//! Запуск/останов.
#define DRIVE_MODBUS_COIL_RUN (DRIVE_MODBUS_COILS_START + 0)
//! Сброс ошибок.
#define DRIVE_MODBUS_COIL_CLEAR_ERRORS (DRIVE_MODBUS_COILS_START + 1)
//! Применение настроек.
#define DRIVE_MODBUS_COIL_APPLY_PARAMS (DRIVE_MODBUS_COILS_START + 2)
//! Сохранение настроек.
#define DRIVE_MODBUS_COIL_SAVE_PARAMS (DRIVE_MODBUS_COILS_START + 3)
//! Калибровка питания.
#define DRIVE_MODBUS_COIL_CALIBRATE_POWER (DRIVE_MODBUS_COILS_START + 4)
//! Очистка событий.
#define DRIVE_MODBUS_COIL_CLEAR_EVENTS (DRIVE_MODBUS_COILS_START + 5)
//! Создаёт событие с записью состояния.
#define DRIVE_MODBUS_COIL_MAKE_STATUS_EVENT (DRIVE_MODBUS_COILS_START + 6)
//! Устанавливает пользовательские цифровые выхода.
#define DRIVE_MODBUS_COIL_DOUT_USER_SET_VALUE (DRIVE_MODBUS_COILS_START + 7)
//! Переключает пользовательские цифровые выхода.
#define DRIVE_MODBUS_COIL_DOUT_USER_TOGGLE (DRIVE_MODBUS_COILS_START + 8)
//! Экстренный останов.
#define DRIVE_MODBUS_COIL_EMERGENCY_STOP (DRIVE_MODBUS_COILS_START + 9)


// Пользовательские функции и коды.
// Запрос: | func | code | [args] |
// Ответ:  | func | code | [data] |

// Константы статуса асинхронных операций.
//! Операция не начата.
#define DRIVE_MODBUS_ASYNC_OP_IDLE 0
//! Операция завершена.
#define DRIVE_MODBUS_ASYNC_OP_DONE 1
//! Операция в процессе.
#define DRIVE_MODBUS_ASYNC_OP_RUNNING 2
//! Ошибка при выполнении операции.
#define DRIVE_MODBUS_ASYNC_OP_ERROR 3

//! Функция доступа к списку событий.
#define DRIVE_MODBUS_FUNC_EVENTS_ACCESS 65
/**
 * Код получения числа событий.
 * Запрос: | 65 | 0 |
 * Ответ:  | 65 | 0 | N |
 * N - число событий, 1 байт.
 */
#define DRIVE_MODBUS_CODE_EVENTS_COUNT 0
/**
 * Код запуска чтения события
 * с заданным номером.
 * Запрос: | 65 | 1 | N |
 * Ответ:  | 65 | 1 | N |
 * N - номер события, 1 байт.
 */
#define DRIVE_MODBUS_CODE_READ_EVENT 1
/**
 * Код получения статуса
 * чтения события.
 * Запрос: | 65 | 2 |
 * Ответ:  | 65 | 2 | N |
 * N - состояние чтения события, 1 байт:
 *     0 - событие не прочитано;
 *     1 - событие прочитано;
 *     2 - чтение в процессе.
 *     3 - ошибка чтения события.
 */
#define DRIVE_MODBUS_CODE_READ_EVENT_STATUS 2
/**
 * Код чтения прочитанного
 * события.
 * Запрос: | 65 | 3 |
 * Ответ:  | 65 | 3 | N | data |
 * N - размер данных события, 1 байт;
 * data - данные события, N байт.
 */
#define DRIVE_MODBUS_CODE_GET_READED_EVENT 3


//! Функция доступа к осциллограммам.
#define DRIVE_MODBUS_FUNC_OSC_ACCESS 66
/**
 * Код получения числа осциллограмм.
 * Запрос: | 66 | 0 |
 * Ответ:  | 66 | 0 | N |
 * N - число осциллограмм, 1 байт.
 */
#define DRIVE_MODBUS_CODE_OSC_COUNT 0
/**
 * Код получения идентификатора
 * события осциллограммы с
 * заданным номером.
 * Запрос: | 66 | 1 | N |
 * Ответ:  | 66 | 1 | ID |
 * N - номер осциллограммы, 1 байт.
 * ID - идентификатор события, 1 байт.
 */
#define DRIVE_MODBUS_CODE_OSC_EVENT_ID 1
/**
 * Код запуска чтения заданного
 * канала осциллограммы
 * с заданным номером.
 * Запрос: | 66 | 2 | N | CH |
 * Ответ:  | 66 | 2 | N | CH |
 * N - номер осциллограммы, 1 байт.
 * CH - канал осциллограммы, 1 байт.
 */
#define DRIVE_MODBUS_CODE_READ_OSC 2
/**
 * Код получения статуса
 * чтения осциллограммы.
 * Запрос: | 66 | 3 |
 * Ответ:  | 66 | 3 | N |
 * N - состояние чтения осциллограммы, 1 байт:
 *     0 - осциллограмма не прочитана;
 *     1 - осциллограмма прочитана;
 *     2 - чтение в процессе.
 *     3 - ошибка чтения осциллограммы.
 */
#define DRIVE_MODBUS_CODE_READ_OSC_STATUS 3
/**
 * Код чтения прочитанной
 * осциллограммы.
 * Запрос: | 66 | 4 | A | N |
 * Ответ:  | 66 | 4 | N | data |
 * A - смещение в данных канала осциллограммы, 2 байта, старшим вперёд.
 * N - размер данных осциллограммы, 1 байт;
 * data - данные осциллограммы, N байт.
 */
#define DRIVE_MODBUS_CODE_GET_READED_OSC 4



DriveWorker::DriveWorker() : QThread()
{
    qRegisterMetaType<QList<size_t>>("QList<size_t>");

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

    events_list = new QList<DriveEvent>();

    readed_osc_list = new QList<DriveOscillogram>();

    osc_list = new QList<drive_event_id_t>();

    modbus = nullptr;

    connected_to_device = false;

    dev_reference = 0;
    dev_running = false;
    dev_state = DRIVE_STATE_IDLE;
    dev_errors = 0;
    dev_warnings = 0;
    dev_power_errors = 0;
    dev_power_warnings = 0;
    dev_phase_errors = 0;
}

DriveWorker::~DriveWorker()
{
    cleanup_modbus();

    delete osc_list;

    delete readed_osc_list;

    delete events_list;

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

    //modbus_set_debug(modbus, true);

    //modbus_set_error_recovery(modbus, static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL));
    modbus_set_error_recovery(modbus, static_cast<modbus_error_recovery_mode>(MODBUS_ERROR_RECOVERY_NONE));

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

    //modbus_set_response_timeout(modbus, &tv_timeout);
    modbus_set_response_timeout(modbus, tv_timeout.tv_sec, tv_timeout.tv_usec);

    tv_timeout.tv_sec = settings.byteTimeout() / 1000;
    tv_timeout.tv_usec = (settings.byteTimeout() % 1000) * 1000;

    //modbus_set_byte_timeout(modbus, &tv_timeout);
    modbus_set_byte_timeout(modbus, tv_timeout.tv_sec, tv_timeout.tv_usec);

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

drive_state_t DriveWorker::state() const
{
    return dev_state;
}

drive_errors_t DriveWorker::errors() const
{
    return dev_errors;
}

drive_warnings_t DriveWorker::warnings() const
{
    return dev_warnings;
}

drive_power_errors_t DriveWorker::powerErrors() const
{
    return dev_power_errors;
}

drive_power_warnings_t DriveWorker::powerWarnings() const
{
    return dev_power_warnings;
}

drive_phase_errors_t DriveWorker::phaseErrors() const
{
    return dev_phase_errors;
}

QList<DriveEvent> DriveWorker::events() const
{
    return QList<DriveEvent>(*events_list);
}

QList<DriveOscillogram> DriveWorker::oscillograms() const
{
    return QList<DriveOscillogram>(*readed_osc_list);
}

QList<drive_event_id_t> DriveWorker::oscillogramsList() const
{
    return QList<drive_event_id_t>(*osc_list);
}

size_t DriveWorker::oscillogramsCount() const
{
    return readed_osc_list->size();
}

DriveOscillogram DriveWorker::oscillogram(size_t index) const
{
    if(index >= static_cast<size_t>(readed_osc_list->size())) return DriveOscillogram();
    return readed_osc_list->at(index);
}

void DriveWorker::addOscillogram(const DriveOscillogram& osc)
{
    readed_osc_list->append(osc);
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

    //res = modbusTry(modbus_report_slave_id, &response[0]);
    res = modbusTry(modbus_report_slave_id, max_id_size, &response[0]);
    if(res == -1){
        emit errorOccured(tr("Нет ответа от устройства.(%1)").arg(modbus_strerror(errno)));
        return;
    }

    /*drive_modbus_id_t* drive_id = reinterpret_cast<drive_modbus_id_t*>(&response[0]);

    if(drive_id->magic != DRIVE_ID_MAGIC){
        emit errorOccured(tr("Неправильный идентификатор устройства."));
        return;
    }*/

    uint8_t id = response[0];
    drive_modbus_id_t* drive_id = reinterpret_cast<drive_modbus_id_t*>(&response[2]);

    if(id != DRIVE_ID || drive_id->magic != DRIVE_ID_MAGIC){
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
    if(!connected_to_device) return;

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
    if(!connected_to_device) return;

    setRunning(true);
    if(dev_running) emit information(tr("Запуск привода."));
}

void DriveWorker::stopDrive()
{
    if(!connected_to_device) return;

    setRunning(false);
    if(!dev_running) emit information(tr("Останов привода."));
}

void DriveWorker::emergencyStopDrive()
{
    if(!connected_to_device) return;

    int res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_EMERGENCY_STOP, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно экстренно остановить привод.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Экстренный останов привода."));
}

void DriveWorker::setReference(unsigned int reference)
{
    if(!connected_to_device) return;

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
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_CLEAR_ERRORS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно очистить ошибки привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Ошибки очищены."));
}

void DriveWorker::clearEvents()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_CLEAR_EVENTS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно очистить сыбытия привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("События очищены."));
}

void DriveWorker::makeStatusEvent()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_MAKE_STATUS_EVENT, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно создать сыбытие состояния привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Событие состояния создано."));
}

void DriveWorker::calibratePower()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_CALIBRATE_POWER, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно калибровать питание привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Калибровка."));
}

void DriveWorker::saveParams()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_SAVE_PARAMS, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно сохранить параметры привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Параметры сохранены."));
}

void DriveWorker::setDateTime(QDateTime datetime)
{
    if(!connected_to_device) return;

    int res = 0;

    uint16_t data[6];

    QDate date = datetime.date();
    QTime time = datetime.time();

    data[0] = date.year();
    data[1] = date.month();
    data[2] = date.day();
    data[3] = time.hour();
    data[4] = time.minute();
    data[5] = time.second();

    res = modbusTry(modbus_write_registers, DRIVE_MODBUS_HOLD_REG_DATETIME_YEAR, 6, data);
    if(res == -1){
        emit errorOccured(tr("Невозможно установить дату и время.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }

    emit information(tr("Дата и время установлены."));
}

bool DriveWorker::readEvent(drive_event_t* event, size_t index)
{
    if(!connected_to_device) return false;

    QVector<uint8_t> buf(MODBUS_RTU_MAX_ADU_LENGTH);

    uint8_t dev_addr = Settings::get().deviceAddress();

    unsigned int status = DRIVE_MODBUS_ASYNC_OP_IDLE;

    int res = 0;

#define REQ_DATA_OFFSET 3

    uint8_t read_event_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_EVENTS_ACCESS,
        DRIVE_MODBUS_CODE_READ_EVENT,
        static_cast<uint8_t>(index)
    };

    uint8_t read_status_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_EVENTS_ACCESS,
        DRIVE_MODBUS_CODE_READ_EVENT_STATUS
    };

    uint8_t get_event_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_EVENTS_ACCESS,
        DRIVE_MODBUS_CODE_GET_READED_EVENT
    };

    res = modbusTryRaw(read_event_req, sizeof(read_event_req), &buf[0]);
    if(res == -1){
        emit errorOccured(tr("Невозможно начать чтение события.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    //qDebug () << buf;

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*index*/ + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при начале чтения события.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    for(;;){
        res = modbusTryRaw(read_status_req, sizeof(read_status_req), &buf[0]);
        if(res == -1){
            emit errorOccured(tr("Невозможно запросить состояние чтения события.(%1)").arg(modbus_strerror(errno)));
            return false;
        }

        if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*status*/ + 2/*crc*/){
            emit errorOccured(tr("Ошибочный ответ при чтении статуса чтения события.(%1)").arg(modbus_strerror(errno)));
            return false;
        }

        status = static_cast<unsigned int>(buf[REQ_DATA_OFFSET]);

        if(status == DRIVE_MODBUS_ASYNC_OP_DONE) break;
        else if(status >= DRIVE_MODBUS_ASYNC_OP_ERROR){
            emit errorOccured(tr("Ошибка чтения события (%1).").arg(status - DRIVE_MODBUS_ASYNC_OP_ERROR));
            return false;
        }
        // wait
        usleep(drive_modbus_wait_time_us);
    }

    res = modbusTryRaw(get_event_req, sizeof(get_event_req), &buf[0]);
    if(res == -1){
        emit errorOccured(tr("Невозможно получить событие.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*size*/ + sizeof(drive_event_t) + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при получении события.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    if(buf[REQ_DATA_OFFSET] != sizeof(drive_event_t)){
        emit errorOccured(tr("Ошибочный размер полученного события.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    memcpy(event, &buf[REQ_DATA_OFFSET + 1], sizeof(drive_event_t));

#undef REQ_DATA_OFFSET

    return true;
}

void DriveWorker::readEvents(Future* future)
{
    if(!connected_to_device) return;

    int res = 0;

#define REQ_DATA_OFFSET 3

    QByteArray ba(MODBUS_RTU_MAX_ADU_LENGTH, 0x0);
    QDataStream ds(&ba, QIODevice::ReadWrite);

    ds.setVersion(QDataStream::Qt_DefaultCompiledVersion);

    uint8_t dev_addr = Settings::get().deviceAddress();

    ds << dev_addr
       << static_cast<uint8_t>(DRIVE_MODBUS_FUNC_EVENTS_ACCESS)
       << static_cast<uint8_t>(DRIVE_MODBUS_CODE_EVENTS_COUNT);

    future->start();

    res = modbusTryRaw(ba.data(), static_cast<int>(ds.device()->pos()), ba.data());
    if(res == -1){
        emit errorOccured(tr("Невозможно получить число событий.(%1)").arg(modbus_strerror(errno)));
        future->finish();
        disconnectFromDevice();
        return;
    }

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*count*/ + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при получении числа событий.(%1)").arg(modbus_strerror(errno)));
        future->finish();
        disconnectFromDevice();
        return;
    }

    size_t events_count = static_cast<size_t>(ba[REQ_DATA_OFFSET]);

    //qDebug () << events_count;

    events_list->clear();

    future->setProgressRange(0, static_cast<int>(events_count));

    drive_event_t event;

    for(size_t index = 0; index < events_count; index ++){
        if(readEvent(&event, index)){
            events_list->append(DriveEvent(event));
        }
        if(future->needCancel()) break;
        future->setProgress(static_cast<int>(index + 1));
    }

    future->finish();

#undef REQ_DATA_OFFSET
}

bool DriveWorker::readOscillogramChannel(DriveOscillogram::Channel *channel, size_t index, size_t ch_index)
{
    if(!connected_to_device) return false;

    QVector<uint8_t> buf(MODBUS_RTU_MAX_ADU_LENGTH);

    uint8_t dev_addr = Settings::get().deviceAddress();

    unsigned int status = DRIVE_MODBUS_ASYNC_OP_IDLE;

    int res = 0;

#define REQ_DATA_OFFSET 3

    uint8_t read_osc_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_OSC_ACCESS,
        DRIVE_MODBUS_CODE_READ_OSC,
        static_cast<uint8_t>(index),
        static_cast<uint8_t>(ch_index)
    };

    uint8_t read_status_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_OSC_ACCESS,
        DRIVE_MODBUS_CODE_READ_OSC_STATUS
    };

    uint8_t get_osc_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_OSC_ACCESS,
        DRIVE_MODBUS_CODE_GET_READED_OSC,
        0, 0, // Адрес.
        0 // Размер.
    };

    res = modbusTryRaw(read_osc_req, sizeof(read_osc_req), &buf[0]);
    if(res == -1){
        emit errorOccured(tr("Невозможно начать чтение канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*index*/ + 1/*channel*/ + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при начале чтения канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    for(;;){
        res = modbusTryRaw(read_status_req, sizeof(read_status_req), &buf[0]);
        if(res == -1){
            emit errorOccured(tr("Невозможно запросить состояние чтения канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
            return false;
        }

        if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*status*/ + 2/*crc*/){
            emit errorOccured(tr("Ошибочный ответ при чтении статуса чтения канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
            return false;
        }

        status = static_cast<unsigned int>(buf[REQ_DATA_OFFSET]);

        if(status == DRIVE_MODBUS_ASYNC_OP_DONE) break;
        else if(status >= DRIVE_MODBUS_ASYNC_OP_ERROR){
            emit errorOccured(tr("Ошибка чтения канала осциллограммы."));
            return false;
        }
        // wait
        usleep(drive_modbus_wait_time_us);
    }

    QVector<uint8_t> osc_data;
    size_t addr = 0;
    size_t size = 0;
    size_t max_size = MODBUS_RTU_MAX_ADU_LENGTH - 1/*addr*/ - 1/*func*/ - 1/*code*/ - 1/*size*/ - 2/*crc*/;

    while(static_cast<unsigned int>(osc_data.size()) < DRIVE_POWER_OSC_CHANNEL_SIZE){

        size = std::min<size_t>(max_size, (DRIVE_POWER_OSC_CHANNEL_SIZE - osc_data.size()));

        get_osc_req[REQ_DATA_OFFSET + 0] = (addr >> 16) & 0xff;
        get_osc_req[REQ_DATA_OFFSET + 1] = addr & 0xff;
        get_osc_req[REQ_DATA_OFFSET + 2] = size;

        addr += size;

        res = modbusTryRaw(get_osc_req, sizeof(get_osc_req), &buf[0]);
        if(res == -1){
            emit errorOccured(tr("Невозможно получить данные канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
            return false;
        }

        if(static_cast<unsigned int>(res) != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*size*/ + size + 2/*crc*/){
            emit errorOccured(tr("Ошибочный ответ при получении данных канала осциллограммы.(%1)").arg(modbus_strerror(errno)));
            return false;
        }
        std::copy(buf.begin() + REQ_DATA_OFFSET + 1, buf.begin() + REQ_DATA_OFFSET + 1 + size, std::back_inserter(osc_data));
    }
    channel->setData(0, reinterpret_cast<osc_value_t*>(osc_data.data()), osc_data.size() / sizeof(osc_value_t));

#undef REQ_DATA_OFFSET

    return true;
}

bool DriveWorker::readOscillogramEventId(size_t index, drive_event_id_t* event_id)
{
    if(!connected_to_device) return false;

    if(event_id == nullptr) return false;

    QVector<uint8_t> buf(MODBUS_RTU_MAX_ADU_LENGTH);

    uint8_t dev_addr = Settings::get().deviceAddress();

    int res = 0;

#define REQ_DATA_OFFSET 3

    uint8_t read_osc_event_id_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_OSC_ACCESS,
        DRIVE_MODBUS_CODE_OSC_EVENT_ID,
        static_cast<uint8_t>(index)
    };

    res = modbusTryRaw(read_osc_event_id_req, sizeof(read_osc_event_id_req), &buf[0]);
    if(res == -1){
        emit errorOccured(tr("Невозможно получить идентификатор события осциллограммы.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*id*/ + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при получении идентификатора события осциллограммы.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    //qDebug() << "ev_id:" << buf[REQ_DATA_OFFSET];

    *event_id = static_cast<drive_event_id_t>(buf[REQ_DATA_OFFSET]);

#undef REQ_DATA_OFFSET

    return true;
}

bool DriveWorker::readOscillogram(DriveOscillogram* oscillogram, size_t index, Future* future)
{
    if(!connected_to_device) return false;

    drive_event_id_t event_id = 0;

    if(!readOscillogramEventId(index, &event_id)){
        return false;
    }

    oscillogram->setEventId(event_id);

    for(size_t i = 0; i < oscillogram->channelsCount(); i ++){
        if(!readOscillogramChannel(oscillogram->channel(i), index, i)) return false;
        if(future->needCancel()) return false;
        future->setProgress(future->progress() + 1);
    }

#undef REQ_DATA_OFFSET

    return true;
}

bool DriveWorker::readOscillogramsCount(size_t* count)
{
    if(!connected_to_device) return false;

    if(count == nullptr) return false;

    QVector<uint8_t> buf(MODBUS_RTU_MAX_ADU_LENGTH);

    uint8_t dev_addr = Settings::get().deviceAddress();

    int res = 0;

#define REQ_DATA_OFFSET 3

    uint8_t read_oscs_count_req[] = {
        dev_addr,
        DRIVE_MODBUS_FUNC_OSC_ACCESS,
        DRIVE_MODBUS_CODE_OSC_COUNT
    };

    res = modbusTryRaw(read_oscs_count_req, sizeof(read_oscs_count_req), &buf[0]);
    if(res == -1){
        emit errorOccured(tr("Невозможно получить число осциллограмм.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    if(res != 1/*addr*/ + 1/*func*/ + 1/*code*/ + 1/*count*/ + 2/*crc*/){
        emit errorOccured(tr("Ошибочный ответ при получении числа осциллограмм.(%1)").arg(modbus_strerror(errno)));
        return false;
    }

    *count = static_cast<size_t>(buf[REQ_DATA_OFFSET]);

    return true;

#undef REQ_DATA_OFFSET
}

void DriveWorker::readOscillograms(Future *future)
{
    if(!connected_to_device) return;

    size_t osc_count = 0;

    future->start();

    if(!readOscillogramsCount(&osc_count)){
        future->finish();
        disconnectFromDevice();
        return;
    }

    //qDebug() << osc_count;

    readed_osc_list->clear();

    DriveOscillogram osc;

    future->setProgressRange(0, static_cast<int>(osc_count) * osc.channelsCount());

    for(size_t index = 0; index < osc_count; index ++){
        if(readOscillogram(&osc, index, future)){
            readed_osc_list->append(osc);
        }
        if(future->needCancel()) break;
        //future->setProgress(static_cast<int>(index + 1));
    }

    future->finish();
}

void DriveWorker::readSelectedOscillograms(Future* future, QList<size_t> osc_list)
{
    if(!connected_to_device) return;

    if(osc_list.empty()) return;

    future->start();

    //qDebug() << osc_count;

    readed_osc_list->clear();

    DriveOscillogram osc;

    future->setProgressRange(0, osc_list.count() * osc.channelsCount());

    for(size_t index = 0; index < static_cast<size_t>(osc_list.count()); index ++){
        if(readOscillogram(&osc, osc_list.at(static_cast<int>(index)), future)){
            readed_osc_list->append(osc);
        }
        if(future->needCancel()) break;
        //future->setProgress(static_cast<int>(index + 1));
    }

    future->finish();
}

void DriveWorker::readOscillogramsList(Future* future)
{
    if(!connected_to_device) return;

    size_t osc_count = 0;

    future->start();

    if(!readOscillogramsCount(&osc_count)){
        future->finish();
        disconnectFromDevice();
        return;
    }

    osc_list->clear();

    drive_event_id_t event_id = 0;

    future->setProgressRange(0, static_cast<int>(osc_count));

    for(size_t index = 0; index < osc_count; index ++){
        if(readOscillogramEventId(index, &event_id)){
            osc_list->append(event_id);
        }
        if(future->needCancel()) break;
        future->setProgress(static_cast<int>(index + 1));
    }

    future->finish();
}

void DriveWorker::doutUserOn()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_DOUT_USER_SET_VALUE, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно установить пользовательские цифровые выхода привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Пользовательские выхода установлены."));
}

void DriveWorker::doutUserOff()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_DOUT_USER_SET_VALUE, 0);
    if(res == -1){
        emit errorOccured(tr("Невозможно установить пользовательские цифровые выхода привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Пользовательские выхода установлены."));
}

void DriveWorker::doutUserToggle()
{
    if(!connected_to_device) return;

    int res = 0;

    res = modbusTry(modbus_write_bit, DRIVE_MODBUS_COIL_DOUT_USER_TOGGLE, 1);
    if(res == -1){
        emit errorOccured(tr("Невозможно переключить пользовательские цифровые выхода привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    emit information(tr("Пользовательские выхода переключены."));
}

void DriveWorker::readNextParams()
{
    if(!connected_to_device) return;

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
    if(!connected_to_device) return;

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
    if(!connected_to_device) return;

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

    res = modbusTry(modbus_read_input_registers, DRIVE_MODBUS_INPUT_REG_STATE, 1, &udata);
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать состояние привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }
    dev_state = static_cast<drive_state_t>(udata);

    #define DRIVE_FLAGS_DATA_LEN 10
    uint16_t drive_flags_data[DRIVE_FLAGS_DATA_LEN];
    res = modbusTry(modbus_read_input_registers, DRIVE_MODBUS_INPUT_REG_ERRORS0, DRIVE_FLAGS_DATA_LEN, drive_flags_data);
    if(res == -1){
        emit errorOccured(tr("Невозможно прочитать ошибки и предупреждения привода.(%1)").arg(modbus_strerror(errno)));
        disconnectFromDevice();
        return;
    }

    drive_errors_t old_errs = dev_errors;

    dev_errors = (static_cast<drive_errors_t>(drive_flags_data[1]) << 16) | drive_flags_data[0];
    dev_warnings = (static_cast<drive_warnings_t>(drive_flags_data[3]) << 16) | drive_flags_data[2];
    dev_power_errors = (static_cast<drive_power_errors_t>(drive_flags_data[5]) << 16) | drive_flags_data[4];
    dev_power_warnings = (static_cast<drive_power_warnings_t>(drive_flags_data[7]) << 16) | drive_flags_data[6];
    dev_phase_errors = (static_cast<drive_phase_errors_t>(drive_flags_data[9]) << 16) | drive_flags_data[8];

    if(old_errs == DRIVE_ERROR_NONE && dev_errors != DRIVE_ERROR_NONE) emit driveErrorOccured();

    upd_mutex->lock();

    for(UpdateParamsList::iterator it = upd_params->begin(); it != upd_params->end(); ++ it){
        res = modbusTry(modbus_read_input_registers, it->first->id(), 1, &udata);
        if(res == -1){
            upd_mutex->unlock();
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

int DriveWorker::modbusTryRaw(void *req, size_t req_size, void *buffer)
{
    int res = -1;
    for(size_t retries = 0; retries < drive_modbus_retries; retries ++){
        res = modbus_send_raw_request(modbus, static_cast<uint8_t*>(req), req_size);
        if(res == -1) continue;
        res = modbus_receive_raw_confirmation(modbus, static_cast<uint8_t*>(buffer));
        if(res != -1) break;
    }
    return res;
}
