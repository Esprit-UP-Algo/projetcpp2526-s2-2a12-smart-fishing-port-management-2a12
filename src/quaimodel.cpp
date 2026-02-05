#include "quaimodel.h"

#include <QLocale>

QuaiModel::QuaiModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Seed sample rows so the UI matches the reference immediately.
    m_items.push_back({"Q-101", "NAV-401", QDateTime::fromString("2026-02-01 08:30", "yyyy-MM-dd HH:mm"), QDateTime::fromString("2026-02-01 18:30", "yyyy-MM-dd HH:mm"), "Occupé", 1200.0});
    m_items.push_back({"Q-102", "NAV-233", QDateTime::fromString("2026-02-01 10:15", "yyyy-MM-dd HH:mm"), QDateTime::fromString("2026-02-01 22:00", "yyyy-MM-dd HH:mm"), "Libre", 950.0});
    m_items.push_back({"Q-103", "NAV-778", QDateTime::fromString("2026-02-03 06:00", "yyyy-MM-dd HH:mm"), QDateTime::fromString("2026-02-03 20:30", "yyyy-MM-dd HH:mm"), "En maintenance", 0.0});
}

int QuaiModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.size();
}

int QuaiModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant QuaiModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_items.size())
        return {};

    const Quai &q = m_items.at(index.row());

    const auto dateFmt = QStringLiteral("yyyy-MM-dd HH:mm");

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case IdQuai:
            return q.idQuai;
        case Matricule:
            return q.matricule;
        case Arrivee:
            return q.dateHeureArrivee.toString(dateFmt);
        case Depart:
            return q.dateHeureDepart.toString(dateFmt);
        case Etat:
            return q.etatQuai;
        case Prix:
            return QLocale().toString(q.prixLocation, 'f', 2);
        case Retard:
            return q.retardMinutes();
        default:
            return {};
        }
    }

    if (role == SortValueRole)
    {
        switch (index.column())
        {
        case Arrivee:
            return q.dateHeureArrivee;
        case Depart:
            return q.dateHeureDepart;
        case Prix:
            return q.prixLocation;
        case Retard:
            return q.retardMinutes();
        default:
            return data(index, Qt::DisplayRole);
        }
    }

    return {};
}

QVariant QuaiModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};

    switch (section)
    {
    case IdQuai:
        return tr("ID Quai");
    case Matricule:
        return tr("Matricule");
    case Arrivee:
        return tr("Arrivée");
    case Depart:
        return tr("Départ");
    case Etat:
        return tr("État quai");
    case Prix:
        return tr("Prix location (DT)");
    case Retard:
        return tr("Retard (min)");
    default:
        return {};
    }
}

Qt::ItemFlags QuaiModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

const QVector<Quai> &QuaiModel::items() const
{
    return m_items;
}

QString QuaiModel::normalize(const QString &s)
{
    return s.trimmed().toLower();
}

bool QuaiModel::validate(const Quai &quai, QString *error)
{
    if (normalize(quai.idQuai).isEmpty())
    {
        if (error)
            *error = tr("IDQuai est obligatoire.");
        return false;
    }
    if (normalize(quai.matricule).isEmpty())
    {
        if (error)
            *error = tr("Matricule est obligatoire.");
        return false;
    }
    if (!quai.dateHeureArrivee.isValid() || !quai.dateHeureDepart.isValid())
    {
        if (error)
            *error = tr("Les dates/heures doivent être valides.");
        return false;
    }
    if (quai.dateHeureDepart < quai.dateHeureArrivee)
    {
        if (error)
            *error = tr("La date/heure départ doit être après l'arrivée.");
        return false;
    }
    if (quai.prixLocation < 0.0)
    {
        if (error)
            *error = tr("Le prix location doit être >= 0.");
        return false;
    }
    return true;
}

int QuaiModel::indexOfId(const QString &idQuai) const
{
    const QString key = normalize(idQuai);
    for (int i = 0; i < m_items.size(); ++i)
    {
        if (normalize(m_items.at(i).idQuai) == key)
            return i;
    }
    return -1;
}

bool QuaiModel::addQuai(const Quai &quai, QString *error)
{
    if (!validate(quai, error))
        return false;

    if (indexOfId(quai.idQuai) >= 0)
    {
        if (error)
            *error = tr("IDQuai existe déjà.");
        return false;
    }

    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(quai);
    endInsertRows();
    return true;
}

bool QuaiModel::updateQuai(int row, const Quai &quai, QString *error)
{
    if (row < 0 || row >= m_items.size())
        return false;

    if (!validate(quai, error))
        return false;

    const int existing = indexOfId(quai.idQuai);
    if (existing >= 0 && existing != row)
    {
        if (error)
            *error = tr("Un autre enregistrement possède déjà ce IDQuai.");
        return false;
    }

    m_items[row] = quai;
    emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
    return true;
}

bool QuaiModel::removeQuai(int row)
{
    if (row < 0 || row >= m_items.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    return true;
}
