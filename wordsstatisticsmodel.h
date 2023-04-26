#ifndef WORDSSTATISTICSMODEL_H
#define WORDSSTATISTICSMODEL_H

#include <QAbstractItemModel>
#include <QThread>

#include <QGuiApplication>
#include <QRandomGenerator>
#include <QDateTime>

class WorkThread : public QThread
{
    Q_OBJECT

public:
    explicit WorkThread()
        : QThread()
    {

    }

protected:
    virtual void run() override
    {
        QRandomGenerator64 randGen(QDateTime::currentDateTime().toSecsSinceEpoch());

        while(true)
        {
            const auto rnd = randGen.generate() & 0xF;
            emit sigProcessWord(QStringLiteral("Word_%1").arg(rnd % 10));
            msleep(10);
        }
    };
signals:
    void sigProcessWord(const QString &word);
};

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

#endif // WORDSSTATISTICSMODEL_H
