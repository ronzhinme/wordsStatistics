#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

class Worker : public QObject
{
    Q_OBJECT
public slots:
    void doWork();

signals:
    void sigProcessWord(const QString &word);
};

class Controller : public QObject
{
    Q_OBJECT
public:
    ~Controller();
    Q_INVOKABLE void start(const QString & filename);
signals:
    void sigProcessWord(const QString &word);
private:
    QThread workerThread_;
    Worker* worker_;
};

#endif // CONTROLLER_H
