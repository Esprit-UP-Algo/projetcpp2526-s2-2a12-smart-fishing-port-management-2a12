#ifndef CAPTURELOTPROXYMODEL_H
#define CAPTURELOTPROXYMODEL_H

#include <QSortFilterProxyModel>

class CaptureLotProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit CaptureLotProxyModel(QObject *parent = nullptr);

public slots:
    void setSearchLot(const QString &text);
    void setSearchEspece(const QString &text);
    void setSearchNavire(const QString &text);
    void setSearchZone(const QString &text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_searchLot;
    QString m_searchEspece;
    QString m_searchNavire;
    QString m_searchZone;
};

#endif // CAPTURELOTPROXYMODEL_H
