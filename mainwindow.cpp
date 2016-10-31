#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "settingsdlg.h"
#include "drive.h"
#include "parameter.h"
#include "paramview.h"
#include "paramsmodel.h"
#include "eventsmodel.h"
#include "eventmodel.h"
#include "future.h"
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLayoutItem>
#include <QString>
#include <QItemSelectionModel>
#include <QFileInfo>
#include <QSettings>
#include <QColor>
#include <QDebug>


struct ParamItem {
    QString name;
    param_id_t id;
    param_type_t type;
    QColor color;
};

static ParamItem default_params[] = {
    {"Ua", PARAM_ID_POWER_U_A, PARAM_TYPE_FRACT_100, Qt::yellow},
    {"Ia", PARAM_ID_POWER_I_A, PARAM_TYPE_FRACT_100, Qt::yellow},
    {"Ub", PARAM_ID_POWER_U_B, PARAM_TYPE_FRACT_100, Qt::green},
    {"Ib", PARAM_ID_POWER_I_B, PARAM_TYPE_FRACT_100, Qt::green},
    {"Uc", PARAM_ID_POWER_U_C, PARAM_TYPE_FRACT_100, Qt::red},
    {"Ic", PARAM_ID_POWER_I_C, PARAM_TYPE_FRACT_100, Qt::red},
    {"Urot", PARAM_ID_POWER_U_ROT, PARAM_TYPE_FRACT_10, Qt::blue},
    {"Irot", PARAM_ID_POWER_I_ROT, PARAM_TYPE_FRACT_10, Qt::blue},
    {"Iexc", PARAM_ID_POWER_I_EXC, PARAM_TYPE_FRACT_1000, Qt::magenta},
    //{"PIDexc", PARAM_ID_DEBUG_6, PARAM_TYPE_FRACT_100, Qt::black}
};

#define PARAM_ITEMS_COLS 2

#define TREEVIEW_PARAMS_COL_NAME_WIDTH 250
#define TREEVIEW_PARAMS_EXPANDED

#define TREEVIEW_EVENT_COL_NAME_WIDTH 150


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    glMain = new QGridLayout();
    ui->tabSensors->setLayout(glMain);

    paramsModel = new ParamsModel();
    ui->tvParams->setModel(paramsModel);
#ifdef TREEVIEW_PARAMS_COL_NAME_WIDTH
    ui->tvParams->setColumnWidth(0, TREEVIEW_PARAMS_COL_NAME_WIDTH);
#endif
#ifdef TREEVIEW_PARAMS_EXPANDED
    ui->tvParams->expandAll();
#endif

    eventsModel = new EventsModel();
    ui->lvEvents->setModel(eventsModel);
    connect(ui->lvEvents->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::lvEvents_currentChanged);

    eventModel = new EventModel();
    ui->tvEvent->setModel(eventModel);
    ui->tvEvent->setColumnWidth(0, TREEVIEW_EVENT_COL_NAME_WIDTH);

    settingsDlg = nullptr;

    drive = new Drive(this);
    connect(drive, &Drive::errorOccured, this, &MainWindow::errorOccured);
    connect(drive, &Drive::information, this, &MainWindow::information);
    connect(drive, &Drive::connected, this, &MainWindow::connected);
    connect(drive, &Drive::disconnected, this, &MainWindow::disconnected);
    connect(drive, &Drive::updated, this, &MainWindow::updated);

    setup();

    refreshViewedParams();

    refreshUi();
}

MainWindow::~MainWindow()
{
    delete drive;
    delete eventModel;
    delete eventsModel;
    delete paramsModel;
    delete settingsDlg;
    delete glMain;
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
    ui->pbClearErrs->setEnabled(connected);
    ui->pbClearEvents->setEnabled(connected);
    ui->pbCalibrate->setEnabled(connected && !running);
    ui->pbReadParams->setEnabled(connected);
    ui->pbWriteParams->setEnabled(connected);
    ui->pbSaveParams->setEnabled(connected);
    ui->pbSetTime->setEnabled(connected);
    ui->pbReadEvents->setEnabled(connected);
    ui->pbReadOscs->setEnabled(connected);
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

    ui->lblErrsVal->setText(QString("0x%1").arg(drive->errors(), 0, 16));
    ui->lblPhErrsVal->setText(QString("0x%1").arg(drive->phaseErrors(), 0, 16));
    ui->lblPwrErrsVal->setText(QString("0x%1").arg(drive->powerErrors(), 0, 16));

    ui->lblWarnsVal->setText(QString("0x%1").arg(drive->warnings(), 0, 16));
    ui->lblPwrWarnsVal->setText(QString("0x%1").arg(drive->powerWarnings(), 0, 16));
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

void MainWindow::on_pbClearErrs_clicked()
{
    drive->clearErrors();
}

void MainWindow::on_pbCalibrate_clicked()
{
    drive->calibratePower();
}

void MainWindow::on_pbImportParams_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Импорт параметров"), Settings::get().lastPath(), tr("Файлы параметров (*.ini)"));

    if(filename.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filename).path());

    auto params = paramsModel->getParamsHash();

    QSettings settings(filename, QSettings::IniFormat);

    settings.beginGroup("Drive_parameters");

    QStringList keys = settings.allKeys();

    for(QString& str_key: keys){
        bool is_ok = false;

        param_id_t id = static_cast<param_id_t>(str_key.toUInt(&is_ok));
        if(!is_ok) continue;

        if(!params.contains(id)) continue;

        uint16_t value = static_cast<uint16_t>(settings.value(str_key).toUInt(&is_ok));
        if(!is_ok) continue;

        params[id]->setRaw(value);
    }

    settings.endGroup();

    paramsModel->paramsUpdated();
}

void MainWindow::on_pbExportParams_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Экспорт параметров"), Settings::get().lastPath(), tr("Файлы параметров (*.ini)"));

    if(filename.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filename).path());

    auto params = paramsModel->getParamsHash();

    QSettings settings(filename, QSettings::IniFormat);

    settings.clear();

    settings.beginGroup("Drive_parameters");

    for(auto it = params.begin(); it != params.end(); ++ it){
        settings.setValue(QString::number(static_cast<unsigned int>(it.key())),
                          static_cast<unsigned int>(it.value()->toRaw()));
    }

    settings.endGroup();
}

void MainWindow::on_pbDefaultParams_clicked()
{
    paramsModel->applyDefault();
}

void MainWindow::on_pbReadParams_clicked()
{
    QList<Parameter*> params = paramsModel->getParamsList();

    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, params.size(), this);
    progress->setWindowTitle(tr("Чтение параметров"));
    progress->setModal(true);
    Future* future = drive->readParams(params);

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
    connect(future, &Future::finished, paramsModel, &ParamsModel::paramsUpdated);
    connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
    connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
    connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);

    progress->show();
}

void MainWindow::on_pbWriteParams_clicked()
{
    QList<Parameter*> params = paramsModel->getParamsList();

    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, params.size(), this);
    progress->setWindowTitle(tr("Запись параметров"));
    progress->setModal(true);
    Future* future = drive->writeParams(params);

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
    connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
    connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
    connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);

    progress->show();
}

void MainWindow::on_pbSaveParams_clicked()
{
    drive->saveParams();
}

void MainWindow::on_pbReadEvents_clicked()
{
    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, 0, this);
    progress->setWindowTitle(tr("Чтение событий"));
    progress->setModal(true);
    Future* future = drive->readEvents();

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
    connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
    connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
    connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);
    connect(future, &Future::finished, future, [this](){
        eventsModel->setEvents(drive->events());
    });

    progress->show();
}

void MainWindow::on_pbClearEvents_clicked()
{
    drive->clearEvents();
}

void MainWindow::on_pbSetTime_clicked()
{
    drive->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::on_pbReadOscs_clicked()
{
    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, 0, this);
    progress->setWindowTitle(tr("Чтение осциллограмм"));
    progress->setModal(true);
    Future* future = drive->readOscillograms();

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
    connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
    connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
    connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);
    connect(future, &Future::finished, future, [this](){
        refreshOscsList(0);
    });

    progress->show();
}

void MainWindow::on_pbSaveOsc_clicked()
{
    if(ui->cbOscs->currentIndex() == -1 ||
       ui->cbOscs->currentIndex() >= static_cast<int>(drive->oscillogramsCount())){
        QMessageBox::critical(this, tr("Сохранение осциллограммы"), tr("Не выбрана осциллограмма для сохранения"));
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение осциллограммы"), Settings::get().lastPath(), tr("Осциллограммы (*.osc)"));

    if(filename.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filename).path());

    DriveOscillogram osc = drive->oscillogram(static_cast<size_t>(ui->cbOscs->currentIndex()));

    if(!osc.save(filename)){
        QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка сохранения файла!"));
    }
}

void MainWindow::on_pbReadOsc_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Загрузка осциллограммы"), Settings::get().lastPath(), tr("Осциллограммы (*.osc)"));

    if(filename.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filename).path());

    DriveOscillogram osc;

    if(!osc.load(filename)){
        QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка загрузки файла!"));
    }else{
        drive->addOscillogram(osc);
        refreshOscsList(static_cast<int>(drive->oscillogramsCount()) - 1);
    }
}

void MainWindow::lvEvents_currentChanged(const QModelIndex &current, const QModelIndex &/*previous*/)
{
    eventModel->setEvent(eventsModel->eventByIndex(current));
}

void MainWindow::on_cbOscs_currentIndexChanged(int index)
{
    ui->oscVoltages->clearWaveforms();
    ui->oscCurrents->clearWaveforms();

    if(index < 0){
        return;
    }

    auto oscs = drive->oscillograms();

    if(index >= oscs.size()) return;

    auto osc = oscs[index];

    static const size_t voltage_channels[] = {
        DRIVE_POWER_OSC_CHANNEL_Ua,
        DRIVE_POWER_OSC_CHANNEL_Ub,
        DRIVE_POWER_OSC_CHANNEL_Uc,
        DRIVE_POWER_OSC_CHANNEL_Urot
    };

    static const Qt::GlobalColor osc_colors[] = {
        Qt::yellow,
        Qt::green,
        Qt::red,
        Qt::blue,
        Qt::magenta
    };

    static const size_t current_channels[] = {
        DRIVE_POWER_OSC_CHANNEL_Ia,
        DRIVE_POWER_OSC_CHANNEL_Ib,
        DRIVE_POWER_OSC_CHANNEL_Ic,
        DRIVE_POWER_OSC_CHANNEL_Irot,
        DRIVE_POWER_OSC_CHANNEL_Iexc
    };

    size_t color_index = 0;
    for(const size_t& channel: voltage_channels){
        if(channel < osc.channelsCount()){
            OscView::Waveform* wf = new OscView::Waveform(osc.channel(channel));
            ui->oscVoltages->setWaveform(static_cast<int>(channel), wf, osc_colors[color_index ++]);
        }
    }
    color_index = 0;
    for(const size_t& channel: current_channels){
        if(channel < osc.channelsCount()){
            OscView::Waveform* wf = new OscView::Waveform(osc.channel(channel));
            ui->oscCurrents->setWaveform(static_cast<int>(channel), wf, osc_colors[color_index ++]);
        }
    }

    ui->oscVoltages->adjustView();
    ui->oscCurrents->adjustView();
}

void MainWindow::closeEvent(QCloseEvent* /*event*/)
{
    qApp->quit();
}

void MainWindow::setup()
{
    drive->setup();
}

void MainWindow::refreshViewedParams()
{
    QWidget* widget = nullptr;
    QLayoutItem* item = nullptr;
    ParamView* paramView = nullptr;
    Parameter* param = nullptr;

    while((item = glMain->takeAt(0)) != nullptr){
        widget = item->widget();
        if(widget){
            paramView = qobject_cast<ParamView*>(widget);
            if(paramView) delete paramView->parameter();
        }
        delete item;
    }

    size_t n_widget = 0;
    for(ParamItem& paramItem : default_params){
        paramView = new ParamView();
        param = new Parameter(paramItem.type, paramItem.id);

        drive->addUpdParam(param);
        connect(drive, &Drive::updated, paramView, &ParamView::updated);

        paramView->viewParam(paramItem.name, param);

        paramView->setColor(paramItem.color);

        glMain->addWidget(paramView, n_widget / PARAM_ITEMS_COLS, n_widget % PARAM_ITEMS_COLS);

        n_widget ++;
    }
}

void MainWindow::refreshOscsList(int set_index)
{
    ui->cbOscs->clear();
    auto oscs = drive->oscillograms();
    if(!oscs.empty()){
        for(auto it = oscs.begin(); it != oscs.end(); ++ it){
            ui->cbOscs->addItem(tr("Событие %1").arg(it->eventId()));
        }
        if(set_index >= 0){
            if(set_index >= ui->cbOscs->count()){
                set_index = ui->cbOscs->count() - 1;
            }
            ui->cbOscs->setCurrentIndex(set_index);
        }
    }
}
