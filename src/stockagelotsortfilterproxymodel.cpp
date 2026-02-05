#include "stockagelotsortfilterproxymodel.h"

#include <QAbstractItemModel>

#include "stockagelotmodel.h"

StockageLotSortFilterProxyModel::StockageLotSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
    setSortRole(StockageLotModel::SortValueRole);
}

void StockageLotSortFilterProxyModel::setSearchLot(const QString &text)
{
    m_searchLot = text;
    invalidateFilter();
}

void StockageLotSortFilterProxyModel::setSearchChambre(const QString &text)
{
    m_searchChambre = text;
    invalidateFilter();
}

void StockageLotSortFilterProxyModel::setSearchEspece(const QString &text)
{
    m_searchEspece = text;
    invalidateFilter();
}

bool StockageLotSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto *m = sourceModel();
    if (!m)
        return true;

    const QString lot = m->data(m->index(sourceRow, StockageLotModel::ColIdLot, sourceParent), Qt::DisplayRole).toString();
    const QString espece = m->data(m->index(sourceRow, StockageLotModel::ColEspece, sourceParent), Qt::DisplayRole).toString();
    const QString chambre = m->data(m->index(sourceRow, StockageLotModel::ColIdChambre, sourceParent), Qt::DisplayRole).toString();

    const bool okLot = m_searchLot.trimmed().isEmpty() || lot.contains(m_searchLot, Qt::CaseInsensitive);
    const bool okEspece = m_searchEspece.trimmed().isEmpty() || espece.contains(m_searchEspece, Qt::CaseInsensitive);
    const bool okChambre = m_searchChambre.trimmed().isEmpty() || chambre.contains(m_searchChambre, Qt::CaseInsensitive);

    return okLot && okEspece && okChambre;
}
