#pragma once

#include <QSortFilterProxyModel>

class QuaiSortProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum class SortMode
    {
        TableColumn = 0,
        TailleAsc,
        Statut,
        LiberationBientot,
        OccupeDepuisLongtemps,
        PrixAsc
    };
    Q_ENUM(SortMode)

    explicit QuaiSortProxyModel(QObject *parent = nullptr);

    void setSortMode(SortMode mode);
    SortMode sortMode() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    SortMode m_sortMode = SortMode::TableColumn;
};
