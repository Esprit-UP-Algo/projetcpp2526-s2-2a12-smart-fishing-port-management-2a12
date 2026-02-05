#include "stockagelotmodel.h"

#include <limits>

#include <QLocale>

static QString fmtDate(const QDateTime &dt)
{
    if (!dt.isValid())
        return QStringLiteral("—");
    return dt.toString("yyyy-MM-dd HH:mm");
}

StockageLotModel::StockageLotModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    const QDateTime now = QDateTime::currentDateTime();

    // Seeded demo data
    m_items.push_back(StockageLot{QStringLiteral("LOT-100"), QStringLiteral("Sardine"), 800, now.addDays(-4), QDateTime(), QStringLiteral("CF-01")});
    m_items.push_back(StockageLot{QStringLiteral("LOT-101"), QStringLiteral("Thon"), 1200, now.addDays(-2), QDateTime(), QStringLiteral("CF-02")});
    m_items.push_back(StockageLot{QStringLiteral("LOT-102"), QStringLiteral("Dorade"), 600, now.addDays(-10), now.addDays(-1), QStringLiteral("CF-01")});
}

int StockageLotModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int StockageLotModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColCount;
}

QVariant StockageLotModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row = index.row();
    const int col = index.column();
    if (row < 0 || row >= m_items.size() || col < 0 || col >= ColCount)
        return {};

    const auto &lot = m_items.at(row);
    const QLocale loc;

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case ColIdLot:
            return lot.idLot;
        case ColEspece:
            return lot.espece;
        case ColQuantite:
            return lot.quantite;
        case ColDateEntree:
            return fmtDate(lot.dateEntree);
        case ColDateSortie:
            return fmtDate(lot.dateSortie);
        case ColIdChambre:
            return lot.idChambre;
        case ColDureeJours:
        {
            const qint64 secs = lot.dureeSecondes();
            const int days = int(secs / (24 * 3600));
            return loc.toString(days);
        }
        default:
            return {};
        }
    }

    if (role == SortValueRole)
    {
        switch (col)
        {
        case ColIdLot:
            return lot.idLot;
        case ColEspece:
            return lot.espece;
        case ColQuantite:
            return lot.quantite;
        case ColDateEntree:
            return lot.dateEntree.isValid() ? lot.dateEntree.toSecsSinceEpoch() : 0;
        case ColDateSortie:
            return lot.dateSortie.isValid() ? lot.dateSortie.toSecsSinceEpoch() : std::numeric_limits<qint64>::max();
        case ColIdChambre:
            return lot.idChambre;
        case ColDureeJours:
            return int(lot.dureeSecondes() / (24 * 3600));
        default:
            return {};
        }
    }

    return {};
}

QVariant StockageLotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case ColIdLot:
            return QStringLiteral("Lot");
        case ColEspece:
            return QStringLiteral("Espèce");
        case ColQuantite:
            return QStringLiteral("Quantité");
        case ColDateEntree:
            return QStringLiteral("Date entrée");
        case ColDateSortie:
            return QStringLiteral("Date sortie");
        case ColIdChambre:
            return QStringLiteral("Chambre");
        case ColDureeJours:
            return QStringLiteral("Durée (j)");
        default:
            return {};
        }
    }

    return section + 1;
}

int StockageLotModel::indexOfId(const QString &idLot) const
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i).idLot.compare(idLot, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

bool StockageLotModel::validate(const StockageLot &lot, QString *errorOut) const
{
    if (lot.idLot.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("ID lot est obligatoire.");
        return false;
    }

    if (lot.espece.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Espèce est obligatoire.");
        return false;
    }

    if (lot.quantite <= 0)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Quantité doit être > 0.");
        return false;
    }

    if (!lot.dateEntree.isValid())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Date entrée invalide.");
        return false;
    }

    if (lot.dateSortie.isValid() && lot.dateSortie < lot.dateEntree)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Date sortie doit être >= date entrée.");
        return false;
    }

    if (lot.idChambre.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("IDChambre est obligatoire.");
        return false;
    }

    return true;
}

bool StockageLotModel::addLot(const StockageLot &lot, QString *errorOut)
{
    if (!validate(lot, errorOut))
        return false;

    if (indexOfId(lot.idLot) >= 0)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Lot existe déjà.");
        return false;
    }

    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(lot);
    endInsertRows();
    return true;
}

bool StockageLotModel::updateLot(int row, const StockageLot &lot, QString *errorOut)
{
    if (row < 0 || row >= m_items.size())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Ligne invalide.");
        return false;
    }

    if (!validate(lot, errorOut))
        return false;

    const int existing = indexOfId(lot.idLot);
    if (existing >= 0 && existing != row)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Lot existe déjà.");
        return false;
    }

    m_items[row] = lot;
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
    return true;
}

bool StockageLotModel::removeLot(int row, QString *errorOut)
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
