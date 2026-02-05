#pragma once

#include <QSortFilterProxyModel>
#include <QDateTime>

class TransactionModel;

class TransactionSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TransactionSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchClient(const QString &text);
    void setSearchLot(const QString &text);
    void setDateFrom(const QDateTime &dt);
    void setDateTo(const QDateTime &dt);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchClient;
    QString m_searchLot;
    QDateTime m_dateFrom;
    QDateTime m_dateTo;
};
