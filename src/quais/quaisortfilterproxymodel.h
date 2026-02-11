#pragma once

#include <QSortFilterProxyModel>
#include <QDate>

class QuaiSortFilterProxyModel final : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    enum class SortMode
    {
        TableColumn = 0,
        TailleAsc,
        Statut,
        LiberationBientot,
        OccupeDepuisLongtemps,
        PrixAsc
    };
    Q_ENUM(SortMode)

    explicit QuaiSortFilterProxyModel(QObject *parent = nullptr);

    void setSearchMatricule(const QString &text);
    void setSearchIdQuai(const QString &text);
    void setSearchDate(const QString &yyyyMmDd);

    void setStatusFilter(const QString &status);
    void setPriceRangeEnabled(bool enabled);
    void setMinPrice(double value);
    void setMaxPrice(double value);

    void setAvailableAtEnabled(bool enabled);
    void setAvailableAt(const QDateTime &dt);

    void setRangeAvailabilityEnabled(bool enabled);
    void setRangeStart(const QDateTime &dt);
    void setRangeEnd(const QDateTime &dt);

    void setSortMode(SortMode mode);
    SortMode sortMode() const;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    QString m_matricule;
    QString m_idQuai;
    QString m_date; // yyyy-MM-dd prefix match

    QString m_status;

    bool m_priceEnabled = false;
    double m_minPrice = 0.0;
    double m_maxPrice = 0.0;

    bool m_availableAtEnabled = false;
    QDateTime m_availableAt;

    bool m_rangeEnabled = false;
    QDateTime m_rangeStart;
    QDateTime m_rangeEnd;

    SortMode m_sortMode = SortMode::TableColumn;

    static QString norm(const QString &s);
};
