#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SettingsDlg;
class Drive;

namespace Ui {
class MainWindow;
}


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
     * @brief Обработчик изменения значения задания.
     * @param value Значение.
     */
    void on_sbReference_valueChanged(int value);

private:
    /**
     * @brief Обновляет настройки.
     */
    void setup();
    /**
     * @brief Графический интерфейс.
     */
    Ui::MainWindow *ui;
    /**
     * @brief Диалог настроек.
     */
    SettingsDlg* settingsDlg;
    /**
     * @brief Интерфейс взаимодействия с
     * устройством привода.
     */
    Drive* drive;
};

#endif // MAINWINDOW_H
