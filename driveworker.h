#ifndef SHAFTBALANCERWORKER_H
#define SHAFTBALANCERWORKER_H

#include <QThread>
#include <QHash>
#include <QPair>
#include <stddef.h>
#include "parameter.h"

class QTimer;
class QMutex;
typedef struct _modbus modbus_t;


/**
 * @brief Класс работы с устройством привода.
 */
class DriveWorker : public QThread
{
    Q_OBJECT
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
     * @brief Добавляет параметр для обновления.
     * @param param Параметр.
     */
    void addUpdParam(Parameter* param);

    /**
     * @brief Удаляет параметр для обновления.
     * @param param Параметр.
     */
    void removeUpdParam(Parameter* param);

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

private slots:

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
     * @brief Точка входа потока.
     */
    void run();

    /**
     * @brief Освобождает контекст Modbus RTU.
     */
    void cleanup_modbus();

    /**
     * @brief Таймер для отсчёта периода обновления.
     */
    QTimer* timer;
    /**
     * @brief Протокол Modbus RTU.
     */
    modbus_t* modbus;
    /**
     * @brief Мютекс.
     */
    QMutex* mutex;
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
     * @brief Распаковывает параметр во float.
     * @param value Значение для распаковки.
     * @param type Тип параметра.
     * @return Распакованное значение.
     */
    float unpack_parameter(int16_t value, param_type_t type);

    UpdateParamsList* upd_params;

    //! Число попыток чтения данных.
    const size_t drive_modbus_retries = 10;

    /**
     * Шаблонная функция попыток
     * обмена данными по Modbus.
     */
    template <typename Func, typename ... Args>
    int modbusTry(Func func, Args ... args);
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

#endif // SHAFTBALANCERWORKER_H
