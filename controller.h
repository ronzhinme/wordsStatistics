#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <QFile>

class WordFileParser
{
public:
    WordFileParser(const QString &filename);
    QString getNextWord();
    double getProcessPercentage() const;
    bool atEnd();
private:
    size_t totalBytes_;
    QFile file_;
};

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
    void setFileName(const QString & filename);
signals:
    void sigProcessWord(const QString &word);
    void sigPercentageChanged(double percentage);
private:
    WorkerState state_;
    QString filename_;
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
    QThread workerThread_;
    Worker worker_;
};

#endif // CONTROLLER_H
