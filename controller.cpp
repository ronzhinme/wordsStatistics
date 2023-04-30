#include "controller.h"

#include <QUrl>

Worker::Worker()
    : QObject()
    , state_(WorkerState::kIdle)
{

}

void Worker::doWork()
{
    state_ = WorkerState::kWork;

    WordFileParser fileParser(filename_);
    while(!QThread::currentThread()->isInterruptionRequested() && state_ != WorkerState::kCancel && !fileParser.atEnd())
    {
        if(state_ == WorkerState::kPause)
        {
            QThread::currentThread()->yieldCurrentThread();
            continue;
        }

        const auto word = fileParser.getNextWord();
        const auto percentage = fileParser.getProcessPercentage();
        if(word.isEmpty())
        {
            continue;
        }

        emit sigProcessWord(word);
        emit sigPercentageChanged(percentage);
        QThread::currentThread()->usleep(0);
    }

    emit sigPercentageChanged(100.0);
    QThread::currentThread()->usleep(0);
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
    connect(&workerThread_, &QThread::finished, this, [this](){emit sigPercentageChanged(100.0);});
    connect(&worker_, &Worker::sigProcessWord, this, &Controller::sigProcessWord);
    connect(&worker_, &Worker::sigPercentageChanged, this, &Controller::sigPercentageChanged);
}

Controller::~Controller()
{
    workerThread_.requestInterruption();
    workerThread_.quit();
    workerThread_.wait();
}

void Controller::start(const QString &filename)
{
    if(worker_.state() == WorkerState::kIdle || worker_.state() == WorkerState::kCancel)
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

WordFileParser::WordFileParser(const QString &filename)
    : file_(QUrl(filename).toLocalFile())
{
    if(!file_.open(QIODevice::ReadOnly))
    {
        return;
    }

    totalBytes_ = file_.size();
}

QString WordFileParser::getNextWord()
{
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return {};
    }

    QByteArray result;
    while (!file_.atEnd())
    {
        char ch;
        file_.read(&ch, 1);
        if(!std::isspace(ch) && !std::ispunct(ch)) // словом считаю любую последовательность символов, но не разрывы и пунктуацию
            result.append(ch);
        else
            break;
    }

    return result;
}

double WordFileParser::getProcessPercentage() const
{
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return 0;
    }

    return static_cast<double>(file_.pos()) / (static_cast<double>(totalBytes_) / 100.0);
}

bool WordFileParser::atEnd()
{
    if(!file_.isOpen() || totalBytes_ == 0)
    {
        return true;
    }

    return file_.atEnd();
}
