#include "quaimodel.h"

#include <QLocale>
#include <QRegularExpression>

QuaiModel::QuaiModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    // Seed sample rows so the UI matches the reference immediately.
    m_items.push_back(Quai{QStringLiteral("Q-101"),
                           QStringLiteral("MQ-401"),
                           QString::fromUtf8(u8"Cargo"),
                           45.0,
                           12.0,
                           28.0,
                           QDateTime::fromString(QStringLiteral("2026-02-01 08:30"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QDateTime::fromString(QStringLiteral("2026-02-01 18:30"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QString::fromUtf8(u8"Occupé"),
                           120.0});

    m_items.push_back(Quai{QStringLiteral("Q-102"),
                           QStringLiteral("MQ-233"),
                           QString::fromUtf8(u8"Plaisance"),
                           32.5,
                           35.0,
                           18.0,
                           QDateTime::fromString(QStringLiteral("2026-02-01 10:15"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QDateTime::fromString(QStringLiteral("2026-02-01 22:00"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QStringLiteral("Libre"),
                           95.0});

    m_items.push_back(Quai{QStringLiteral("Q-103"),
                           QStringLiteral("MQ-778"),
                           QString::fromUtf8(u8"Général"),
                           60.0,
                           55.0,
                           44.0,
                           QDateTime::fromString(QStringLiteral("2026-02-03 06:00"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QDateTime::fromString(QStringLiteral("2026-02-03 20:30"), QStringLiteral("yyyy-MM-dd HH:mm")),
                           QStringLiteral("En maintenance"),
                           0.0});

    // In-memory traceability log (snapshots). This enables multiple occupation/revenue records per quai
    // without requiring persistence.
    m_history = m_items;

    // Seed a few additional historical records so history/rotation panels show meaningful values.
    {
        Quai h = m_items.at(0);
        h.dateHeureArrivee = QDateTime::fromString(QStringLiteral("2026-01-30 07:00"), QStringLiteral("yyyy-MM-dd HH:mm"));
        h.dateHeureDepart = QDateTime::fromString(QStringLiteral("2026-01-30 15:00"), QStringLiteral("yyyy-MM-dd HH:mm"));
        h.etatQuai = QString::fromUtf8(u8"Occupé");
        m_history.push_back(h);

        h.dateHeureArrivee = QDateTime::fromString(QStringLiteral("2026-01-31 09:00"), QStringLiteral("yyyy-MM-dd HH:mm"));
        h.dateHeureDepart = QDateTime::fromString(QStringLiteral("2026-01-31 20:00"), QStringLiteral("yyyy-MM-dd HH:mm"));
        m_history.push_back(h);
    }

    {
        Quai h = m_items.at(1);
        h.dateHeureArrivee = QDateTime::fromString(QStringLiteral("2026-01-29 10:00"), QStringLiteral("yyyy-MM-dd HH:mm"));
        h.dateHeureDepart = QDateTime::fromString(QStringLiteral("2026-01-29 18:30"), QStringLiteral("yyyy-MM-dd HH:mm"));
        h.etatQuai = QString::fromUtf8(u8"Occupé");
        m_history.push_back(h);
    }
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
        case Taille:
            return QLocale().toString(q.taille, 'f', 1);
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

    if (role == Qt::TextAlignmentRole)
    {
        return static_cast<int>(Qt::AlignCenter);
    }

    if (role == SortValueRole)
    {
        switch (index.column())
        {
        case Taille:
            return q.taille;
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
        return tr("Matricule quai");
    case Taille:
        return tr("Taille (m)");
    case Arrivee:
        return tr("Arrivée");
    case Depart:
        return tr("Départ");
    case Etat:
        return tr("État");
    case Prix:
        return tr("Prix (DT/h)");
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

const QVector<Quai> &QuaiModel::history() const
{
    return m_history;
}

QVector<Quai> QuaiModel::historyForId(const QString &idQuai) const
{
    QVector<Quai> out;
    const QString key = normalize(idQuai);
    if (key.isEmpty())
        return out;
    out.reserve(8);
    for (const auto &q : m_history)
    {
        if (normalize(q.idQuai) == key)
            out.push_back(q);
    }
    return out;
}

QString QuaiModel::normalize(const QString &s)
{
    return s.trimmed().toLower();
}

bool QuaiModel::validate(const Quai &quai, QString *error)
{
    if (normalize(quai.matricule).isEmpty())
    {
        if (error)
            *error = tr("Matricule est obligatoire.");
        return false;
    }
    if (quai.taille < 0.0)
    {
        if (error)
            *error = tr("La taille du quai doit être >= 0.");
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

static QString nextAutoId(const QVector<Quai> &items)
{
    // Keep a familiar format like "Q-104".
    int maxNum = 0;
    const QRegularExpression re(QStringLiteral(R"(^\s*Q-(\d+)\s*$)"), QRegularExpression::CaseInsensitiveOption);
    for (const auto &q : items)
    {
        const auto m = re.match(q.idQuai);
        if (!m.hasMatch())
            continue;
        bool ok = false;
        const int n = m.captured(1).toInt(&ok);
        if (ok)
            maxNum = qMax(maxNum, n);
    }
    const int next = (maxNum > 0) ? (maxNum + 1) : 101;
    return QStringLiteral("Q-%1").arg(next);
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
    Quai toInsert = quai;
    if (normalize(toInsert.idQuai).isEmpty())
        toInsert.idQuai = nextAutoId(m_items);

    if (!validate(toInsert, error))
        return false;

    if (indexOfId(toInsert.idQuai) >= 0)
    {
        if (error)
            *error = tr("IDQuai existe déjà.");
        return false;
    }

    const int row = m_items.size();
    beginInsertRows(QModelIndex(), row, row);
    m_items.push_back(toInsert);
    endInsertRows();

    m_history.push_back(toInsert);
    return true;
}

bool QuaiModel::updateQuai(int row, const Quai &quai, QString *error)
{
    if (row < 0 || row >= m_items.size())
        return false;

    Quai toUpdate = quai;
    if (normalize(toUpdate.idQuai).isEmpty())
        toUpdate.idQuai = m_items.at(row).idQuai;

    if (!validate(toUpdate, error))
        return false;

    const int existing = indexOfId(toUpdate.idQuai);
    if (existing >= 0 && existing != row)
    {
        if (error)
            *error = tr("Un autre enregistrement possède déjà ce IDQuai.");
        return false;
    }

    m_items[row] = toUpdate;
    m_history.push_back(toUpdate);
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
