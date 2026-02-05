#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "transaction.h"

class TransactionModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns
    {
        ColIdVente = 0,
        ColClient,
        ColLot,
        ColPrixKg,
        ColQuantite,
        ColTotal,
        ColDate,
        ColModePaiement,
        ColStatutPaiement,
        ColCount
    };

    enum Roles
    {
        SortValueRole = Qt::UserRole + 1
    };

    explicit TransactionModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<Transaction> &items() const { return m_items; }

    bool addTransaction(const Transaction &t, QString *errorOut = nullptr);
    bool updateTransaction(int row, const Transaction &t, QString *errorOut = nullptr);
    bool removeTransaction(int row, QString *errorOut = nullptr);

    int indexOfId(const QString &idVente) const;

private:
    bool validate(const Transaction &t, QString *errorOut) const;

    QVector<Transaction> m_items;
};
