#include "quaisortproxymodel.h"

#include "quaimodel.h"

QuaiSortProxyModel::QuaiSortProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

void QuaiSortProxyModel::setSortMode(SortMode mode)
{
    if (m_sortMode == mode)
        return;
    m_sortMode = mode;
    invalidate();
}

QuaiSortProxyModel::SortMode QuaiSortProxyModel::sortMode() const
{
    return m_sortMode;
}

bool QuaiSortProxyModel::filterAcceptsRow(int, const QModelIndex &) const
{
    // Sorting-only proxy: do not filter.
    return true;
}

static bool isOccupied(const QString &etat)
{
    return etat.compare("Occupé", Qt::CaseInsensitive) == 0 || etat.compare("Occupe", Qt::CaseInsensitive) == 0;
}

bool QuaiSortProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    if (m_sortMode != SortMode::TableColumn)
    {
        const auto getStr = [&](int column, const QModelIndex &idx) {
            return sourceModel()->data(sourceModel()->index(idx.row(), column, idx.parent()), Qt::DisplayRole).toString();
        };
        const auto getDt = [&](int column, const QModelIndex &idx) {
            return sourceModel()->data(sourceModel()->index(idx.row(), column, idx.parent()), QuaiModel::SortValueRole).toDateTime();
        };
        const auto getNum = [&](int column, const QModelIndex &idx) {
            return sourceModel()->data(sourceModel()->index(idx.row(), column, idx.parent()), QuaiModel::SortValueRole).toDouble();
        };

        switch (m_sortMode)
        {
        case SortMode::TailleAsc:
            return getNum(QuaiModel::Taille, sourceLeft) < getNum(QuaiModel::Taille, sourceRight);
        case SortMode::Statut:
            return getStr(QuaiModel::Etat, sourceLeft).localeAwareCompare(getStr(QuaiModel::Etat, sourceRight)) < 0;
        case SortMode::PrixAsc:
            return getNum(QuaiModel::Prix, sourceLeft) < getNum(QuaiModel::Prix, sourceRight);
        case SortMode::LiberationBientot:
        {
            const QString lEtat = getStr(QuaiModel::Etat, sourceLeft);
            const QString rEtat = getStr(QuaiModel::Etat, sourceRight);
            const int lRank = isOccupied(lEtat) ? 0 : 1;
            const int rRank = isOccupied(rEtat) ? 0 : 1;
            if (lRank != rRank)
                return lRank < rRank;
            return getDt(QuaiModel::Depart, sourceLeft) < getDt(QuaiModel::Depart, sourceRight);
        }
        case SortMode::OccupeDepuisLongtemps:
        {
            const QString lEtat = getStr(QuaiModel::Etat, sourceLeft);
            const QString rEtat = getStr(QuaiModel::Etat, sourceRight);
            const int lRank = isOccupied(lEtat) ? 0 : 1;
            const int rRank = isOccupied(rEtat) ? 0 : 1;
            if (lRank != rRank)
                return lRank < rRank;
            return getDt(QuaiModel::Arrivee, sourceLeft) < getDt(QuaiModel::Arrivee, sourceRight);
        }
        case SortMode::TableColumn:
        default:
            break;
        }
    }

    const QVariant leftSort = sourceModel()->data(sourceLeft, QuaiModel::SortValueRole);
    const QVariant rightSort = sourceModel()->data(sourceRight, QuaiModel::SortValueRole);

    // QDateTime sorts naturally
    if (leftSort.userType() == QMetaType::QDateTime && rightSort.userType() == QMetaType::QDateTime)
        return leftSort.toDateTime() < rightSort.toDateTime();

    // Numeric
    if (leftSort.canConvert<double>() && rightSort.canConvert<double>())
        return leftSort.toDouble() < rightSort.toDouble();

    // Fallback string compare
    return leftSort.toString().localeAwareCompare(rightSort.toString()) < 0;
}
