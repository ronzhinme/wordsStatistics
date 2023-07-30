#include "controller.h"

std::mutex ReadMutex;
Worker::Worker()
    : QObject()
    , state_(WorkerState::kIdle)
    , fileParser_(nullptr)
{

}

void Worker::doWork()
{
    if(!fileParser_)
    {
        return;
    }

    state_ = WorkerState::kWork;
    while(!QThread::currentThread()->isInterruptionRequested() && state_ != WorkerState::kCancel && !fileParser_->atEnd())
    {
        if(state_ == WorkerState::kPause)
        {
            QThread::currentThread()->sleep(1);
            continue;
        }

        std::lock_guard<std::mutex> lock(ReadMutex);
        const auto percentage = fileParser_->getProcessPercentage();
        emit sigPercentageChanged(percentage);

        const auto word = fileParser_->getNextWord();
        emit sigProcessWord(word);
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
    case WorkerState::kIdle:
    {
        if(state_ != WorkerState::kCancel)
            break;

        state_ = state;
        break;
    }
    case WorkerState::kCancel:
    {
        state_ = state;
        break;
    }
    }
}

void Worker::setFileParser(QSharedPointer<WordFileParser> parser)
{
    fileParser_= parser;
}

Controller::Controller()
    : QObject()
    , workers_(100)
    , fileParser_(nullptr)
{
}

Controller::~Controller()
{
    for(auto i = 0 ; i < workers_.size(); ++i)
    {
        auto [thread, worker] = workers_[i];
        thread->requestInterruption();
    }
}

void Controller::start(const QString &filename)
{
    fileParser_.reset(new WordFileParser(filename));
    for(auto i = 0 ; i < workers_.size(); ++i)
    {
        workers_[i] = {new QThread, new Worker};
        auto [thread, worker] = workers_[i];

        worker->moveToThread(thread);
        connect(thread, &QThread::started, worker, &Worker::doWork);
        connect(thread, &QThread::finished, this, [this](){emit sigPercentageChanged(100.0);});
        connect(thread, &QThread::finished, worker, &QObject::deleteLater);
        connect(thread, &QThread::finished, thread, &QObject::deleteLater);
        connect(worker, &Worker::sigProcessWord, this, &Controller::sigProcessWord);
        connect(worker, &Worker::sigPercentageChanged, this, &Controller::sigPercentageChanged);

        if(worker->state() == WorkerState::kIdle ||
                worker->state() == WorkerState::kCancel)
        {
            emit sigStarted();
            worker->setFileParser(fileParser_);
            thread->start();
        }
    }
}

void Controller::pause()
{
    for(auto i = 0 ; i < workers_.size(); ++i)
    {
        auto [thread, worker] = workers_[i];
        worker->requestChangeState(WorkerState::kPause);
    }
}

void Controller::resume()
{
    for(auto i = 0 ; i < workers_.size(); ++i)
    {
        auto [thread, worker] = workers_[i];
        worker->requestChangeState(WorkerState::kWork);
    }
}

void Controller::cancel()
{
    for(auto i = 0 ; i < workers_.size(); ++i)
    {
        auto [thread, worker] = workers_[i];
        worker->requestChangeState(WorkerState::kCancel);
    }
}
