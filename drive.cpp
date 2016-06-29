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
    connect(this, &Drive::saveParams, worker, &DriveWorker::saveParams);
    connect(this, &Drive::readNextParams, worker, &DriveWorker::readNextParams);
    connect(this, &Drive::writeNextParams, worker, &DriveWorker::writeNextParams);
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
