#include "future.h"

Future::Future(QObject *parent) : QObject(parent)
{
    op_run = false;
    op_done = false;
    op_cancel = false;
    op_progress_min = 0;
    op_progress_max = 100;
    op_progress = 0;
}

/*Future::Future(const Future &future)
{
    op_run = future.op_run;
    op_done = future.op_done;
    op_cancel = future.op_cancel;
    op_progress_min = future.op_progress_min;
    op_progress_max = future.op_progress_max;
    op_progress = future.op_progress;
}*/

Future::~Future()
{
}

bool Future::isRunning() const
{
    return op_run;
}

bool Future::isDone() const
{
    return op_done;
}

bool Future::needCancel() const
{
    return op_cancel;
}

int Future::progress() const
{
    return op_progress;
}

void Future::wait() const
{
    while(!op_done);
}

void Future::start()
{
    op_done = false;
    op_run = true;
    op_cancel = false;

    emit started();
}

void Future::setProgressMinimum(int progress_min)
{
    op_progress_min = progress_min;

    emit progressMinimumChanged(op_progress_min);
}

void Future::setProgressMaximum(int progress_max)
{
    op_progress_max = progress_max;

    emit progressMaximumChanged(op_progress_max);
}

void Future::setProgressRange(int progress_min, int progress_max)
{
    setProgressMinimum(progress_min);
    setProgressMaximum(progress_max);

    emit progressRangeChanged(op_progress_min, op_progress_max);
}

void Future::setProgress(int progress_val)
{
    op_progress = progress_val;

    emit progressChanged(op_progress);
}

void Future::finish()
{
    op_done = true;
    op_run = false;

    emit finished();
}

void Future::finish(const QVariant &res)
{
    op_res = res;
    finish();
}

void Future::cancel()
{
    op_cancel = true;
}
