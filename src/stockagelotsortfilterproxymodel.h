#pragma once

#include <QSortFilterProxyModel>

class StockageLotModel;

class StockageLotSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit StockageLotSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchLot(const QString &text);
    void setSearchChambre(const QString &text);
    void setSearchEspece(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchLot;
    QString m_searchChambre;
    QString m_searchEspece;
};
