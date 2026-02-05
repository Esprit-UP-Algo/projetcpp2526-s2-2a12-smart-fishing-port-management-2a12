#include "utilisateursortfilterproxymodel.h"

#include <QAbstractItemModel>

#include "utilisateurmodel.h"

UtilisateurSortFilterProxyModel::UtilisateurSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
    setSortRole(UtilisateurModel::SortValueRole);
}

void UtilisateurSortFilterProxyModel::setSearchNom(const QString &text)
{
    m_searchNom = text;
    invalidateFilter();
}

void UtilisateurSortFilterProxyModel::setSearchEmail(const QString &text)
{
    m_searchEmail = text;
    invalidateFilter();
}

void UtilisateurSortFilterProxyModel::setSearchRole(const QString &text)
{
    m_searchRole = text;
    invalidateFilter();
}

void UtilisateurSortFilterProxyModel::setSearchStatut(const QString &text)
{
    m_searchStatut = text;
    invalidateFilter();
}

bool UtilisateurSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    const auto *m = sourceModel();
    if (!m)
        return true;

    const QString nom = m->data(m->index(sourceRow, UtilisateurModel::ColNom, sourceParent), Qt::DisplayRole).toString();
    const QString email = m->data(m->index(sourceRow, UtilisateurModel::ColEmail, sourceParent), Qt::DisplayRole).toString();
    const QString role = m->data(m->index(sourceRow, UtilisateurModel::ColRole, sourceParent), Qt::DisplayRole).toString();
    const QString statut = m->data(m->index(sourceRow, UtilisateurModel::ColStatut, sourceParent), Qt::DisplayRole).toString();

    const bool okNom = m_searchNom.trimmed().isEmpty() || nom.contains(m_searchNom, Qt::CaseInsensitive);
    const bool okEmail = m_searchEmail.trimmed().isEmpty() || email.contains(m_searchEmail, Qt::CaseInsensitive);
    const bool okRole = m_searchRole.trimmed().isEmpty() || role.contains(m_searchRole, Qt::CaseInsensitive);
    const bool okStatut = m_searchStatut.trimmed().isEmpty() || statut.contains(m_searchStatut, Qt::CaseInsensitive);

    return okNom && okEmail && okRole && okStatut;
}
