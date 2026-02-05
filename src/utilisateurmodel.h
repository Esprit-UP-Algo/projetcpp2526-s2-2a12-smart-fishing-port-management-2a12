#pragma once

#include <QAbstractTableModel>
#include <QVector>

#include "utilisateur.h"

class UtilisateurModel final : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns
    {
        ColId = 0,
        ColNom,
        ColPrenom,
        ColEmail,
        ColLogin,
        ColRole,
        ColStatut,
        ColDateCreation,
        ColCount
    };

    enum Roles
    {
        SortValueRole = Qt::UserRole + 1
    };

    explicit UtilisateurModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    const QVector<Utilisateur> &items() const { return m_items; }

    bool addUtilisateur(const Utilisateur &u, QString *errorOut = nullptr);
    bool updateUtilisateur(int row, const Utilisateur &u, QString *errorOut = nullptr);
    bool removeUtilisateur(int row, QString *errorOut = nullptr);

    int indexOfId(const QString &id) const;

private:
    bool validate(const Utilisateur &u, QString *errorOut) const;

    QVector<Utilisateur> m_items;
};
