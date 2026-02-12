#pragma once

#include <QSortFilterProxyModel>

namespace stockage {

class StockageLotSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit StockageLotSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchLot(const QString &text);
    void setSearchChambre(const QString &text);
    void setSearchEspece(const QString &text);
    void setShowOnlyAssigned(bool onlyAssigned);
    void setShowOnlyUnassigned(bool onlyUnassigned);
    
    void refresh();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchLot;
    QString m_searchChambre;
    QString m_searchEspece;
    bool m_showOnlyAssigned = false;
    bool m_showOnlyUnassigned = false;
};

} // namespace stockage
