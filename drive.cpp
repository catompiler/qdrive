#include "drive.h"
#include "driveworker.h"
#include "settings.h"
#include "future.h"
#include <QDebug>

#define WORKER_THREAD_WAIT_MAX 1000000UL


Drive::Drive(QObject *parent) : QObject(parent)
{
    worker = new DriveWorker();
    connect(worker, &DriveWorker::errorOccured, this, &Drive::errorOccured);
    connect(worker, &DriveWorker::information, this, &Drive::information);
    connect(worker, &DriveWorker::connected, this, &Drive::connected);
    connect(worker, &DriveWorker::disconnected, this, &Drive::disconnected);
    connect(worker, &DriveWorker::updated, this, &Drive::updated);
    connect(worker, &DriveWorker::driveErrorOccured, this, &Drive::driveErrorOccured);
    connect(this, &Drive::connectToDevice, worker, &DriveWorker::connectToDevice);
    connect(this, &Drive::disconnectFromDevice, worker, &DriveWorker::disconnectFromDevice);
    connect(this, &Drive::setRunning, worker, &DriveWorker::setRunning);
    connect(this, &Drive::start, worker, &DriveWorker::startDrive);
    connect(this, &Drive::stop, worker, &DriveWorker::stopDrive);
    connect(this, &Drive::emergencyStop, worker, &DriveWorker::emergencyStopDrive);
    connect(this, &Drive::reboot, worker, &DriveWorker::rebootDrive);
    connect(this, &Drive::setReference, worker, &DriveWorker::setReference);
    connect(this, &Drive::clearErrors, worker, &DriveWorker::clearErrors);
    connect(this, &Drive::clearEvents, worker, &DriveWorker::clearEvents);
    connect(this, &Drive::makeStatusEvent, worker, &DriveWorker::makeStatusEvent);
    connect(this, &Drive::calibratePower, worker, &DriveWorker::calibratePower);
    connect(this, &Drive::saveParams, worker, &DriveWorker::saveParams);
    connect(this, &Drive::setDateTime, worker, &DriveWorker::setDateTime);
    connect(this, &Drive::doutUserOn, worker, &DriveWorker::doutUserOn);
    connect(this, &Drive::doutUserOff, worker, &DriveWorker::doutUserOff);
    connect(this, &Drive::doutUserToggle, worker, &DriveWorker::doutUserToggle);
    connect(this, &Drive::resetFanRuntime, worker, &DriveWorker::resetFanRuntime);
    connect(this, &Drive::selftune, worker, &DriveWorker::selftune);
    connect(this, &Drive::readNextParams, worker, &DriveWorker::readNextParams);
    connect(this, &Drive::writeNextParams, worker, &DriveWorker::writeNextParams);
    connect(this, &Drive::doReadEvents, worker, &DriveWorker::readEvents);
    connect(this, &Drive::doReadOscillograms, worker, &DriveWorker::readOscillograms);
    connect(this, &Drive::doReadSelectedOscillograms, worker, &DriveWorker::readSelectedOscillograms);
    connect(this, &Drive::doReadOscillogramsList, worker, &DriveWorker::readOscillogramsList);
    connect(this, &Drive::doClearReadedOscillograms, worker, &DriveWorker::clearReadedOscillograms);
}

Drive::~Drive()
{
    stopWorkerThread();

    delete worker;
}

bool Drive::setup()
{
    stopWorkerThread();

    if(!worker->setup()) return false;

    startWorkerThread();

    return true;
}

bool Drive::good()
{
    return worker->good();
}

bool Drive::connectedToDevice() const
{
    return worker->connectedToDevice();
}

float Drive::reference() const
{
    return worker->reference();
}

bool Drive::running() const
{
    return worker->running();
}

drive_state_t Drive::state() const
{
    return worker->state();
}

unsigned int Drive::devLifetime() const
{
    return worker->devLifetime();
}

unsigned int Drive::devRuntime() const
{
    return worker->devRuntime();
}

unsigned int Drive::devFanRuntime() const
{
    return worker->devFanRuntime();
}

unsigned int Drive::devLastRuntime() const
{
    return worker->devLastRuntime();
}

drive_errors_t Drive::errors() const
{
    return worker->errors();
}

drive_warnings_t Drive::warnings() const
{
    return worker->warnings();
}

drive_power_errors_t Drive::powerErrors() const
{
    return worker->powerErrors();
}

drive_power_warnings_t Drive::powerWarnings() const
{
    return worker->powerWarnings();
}

drive_phase_errors_t Drive::phaseErrors() const
{
    return worker->phaseErrors();
}

QList<DriveEvent> Drive::events() const
{
    return worker->events();
}

QList<DriveOscillogram> Drive::oscillograms() const
{
    return worker->oscillograms();
}

QList<drive_event_id_t> Drive::oscillogramsList() const
{
    return worker->oscillogramsList();
}

size_t Drive::oscillogramsCount() const
{
    return worker->oscillogramsCount();
}

DriveOscillogram Drive::oscillogram(size_t index) const
{
    return worker->oscillogram(index);
}

void Drive::addOscillogram(const DriveOscillogram& osc)
{
    worker->addOscillogram(osc);
}

void Drive::addUpdParam(Parameter *param)
{
    worker->addUpdParam(param);
}

void Drive::removeUpdParam(Parameter *param)
{
    worker->removeUpdParam(param);
}

Future *Drive::readParams(QList<Parameter *> &params)
{
    Future* future = new Future();
    future->moveToThread(worker);

    worker->addReadParams(params, future);
    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit readNextParams();

    return future;
}

Future *Drive::writeParams(QList<Parameter *> &params)
{
    Future* future = new Future();
    future->moveToThread(worker);

    worker->addWriteParams(params, future);
    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit writeNextParams();

    return future;
}

Future *Drive::readEvents()
{
    Future* future = new Future();
    future->moveToThread(worker);

    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit doReadEvents(future);

    return future;
}

Future *Drive::readOscillograms()
{
    Future* future = new Future();
    future->moveToThread(worker);

    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit doReadOscillograms(future);

    return future;
}

Future*Drive::readSelectedOscillograms(QList<size_t> osc_list)
{
    Future* future = new Future();
    future->moveToThread(worker);

    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit doReadSelectedOscillograms(future, osc_list);

    return future;
}

Future *Drive::readOscillogramsList()
{
    Future* future = new Future();
    future->moveToThread(worker);

    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit doReadOscillogramsList(future);

    return future;
}

Future *Drive::clearReadedOscillograms()
{
    Future* future = new Future();
    future->moveToThread(worker);

    connect(worker, &DriveWorker::finished, future, &Future::deleteLater);

    emit doClearReadedOscillograms(future);

    return future;
}

QString Drive::errorToString(drive_error_t err)
{
    switch(err){
    default:
    case DRIVE_ERROR_NONE:
        return QString("DRIVE_ERROR_NONE");
    case DRIVE_ERROR_POWER_DATA_NOT_AVAIL:
        return QString("DRIVE_ERROR_POWER_DATA_NOT_AVAIL");
    case DRIVE_ERROR_POWER_INVALID:
        return QString("DRIVE_ERROR_POWER_INVALID");
    case DRIVE_ERROR_EMERGENCY_STOP:
        return QString("DRIVE_ERROR_EMERGENCY_STOP");
    case DRIVE_ERROR_PHASE:
        return QString("DRIVE_ERROR_PHASE");
    }
    return QString();
}

QStringList Drive::errorsToString(drive_errors_t errs)
{
    QStringList list;

    while(errs != 0){
        if(errs & 0x1)
            list << errorToString(static_cast<drive_error_t>(errs & 0x1));
        errs >>= 1;
    }

    return list;
}

void Drive::startWorkerThread()
{
    if(!worker->isRunning()){
        worker->start();
    }
}

void Drive::stopWorkerThread()
{
    if(worker->isRunning()){
        worker->quit();
        worker->wait(WORKER_THREAD_WAIT_MAX);
    }
}
