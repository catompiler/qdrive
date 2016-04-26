#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include <QStringList>
#include <QString>
#include <QCompleter>
#include "settings.h"


SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDlg)
{
    ui->setupUi(this);

    populatePortLists();
}

SettingsDlg::~SettingsDlg()
{
    delete ui;
}

void SettingsDlg::loadSettings()
{
    Settings& settings = Settings::get();

    ui->cbParity->setCurrentIndex(static_cast<int>(settings.parity()));
    ui->cbStopBits->setCurrentIndex(static_cast<int>(settings.stopBits()));

    int index = -1;

    QString baud_str = QString::number(settings.baud());
    index = ui->cbBaud->findText(baud_str);
    if(index != -1) ui->cbBaud->setCurrentIndex(index);
    else{
        ui->cbBaud->addItem(baud_str);
        ui->cbBaud->setCurrentIndex(ui->cbBaud->count() - 1);
    }

    QString dev_str = settings.ioDeviceName();
    index = ui->cbPortDev->findText(dev_str, Qt::MatchFixedString);
    if(index != -1) ui->cbPortDev->setCurrentIndex(index);
    else{
        ui->cbPortDev->addItem(dev_str);
        ui->cbPortDev->setCurrentIndex(ui->cbPortDev->count() - 1);
    }

    ui->sbDevAddr->setValue(settings.deviceAddress());
    ui->sbTimeOut->setValue(settings.deviceTimeout());
    ui->sbPeriod->setValue(settings.devicePeriod());
}

void SettingsDlg::storeSettings()
{
    Settings& settings = Settings::get();

    settings.setParity(static_cast<Settings::Parity>(ui->cbParity->currentIndex()));
    settings.setStopBits(static_cast<Settings::StopBits>(ui->cbStopBits->currentIndex()));
    settings.setBaud(ui->cbBaud->currentData().toUInt());
    settings.setIODeviceName(ui->cbPortDev->currentText());

    settings.setDeviceAddress(ui->sbDevAddr->value());
    settings.setDeviceTimeout(ui->sbTimeOut->value());
    settings.setDevicePeriod(ui->sbPeriod->value());
}

void SettingsDlg::populatePortLists()
{
    ui->cbParity->addItems(QStringList() << "Нет" << "Чётный" << "Нечётный");
    ui->cbStopBits->addItems(QStringList() << "1" << "2");

    unsigned int speed = 0;
    unsigned int mult = 1;
    unsigned int base = 1200;
    unsigned int old_mult = mult;

    const unsigned int speed_max = 2000000;

    while(speed < speed_max){
        speed = base * mult;
        ui->cbBaud->addItem(QString::number(speed), speed);
        if(mult == 1 || (mult & (mult - 1))){
            mult = old_mult * 2;
        }else{
            old_mult = mult;
            mult = mult + mult / 2;
        }
    }

#ifdef Q_OS_WIN

    const unsigned int com_port_first = 1;
    const unsigned int com_port_max = 100;

    unsigned int i;
    for(i = com_port_first; i <= 9; i ++){
        ui->cbPortDev->addItem(QString("COM%1").arg(i));
    }
    for(; i <= com_port_max; i ++){
        ui->cbPortDev->addItem(QString("\\\\.\\COM%1").arg(i));
    }
#else

    const unsigned int com_port_first = 0;
    const unsigned int com_port_max = 99;

    unsigned int i;
    for(i = com_port_first; i <= com_port_max; i ++){
        ui->cbPortDev->addItem(QString("/dev/ttyS%1").arg(i));
    }
    for(i = com_port_first; i <= com_port_max; i ++){
        ui->cbPortDev->addItem(QString("/dev/ttyUSB%1").arg(i));
    }
    for(i = com_port_first; i <= com_port_max; i ++){
        ui->cbPortDev->addItem(QString("/dev/ttyACM%1").arg(i));
    }
#endif

    QCompleter* completer = ui->cbPortDev->completer();
    if(completer != nullptr){
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setModelSorting(QCompleter::UnsortedModel);
        completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    }
}
