#ifndef CAPTURELOTMODEL_H
#define CAPTURELOTMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "capturelot.h"

class CaptureLotModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CaptureLotModel(QObject *parent = nullptr);

    // Getter
    const QList<CaptureLot> &items() const { return m_lots; }
    int indexOfId(const QString &idLot) const;

    // Model interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // CRUD
    bool addLot(const CaptureLot &lot, QString *error = nullptr);
    bool updateLot(int row, const CaptureLot &lot, QString *error = nullptr);
    bool removeLot(int row, QString *error = nullptr);
    void clear();

private:
    QList<CaptureLot> m_lots;
};

#endif // CAPTURELOTMODEL_H
