#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "settingsdlg.h"
#include "selectoscsdlg.h"
#include "drive.h"
#include "parameter.h"
#include "paramview.h"
#include "paramsmodel.h"
#include "paramsdelegate.h"
#include "eventsmodel.h"
#include "eventmodel.h"
#include "future.h"
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QProgressDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLayoutItem>
#include <QString>
#include <QStringList>
#include <QItemSelectionModel>
#include <QFileInfo>
#include <QSettings>
#include <QColor>
#include <QDebug>


struct ParamItem {
    QString name;
    param_id_t id;
    QColor color;
};

static ParamItem default_params[] = {
    {"Ua", PARAM_ID_POWER_U_A, Qt::yellow},
    {"Ia", PARAM_ID_POWER_I_A, Qt::yellow},
    {"Ub", PARAM_ID_POWER_U_B, Qt::green},
    {"Ib", PARAM_ID_POWER_I_B, Qt::green},
    {"Uc", PARAM_ID_POWER_U_C, Qt::red},
    {"Ic", PARAM_ID_POWER_I_C, Qt::red},
    {"Urot", PARAM_ID_POWER_U_ROT, Qt::blue},
    {"Irot", PARAM_ID_POWER_I_ROT, Qt::blue},
    {"Iexc", PARAM_ID_POWER_I_EXC, Qt::darkMagenta},
    {"Ifan", PARAM_ID_POWER_I_FAN, Qt::darkCyan},
    {"PIDspd", PARAM_ID_PID_ROT_SPEED, Qt::black},
    {"PIDrot", PARAM_ID_PID_ROT_CURRENT, Qt::black},
    {"PIDexc", PARAM_ID_PID_EXC_CURRENT, Qt::black},
    {"Calc Urot", PARAM_ID_POWER_CALC_U_ROT, Qt::blue},
    {"RPM", PARAM_ID_MOTOR_RPM, Qt::darkBlue},
    {"Torque", PARAM_ID_MOTOR_TORQUE, Qt::darkBlue},
    {"Temperature", PARAM_ID_HEATSINK_TEMP, Qt::darkGreen},
    {"Erot", PARAM_ID_MOTOR_E, Qt::blue},
    {"PCA resets", PARAM_ID_DEBUG_2, Qt::black},
    {"LM75 resets", PARAM_ID_DEBUG_3, Qt::black},
    //{"I_c_zero", PARAM_ID_DEBUG_8, Qt::black},
    //{"Rrot", PARAM_ID_MOTOR_R_ROT, Qt::darkYellow},
    //{"Eff", PARAM_ID_MOTOR_EFF, Qt::darkYellow},
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

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/qdrive.png"));

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

    paramsDelegate = new ParamsDelegate();
    ui->tvParams->setItemDelegate(paramsDelegate);

    eventsModel = new EventsModel();
    ui->lvEvents->setModel(eventsModel);
    connect(ui->lvEvents->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &MainWindow::lvEvents_currentChanged);

    eventModel = new EventModel();
    ui->tvEvent->setModel(eventModel);
    ui->tvEvent->setColumnWidth(0, TREEVIEW_EVENT_COL_NAME_WIDTH);

    settingsDlg = nullptr;

    selectOscsDlg = nullptr;

    drive = new Drive(this);
    connect(drive, &Drive::errorOccured, this, &MainWindow::errorOccured);
    connect(drive, &Drive::information, this, &MainWindow::information);
    connect(drive, &Drive::connected, this, &MainWindow::connected);
    connect(drive, &Drive::disconnected, this, &MainWindow::disconnected);
    connect(drive, &Drive::updated, this, &MainWindow::updated);
    connect(drive, &Drive::driveErrorOccured, this, &MainWindow::driveErrorOccured);

    setup();

    refreshViewedParams();

    refreshUi();

    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete drive;
    delete eventModel;
    delete eventsModel;
    delete paramsDelegate;
    delete paramsModel;
    delete selectOscsDlg;
    delete settingsDlg;
    delete glMain;
    delete trayIcon;
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
    ui->tbEmStop->setEnabled(connected);
    ui->pbReboot->setEnabled(connected);
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
    ui->pbReadAllOscs->setEnabled(connected);
    ui->pbReadOscs->setEnabled(connected);
    ui->pbDoutUserOn->setEnabled(connected);
    ui->pbDoutUserOff->setEnabled(connected);
    ui->pbDoutUserToggle->setEnabled(connected);
    ui->tbtnResetFanRunTime->setEnabled(connected);
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
    bool connected = drive->connectedToDevice();
    bool running = drive->running();

    ui->pbCalibrate->setEnabled(connected && !running);

    ui->sbReference->blockSignals(true);

    ui->pbStart->setEnabled(!running);
    ui->pbStop->setEnabled(running);
    ui->hsReference->setValue(drive->reference());

    ui->sbReference->blockSignals(false);

    ui->lblLifeTimeVal->setText(QString::number(drive->devLifetime()));
    ui->lblRunTimeVal->setText(QString::number(drive->devRuntime()));
    ui->lblFanRunTimeVal->setText(QString::number(drive->devFanRuntime()));
    ui->lblLastRunTimeVal->setText(QString::number(drive->devLastRuntime()));

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

void MainWindow::driveErrorOccured()
{
    trayIcon->showMessage(tr("QDrive"),
                          tr("Ошибка привода 0x%1").arg(drive->errors(), 0, 16),
                          QSystemTrayIcon::Critical);
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

void MainWindow::on_tbEmStop_clicked()
{
    drive->emergencyStop();
}

void MainWindow::on_pbReboot_clicked()
{
    drive->reboot();
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

    auto params = paramsModel->getRealParamsHash();

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

    auto params = paramsModel->getRealParamsHash();

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
    QList<Parameter*> params = paramsModel->getRealParamsList();

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

void MainWindow::on_pbMakeStatusEvent_clicked()
{
    drive->makeStatusEvent();
}

void MainWindow::on_pbSetTime_clicked()
{
    drive->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::on_pbReadAllOscs_clicked()
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
        refreshOscsList(ui->cbOscs->count());
    });

    progress->show();
}

void MainWindow::on_pbReadOscs_clicked()
{
    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, 0, this);
    progress->setWindowTitle(tr("Чтение осциллограмм"));
    progress->setModal(true);
    Future* future = drive->readOscillogramsList();

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
    connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
    connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
    connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);
    connect(future, &Future::finished, this, &MainWindow::selectAndReadOscs);

    progress->show();
}

void MainWindow::on_pbSaveOsc_clicked()
{
    if(ui->cbOscs->currentIndex() == -1 ||
       ui->cbOscs->currentIndex() >= static_cast<int>(drive->oscillogramsCount())){
        QMessageBox::critical(this, tr("Сохранение осциллограммы"), tr("Не выбрана осциллограмма для сохранения"));
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение осциллограммы"), Settings::get().lastPath(),
                                                    tr("Осциллограммы (*.osc);;CSV (*.csv)"));

    if(filename.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filename).path());

    DriveOscillogram osc = drive->oscillogram(static_cast<size_t>(ui->cbOscs->currentIndex()));

    QString ext = filename.right(4).toLower();

    if(ext == ".osc"){
        if(!osc.save(filename)){
            QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка сохранения файла осциллограммы!"));
        }
    }else if(ext == ".csv"){
        if(!osc.saveCsv(filename)){
            QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка сохранения файла csv!"));
        }
    }else{
        QMessageBox::critical(this, tr("Ошибка"), tr("Неизвестный формат осциллограммы!"));
    }
}

void MainWindow::on_pbReadOsc_clicked()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Загрузка осциллограмм"), Settings::get().lastPath(), tr("Осциллограммы (*.osc)"));

    if(filenames.isEmpty()) return;

    Settings::get().setLastPath(QFileInfo(filenames.first()).path());

    QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                    0, 0, this);
    progress->setWindowTitle(tr("Чтение файлов осциллограмм"));
    progress->setModal(true);
    progress->setRange(0, filenames.size());
    progress->show();

    for(int i = 0; i < filenames.size(); i ++){

        progress->setValue(i);

        QApplication::processEvents();

        if(progress->wasCanceled()) break;

        const QString& filename = filenames.at(i);

        DriveOscillogram osc;

        if(!osc.load(filename)){
            QMessageBox::critical(this, tr("Ошибка"), tr("Ошибка загрузки файла %1!").arg(filename));
        }else{
            drive->addOscillogram(osc);
            refreshOscsList(static_cast<int>(drive->oscillogramsCount()) - 1);
        }
    }

    progress->close();
}

void MainWindow::on_tbClearOscs_clicked()
{
    Future* future = drive->clearReadedOscillograms();

    connect(future, &Future::finished, future, &Future::deleteLater);
    connect(future, &Future::finished, this, [this](){
        refreshOscsList();
    });
}

void MainWindow::on_pbDoutUserOn_clicked()
{
    drive->doutUserOn();
}

void MainWindow::on_pbDoutUserOff_clicked()
{
    drive->doutUserOff();
}

void MainWindow::on_pbDoutUserToggle_clicked()
{
    drive->doutUserToggle();
}

void MainWindow::on_tbtnResetFanRunTime_clicked()
{
    drive->resetFanRuntime();
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
        Qt::darkMagenta
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
        param = Parameter::byId(paramItem.id);

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

void MainWindow::selectAndReadOscs()
{
    auto oscs_list = drive->oscillogramsList();

    if(selectOscsDlg == nullptr){
        selectOscsDlg = new SelectOscsDlg(this);
    }

    selectOscsDlg->setOscillograms(drive->oscillogramsList());

    if(selectOscsDlg->exec()){
        auto read_list = selectOscsDlg->selectedOscillograms();

        if(!read_list.empty()){
            QProgressDialog* progress = new QProgressDialog(tr("Подождите..."), tr("Прервать"),
                                                            0, 0, this);
            progress->setWindowTitle(tr("Чтение выбранных осциллограмм"));
            progress->setModal(true);
            Future* future = drive->readSelectedOscillograms(read_list);

            connect(future, &Future::finished, future, &Future::deleteLater);
            connect(future, &Future::finished, progress, &QProgressDialog::deleteLater);
            connect(future, &Future::progressRangeChanged, progress, &QProgressDialog::setRange);
            connect(future, &Future::progressChanged, progress, &QProgressDialog::setValue);
            connect(progress, &QProgressDialog::canceled, future, &Future::cancel, Qt::DirectConnection);
            connect(future, &Future::finished, future, [this](){
                refreshOscsList(ui->cbOscs->count());
            });

            progress->show();
        }
    }
}
