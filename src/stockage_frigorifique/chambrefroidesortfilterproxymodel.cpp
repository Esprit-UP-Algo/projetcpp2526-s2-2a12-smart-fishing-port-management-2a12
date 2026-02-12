#include "chambrefroidesortfilterproxymodel.h"

#include <QAbstractItemModel>

#include "chambrefroidemodel.h"

namespace stockage {

ChambreFroideSortFilterProxyModel::ChambreFroideSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
    setSortRole(ChambreFroideModel::SortValueRole);
}

void ChambreFroideSortFilterProxyModel::setSearchIdChambre(const QString &text)
{
    m_searchId = text;
    invalidateFilter();
}

void ChambreFroideSortFilterProxyModel::setSearchEmplacement(const QString &text)
{
    m_searchEmplacement = text;
    invalidateFilter();
}

bool ChambreFroideSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto *m = sourceModel();
    if (!m)
        return true;

    const QString id = m->data(m->index(sourceRow, ChambreFroideModel::ColIdChambre, sourceParent), Qt::DisplayRole).toString();
    const QString empl = m->data(m->index(sourceRow, ChambreFroideModel::ColEmplacement, sourceParent), Qt::DisplayRole).toString();

    const bool okId = m_searchId.trimmed().isEmpty() || id.contains(m_searchId, Qt::CaseInsensitive);
    const bool okEmpl = m_searchEmplacement.trimmed().isEmpty() || empl.contains(m_searchEmplacement, Qt::CaseInsensitive);

    return okId && okEmpl;
}

} // namespace stockage
