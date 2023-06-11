#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QSharedPointer>

#include "fileparser.h"

enum class WorkerState
{
    kIdle,
    kWork,
    kPause,
    kCancel,
};

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker();
public slots:
    void doWork();
    void requestChangeState(WorkerState state);
    WorkerState state() const {return state_;}
    void setFileParser(QSharedPointer<WordFileParser> parser);
signals:
    void sigProcessWord(const QString &word);
    void sigPercentageChanged(double percentage);
private:
    WorkerState state_;
    QSharedPointer<WordFileParser> fileParser_;
};

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();
    Q_INVOKABLE void start(const QString & filename);
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void cancel();
signals:
    void sigStarted();
    void sigProcessWord(const QString &word);
    void sigPercentageChanged(double percentage);
    void sigPauseRequest();
    void sigResumeRequest();
private:
    QList<QPair<QThread*, Worker*>> workers_;
    QSharedPointer<WordFileParser> fileParser_;
};

#endif // CONTROLLER_H
