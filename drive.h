#ifndef DRIVE_H
#define DRIVE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>
#include "driveevent.h"
#include "drive_types.h"
#include "driveevent.h"
#include "driveoscillogram.h"

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
    float reference() const;

    /**
     * @brief Получает флаг работы.
     * @return Флаг работы.
     */
    bool running() const;

    /**
     * @brief Получает состояние.
     * @return Состояние.
     */
    drive_state_t state() const;

    /**
     * @brief Получает общее время включения, ч.
     * @return Общее время включения, ч.
     */
    unsigned int devLifetime() const;

    /**
     * @brief Получает общее время работы, ч.
     * @return Общее время работы, ч.
     */
    unsigned int devRuntime() const;

    /**
     * @brief Получает общее время работы вентилятора, ч.
     * @return Общее время работы, ч.
     */
    unsigned int devFanRuntime() const;

    /**
     * @brief Получает время работы после включения, ч.
     * @return Время работы после включения, ч.
     */
    unsigned int devLastRuntime() const;

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
     * @brief Получает список осциллограмм.
     * @return Список осциллограмм.
     */
    QList<drive_event_id_t> oscillogramsList() const;

    /**
     * @brief Получает количество осциллограмм.
     * @return Количество осциллограмм.
     */
    size_t oscillogramsCount() const;

    /**
     * @brief Получает осциллограмму с заданным индексом.
     * @param index Индекс осциллограммы.
     * @return Осциллограмма.
     */
    DriveOscillogram oscillogram(size_t index) const;

    /**
     * @brief Добавляет осциллограмму к осциллограммам привода.
     * @param osc Осциллограмма.
     */
    void addOscillogram(const DriveOscillogram& osc);

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

    /**
     * @brief Читает события.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @return Будущее.
     */
    Future* readEvents();

    /**
     * @brief Читает осциллограммы.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @return Будущее.
     */
    Future* readOscillograms();

    /**
     * @brief Читает осциллограммы.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @return Будущее.
     */
    Future* readSelectedOscillograms(QList<size_t> osc_list);

    /**
     * @brief Читает осциллограммы.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @return Будущее.
     */
    Future* readOscillogramsList();

    /**
     * @brief Очищает считанные осциллограммы.
     * Возвращаемое будущее должно быть удалено
     * посредством deleteLater.
     * @return Будущее.
     */
    Future*  clearReadedOscillograms();

    /**
     * @brief Получает строковое представление ошибки привода.
     * @param err Ошибка привода.
     * @return Строковое представление ошибки привода.
     */
    static QString errorToString(drive_error_t err);

    /**
     * @brief Получает строковые представления ошибок привода.
     * @param errs Ошибки привода.
     * @return Строковые представления ошибок привода.
     */
    static QStringList errorsToString(drive_errors_t errs);

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
     * @brief Сигнал ошибки привода.
     */
    void driveErrorOccured();

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
     * @brief Экстренно останавливает устройство.
     */
    void emergencyStop();

    /**
     * @brief Перезагружает привод.
     */
    void reboot();

    /**
     * @brief Устанавливает задание.
     * @param reference Задание.
     */
    void setReference(float reference);

    /**
     * @brief Очищает ошибки привода.
     */
    void clearErrors();

    /**
     * @brief Очищает список событий.
     */
    void clearEvents();

    /**
     * @brief Создаёт событие состояния.
     */
    void makeStatusEvent();

    /**
     * @brief Калибрует питание.
     */
    void calibratePower();

    /**
     * @brief Сохраняет параметры в ПЗУ.
     */
    void saveParams();

    /**
     * @brief Устанавливает дату и время.
     * @param datetime Дата и время.
     */
    void setDateTime(QDateTime datetime);

    /**
     * @brief Включает пользовательские цифровые выхода.
     */
    void doutUserOn();

    /**
     * @brief Выключает пользовательские цифровые выхода.
     */
    void doutUserOff();

    /**
     * @brief Переключает пользовательские цифровые выхода.
     */
    void doutUserToggle();

    /**
     * @brief Сбрасывает время работы вентилятора.
     */
    void resetFanRuntime();

    /**
     * @brief Запускает самонастройку.
     */
    void selftune();

//private signals:
    /**
     * @brief Читает следующий список параметров.
     */
    void readNextParams();

    /**
     * @brief Записывает следующий список параметров.
     */
    void writeNextParams();

    /**
     * @brief Читает события.
     */
    void doReadEvents(Future* future);

    /**
     * @brief Читает осциллограммы.
     */
    void doReadOscillograms(Future* future);

    /**
     * @brief Читает осциллограммы.
     */
    void doReadSelectedOscillograms(Future* future, QList<size_t> osc_list);

    /**
     * @brief Читает осциллограммы.
     */
    void doReadOscillogramsList(Future* future);

    /**
     * @brief Очищает прочитанные осциллограммы.
     */
    void doClearReadedOscillograms(Future* future);

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
