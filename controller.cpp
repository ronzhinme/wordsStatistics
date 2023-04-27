#include "controller.h"


#include <QDateTime>
#include <QRandomGenerator>
Worker::Worker()
    : QObject()
    , state_(WorkerState::kIdle)
{

}

void Worker::doWork()
{
    QRandomGenerator64 randGen(QDateTime::currentDateTime().toSecsSinceEpoch());

    state_ = WorkerState::kWork;
    while(!QThread::currentThread()->isInterruptionRequested() && state_ != WorkerState::kCancel)
    {
        if(state_ == WorkerState::kPause)
        {
            QThread::currentThread()->yieldCurrentThread();
            continue;
        }

        emit sigProcessWord(QStringLiteral("Word_%1").arg(randGen.generate() & 0xF));
        QThread::currentThread()->usleep(10);
    }

    state_ = WorkerState::kIdle;
}

void Worker::requestChangeState(WorkerState state)
{
    switch(state)
    {
    case WorkerState::kPause:
    {
        if(state_ != WorkerState::kWork)
            break;

        state_ = state;
        break;
    }
    case WorkerState::kWork:
    {
        if(state_ != WorkerState::kPause)
            break;

        state_ = state;
        break;
    }
    case WorkerState::kIdle: [[fallthrough]];
    case WorkerState::kCancel:
    {
        state_ = state;
        break;
    }
    }
}

void Worker::setFileName(const QString &filename)
{
    if(state_ != WorkerState::kIdle)
    {
        return;
    }

    filename_ = filename;
}

Controller::Controller()
    : QObject()
{
    worker_.moveToThread(&workerThread_);
    connect(&workerThread_, &QThread::started, &worker_, &Worker::doWork);
    connect(&worker_, &Worker::sigProcessWord, this, &Controller::sigProcessWord);
}

Controller::~Controller()
{
    workerThread_.requestInterruption();
    workerThread_.quit();
    workerThread_.wait();
}

void Controller::start(const QString &filename)
{
    if(worker_.state() == WorkerState::kIdle)
    {
        emit sigStarted();
        worker_.setFileName(filename);
        workerThread_.start();
    }
}

void Controller::pause()
{
    worker_.requestChangeState(WorkerState::kPause);
}

void Controller::resume()
{
    worker_.requestChangeState(WorkerState::kWork);
}

void Controller::cancel()
{
    worker_.requestChangeState(WorkerState::kCancel);
    workerThread_.quit();
    workerThread_.wait();
}
