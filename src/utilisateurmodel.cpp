#include "utilisateurmodel.h"

#include <QLocale>
#include <limits>

static QString fmtDate(const QDateTime &dt)
{
    if (!dt.isValid())
        return QStringLiteral("—");
    return dt.toString("yyyy-MM-dd HH:mm");
}

UtilisateurModel::UtilisateurModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    const QDateTime now = QDateTime::currentDateTime();

    // Seeded demo data
    m_items.push_back(Utilisateur{QStringLiteral("U-001"), QStringLiteral("Dupont"), QStringLiteral("Alice"), QStringLiteral("alice@example.com"), QStringLiteral("alice"), QStringLiteral("pwd"), QStringLiteral("Admin"), QStringLiteral("Actif"), now.addDays(-30)});
    m_items.push_back(Utilisateur{QStringLiteral("U-002"), QStringLiteral("Martin"), QStringLiteral("Bob"), QStringLiteral("bob@example.com"), QStringLiteral("bob"), QStringLiteral("pwd"), QStringLiteral("User"), QStringLiteral("Inactif"), now.addDays(-10)});
}

int UtilisateurModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int UtilisateurModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColCount;
}

QVariant UtilisateurModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row = index.row();
    const int col = index.column();
    if (row < 0 || row >= m_items.size() || col < 0 || col >= ColCount)
        return {};

    const auto &u = m_items.at(row);
    const QLocale loc;

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case ColId:
            return u.idUtilisateur;
        case ColNom:
            return u.nom;
        case ColPrenom:
            return u.prenom;
        case ColEmail:
            return u.email;
        case ColLogin:
            return u.login;
        case ColRole:
            return u.role;
        case ColStatut:
            return u.statut;
        case ColDateCreation:
            return fmtDate(u.dateCreation);
        default:
            return {};
        }
    }

    if (role == SortValueRole)
    {
        switch (col)
        {
        case ColId:
            return u.idUtilisateur;
        case ColNom:
            return u.nom;
        case ColPrenom:
            return u.prenom;
        case ColEmail:
            return u.email;
        case ColLogin:
            return u.login;
        case ColRole:
            return u.role;
        case ColStatut:
            return u.statut;
        case ColDateCreation:
            return u.dateCreation.isValid() ? u.dateCreation.toSecsSinceEpoch() : 0;
        default:
            return {};
        }
    }

    return {};
}

QVariant UtilisateurModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case ColId:
            return QStringLiteral("ID");
        case ColNom:
            return QStringLiteral("Nom");
        case ColPrenom:
            return QStringLiteral("Prénom");
        case ColEmail:
            return QStringLiteral("Email");
        case ColLogin:
            return QStringLiteral("Login");
        case ColRole:
            return QStringLiteral("Rôle");
        case ColStatut:
            return QStringLiteral("Statut");
        case ColDateCreation:
            return QStringLiteral("Date création");
        default:
            return {};
        }
    }

    return section + 1;
}

int UtilisateurModel::indexOfId(const QString &id) const
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i).idUtilisateur.compare(id, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

bool UtilisateurModel::validate(const Utilisateur &u, QString *errorOut) const
{
    if (u.idUtilisateur.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("ID utilisateur est obligatoire.");
        return false;
    }

    if (u.nom.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Nom est obligatoire.");
        return false;
    }

    if (u.prenom.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Prénom est obligatoire.");
        return false;
    }

    if (u.email.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Email est obligatoire.");
        return false;
    }

    if (u.login.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Login est obligatoire.");
        return false;
    }

    return true;
}

bool UtilisateurModel::addUtilisateur(const Utilisateur &u, QString *errorOut)
{
    if (!validate(u, errorOut))
        return false;

    if (indexOfId(u.idUtilisateur) >= 0)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Utilisateur existe déjà.");
        return false;
    }

    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(u);
    endInsertRows();
    return true;
}

bool UtilisateurModel::updateUtilisateur(int row, const Utilisateur &u, QString *errorOut)
{
    if (row < 0 || row >= m_items.size())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Ligne invalide.");
        return false;
    }

    if (!validate(u, errorOut))
        return false;

    const int existing = indexOfId(u.idUtilisateur);
    if (existing >= 0 && existing != row)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Utilisateur existe déjà.");
        return false;
    }

    m_items[row] = u;
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
    return true;
}

bool UtilisateurModel::removeUtilisateur(int row, QString *errorOut)
{
    if (row < 0 || row >= m_items.size())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Ligne invalide.");
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    return true;
}
