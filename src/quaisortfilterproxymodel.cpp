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

bool QuaiSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const QModelIndex idxId = sourceModel()->index(sourceRow, QuaiModel::IdQuai, sourceParent);
    const QModelIndex idxMat = sourceModel()->index(sourceRow, QuaiModel::Matricule, sourceParent);
    const QModelIndex idxArr = sourceModel()->index(sourceRow, QuaiModel::Arrivee, sourceParent);

    const QString id = norm(sourceModel()->data(idxId, Qt::DisplayRole).toString());
    const QString mat = norm(sourceModel()->data(idxMat, Qt::DisplayRole).toString());
    const QString arrStr = norm(sourceModel()->data(idxArr, Qt::DisplayRole).toString());

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

    return true;
}

bool QuaiSortFilterProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
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
