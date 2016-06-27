#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include "settingsdlg.h"
#include "drive.h"
#include "parameter.h"
#include "paramview.h"
#include "paramsmodel.h"
#include "future.h"
#include <QApplication>
#include <QMessageBox>
#include <QProgressDialog>
#include <QGridLayout>
#include <QLayoutItem>
#include <QString>
#include <QDebug>


struct ParamItem {
    QString name;
    param_id_t id;
    param_type_t type;
};

static ParamItem default_params[] = {
    {"Ua", PARAM_ID_POWER_U_A, PARAM_TYPE_FRACT_100},
    {"Ia", PARAM_ID_POWER_I_A, PARAM_TYPE_FRACT_100},
    {"Ub", PARAM_ID_POWER_U_B, PARAM_TYPE_FRACT_100},
    {"Ib", PARAM_ID_POWER_I_B, PARAM_TYPE_FRACT_100},
    {"Uc", PARAM_ID_POWER_U_C, PARAM_TYPE_FRACT_100},
    {"Ic", PARAM_ID_POWER_I_C, PARAM_TYPE_FRACT_100},
    {"Urot", PARAM_ID_POWER_U_ROT, PARAM_TYPE_FRACT_10},
    {"Irot", PARAM_ID_POWER_I_ROT, PARAM_TYPE_FRACT_10},
    {"Iexc", PARAM_ID_POWER_I_EXC, PARAM_TYPE_FRACT_1000}
};

#define PARAM_ITEMS_COLS 2

#define TREEVIEW_PARAMS_COL_NAME_WIDTH 250
#define TREEVIEW_PARAMS_EXPANDED


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
    ui->pbReadParams->setEnabled(connected);
    ui->pbWriteParams->setEnabled(connected);
    ui->pbSaveParams->setEnabled(connected);
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

void MainWindow::setup()
{
    drive->setup();
}

void MainWindow::refreshViewedParams()
{
    QWidget* widget;
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

        glMain->addWidget(paramView, n_widget / PARAM_ITEMS_COLS, n_widget % PARAM_ITEMS_COLS);

        n_widget ++;
    }
}
