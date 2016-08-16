#ifndef DRIVEWORKER_H
#define DRIVEWORKER_H

#include <QThread>
#include <QHash>
#include <QPair>
#include <QQueue>
#include <QList>
#include <stddef.h>
#include "driveevent.h"
#include "driveoscillogram.h"
#include "drive_types.h"


class QTimer;
class QMutex;
typedef struct _modbus modbus_t;
class Future;
class Parameter;


/**
 * @brief Класс работы с устройством привода.
 */
class DriveWorker : public QThread
{
    Q_OBJECT

    friend class Drive;

public:
    /**
     * @brief Конструктор.
     */
    explicit DriveWorker();
    ~DriveWorker();

    /**
     * @brief Применяет настройки.
     * @return Флаг успеха.
     */
    bool setup();

    /**
     * @brief Получает флаг возможности начать работу с устройством.
     * @return Флаг возможности начать работу с устройством.
     */
    bool good() const;

    /**
     * @brief Получает флаг соединения с устройством.
     * @return Флаг соединения с устройством.
     */
    bool connectedToDevice() const;

    /**
     * @brief Получает текущее задание.
     * @return Текущее задание.
     */
    unsigned int reference() const;

    /**
     * @brief Получает флаг работы.
     * @return Флаг работы.
     */
    bool running() const;

    /**
     * @brief Получает ошибки.
     * @return Ошибки.
     */
    drive_errors_t errors() const;

    /**
     * @brief Получает предупреждения.
     * @return Предупреждения.
     */
    drive_warnings_t warnings() const;

    /**
     * @brief Получает ошибки питания.
     * @return Ошибки питания.
     */
    drive_power_errors_t powerErrors() const;

    /**
     * @brief Получает предупреждения питания.
     * @return Предупреждения питания.
     */
    drive_power_warnings_t powerWarnings() const;

    /**
     * @brief Получает ошибки фаз.
     * @return Ошибки фаз.
     */
    drive_phase_errors_t phaseErrors() const;

    /**
     * @brief Получает события привода.
     * @return События привода.
     */
    QList<DriveEvent> events() const;

    /**
     * @brief Получает осциллограммы привода.
     * @return Осциллограммы привода.
     */
    QList<DriveOscillogram> oscillograms() const;

    /**
     * @brief Добавляет параметр для обновления.
     * @param param Параметр.
     */
    void addUpdParam(Parameter* param);

    /**
     * @brief Удаляет параметр для обновления.
     * @param param Параметр.
     */
    void removeUpdParam(Parameter* param);

    /**
     * @brief Добавляет параметры для чтения.
     * @param params Список параметров для чтения.
     * @param future Будущее для контроля.
     */
    void addReadParams(QList<Parameter*>& params, Future* future);

    /**
     * @brief Добавляет параметры для записи.
     * @param params Список параметров для записи.
     * @param future Будущее для контроля.
     */
    void addWriteParams(QList<Parameter*>& params, Future* future);

signals:

    /**
     * @brief Сигнал обновления данных.
     */
    void updated();

    /**
     * @brief Сигнал возникновения ошибки.
     * @param error_text Текст ошибки.
     */
    void errorOccured(const QString& error_text);

    /**
     * @brief Сигнал информации о событиях.
     * @param info_text Информация о событии.
     */
    void information(const QString& info_text);

    /**
     * @brief Сигнал подключения к устройству.
     */
    void connected();

    /**
     * @brief Сигнал отключения от устройства.
     */
    void disconnected();

public slots:

    /**
     * @brief Соединяется с устройством.
     * Вызывает сигнал connected() в случае успеха.
     */
    void connectToDevice();

    /**
     * @brief Отсоединяется от устройства.
     * Вызывает сигнал disconnected().
     */
    void disconnectFromDevice();

    /**
     * @brief Запускает/останавливает устройство.
     * @param run Флаг запуска.
     */
    void setRunning(bool run);

    /**
     * @brief Запускает устройство.
     */
    void startDrive();

    /**
     * @brief останавливает устройство.
     */
    void stopDrive();

    /**
     * @brief Устанавливает задание.
     * @param reference Задание.
     */
    void setReference(unsigned int reference);

    /**
     * @brief Очищает ошибки привода.
     */
    void clearErrors();

    /**
     * @brief Калибрует питание.
     */
    void calibratePower();

    /**
     * @brief Сохраняет параметры в ПЗУ.
     */
    void saveParams();

    /**
     * @brief Читает события.
     */
    void readEvents(Future* future);

    /**
     * @brief Читает осциллограммы.
     */
    void readOscillograms(Future* future);

private slots:

    /**
     * @brief Читает следующий список параметров.
     */
    void readNextParams();

    /**
     * @brief Записывает следующий список параметров.
     */
    void writeNextParams();

    /**
     * @brief Обновляет статус устройства и балансировки.
     */
    void update();

private:

    /**
     * @brief Тип списка параметров для обновления.
     * QHash< Идентификатор, QPair< Параметр, Количество ссылок >>;
     */
    typedef QHash<param_id_t, QPair<Parameter*, size_t>> UpdateParamsList;

    /**
     * @brief Тип списка параметров для чтения.
     * QList< QPair< QList<Параметр>, Будущее >>;
     * Qt использует подсчёт ссылок в контейнерах,
     * поэтому ничего страшного в хранении в переменной
     * самого списка, а не его указателя нет.
     */
    typedef QQueue<QPair<QList<Parameter*>, Future*>> ReadParamsList;

    /**
     * @brief Тип списка параметров для записи.
     * QList< QPair< QList<Параметр>, Будущее >>;
     * Qt использует подсчёт ссылок в контейнерах,
     * поэтому ничего страшного в хранении в переменной
     * самого списка, а не его указателя нет.
     */
    typedef QQueue<QPair<QList<Parameter*>, Future*>> WriteParamsList;

    /**
     * @brief Точка входа потока.
     */
    void run();

    /**
     * @brief Освобождает контекст Modbus RTU.
     */
    void cleanup_modbus();

    /**
     * @brief Очищает списки параметров для чтения / записи.
     */
    void cleanup_rw_params();

    /**
     * @brief Читает событие.
     * @param event Событие.
     * @param index Индекс события.
     * @return Флаг успешного чтения события.
     */
    bool readEvent(drive_event_t* event, size_t index);

    /**
     * @brief Читает канал осциллограммы.
     * @param channel Канал осциллограммы.
     * @param index Индекс осциллограммы.
     * @param ch_index Индекс канала осциллограммы.
     * @return Флаг успешного чтения канала осциллограммы.
     */
    bool readOscillogramChannel(DriveOscillogram::Channel* channel, size_t index, size_t ch_index);

    /**
     * @brief Читает осциллограмму.
     * @param oscillogram Осциллограмма.
     * @param index Индекс осциллограммы.
     * @param future Будущее.
     * @return Флаг успешного чтения осциллограммы.
     */
    bool readOscillogram(DriveOscillogram* oscillogram, size_t index, Future* future);

    /**
     * @brief Таймер для отсчёта периода обновления.
     */
    QTimer* timer;
    /**
     * @brief Протокол Modbus RTU.
     */
    modbus_t* modbus;
    /**
     * @brief Мютекс обновления параметров.
     */
    QMutex* upd_mutex;
    /**
     * @brief Мютекс чтения параметров.
     */
    QMutex* read_mutex;
    /**
     * @brief Мютекс записи параметров.
     */
    QMutex* write_mutex;
    /**
     * @brief Флаг подключения к устройству.
     */
    bool connected_to_device;
    /**
     * @brief Задание.
     */
    unsigned int dev_reference;
    /**
     * @brief Работа.
     */
    bool dev_running;
    /**
     * @brief Ошибки.
     */
    drive_errors_t dev_errors;
    /**
     * @brief Предупреждения.
     */
    drive_warnings_t dev_warnings;
    /**
     * @brief Ошибки питания.
     */
    drive_errors_t dev_power_errors;
    /**
     * @brief Предупреждения питания.
     */
    drive_warnings_t dev_power_warnings;
    /**
     * @brief Ошибки фаз.
     */
    drive_phase_errors_t dev_phase_errors;

    /**
     * @brief Распаковывает параметр во float.
     * @param value Значение для распаковки.
     * @param type Тип параметра.
     * @return Распакованное значение.
     */
    float unpack_parameter(int16_t value, param_type_t type);

    /**
     * @brief Список параметров для обновления.
     */
    UpdateParamsList* upd_params;

    /**
     * @brief Список параметров для чтения.
     */
    ReadParamsList* read_params;

    /**
     * @brief Список параметров для записи.
     */
    WriteParamsList* write_params;

    /**
     * @brief Список событий.
     */
    QList<DriveEvent>* events_list;

    /**
     * @brief Список осциллограмм.
     */
    QList<DriveOscillogram>* osc_list;

    //! Число попыток чтения данных.
    const size_t drive_modbus_retries = 10;

    //! Ожидание чтения данных.
    const size_t drive_modbus_wait_time_us = 1000;

    /**
     * Шаблонная функция попыток
     * обмена данными по Modbus.
     */
    template <typename Func, typename ... Args>
    int modbusTry(Func func, Args ... args);

    /**
     * @brief Функция попыток сырого запроса к устройству.
     * @param req Запрос.
     * @param req_size Размер запроса.
     * @param buffer Буфер 256 байт для ответа.
     * @return Число полученных байт, либо -1 при ошибке.
     */
    int modbusTryRaw(void* req, size_t req_size, void* buffer);
};

template <typename Func, typename ... Args>
int DriveWorker::modbusTry(Func func, Args ... args)
{
    int res = -1;
    for(size_t retries = 0; retries < drive_modbus_retries; retries ++){
        res = func(modbus, args...);
        if(res != -1) break;
    }
    return res;
}

#endif // DRIVEWORKER_H
