#pragma once

#include <QSortFilterProxyModel>
#include <QDate>

class QuaiSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit QuaiSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchMatricule(const QString &text);
    void setSearchIdQuai(const QString &text);
    void setSearchDate(const QString &yyyyMmDd);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    QString m_matricule;
    QString m_idQuai;
    QString m_date; // yyyy-MM-dd prefix match

    static QString norm(const QString &s);
};
