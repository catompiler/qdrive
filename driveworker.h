#ifndef SHAFTBALANCERWORKER_H
#define SHAFTBALANCERWORKER_H

#include <QThread>
#include <stddef.h>

class QTimer;
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

private slots:

    /**
     * @brief Обновляет статус устройства и балансировки.
     */
    void update();

private:
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
     * @brief Преобразует fixed10_6_t во float.
     * @param value Значение для распаковки.
     * @return Распакованное значение.
     */
    float unpack_fxd_10_6(int16_t value);
};

#endif // SHAFTBALANCERWORKER_H
