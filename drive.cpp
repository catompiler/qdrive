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
    connect(this, &Drive::connectToDevice, worker, &DriveWorker::connectToDevice);
    connect(this, &Drive::disconnectFromDevice, worker, &DriveWorker::disconnectFromDevice);
    connect(this, &Drive::setRunning, worker, &DriveWorker::setRunning);
    connect(this, &Drive::start, worker, &DriveWorker::startDrive);
    connect(this, &Drive::stop, worker, &DriveWorker::stopDrive);
    connect(this, &Drive::setReference, worker, &DriveWorker::setReference);
    connect(this, &Drive::clearErrors, worker, &DriveWorker::clearErrors);
    connect(this, &Drive::calibratePower, worker, &DriveWorker::calibratePower);
    connect(this, &Drive::saveParams, worker, &DriveWorker::saveParams);
    connect(this, &Drive::readNextParams, worker, &DriveWorker::readNextParams);
    connect(this, &Drive::writeNextParams, worker, &DriveWorker::writeNextParams);
    connect(this, &Drive::doReadEvents, worker, &DriveWorker::readEvents);
    connect(this, &Drive::doReadOscillograms, worker, &DriveWorker::readOscillograms);
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

unsigned int Drive::reference() const
{
    return worker->reference();
}

bool Drive::running() const
{
    return worker->running();
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
