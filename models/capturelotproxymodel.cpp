#include "capturelotproxymodel.h"
#include "capturelotmodel.h"

CaptureLotProxyModel::CaptureLotProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterKeyColumn(-1); // Filtre sur toutes les colonnes
}

void CaptureLotProxyModel::setSearchLot(const QString &text)
{
    m_searchLot = text;
    invalidateFilter();
}

void CaptureLotProxyModel::setSearchEspece(const QString &text)
{
    m_searchEspece = text;
    invalidateFilter();
}

void CaptureLotProxyModel::setSearchNavire(const QString &text)
{
    m_searchNavire = text;
    invalidateFilter();
}

void CaptureLotProxyModel::setSearchZone(const QString &text)
{
    m_searchZone = text;
    invalidateFilter();
}

bool CaptureLotProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QAbstractItemModel *model = sourceModel();
    if (!model)
        return false;

    // Filtrer par ID Lot (colonne 0)
    if (!m_searchLot.isEmpty())
    {
        const QString idLot = model->data(model->index(sourceRow, 0, sourceParent)).toString();
        if (!idLot.contains(m_searchLot, Qt::CaseInsensitive))
            return false;
    }

    // Filtrer par Espèce (colonne 1)
    if (!m_searchEspece.isEmpty())
    {
        const QString espece = model->data(model->index(sourceRow, 1, sourceParent)).toString();
        if (!espece.contains(m_searchEspece, Qt::CaseInsensitive))
            return false;
    }

    // Filtrer par Navire (colonne 6)
    if (!m_searchNavire.isEmpty())
    {
        const QString navire = model->data(model->index(sourceRow, 6, sourceParent)).toString();
        if (!navire.contains(m_searchNavire, Qt::CaseInsensitive))
            return false;
    }

    // Filtrer par Zone de Pêche (colonne 3)
    if (!m_searchZone.isEmpty())
    {
        const QString zone = model->data(model->index(sourceRow, 3, sourceParent)).toString();
        if (!zone.contains(m_searchZone, Qt::CaseInsensitive))
            return false;
    }

    return true;
}
