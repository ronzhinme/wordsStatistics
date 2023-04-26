#ifndef WORDSSTATISTICSMODEL_H
#define WORDSSTATISTICSMODEL_H

#include <QAbstractItemModel>
#include <QThread>

#include <QGuiApplication>
#include <QRandomGenerator>
#include <QDateTime>

class WordsStatisticsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalWordCount READ getTotalWordCount NOTIFY sigTotalWordsCountChanged)
public:
    WordsStatisticsModel();

    quint64 getTotalWordCount() const;

public slots:
    void appendWord(const QString& word);

private:
    quint64 totalWordCount_;
    std::unordered_map<QString, quint64> wordMap_;
    QHash<int, QByteArray> roles_ {{Qt::DisplayRole, "display"}, {Qt::UserRole + 1, "wordCount"}};

signals:
    void sigTotalWordsCountChanged();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
};

class Worker : public QObject
{
    Q_OBJECT
public slots:
    void doWork()
    {
        QRandomGenerator64 randGen(QDateTime::currentDateTime().toSecsSinceEpoch());
        while(!QThread::currentThread()->isInterruptionRequested())
        {
            emit sigProcessWord(QStringLiteral("Word_%1").arg((randGen.generate() & 0xF) % 10));
            QThread::currentThread()->usleep(10);
        }
    }

signals:
    void sigProcessWord(const QString &word);
};

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller() {
    }
    ~Controller() {
        workerThread_.requestInterruption();
        workerThread_.quit();
        workerThread_.wait();
    }

    void start()
    {
        worker_ = new Worker;
        worker_->moveToThread(&workerThread_);
        connect(&workerThread_, &QThread::finished, worker_, &QObject::deleteLater);
        connect(&workerThread_, &QThread::started, worker_, &Worker::doWork);
        connect(worker_, &Worker::sigProcessWord, this, &Controller::sigProcessWord);
        workerThread_.start();
    }
signals:
    void sigProcessWord(const QString &word);
private:
    QThread workerThread_;
    Worker* worker_; // deleteLater
};

#endif // WORDSSTATISTICSMODEL_H
