#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "settingsdlg.h"
#include "drive.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsDlg = nullptr;

    drive = new Drive(this);
    connect(drive, &Drive::errorOccured, this, &MainWindow::errorOccured);
    connect(drive, &Drive::information, this, &MainWindow::information);
    connect(drive, &Drive::connected, this, &MainWindow::connected);
    connect(drive, &Drive::disconnected, this, &MainWindow::disconnected);
    connect(drive, &Drive::updated, this, &MainWindow::updated);

    setup();

    refreshUi();
}

MainWindow::~MainWindow()
{
    delete drive;
    delete settingsDlg;
    delete ui;
}

void MainWindow::refreshUi()
{
    bool good = drive->good();
    bool connected = drive->connectedToDevice();
    bool running = drive->running();

    ui->actSettings->setEnabled(!connected);
    ui->actConnect->setEnabled(good & !connected);
    ui->actDisconnect->setEnabled(connected);
    ui->pbStart->setEnabled(connected && !running);
    ui->pbStop->setEnabled(connected && running);
    ui->hsReference->setEnabled(connected);
    ui->sbReference->setEnabled(connected);
}

void MainWindow::connected()
{
    refreshUi();
}

void MainWindow::disconnected()
{
    refreshUi();
}

void MainWindow::updated()
{
    bool running = drive->running();

    ui->sbReference->blockSignals(true);

    ui->pbStart->setEnabled(!running);
    ui->pbStop->setEnabled(running);
    ui->hsReference->setValue(drive->reference());

    ui->sbReference->blockSignals(false);

    ui->lblUa->setText(QString::number(drive->powerUa(), 'f', 2));
    ui->lblUb->setText(QString::number(drive->powerUb(), 'f', 2));
    ui->lblUc->setText(QString::number(drive->powerUc(), 'f', 2));
}

void MainWindow::errorOccured(const QString &error_text)
{
    QMessageBox::critical(this, tr("Ошибка!"), error_text);
    refreshUi();
}

void MainWindow::information(const QString &info_text)
{
    statusBar()->showMessage(info_text, 5000);
}

void MainWindow::on_actQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actSettings_triggered()
{
    if(settingsDlg == nullptr) settingsDlg = new SettingsDlg(this);

    settingsDlg->loadSettings();

    if(settingsDlg->exec()){
        settingsDlg->storeSettings();

        setup();

        refreshUi();
    }
}

void MainWindow::on_actConnect_triggered()
{
    ui->actConnect->setEnabled(false);
    drive->connectToDevice();
}

void MainWindow::on_actDisconnect_triggered()
{
    ui->actDisconnect->setEnabled(false);
    drive->disconnectFromDevice();
}

void MainWindow::on_pbStart_clicked()
{
    ui->pbStart->setEnabled(false);
    drive->start();
}

void MainWindow::on_pbStop_clicked()
{
    ui->pbStop->setEnabled(false);
    drive->stop();
}

void MainWindow::on_sbReference_valueChanged(int value)
{
    drive->setReference(value);
}

void MainWindow::setup()
{
    drive->setup();
}
