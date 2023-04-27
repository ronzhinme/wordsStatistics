#include "controller.h"


#include <QDateTime>
#include <QRandomGenerator>
void Worker::doWork()
{
    QRandomGenerator64 randGen(QDateTime::currentDateTime().toSecsSinceEpoch());
    while(!QThread::currentThread()->isInterruptionRequested())
    {
        emit sigProcessWord(QStringLiteral("Word_%1").arg((randGen.generate() & 0xF) % 10));
        QThread::currentThread()->usleep(10);
    }
}

Controller::~Controller()
{
    workerThread_.requestInterruption();
    workerThread_.quit();
    workerThread_.wait();
}

void Controller::start(const QString &filename)
{
    worker_ = new Worker;
    worker_->moveToThread(&workerThread_);
    connect(&workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
    connect(&workerThread_, &QThread::started, worker_, &Worker::doWork);
    connect(worker_, &Worker::sigProcessWord, this, &Controller::sigProcessWord);
    workerThread_.start();
}
