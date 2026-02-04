#include "capturelotmodel.h"

CaptureLotModel::CaptureLotModel(QObject *parent)
    : QAbstractTableModel(parent)
{
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
    return 8; // ID, Espèce, Poids, Zone, Date Capture, Navire, Qualité, Date Entrée
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
        case 2: return QString::number(lot.poids);
        case 3: return lot.zonePeche;
        case 4: return lot.dateCapture.toString("yyyy-MM-dd HH:mm");
        case 5: return lot.navire;
        case 6: return lot.qualite;
        case 7: return lot.dateEntreeStock.toString("yyyy-MM-dd HH:mm");
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
    case 5: return QObject::tr("Navire");
    case 6: return QObject::tr("Qualité");
    case 7: return QObject::tr("Date Entrée Stock");
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
