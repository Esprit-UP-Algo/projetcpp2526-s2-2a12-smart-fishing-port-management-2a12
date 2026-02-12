#pragma once

#include <QSortFilterProxyModel>

namespace stockage {

class ChambreFroideSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ChambreFroideSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchIdChambre(const QString &text);
    void setSearchEmplacement(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchId;
    QString m_searchEmplacement;
};

} // namespace stockage
