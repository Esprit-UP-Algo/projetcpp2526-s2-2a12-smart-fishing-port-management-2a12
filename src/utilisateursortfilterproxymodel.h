#pragma once

#include <QSortFilterProxyModel>

class UtilisateurModel;

class UtilisateurSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UtilisateurSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchNom(const QString &text);
    void setSearchEmail(const QString &text);
    void setSearchRole(const QString &text);
    void setSearchStatut(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchNom;
    QString m_searchEmail;
    QString m_searchRole;
    QString m_searchStatut;
};
