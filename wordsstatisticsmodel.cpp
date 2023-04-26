#include "wordsstatisticsmodel.h"

WordsStatisticsModel::WordsStatisticsModel()
    : QAbstractListModel()
    , totalWordCount_(0)
{

}

void WordsStatisticsModel::appendWord(const QString &word)
{
    ++wordMap_[word];
    ++totalWordCount_;
    emit sigTotalWordsCountChanged();
    emit layoutChanged();
}

quint64 WordsStatisticsModel::getTotalWordCount() const
{
    return totalWordCount_;
}

int WordsStatisticsModel::rowCount(const QModelIndex &parent) const
{
    return wordMap_.size();
}

QVariant WordsStatisticsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    auto iter = wordMap_.begin();
    std::advance(iter, index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return iter->first;
    case Qt::UserRole + 1:
        return iter->second;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> WordsStatisticsModel::roleNames() const
{
    return roles_;
}
