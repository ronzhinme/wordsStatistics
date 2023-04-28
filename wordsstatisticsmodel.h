#ifndef WORDSSTATISTICSMODEL_H
#define WORDSSTATISTICSMODEL_H

#include "qtmetamacros.h"
#include <QAbstractItemModel>

class WordsStatisticsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalWordCount READ totalWordCount NOTIFY sigTotalWordsCountChanged)
    Q_PROPERTY(double percentage READ percentage WRITE setPercentage NOTIFY sigPercentageChanged)
public:
    WordsStatisticsModel();
    quint64 totalWordCount() const;

public slots:
    void appendWord(const QString& word);
    void clearModel();
    void setPercentage(double val);
    double percentage() const;
private:
    std::unordered_map<QString, quint64> wordMap_;
    QHash<int, QByteArray> roles_ {{Qt::DisplayRole, "display"}, {Qt::UserRole + 1, "wordCount"}};
    double percentage_;
    quint64 totalWordCount_;
signals:
    void sigTotalWordsCountChanged();
    void sigPercentageChanged();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int,QByteArray> roleNames() const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // WORDSSTATISTICSMODEL_H
