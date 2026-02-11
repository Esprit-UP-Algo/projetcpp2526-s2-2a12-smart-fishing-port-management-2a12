#include "quaisortfilterproxymodel.h"

#include <QDateTime>

#include "quaimodel.h"

QuaiSortFilterProxyModel::QuaiSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

QString QuaiSortFilterProxyModel::norm(const QString &s)
{
    return s.trimmed().toLower();
}

void QuaiSortFilterProxyModel::setSearchMatricule(const QString &text)
{
    m_matricule = norm(text);
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setSearchIdQuai(const QString &text)
{
    m_idQuai = norm(text);
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setSearchDate(const QString &yyyyMmDd)
{
    m_date = norm(yyyyMmDd);
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setStatusFilter(const QString &status)
{
    m_status = norm(status);
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setPriceRangeEnabled(bool enabled)
{
    m_priceEnabled = enabled;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setMinPrice(double value)
{
    m_minPrice = value;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setMaxPrice(double value)
{
    m_maxPrice = value;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setAvailableAtEnabled(bool enabled)
{
    m_availableAtEnabled = enabled;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setAvailableAt(const QDateTime &dt)
{
    m_availableAt = dt;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setRangeAvailabilityEnabled(bool enabled)
{
    m_rangeEnabled = enabled;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setRangeStart(const QDateTime &dt)
{
    m_rangeStart = dt;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setRangeEnd(const QDateTime &dt)
{
    m_rangeEnd = dt;
    invalidateFilter();
}

void QuaiSortFilterProxyModel::setSortMode(SortMode mode)
{
    if (m_sortMode == mode)
        return;
    m_sortMode = mode;
    invalidate();
}

QuaiSortFilterProxyModel::SortMode QuaiSortFilterProxyModel::sortMode() const
{
    return m_sortMode;
}

static bool isOccupied(const QString &etat)
{
    return etat.compare("Occupé", Qt::CaseInsensitive) == 0 || etat.compare("Occupe", Qt::CaseInsensitive) == 0;
}

static bool isFree(const QString &etat)
{
    return etat.compare("Libre", Qt::CaseInsensitive) == 0;
}

static bool isMaintenance(const QString &etat)
{
    return etat.contains("maintenance", Qt::CaseInsensitive);
}

static bool intervalOverlaps(const QDateTime &a1, const QDateTime &a2, const QDateTime &b1, const QDateTime &b2)
{
    // Overlap for closed-open intervals: [a1,a2) overlaps [b1,b2) if max(start) < min(end)
    if (!a1.isValid() || !a2.isValid() || !b1.isValid() || !b2.isValid())
        return false;
    const QDateTime start = (a1 > b1) ? a1 : b1;
    const QDateTime end = (a2 < b2) ? a2 : b2;
    return start < end;
}

bool QuaiSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex idxId = sourceModel()->index(sourceRow, QuaiModel::IdQuai, sourceParent);
    const QModelIndex idxMat = sourceModel()->index(sourceRow, QuaiModel::Matricule, sourceParent);
    const QModelIndex idxArr = sourceModel()->index(sourceRow, QuaiModel::Arrivee, sourceParent);
    const QModelIndex idxDep = sourceModel()->index(sourceRow, QuaiModel::Depart, sourceParent);
    const QModelIndex idxEtat = sourceModel()->index(sourceRow, QuaiModel::Etat, sourceParent);
    const QModelIndex idxPrix = sourceModel()->index(sourceRow, QuaiModel::Prix, sourceParent);

    const QString id = norm(sourceModel()->data(idxId, Qt::DisplayRole).toString());
    const QString mat = norm(sourceModel()->data(idxMat, Qt::DisplayRole).toString());
    const QString arrStr = norm(sourceModel()->data(idxArr, Qt::DisplayRole).toString());
    const QString etat = sourceModel()->data(idxEtat, Qt::DisplayRole).toString();

    const QDateTime arrivee = sourceModel()->data(idxArr, QuaiModel::SortValueRole).toDateTime();
    const QDateTime depart = sourceModel()->data(idxDep, QuaiModel::SortValueRole).toDateTime();
    const double prix = sourceModel()->data(idxPrix, QuaiModel::SortValueRole).toDouble();

    if (!m_idQuai.isEmpty() && !id.contains(m_idQuai))
        return false;
    if (!m_matricule.isEmpty() && !mat.contains(m_matricule))
        return false;
    if (!m_date.isEmpty())
    {
        // Expected input like "2026-01-31"; accept prefix match in the arrival string.
        if (!arrStr.startsWith(m_date))
            return false;
    }

    if (!m_status.isEmpty() && m_status != norm(QStringLiteral("Tous")) && m_status != norm(QStringLiteral("All")))
    {
        if (norm(etat) != m_status)
            return false;
    }

    if (m_priceEnabled)
    {
        // If max is 0, treat it as 'no max' unless min is also 0.
        if (prix < m_minPrice)
            return false;
        if (m_maxPrice > 0.0 && prix > m_maxPrice)
            return false;
    }

    if (m_availableAtEnabled && m_availableAt.isValid())
    {
        // Maintenance is never available.
        if (isMaintenance(etat))
            return false;

        if (!isFree(etat))
        {
            // Occupied blocks inside [arrivee, depart)
            if (intervalOverlaps(arrivee, depart, m_availableAt, m_availableAt.addSecs(1)))
                return false;
        }
    }

    if (m_rangeEnabled && m_rangeStart.isValid() && m_rangeEnd.isValid())
    {
        if (m_rangeEnd <= m_rangeStart)
            return false;
        if (isMaintenance(etat))
            return false;
        if (!isFree(etat))
        {
            if (intervalOverlaps(arrivee, depart, m_rangeStart, m_rangeEnd))
                return false;
        }
    }

    return true;
}

bool QuaiSortFilterProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
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
