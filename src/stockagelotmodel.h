#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "stockagelot.h"

class StockageLotModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns
    {
        ColIdLot = 0,
        ColEspece,
        ColQuantite,
        ColDateEntree,
        ColDateSortie,
        ColIdChambre,
        ColDureeJours,
        ColCount
    };

    enum Roles
    {
        SortValueRole = Qt::UserRole + 1
    };

    explicit StockageLotModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<StockageLot> &items() const { return m_items; }

    bool addLot(const StockageLot &lot, QString *errorOut = nullptr);
    bool updateLot(int row, const StockageLot &lot, QString *errorOut = nullptr);
    bool removeLot(int row, QString *errorOut = nullptr);

    int indexOfId(const QString &idLot) const;

private:
    bool validate(const StockageLot &lot, QString *errorOut) const;

    QVector<StockageLot> m_items;
};
