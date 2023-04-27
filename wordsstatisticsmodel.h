#ifndef WORDSSTATISTICSMODEL_H
#define WORDSSTATISTICSMODEL_H

#include <QAbstractItemModel>

class WordsStatisticsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalWordCount READ getTotalWordCount NOTIFY sigTotalWordsCountChanged)
public:
    WordsStatisticsModel();

    quint64 getTotalWordCount() const;

public slots:
    void appendWord(const QString& word);
    void clearModel();
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
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // WORDSSTATISTICSMODEL_H
