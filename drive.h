#ifndef DRIVE_H
#define DRIVE_H

#include <QObject>

class DriveWorker;
class Parameter;
class Future;

/**
 * @brief Класс интерфейса взаимодействия с устройством балансировки.
 */
class Drive : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор.
     * @param parent Родитель.
     */
    explicit Drive(QObject *parent = 0);
    ~Drive();

    /**
     * @brief Применяет настройки.
     * @return Флаг успеха.
     */
    bool setup();

    /**
     * @brief Получает флаг возможности начать работу с устройством.
     * @return Флаг возможности начать работу с устройством.
     */
    bool good();

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

    /**
     * @brief Читает список параметров из привода.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @param params Список параметров.
     * @return Будущее.
     */
    Future* readParams(QList<Parameter*>& params);

    /**
     * @brief Записывает список параметров в привод.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @param params Список параметров.
     * @return Будущее.
     */
    Future* writeParams(QList<Parameter*>& params);

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
    void start();

    /**
     * @brief останавливает устройство.
     */
    void stop();

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
     * @brief Сохраняет параметры в ПЗУ.
     */
    void saveParams();

//private signals:
    /**
     * @brief Читает следующий список параметров.
     */
    void readNextParams();

    /**
     * @brief Записывает следующий список параметров.
     */
    void writeNextParams();

private:
    /**
     * @brief Класс работы с устройством балансировки.
     */
    DriveWorker* worker;

    /**
     * @brief Запускает поток работы с устройством балансировки.
     */
    void startWorkerThread();
    /**
     * @brief Останавливает поток работы с устройством балансировки.
     */
    void stopWorkerThread();
};

#endif // DRIVE_H
