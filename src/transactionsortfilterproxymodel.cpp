#include "transactionsortfilterproxymodel.h"

#include <QAbstractItemModel>
#include "transactionmodel.h"

TransactionSortFilterProxyModel::TransactionSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
    setSortRole(TransactionModel::SortValueRole);
}

void TransactionSortFilterProxyModel::setSearchClient(const QString &text)
{
    m_searchClient = text;
    invalidateFilter();
}

void TransactionSortFilterProxyModel::setSearchLot(const QString &text)
{
    m_searchLot = text;
    invalidateFilter();
}

void TransactionSortFilterProxyModel::setDateFrom(const QDateTime &dt)
{
    m_dateFrom = dt;
    invalidateFilter();
}

void TransactionSortFilterProxyModel::setDateTo(const QDateTime &dt)
{
    m_dateTo = dt;
    invalidateFilter();
}

bool TransactionSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto *m = sourceModel();
    if (!m)
        return true;

    const QString client = m->data(m->index(sourceRow, TransactionModel::ColClient, sourceParent), Qt::DisplayRole).toString();
    const QString lot = m->data(m->index(sourceRow, TransactionModel::ColLot, sourceParent), Qt::DisplayRole).toString();
    const QVariant dateVar = m->data(m->index(sourceRow, TransactionModel::ColDate, sourceParent), TransactionModel::SortValueRole);

    const bool okClient = m_searchClient.trimmed().isEmpty() || client.contains(m_searchClient, Qt::CaseInsensitive);
    const bool okLot = m_searchLot.trimmed().isEmpty() || lot.contains(m_searchLot, Qt::CaseInsensitive);

    bool okDate = true;
    if (m_dateFrom.isValid() || m_dateTo.isValid()) {
        QDateTime rowDate;
        if (dateVar.canConvert<QDateTime>())
            rowDate = dateVar.toDateTime();
        else
            rowDate = QDateTime::fromString(dateVar.toString(), QStringLiteral("yyyy-MM-dd HH:mm"));

        if (m_dateFrom.isValid() && rowDate.isValid() && rowDate < m_dateFrom)
            okDate = false;
        if (m_dateTo.isValid() && rowDate.isValid() && rowDate > m_dateTo)
            okDate = false;
    }

    return okClient && okLot && okDate;
}
