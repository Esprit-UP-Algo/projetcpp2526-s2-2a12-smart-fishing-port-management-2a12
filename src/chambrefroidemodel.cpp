#include "chambrefroidemodel.h"

#include <QLocale>

ChambreFroideModel::ChambreFroideModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Seeded demo data (can be removed later)
    m_items.push_back(ChambreFroide{QStringLiteral("CF-01"), 5000, -18.0, QStringLiteral("Zone A"), -17.4});
    m_items.push_back(ChambreFroide{QStringLiteral("CF-02"), 3500, -10.0, QStringLiteral("Zone B"), -9.6});
    m_items.push_back(ChambreFroide{QStringLiteral("CF-03"), 4200, -5.0, QStringLiteral("Zone C"), -4.8});
}

int ChambreFroideModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int ChambreFroideModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColCount;
}

QVariant ChambreFroideModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int row = index.row();
    const int col = index.column();
    if (row < 0 || row >= m_items.size() || col < 0 || col >= ColCount)
        return {};

    const auto &c = m_items.at(row);
    const QLocale loc;

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case ColIdChambre:
            return c.idChambre;
        case ColCapacite:
            return c.capacite;
        case ColTempConsigne:
            return loc.toString(c.tempConsigne, 'f', 1);
        case ColEmplacement:
            return c.emplacement;
        case ColTemperature:
            return loc.toString(c.temperature, 'f', 1);
        default:
            return {};
        }
    }

    if (role == SortValueRole)
    {
        switch (col)
        {
        case ColIdChambre:
            return c.idChambre;
        case ColCapacite:
            return c.capacite;
        case ColTempConsigne:
            return c.tempConsigne;
        case ColEmplacement:
            return c.emplacement;
        case ColTemperature:
            return c.temperature;
        default:
            return {};
        }
    }

    return {};
}

QVariant ChambreFroideModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case ColIdChambre:
            return QStringLiteral("IDChambre");
        case ColCapacite:
            return QStringLiteral("Capacité");
        case ColTempConsigne:
            return QStringLiteral("TempConsigne");
        case ColEmplacement:
            return QStringLiteral("Emplacement");
        case ColTemperature:
            return QStringLiteral("Température");
        default:
            return {};
        }
    }

    return section + 1;
}

int ChambreFroideModel::indexOfId(const QString &idChambre) const
{
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (m_items.at(i).idChambre.compare(idChambre, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

bool ChambreFroideModel::validate(const ChambreFroide &c, QString *errorOut) const
{
    if (c.idChambre.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("IDChambre est obligatoire.");
        return false;
    }

    if (c.capacite <= 0)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Capacité doit être > 0.");
        return false;
    }

    if (c.emplacement.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Emplacement est obligatoire.");
        return false;
    }

    return true;
}

bool ChambreFroideModel::addChambre(const ChambreFroide &c, QString *errorOut)
{
    if (!validate(c, errorOut))
        return false;

    if (indexOfId(c.idChambre) >= 0)
    {
        if (errorOut)
            *errorOut = QStringLiteral("IDChambre existe déjà.");
        return false;
    }

    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(c);
    endInsertRows();
    return true;
}

bool ChambreFroideModel::updateChambre(int row, const ChambreFroide &c, QString *errorOut)
{
    if (row < 0 || row >= m_items.size())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Ligne invalide.");
        return false;
    }

    if (!validate(c, errorOut))
        return false;

    const int existing = indexOfId(c.idChambre);
    if (existing >= 0 && existing != row)
    {
        if (errorOut)
            *errorOut = QStringLiteral("IDChambre existe déjà.");
        return false;
    }

    m_items[row] = c;
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
    return true;
}

bool ChambreFroideModel::removeChambre(int row, QString *errorOut)
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
