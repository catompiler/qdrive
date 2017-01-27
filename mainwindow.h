#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class QCloseEvent;

class SettingsDlg;
class SelectOscsDlg;
class Drive;
class ParamsModel;
class ParamsDelegate;
class EventsModel;
class EventModel;

namespace Ui {
class MainWindow;
}

class QGridLayout;
class QSystemTrayIcon;


/**
 * @brief Класс главного окна приложения.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param parent Родитель.
     */
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /**
     * @brief Обновляет интерфейс приложения.
     */
    void refreshUi();

    /**
     * @brief Обработчик подключения к устройству.
     */
    void connected();

    /**
     * @brief Обработчик отключения от устройства.
     */
    void disconnected();

    /**
     * @brief Обработчик обновления данных.
     */
    void updated();

    /**
     * @brief Обработчик возникновения ошибки.
     * @param error_text Текст ошибки.
     */
    void errorOccured(const QString& error_text);

    /**
     * @brief Обработчик возникновения ошибки привода.
     */
    void driveErrorOccured();

    /**
     * @brief Обработчик информации о событиях.
     * @param info_text Информация о событии.
     */
    void information(const QString& info_text);

    /**
     * @brief Обработчик действия выхода.
     */
    void on_actQuit_triggered();

    /**
     * @brief Обработчик действия вызова настроек.
     */
    void on_actSettings_triggered();

    /**
     * @brief Обработчик действия соединения.
     */
    void on_actConnect_triggered();

    /**
     * @brief Обработчик действия разъединения.
     */
    void on_actDisconnect_triggered();

    /**
     * @brief Обработчик кнопки запуска.
     */
    void on_pbStart_clicked();

    /**
     * @brief Обработчик кнопки останова.
     */
    void on_pbStop_clicked();

    /**
     * @brief Обработчик кнопки останова экстренного останова.
     */
    void on_tbEmStop_clicked();

    /**
     * @brief Обработчик изменения значения задания.
     * @param value Значение.
     */
    void on_sbReference_valueChanged(int value);

    /**
     * @brief Обработчик нажатия кнопки сброса ошибок.
     */
    void on_pbClearErrs_clicked();

    /**
     * @brief Обработчик нажатия кнопки калибровки питания.
     */
    void on_pbCalibrate_clicked();

    /**
     * @brief Обработчик нажатия кнопки импорта параметров.
     */
    void on_pbImportParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки экспорта параметров.
     */
    void on_pbExportParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки параметров по-умолчанию.
     */
    void on_pbDefaultParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки чтения параметров.
     */
    void on_pbReadParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки записи параметров.
     */
    void on_pbWriteParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки сохранения параметров.
     */
    void on_pbSaveParams_clicked();

    /**
     * @brief Обработчик нажатия кнопки чтения событий.
     */
    void on_pbReadEvents_clicked();

    /**
     * @brief Обработчик нажатия кнопки очистки событий.
     */
    void on_pbClearEvents_clicked();

    /**
     * @brief Обработчик нажатия кнопки сохранения состояния.
     */
    void on_pbMakeStatusEvent_clicked();

    /**
     * @brief Обработчик нажатия кнопки установки времени.
     */
    void on_pbSetTime_clicked();

    /**
     * @brief Обработчик нажатия кнопки чтения всех осциллограмм.
     */
    void on_pbReadAllOscs_clicked();

    /**
     * @brief Обработчик нажатия кнопки чтения осциллограмм.
     */
    void on_pbReadOscs_clicked();

    /**
     * @brief Обработчик нажатия кнопки сохранения осциллограммы.
     */
    void on_pbSaveOsc_clicked();

    /**
     * @brief Обработчик нажатия кнопки чтения осциллограммы.
     */
    void on_pbReadOsc_clicked();

    /**
     * @brief Обработчик нажатия кнопки включения пользовательских цифровых выходов.
     */
    void on_pbDoutUserOn_clicked();

    /**
     * @brief Обработчик нажатия кнопки выключения пользовательских цифровых выходов.
     */
    void on_pbDoutUserOff_clicked();

    /**
     * @brief Обработчик нажатия кнопки переключения пользовательских цифровых выходов.
     */
    void on_pbDoutUserToggle_clicked();

    /**
     * @brief Обработчик выделения события.
     * @param current Текущий индекс.
     * @param previous Предыдущий индекс.
     */
    void lvEvents_currentChanged(const QModelIndex& current, const QModelIndex& previous);

    /**
     * @brief Обработчик выбора осциллограмм для отображения.
     * @param index Индекс осциллограммы.
     */
    void on_cbOscs_currentIndexChanged(int index);

    /**
     * @brief Выбирает и читает осциллограммы.
     */
    void selectAndReadOscs();

protected:
    /**
     * @brief Переопределённая функция
     * обработки события закрытия окна.
     * @param event Событие.
     */
    void closeEvent(QCloseEvent* event);

private:
    /**
     * @brief Обновляет настройки.
     */
    void setup();
    /**
     * @brief Обновляет список просматриваемых параметров.
     */
    void refreshViewedParams();
    /**
     * @brief Обновляет список осциллограмм в списке.
     */
    void refreshOscsList(int set_index = -1);
    /**
     * @brief Графический интерфейс.
     */
    Ui::MainWindow *ui;
    /**
     * @brief Иконка в трее.
     */
    QSystemTrayIcon* trayIcon;
    /**
     * @brief Диалог настроек.
     */
    SettingsDlg* settingsDlg;
    /**
     * @brief Диалог выбора осциллограмм.
     */
    SelectOscsDlg* selectOscsDlg;
    /**
     * @brief Интерфейс взаимодействия с
     * устройством привода.
     */
    Drive* drive;
    /**
     * @brief Разметка виджетов параметров.
     */
    QGridLayout* glMain;
    /**
     * @brief Модель списка параметров.
     */
    ParamsModel* paramsModel;
    /**
     * @brief Делегат списка параметров.
     */
    ParamsDelegate* paramsDelegate;
    /**
     * @brief Модель списка событий.
     */
    EventsModel* eventsModel;
    /**
     * @brief Модель события.
     */
    EventModel* eventModel;
};

#endif // MAINWINDOW_H
