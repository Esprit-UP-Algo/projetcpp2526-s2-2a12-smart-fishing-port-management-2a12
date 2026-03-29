#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QVector>

#include "quai.h"

class QSqlRecord;

class QuaiModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Column
    {
        IdQuai = 0,
        Matricule,
        Taille,
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

    // (Re)load data from Oracle table QUAI.
    // If it fails, the model remains unchanged.
    bool reloadFromDatabase(QString *error = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    const QVector<Quai> &items() const;
    const QVector<Quai> &history() const;
    QVector<Quai> historyForId(const QString &idQuai) const;

    bool addQuai(const Quai &quai, QString *error = nullptr);
    bool updateQuai(int row, const Quai &quai, QString *error = nullptr);
    bool removeQuai(int row);
    bool removeQuai(int row, QString *error);

    int indexOfId(const QString &idQuai) const;

private:
    QVector<Quai> m_items;
    QVector<Quai> m_history;

    struct DbColumns
    {
        QString table = QStringLiteral("QUAI");
        QString id;
        QString matricule;
        QString type;
        QString taille;
        QString posX;
        QString posY;
        QString arrivee;
        QString depart;
        QString etat;
        QString prix;
        bool tailleIsText = false;
        bool arriveeIsDate = false;
        bool departIsDate = false;
        bool idIsNumeric = false;
        bool resolved = false;
    };

    DbColumns m_cols;

    bool ensureDbColumns(QString *error);
    static int fieldIndexOf(const QSqlRecord &rec, const QStringList &candidates);
    bool fetchAllFromDatabase(QVector<Quai> *out, QString *error);
    bool insertIntoDatabase(const Quai &quai, QString *error);
    bool updateInDatabase(const Quai &quai, QString *error);
    bool deleteFromDatabase(const QString &idQuai, QString *error);

    static QString normalize(const QString &s);
    static bool validate(const Quai &quai, QString *error, bool allowEmptyId);
};
