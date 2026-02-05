#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "chambrefroide.h"

class ChambreFroideModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns
    {
        ColIdChambre = 0,
        ColCapacite,
        ColTempConsigne,
        ColEmplacement,
        ColTemperature,
        ColCount
    };

    enum Roles
    {
        SortValueRole = Qt::UserRole + 1
    };

    explicit ChambreFroideModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<ChambreFroide> &items() const { return m_items; }

    bool addChambre(const ChambreFroide &c, QString *errorOut = nullptr);
    bool updateChambre(int row, const ChambreFroide &c, QString *errorOut = nullptr);
    bool removeChambre(int row, QString *errorOut = nullptr);

    int indexOfId(const QString &idChambre) const;

private:
    bool validate(const ChambreFroide &c, QString *errorOut) const;

    QVector<ChambreFroide> m_items;
};
