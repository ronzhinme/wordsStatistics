#include "wordsstatisticsmodel.h"

WordsStatisticsModel::WordsStatisticsModel()
    : QAbstractListModel()
    , totalWordCount_(0)
{

}

void WordsStatisticsModel::appendWord(const QString &word)
{
    ++totalWordCount_;

    const auto iter = wordMap_.find(word);
    const auto row = std::distance(wordMap_.begin(), iter);

    if(iter == wordMap_.end())
    {
        beginInsertRows(index(row), row, row);
        ++wordMap_[word];
        endInsertRows();
        return;
    }

    setData(index(row), QVariant::fromValue(wordMap_[word] + 1));
}

void WordsStatisticsModel::clearModel()
{
    beginResetModel();
    wordMap_.clear();
    endResetModel();
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

bool WordsStatisticsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    auto iter = wordMap_.begin();
    std::advance(iter, index.row());

    switch (role)
    {
    case Qt::EditRole:
    {
        iter->second = value.toLongLong();
        emit dataChanged(index, index);
        return true;
    }
    default:
        return false;
    }
}
