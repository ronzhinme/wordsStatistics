#ifndef WORDSSTATISTICSMODEL_H
#define WORDSSTATISTICSMODEL_H

#include "qtmetamacros.h"
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>

class WordsStatisticsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(quint64 totalWordCount READ totalWordCount NOTIFY sigTotalWordsCountChanged)
    Q_PROPERTY(double percentage READ percentage WRITE setPercentage NOTIFY sigPercentageChanged)
    Q_PROPERTY(QString progressText READ progressText NOTIFY sigPercentageChanged)
public:
    WordsStatisticsModel();
    quint64 totalWordCount() const;
public slots:
    void appendWord(const QString& word);
    void clearModel();
    void setPercentage(double val);
    double percentage() const;
private:
    QList<QPair<QString, quint64>> words_;
    QHash<int, QByteArray> roles_ {{Qt::DisplayRole, "display"},
                                   {Qt::UserRole + 1, "wordCount"},
                                   {Qt::UserRole + 2, "wordPercentage"},
                                   {Qt::UserRole + 3, "wordStatText"}};
    double percentage_;
    quint64 totalWordCount_;
    QString progressText() const;
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

class SortAndFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(quint64 maxRows READ maxRows WRITE setMaxRows NOTIFY maxRowsChanged)
public:
    void setMaxRows(quint64 val){maxRows_ = val; emit maxRowsChanged();}
    quint64 maxRows()const {return maxRows_;}
private:
    quint64 maxRows_= 1000;
signals:
    void maxRowsChanged();
    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    virtual bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;
};

#endif // WORDSSTATISTICSMODEL_H
