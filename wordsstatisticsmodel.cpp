#include "wordsstatisticsmodel.h"

/// WordsStatisticsModel
WordsStatisticsModel::WordsStatisticsModel()
    : QAbstractListModel()
    , totalWordCount_(0)
    , percentage_(0)
{

}

quint64 WordsStatisticsModel::totalWordCount() const
{
    return totalWordCount_;
}

void WordsStatisticsModel::appendWord(const QString &word)
{
    if(word.trimmed().isEmpty())
    {
        return;
    }

    ++totalWordCount_;
    emit sigTotalWordsCountChanged();

    const auto iter = std::find_if(words_.begin(), words_.end(), [&word](const QPair<QString, quint64> x){return x.first == word;});
    const auto row = std::distance(words_.begin(), iter);

    if(iter == words_.end())
    {
        beginInsertRows(index(row), row, row);
        words_.push_back({word, 1});
        endInsertRows();
        return;
    }

    setData(index(row), QVariant::fromValue(iter->second + 1));
}

void WordsStatisticsModel::clearModel()
{
    beginResetModel();
    words_.clear();
    totalWordCount_ = 0;
    emit sigTotalWordsCountChanged();
    endResetModel();
}

void WordsStatisticsModel::setPercentage(double val)
{
    percentage_ = val;
    emit sigPercentageChanged();
}

double WordsStatisticsModel::percentage() const
{
    return percentage_;
}

QString WordsStatisticsModel::progressText() const
{
 return QString("Processed words: %1 = %2 %").arg(totalWordCount_).arg(QString::number(percentage_, 'f', 2));
}

int WordsStatisticsModel::rowCount(const QModelIndex &parent) const
{
    return words_.size();
}

QVariant WordsStatisticsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    auto iter = words_.begin();
    std::advance(iter, index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return iter->first;
    case Qt::UserRole + 1:
        return iter->second;
    case Qt::UserRole + 2:
        return static_cast<double>(iter->second) / totalWordCount_ * 100;
    case Qt::UserRole + 3:
    {
        const auto wordPercentage = QString::number(static_cast<double>(iter->second) / totalWordCount_ * 100, 'f', 2);
        return QVariant::fromValue(QString("[%1] : %2 (%3%)").arg(iter->first).arg(iter->second).arg(wordPercentage));
    }
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

    auto iter = words_.begin();
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

/// SortAndFilterProxy
bool SortAndFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    const auto lastAccessibleRowWordCount = index(maxRows_.value(), 0).data(sortRole()).toUInt();
    const auto sourceRowWordCount = sourceModel()->index(source_row, 0, source_parent).data(sortRole()).toUInt();
    return sourceRowWordCount > lastAccessibleRowWordCount;
}

bool SortAndFilterProxy::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    return !(source_left.data(sortRole()).toUInt() > source_right.data(sortRole()).toUInt());
}


int SortAndFilterProxy::rowCount(const QModelIndex &parent) const
{
    auto srcRowCount = 0;
    if(sourceModel())
    {
        srcRowCount = sourceModel()->rowCount(parent);
    }

    return maxRows_.has_value() && maxRows_.value() < srcRowCount ? maxRows_.value() : srcRowCount;
}
