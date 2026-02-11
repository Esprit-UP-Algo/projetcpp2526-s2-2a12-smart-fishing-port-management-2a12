#include "capturelotmodel.h"

CaptureLotModel::CaptureLotModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Données de démonstration
    addLot(CaptureLot("LOT-001", "Sardine", 250, "Atlantique", QDateTime(QDate(2024, 1, 15), QTime(8, 30)), "Stella Maris", "Premium", QDateTime(QDate(2024, 1, 15), QTime(14, 0))));
    addLot(CaptureLot("LOT-002", "Anchois", 180, "Méditerranée", QDateTime(QDate(2024, 1, 14), QTime(10, 15)), "Marie-Jeanne", "Standard", QDateTime(QDate(2024, 1, 15), QTime(9, 30))));
    addLot(CaptureLot("LOT-003", "Thon", 500, "Atlantique", QDateTime(QDate(2024, 1, 15), QTime(12, 0)), "Neptune", "Premium", QDateTime(QDate(2024, 1, 15), QTime(18, 0))));
    addLot(CaptureLot("LOT-004", "Morue", 320, "Manche", QDateTime(QDate(2024, 1, 13), QTime(6, 45)), "Chalutier Breton", "Économique", QDateTime(QDate(2024, 1, 14), QTime(11, 0))));
}

int CaptureLotModel::indexOfId(const QString &idLot) const
{
    for (int i = 0; i < m_lots.size(); ++i)
    {
        if (m_lots.at(i).idLot.compare(idLot, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

int CaptureLotModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_lots.count();
}

int CaptureLotModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 9; // ID, Espèce, Poids, Zone, Date Capture, Etat, Navire, Qualité, Date Entrée
}

QVariant CaptureLotModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_lots.size())
        return QVariant();

    const auto &lot = m_lots.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case 0: return lot.idLot;
        case 1: return lot.espece;
        case 2: // Poids
            if (role == Qt::DisplayRole)
                return QString::number(lot.poids) + " kg";
            return lot.poids; // Pour le tri (EditRole)

        case 3: return lot.zonePeche;
        case 4: // Date Capture
            if (role == Qt::DisplayRole)
                return lot.dateCapture.toString("yyyy-MM-dd HH:mm");
            return lot.dateCapture; // Pour le tri

        case 5: return lot.etat;
        case 6: return lot.navire;
        case 7: return lot.qualite;
        case 8: // Date Stock
            if (role == Qt::DisplayRole)
                return lot.dateEntreeStock.toString("yyyy-MM-dd HH:mm");
            return lot.dateEntreeStock; // Pour le tri
            
        default: return QVariant();
        }
    }

    return QVariant();
}

QVariant CaptureLotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case 0: return QObject::tr("ID Lot");
    case 1: return QObject::tr("Espèce");
    case 2: return QObject::tr("Poids (kg)");
    case 3: return QObject::tr("Zone Pêche");
    case 4: return QObject::tr("Date Capture");
    case 5: return QObject::tr("Etat");
    case 6: return QObject::tr("Navire");
    case 7: return QObject::tr("Qualité");
    case 8: return QObject::tr("Date Entrée Stock");
    default: return QVariant();
    }
}

Qt::ItemFlags CaptureLotModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool CaptureLotModel::addLot(const CaptureLot &lot, QString *error)
{
    if (!lot.isValid())
    {
        if (error)
            *error = QObject::tr("Données invalides");
        return false;
    }

    // Vérifier l'unicité de l'ID
    if (indexOfId(lot.idLot) >= 0)
    {
        if (error)
            *error = QObject::tr("Un lot avec cet ID existe déjà");
        return false;
    }

    beginInsertRows(QModelIndex(), m_lots.size(), m_lots.size());
    m_lots.append(lot);
    endInsertRows();

    return true;
}

bool CaptureLotModel::updateLot(int row, const CaptureLot &lot, QString *error)
{
    if (row < 0 || row >= m_lots.size())
    {
        if (error)
            *error = QObject::tr("Index invalide");
        return false;
    }

    if (!lot.isValid())
    {
        if (error)
            *error = QObject::tr("Données invalides");
        return false;
    }

    // Vérifier que l'ID ne duplique pas un autre lot
    const int idx = indexOfId(lot.idLot);
    if (idx >= 0 && idx != row)
    {
        if (error)
            *error = QObject::tr("Un lot avec cet ID existe déjà");
        return false;
    }

    m_lots[row] = lot;
    emit dataChanged(index(row, 0), index(row, columnCount() - 1));

    return true;
}

bool CaptureLotModel::removeLot(int row, QString *error)
{
    if (row < 0 || row >= m_lots.size())
    {
        if (error)
            *error = QObject::tr("Index invalide");
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_lots.removeAt(row);
    endRemoveRows();

    return true;
}

void CaptureLotModel::clear()
{
    if (!m_lots.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, m_lots.size() - 1);
        m_lots.clear();
        endRemoveRows();
    }
}

QList<CaptureLot> CaptureLotModel::searchByEspece(const QString &espece) const
{
    QList<CaptureLot> out;
    for (const auto &lot : m_lots)
    {
        if (lot.espece.contains(espece, Qt::CaseInsensitive))
            out.append(lot);
    }
    return out;
}

QList<CaptureLot> CaptureLotModel::searchByZone(const QString &zone) const
{
    QList<CaptureLot> out;
    for (const auto &lot : m_lots)
    {
        if (lot.zonePeche.contains(zone, Qt::CaseInsensitive))
            out.append(lot);
    }
    return out;
}

QList<CaptureLot> CaptureLotModel::searchByEtat(const QString &etat) const
{
    QList<CaptureLot> out;
    for (const auto &lot : m_lots)
    {
        if (lot.etat.contains(etat, Qt::CaseInsensitive))
            out.append(lot);
    }
    return out;
}
