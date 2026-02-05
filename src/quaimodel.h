#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "quai.h"

class QuaiModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        IdQuai = 0,
        Matricule,
        Arrivee,
        Depart,
        Etat,
        Prix,
        Retard,
        ColumnCount
    };

    enum Role
    {
        SortValueRole = Qt::UserRole + 1
    };

    explicit QuaiModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    const QVector<Quai> &items() const;

    bool addQuai(const Quai &quai, QString *error = nullptr);
    bool updateQuai(int row, const Quai &quai, QString *error = nullptr);
    bool removeQuai(int row);

    int indexOfId(const QString &idQuai) const;

private:
    QVector<Quai> m_items;

    static QString normalize(const QString &s);
    static bool validate(const Quai &quai, QString *error);
};
