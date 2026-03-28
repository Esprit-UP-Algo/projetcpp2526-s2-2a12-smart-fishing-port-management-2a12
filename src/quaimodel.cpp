#include "quaimodel.h"

#include "connection.h"

#include <QLocale>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QMetaType>

#include <functional>

static QString sqlErrorDetails(const QSqlError &e)
{
    QStringList parts;
    const QString t = e.text().trimmed();
    const QString drv = e.driverText().trimmed();
    const QString dbt = e.databaseText().trimmed();
    const QString native = e.nativeErrorCode().trimmed();

    if (!t.isEmpty())
        parts << t;
    if (!dbt.isEmpty() && dbt != t)
        parts << dbt;
    if (!drv.isEmpty() && drv != t)
        parts << drv;
    if (!native.isEmpty())
        parts << QStringLiteral("Native=%1").arg(native);

    return parts.isEmpty() ? QStringLiteral("SQL error") : parts.join(QStringLiteral(" | "));
}

static QString sqlQuote(const QString &s)
{
    QString t = s;
    // Oracle SQL escaping for string literals: single quote is doubled.
    t.replace(QLatin1Char('\''), QLatin1String("''"));
    return QStringLiteral("'") + t + QStringLiteral("'");
}

static QString sqlNumber(double v)
{
    // Locale-independent decimal point.
    QString s = QString::number(v, 'f', 6);
    // Trim trailing zeros.
    while (s.contains('.') && s.endsWith('0'))
        s.chop(1);
    if (s.endsWith('.'))
        s.chop(1);
    return s.isEmpty() ? QStringLiteral("0") : s;
}

static QDateTime toDateTimeSafe(const QVariant &v)
{
    if (!v.isValid() || v.isNull())
        return {};
    if (v.canConvert<QDateTime>())
        return v.toDateTime();
    if (v.canConvert<QDate>())
        return QDateTime(v.toDate(), QTime(0, 0));
    const QString s = v.toString().trimmed();
    if (s.isEmpty())
        return {};

    // Common formats seen with ODBC drivers.
    QDateTime dt = QDateTime::fromString(s, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    if (dt.isValid())
        return dt;
    dt = QDateTime::fromString(s, QStringLiteral("yyyy-MM-dd HH:mm"));
    if (dt.isValid())
        return dt;
    dt = QDateTime::fromString(s, Qt::ISODate);
    return dt;
}

static QString dbErrText(const QSqlQuery &q)
{
    const QString base = sqlErrorDetails(q.lastError());
    const QString lastQ = q.lastQuery().trimmed();
    if (!lastQ.isEmpty())
        return base + QStringLiteral(" | SQL=%1").arg(lastQ);
    return base;
}

static QVariant bindIdValue(const QString &idText, bool idIsNumeric, const QString &colName, QString *error)
{
    if (!idIsNumeric)
        return idText;

    const QString s = idText.trimmed();
    if (s.isEmpty())
        return {};

    bool ok = false;
    const qlonglong ll = s.toLongLong(&ok);
    if (ok)
        return ll;

    const double d = s.toDouble(&ok);
    if (ok)
        return d;

    if (error)
        *error = QObject::tr("ID Quai doit être numérique (colonne %1).").arg(colName);
    return {};
}

static QString toOracleDateTimeString(const QDateTime &dt)
{
    if (!dt.isValid())
        return {};
    // Oracle DATE supports time; use a stable, locale-independent format.
    return dt.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
}

QuaiModel::QuaiModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    QString err;
    if (!reloadFromDatabase(&err))
    {
        // Keep the model empty if DB load fails. CRUD operations will still
        // report actionable errors to the UI.
        if (!err.isEmpty())
            qWarning() << "QuaiModel: reloadFromDatabase failed:" << err;
    }
}

int QuaiModel::fieldIndexOf(const QSqlRecord &rec, const QStringList &candidates)
{
    for (const QString &name : candidates)
    {
        const int idx = rec.indexOf(name);
        if (idx >= 0)
            return idx;
        // Some ODBC drivers return upper-cased names.
        const int idx2 = rec.indexOf(name.toUpper());
        if (idx2 >= 0)
            return idx2;
        const int idx3 = rec.indexOf(name.toLower());
        if (idx3 >= 0)
            return idx3;
    }
    return -1;
}

bool QuaiModel::ensureDbColumns(QString *error)
{
    if (m_cols.resolved)
        return true;

    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isValid() || !db.isOpen())
    {
        if (error)
            *error = tr("Base de données non connectée.");
        return false;
    }

    QSqlQuery q(db);
    // We only need metadata.
    if (!q.exec(QStringLiteral("SELECT * FROM QUAI WHERE 1=0")))
    {
        if (error)
            *error = tr("Impossible de lire le schéma de la table QUAI: %1").arg(dbErrText(q));
        return false;
    }

    const QSqlRecord rec = q.record();
    if (rec.count() <= 0)
    {
        if (error)
            *error = tr("La table QUAI ne contient aucune colonne (schéma non lisible).");
        return false;
    }

    const auto pick = [&](const QStringList &cands) -> QString {
        const int idx = fieldIndexOf(rec, cands);
        return idx >= 0 ? rec.fieldName(idx) : QString();
    };

    const auto pickContains = [&](const QStringList &needles) -> QString {
        for (int i = 0; i < rec.count(); ++i)
        {
            const QString fn = rec.fieldName(i);
            for (const QString &n : needles)
            {
                if (fn.contains(n, Qt::CaseInsensitive))
                    return fn;
            }
        }
        return {};
    };

    // Try common Oracle naming variants.
    m_cols.id = pick({QStringLiteral("IDQUAI"),
                      QStringLiteral("ID_QUAI"),
                      QStringLiteral("ID"),
                      QStringLiteral("IDQ"),
                      QStringLiteral("ID_QUAI_PK")});

    m_cols.matricule = pick({QStringLiteral("MATRICULE"),
                             QStringLiteral("MATRICULEQUAI"),
                             QStringLiteral("MATRICULE_QUAI"),
                             QStringLiteral("MATRICULEQ"),
                             QStringLiteral("MAT_QUAI"),
                             QStringLiteral("MATRICULE_BATEAU"),
                             QStringLiteral("MATRICULE_NAVIRE"),
                             QStringLiteral("IMMATRICULATION"),
                             QStringLiteral("IMMATRICULE"),
                             QStringLiteral("MAT"),
                             QStringLiteral("MATR")});

    if (m_cols.matricule.isEmpty())
        m_cols.matricule = pickContains({QStringLiteral("MATRICULE"), QStringLiteral("IMMATRIC"), QStringLiteral("MAT")});

    m_cols.type = pick({QStringLiteral("TYPE_QUAI"), QStringLiteral("TYPEQUAI"), QStringLiteral("TYPE"), QStringLiteral("TYPEQ"), QStringLiteral("TYPE_QUAI_LIB")});
    m_cols.taille = pick({QStringLiteral("TAILLE"), QStringLiteral("TAILLE_M"), QStringLiteral("LONGUEUR"), QStringLiteral("LONGUEUR_M")});
    m_cols.posX = pick({QStringLiteral("POSITIONX"), QStringLiteral("POSITION_X"), QStringLiteral("POSX"), QStringLiteral("POS_X"), QStringLiteral("X"), QStringLiteral("POSITION_X")});
    m_cols.posY = pick({QStringLiteral("POSITIONY"), QStringLiteral("POSITION_Y"), QStringLiteral("POSY"), QStringLiteral("POS_Y"), QStringLiteral("Y"), QStringLiteral("POSITION_Y")});
    m_cols.arrivee = pick({QStringLiteral("DATEHEUREARRIVEE"), QStringLiteral("DATE_HEURE_ARRIVEE"), QStringLiteral("DATE_ARRIVEE"), QStringLiteral("ARRIVEE"), QStringLiteral("DATEARR")});
    m_cols.depart = pick({QStringLiteral("DATEHEUREDEPART"), QStringLiteral("DATE_HEURE_DEPART"), QStringLiteral("DATE_DEPART"), QStringLiteral("DEPART"), QStringLiteral("DATEDEP")});
    m_cols.etat = pick({QStringLiteral("ETAT"), QStringLiteral("ETAT_QUAI"), QStringLiteral("STATUT"), QStringLiteral("STATUS")});
    m_cols.prix = pick({QStringLiteral("PRIXLOCATION"), QStringLiteral("PRIX_LOCATION"), QStringLiteral("PRIX"), QStringLiteral("TARIF"), QStringLiteral("PRIX_H")});

    // Discover actual types for a few columns that are often mismatched (ODBC conversions).
    m_cols.tailleIsText = false;
    m_cols.arriveeIsDate = false;
    m_cols.departIsDate = false;
    if (const int idx = m_cols.taille.isEmpty() ? -1 : rec.indexOf(m_cols.taille); idx >= 0)
    {
        const QSqlField f = rec.field(idx);
        const QVariant::Type vt = f.type();
        m_cols.tailleIsText = (vt == QVariant::String);
    }
    if (const int idx = m_cols.arrivee.isEmpty() ? -1 : rec.indexOf(m_cols.arrivee); idx >= 0)
    {
        const QSqlField f = rec.field(idx);
        const QVariant::Type vt = f.type();
        m_cols.arriveeIsDate = (vt == QVariant::Date || vt == QVariant::DateTime);
    }
    if (const int idx = m_cols.depart.isEmpty() ? -1 : rec.indexOf(m_cols.depart); idx >= 0)
    {
        const QSqlField f = rec.field(idx);
        const QVariant::Type vt = f.type();
        m_cols.departIsDate = (vt == QVariant::Date || vt == QVariant::DateTime);
    }

    // Detect if ID column is numeric (common with Oracle sequences/identity).
    // With some ODBC drivers the metaType can be surprising, so we check both metaType() and type().
    m_cols.idIsNumeric = false;
    if (!m_cols.id.isEmpty())
    {
        const int idxId = rec.indexOf(m_cols.id);
        if (idxId >= 0)
        {
            const QSqlField f = rec.field(idxId);
            const QMetaType mt = f.metaType();
            const QVariant::Type vt = f.type();

            const bool metaNumeric = (mt == QMetaType(QMetaType::Int)
                                      || mt == QMetaType(QMetaType::UInt)
                                      || mt == QMetaType(QMetaType::LongLong)
                                      || mt == QMetaType(QMetaType::ULongLong)
                                      || mt == QMetaType(QMetaType::Double)
                                      || mt == QMetaType(QMetaType::Float));
            const bool varNumeric = (vt == QVariant::Int
                                     || vt == QVariant::UInt
                                     || vt == QVariant::LongLong
                                     || vt == QVariant::ULongLong
                                     || vt == QVariant::Double);

            m_cols.idIsNumeric = metaNumeric || varNumeric;
        }

        // Practical Oracle schema convention (matches your table screenshot).
        if (m_cols.id.compare(QStringLiteral("IDQUAI"), Qt::CaseInsensitive) == 0)
            m_cols.idIsNumeric = true;
    }

    // Minimum required for this UI.
    if (m_cols.id.isEmpty() || m_cols.matricule.isEmpty())
    {
        QStringList cols;
        cols.reserve(rec.count());
        for (int i = 0; i < rec.count(); ++i)
            cols << rec.fieldName(i);
        if (error)
            *error = tr("Colonnes obligatoires introuvables dans QUAI (ID + MATRICULE). ")
                     + tr("Trouvé: %1").arg(cols.join(QStringLiteral(", ")));
        return false;
    }

    m_cols.resolved = true;
    return true;
}

bool QuaiModel::fetchAllFromDatabase(QVector<Quai> *out, QString *error)
{
    if (!out)
        return false;

    if (!ensureDbColumns(error))
        return false;

    QSqlDatabase db = Connection::createInstance().database();
    QSqlQuery q(db);
    if (!q.exec(QStringLiteral("SELECT * FROM QUAI")))
    {
        if (error)
            *error = tr("SELECT sur QUAI a échoué: %1").arg(dbErrText(q));
        return false;
    }

    const QSqlRecord rec = q.record();
    const int idxId = rec.indexOf(m_cols.id);
    const int idxMat = rec.indexOf(m_cols.matricule);
    const int idxType = m_cols.type.isEmpty() ? -1 : rec.indexOf(m_cols.type);
    const int idxTaille = m_cols.taille.isEmpty() ? -1 : rec.indexOf(m_cols.taille);
    const int idxX = m_cols.posX.isEmpty() ? -1 : rec.indexOf(m_cols.posX);
    const int idxY = m_cols.posY.isEmpty() ? -1 : rec.indexOf(m_cols.posY);
    const int idxArr = m_cols.arrivee.isEmpty() ? -1 : rec.indexOf(m_cols.arrivee);
    const int idxDep = m_cols.depart.isEmpty() ? -1 : rec.indexOf(m_cols.depart);
    const int idxEtat = m_cols.etat.isEmpty() ? -1 : rec.indexOf(m_cols.etat);
    const int idxPrix = m_cols.prix.isEmpty() ? -1 : rec.indexOf(m_cols.prix);

    out->clear();
    out->reserve(64);

    while (q.next())
    {
        Quai row;
        row.idQuai = idxId >= 0 ? q.value(idxId).toString() : QString();
        row.matricule = idxMat >= 0 ? q.value(idxMat).toString() : QString();
        row.typeQuai = idxType >= 0 ? q.value(idxType).toString() : QString();
        row.taille = idxTaille >= 0 ? q.value(idxTaille).toDouble() : 0.0;
        row.posX = idxX >= 0 ? q.value(idxX).toDouble() : 0.0;
        row.posY = idxY >= 0 ? q.value(idxY).toDouble() : 0.0;
        row.dateHeureArrivee = idxArr >= 0 ? toDateTimeSafe(q.value(idxArr)) : QDateTime();
        row.dateHeureDepart = idxDep >= 0 ? toDateTimeSafe(q.value(idxDep)) : QDateTime();
        row.etatQuai = idxEtat >= 0 ? q.value(idxEtat).toString() : QString();
        row.prixLocation = idxPrix >= 0 ? q.value(idxPrix).toDouble() : 0.0;

        out->push_back(row);
    }

    return true;
}

bool QuaiModel::reloadFromDatabase(QString *error)
{
    QVector<Quai> rows;
    if (!fetchAllFromDatabase(&rows, error))
        return false;

    beginResetModel();
    m_items = rows;
    // Traceability remains in-app snapshots unless you have a dedicated history table.
    m_history = m_items;
    endResetModel();
    return true;
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

bool QuaiModel::validate(const Quai &quai, QString *error, bool allowEmptyId)
{
    if (!allowEmptyId && normalize(quai.idQuai).isEmpty())
    {
        if (error)
            *error = tr("ID Quai est obligatoire.");
        return false;
    }
    if (normalize(quai.matricule).isEmpty())
    {
        if (error)
            *error = tr("Matricule est obligatoire.");
        return false;
    }
    if (quai.matricule.contains(' '))
    {
        if (error)
            *error = tr("Matricule ne doit pas contenir d'espaces.");
        return false;
    }
    if (quai.taille < 0.0)
    {
        if (error)
            *error = tr("La taille du quai doit être >= 0.");
        return false;
    }
    if (normalize(quai.etatQuai).isEmpty())
    {
        if (error)
            *error = tr("État quai est obligatoire.");
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

    // Resolve schema early so we can decide how to handle the ID field.
    if (!ensureDbColumns(error))
        return false;

    // If the DB uses a numeric/identity ID, keep it empty and let Oracle generate it.
    if (normalize(toInsert.idQuai).isEmpty() && !m_cols.idIsNumeric)
        toInsert.idQuai = nextAutoId(m_items);

    // If the dialog doesn't set type yet, keep it optional.

    if (!validate(toInsert, error, m_cols.idIsNumeric))
        return false;

    if (!normalize(toInsert.idQuai).isEmpty() && indexOfId(toInsert.idQuai) >= 0)
    {
        if (error)
            *error = tr("IDQuai existe déjà.");
        return false;
    }

    if (!insertIntoDatabase(toInsert, error))
        return false;

    // If ID is generated by the DB, we need to reload to get it.
    if (normalize(toInsert.idQuai).isEmpty())
    {
        QString reloadErr;
        const bool ok = reloadFromDatabase(&reloadErr);
        if (!ok && error)
            *error = tr("Insertion effectuée, mais rechargement a échoué: %1").arg(reloadErr);
        return ok;
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

    if (!validate(toUpdate, error, false))
        return false;

    const int existing = indexOfId(toUpdate.idQuai);
    if (existing >= 0 && existing != row)
    {
        if (error)
            *error = tr("Un autre enregistrement possède déjà ce IDQuai.");
        return false;
    }

    if (!updateInDatabase(toUpdate, error))
        return false;

    m_items[row] = toUpdate;
    m_history.push_back(toUpdate);
    emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
    return true;
}

bool QuaiModel::removeQuai(int row)
{
    QString err;
    return removeQuai(row, &err);
}

bool QuaiModel::removeQuai(int row, QString *error)
{
    if (row < 0 || row >= m_items.size())
        return false;

    if (!deleteFromDatabase(m_items.at(row).idQuai, error))
        return false;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    return true;
}

bool QuaiModel::insertIntoDatabase(const Quai &quai, QString *error)
{
    if (!ensureDbColumns(error))
        return false;

    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen())
    {
        if (error)
            *error = tr("Connexion Oracle fermée.");
        return false;
    }

    const bool useTx = db.driver() && db.driver()->hasFeature(QSqlDriver::Transactions);

    auto execInsert = [&](bool includeId, const QVariant &explicitId, QString *outErr) -> bool {
        if (useTx)
            db.transaction();

        QStringList cols;
        QStringList vals;

        // Keep the INSERT order aligned with the Oracle table schema (for readability/debugging).
        if (includeId)                { cols << m_cols.id;        vals << QStringLiteral(":id"); }
        if (!m_cols.etat.isEmpty())   { cols << m_cols.etat;      vals << QStringLiteral(":etat"); }
        if (!m_cols.taille.isEmpty()) { cols << m_cols.taille;    vals << QStringLiteral(":taille"); }
        if (!m_cols.prix.isEmpty())   { cols << m_cols.prix;      vals << QStringLiteral(":prix"); }
        cols << m_cols.matricule;      vals << QStringLiteral(":mat");
        if (!m_cols.arrivee.isEmpty())
        {
            cols << m_cols.arrivee;
            vals << (m_cols.arriveeIsDate
                         ? QStringLiteral("TO_DATE(:arr,'YYYY-MM-DD HH24:MI:SS')")
                         : QStringLiteral(":arr"));
        }
        if (!m_cols.depart.isEmpty())
        {
            cols << m_cols.depart;
            vals << (m_cols.departIsDate
                         ? QStringLiteral("TO_DATE(:dep,'YYYY-MM-DD HH24:MI:SS')")
                         : QStringLiteral(":dep"));
        }
        if (!m_cols.posX.isEmpty())   { cols << m_cols.posX;      vals << QStringLiteral(":x"); }
        if (!m_cols.posY.isEmpty())   { cols << m_cols.posY;      vals << QStringLiteral(":y"); }
        if (!m_cols.type.isEmpty())   { cols << m_cols.type;      vals << QStringLiteral(":type"); }

        const QString sql = QStringLiteral("INSERT INTO QUAI (%1) VALUES (%2)")
                                .arg(cols.join(QStringLiteral(", ")), vals.join(QStringLiteral(", ")));

        QSqlQuery q(db);
        if (!q.prepare(sql))
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("Préparation INSERT a échoué: %1").arg(dbErrText(q));
            return false;
        }

        if (includeId)
            q.bindValue(QStringLiteral(":id"), explicitId);

        if (!m_cols.etat.isEmpty())    q.bindValue(QStringLiteral(":etat"), quai.etatQuai);
        if (!m_cols.taille.isEmpty())
        {
            if (m_cols.tailleIsText)
                q.bindValue(QStringLiteral(":taille"), QString::number(quai.taille, 'f', 1));
            else
                q.bindValue(QStringLiteral(":taille"), quai.taille);
        }
        if (!m_cols.prix.isEmpty())    q.bindValue(QStringLiteral(":prix"), quai.prixLocation);
        q.bindValue(QStringLiteral(":mat"), quai.matricule);
        if (!m_cols.arrivee.isEmpty())
        {
            if (m_cols.arriveeIsDate)
                q.bindValue(QStringLiteral(":arr"), toOracleDateTimeString(quai.dateHeureArrivee));
            else
                q.bindValue(QStringLiteral(":arr"), quai.dateHeureArrivee);
        }
        if (!m_cols.depart.isEmpty())
        {
            if (m_cols.departIsDate)
                q.bindValue(QStringLiteral(":dep"), toOracleDateTimeString(quai.dateHeureDepart));
            else
                q.bindValue(QStringLiteral(":dep"), quai.dateHeureDepart);
        }
        if (!m_cols.posX.isEmpty())    q.bindValue(QStringLiteral(":x"), quai.posX);
        if (!m_cols.posY.isEmpty())    q.bindValue(QStringLiteral(":y"), quai.posY);
        if (!m_cols.type.isEmpty())    q.bindValue(QStringLiteral(":type"), quai.typeQuai);

        if (!q.exec())
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("INSERT dans QUAI a échoué: %1").arg(dbErrText(q));
            return false;
        }

        if (useTx && !db.commit())
        {
            if (outErr)
                *outErr = tr("Commit a échoué: %1").arg(db.lastError().text());
            return false;
        }

        return true;
    };

    const bool includeId = !(m_cols.idIsNumeric && normalize(quai.idQuai).isEmpty());
    if (includeId)
    {
        QString convErr;
        const QVariant idVal = bindIdValue(quai.idQuai, m_cols.idIsNumeric, m_cols.id, &convErr);
        if (m_cols.idIsNumeric && (!idVal.isValid() || idVal.isNull()))
        {
            if (error)
                *error = convErr.isEmpty() ? tr("ID Quai invalide.") : convErr;
            return false;
        }
        return execInsert(true, idVal, error);
    }

    QString firstErr;
    if (execInsert(false, {}, &firstErr))
        return true;

    // Fallback: if Oracle rejects NULL ID for a numeric ID column, generate MAX(id)+1.
    const bool looksLikeNullId = m_cols.idIsNumeric
                             && firstErr.contains(QStringLiteral("cannot insert NULL"), Qt::CaseInsensitive);
    if (!looksLikeNullId)
    {
        if (error)
            *error = firstErr;
        return false;
    }

    QSqlQuery qNext(db);
    const QString nextSql = QStringLiteral("SELECT NVL(MAX(%1), 0) + 1 FROM QUAI").arg(m_cols.id);
    if (!qNext.exec(nextSql) || !qNext.next())
    {
        if (error)
            *error = tr("INSERT a échoué (ID NULL) et calcul du prochain ID a échoué: %1").arg(dbErrText(qNext));
        return false;
    }
    const QVariant nextId = qNext.value(0);
    if (!nextId.isValid() || nextId.isNull())
    {
        if (error)
            *error = tr("Impossible de générer un ID pour %1.").arg(m_cols.id);
        return false;
    }

    if (!execInsert(true, nextId, error))
        return false;

    return true;
}

bool QuaiModel::updateInDatabase(const Quai &quai, QString *error)
{
    if (!ensureDbColumns(error))
        return false;

    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen())
    {
        if (error)
            *error = tr("Connexion Oracle fermée.");
        return false;
    }

    const bool useTx = db.driver() && db.driver()->hasFeature(QSqlDriver::Transactions);
    if (useTx)
        db.transaction();

    QString convErr;
    const QVariant idVal = bindIdValue(quai.idQuai, m_cols.idIsNumeric, m_cols.id, &convErr);
    if (m_cols.idIsNumeric && (!idVal.isValid() || idVal.isNull()))
    {
        if (useTx) db.rollback();
        if (error)
            *error = convErr.isEmpty() ? tr("ID Quai invalide.") : convErr;
        return false;
    }

    auto execUpdate = [&](bool useToDate, QString *outErr) -> bool {
        if (useTx)
            db.transaction();

        // QODBC is more reliable with positional parameters.
        QStringList sets;
        QVector<QVariant> binds;
        binds.reserve(16);

        const auto addSet = [&](const QString &expr, const QVariant &value) {
            sets << expr;
            binds << value;
        };

        addSet(QStringLiteral("%1=?").arg(m_cols.matricule), quai.matricule);

        if (!m_cols.type.isEmpty())
            addSet(QStringLiteral("%1=?").arg(m_cols.type), quai.typeQuai);

        if (!m_cols.taille.isEmpty())
        {
            if (m_cols.tailleIsText)
                addSet(QStringLiteral("%1=?").arg(m_cols.taille), QString::number(quai.taille, 'f', 1));
            else
                addSet(QStringLiteral("%1=?").arg(m_cols.taille), quai.taille);
        }

        if (!m_cols.posX.isEmpty())
            addSet(QStringLiteral("%1=?").arg(m_cols.posX), quai.posX);
        if (!m_cols.posY.isEmpty())
            addSet(QStringLiteral("%1=?").arg(m_cols.posY), quai.posY);

        if (!m_cols.arrivee.isEmpty())
        {
            if (useToDate && m_cols.arriveeIsDate)
                addSet(QStringLiteral("%1=TO_DATE(?, 'YYYY-MM-DD HH24:MI:SS')").arg(m_cols.arrivee),
                       toOracleDateTimeString(quai.dateHeureArrivee));
            else
                addSet(QStringLiteral("%1=?").arg(m_cols.arrivee), quai.dateHeureArrivee);
        }

        if (!m_cols.depart.isEmpty())
        {
            if (useToDate && m_cols.departIsDate)
                addSet(QStringLiteral("%1=TO_DATE(?, 'YYYY-MM-DD HH24:MI:SS')").arg(m_cols.depart),
                       toOracleDateTimeString(quai.dateHeureDepart));
            else
                addSet(QStringLiteral("%1=?").arg(m_cols.depart), quai.dateHeureDepart);
        }

        if (!m_cols.etat.isEmpty())
            addSet(QStringLiteral("%1=?").arg(m_cols.etat), quai.etatQuai);
        if (!m_cols.prix.isEmpty())
            addSet(QStringLiteral("%1=?").arg(m_cols.prix), quai.prixLocation);

        const QString sql = QStringLiteral("UPDATE QUAI SET %1 WHERE %2=?")
                                .arg(sets.join(QStringLiteral(", ")), m_cols.id);

        QSqlQuery q(db);
        if (!q.prepare(sql))
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("Préparation UPDATE a échoué: %1").arg(dbErrText(q));
            return false;
        }

        for (const QVariant &v : binds)
            q.addBindValue(v);
        q.addBindValue(idVal);

        if (!q.exec())
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("UPDATE dans QUAI a échoué: %1").arg(dbErrText(q));
            return false;
        }

        if (useTx && !db.commit())
        {
            if (outErr)
                *outErr = tr("Commit a échoué: %1").arg(db.lastError().text());
            return false;
        }
        return true;
    };

    QString err1;
    if (execUpdate(true, &err1))
        return true;

    // Fallback for some ODBC drivers: bind QDateTime directly (no TO_DATE).
    QString err2;
    if (execUpdate(false, &err2))
        return true;

    // Final fallback: some Oracle ODBC drivers mis-handle parameter binding entirely.
    // Build a literal SQL UPDATE (with escaping) to get a working update.
    const auto idToSql = [&]() -> QString {
        if (!m_cols.idIsNumeric)
            return sqlQuote(quai.idQuai);
        bool ok = false;
        const qlonglong ll = quai.idQuai.trimmed().toLongLong(&ok);
        if (ok)
            return QString::number(ll);
        const double d = quai.idQuai.trimmed().toDouble(&ok);
        if (ok)
            return sqlNumber(d);
        return QString();
    };

    const QString idSql = idToSql();
    if (!idSql.isEmpty())
    {
        QStringList sets;
        sets << QStringLiteral("%1=%2").arg(m_cols.matricule, sqlQuote(quai.matricule));

        if (!m_cols.type.isEmpty())
            sets << QStringLiteral("%1=%2").arg(m_cols.type, sqlQuote(quai.typeQuai));

        if (!m_cols.taille.isEmpty())
        {
            const QString tailleText = m_cols.tailleIsText ? QString::number(quai.taille, 'f', 1) : sqlNumber(quai.taille);
            sets << QStringLiteral("%1=%2").arg(m_cols.taille, sqlQuote(tailleText));
        }

        if (!m_cols.posX.isEmpty())
            sets << QStringLiteral("%1=%2").arg(m_cols.posX, sqlNumber(quai.posX));
        if (!m_cols.posY.isEmpty())
            sets << QStringLiteral("%1=%2").arg(m_cols.posY, sqlNumber(quai.posY));

        if (!m_cols.arrivee.isEmpty())
        {
            const QString s = toOracleDateTimeString(quai.dateHeureArrivee);
            if (m_cols.arriveeIsDate)
                sets << QStringLiteral("%1=TO_DATE(%2,'YYYY-MM-DD HH24:MI:SS')").arg(m_cols.arrivee, sqlQuote(s));
            else
                sets << QStringLiteral("%1=%2").arg(m_cols.arrivee, sqlQuote(s));
        }

        if (!m_cols.depart.isEmpty())
        {
            const QString s = toOracleDateTimeString(quai.dateHeureDepart);
            if (m_cols.departIsDate)
                sets << QStringLiteral("%1=TO_DATE(%2,'YYYY-MM-DD HH24:MI:SS')").arg(m_cols.depart, sqlQuote(s));
            else
                sets << QStringLiteral("%1=%2").arg(m_cols.depart, sqlQuote(s));
        }

        if (!m_cols.etat.isEmpty())
            sets << QStringLiteral("%1=%2").arg(m_cols.etat, sqlQuote(quai.etatQuai));
        if (!m_cols.prix.isEmpty())
            sets << QStringLiteral("%1=%2").arg(m_cols.prix, sqlNumber(quai.prixLocation));

        const QString literalSql = QStringLiteral("UPDATE QUAI SET %1 WHERE %2=%3")
                                       .arg(sets.join(QStringLiteral(", ")), m_cols.id, idSql);

        QSqlQuery qLit(db);
        if (useTx)
            db.transaction();
        if (qLit.exec(literalSql))
        {
            if (!useTx || db.commit())
                return true;
            if (error)
                *error = tr("Commit a échoué: %1").arg(db.lastError().text());
            return false;
        }
        if (useTx)
            db.rollback();

        const QString err3 = tr("UPDATE (fallback littéral) a échoué: %1").arg(dbErrText(qLit));
        if (error)
            *error = err1 + QStringLiteral("\n") + err2 + QStringLiteral("\n") + err3;
        return false;
    }

    if (error)
        *error = err2.isEmpty() ? err1 : (err1 + QStringLiteral("\n") + err2);
    return false;
}

bool QuaiModel::deleteFromDatabase(const QString &idQuai, QString *error)
{
    if (!ensureDbColumns(error))
        return false;

    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen())
    {
        if (error)
            *error = tr("Connexion Oracle fermée.");
        return false;
    }

    QString convErr;
    const QVariant idVal = bindIdValue(idQuai, m_cols.idIsNumeric, m_cols.id, &convErr);
    if (m_cols.idIsNumeric && (!idVal.isValid() || idVal.isNull()))
    {
        if (error)
            *error = convErr.isEmpty() ? tr("ID Quai invalide.") : convErr;
        return false;
    }

    const bool useTx = db.driver() && db.driver()->hasFeature(QSqlDriver::Transactions);

    auto tryDelete = [&](const QString &sql, const std::function<void(QSqlQuery &)> &binder, QString *outErr) -> int {
        if (useTx)
            db.transaction();
        QSqlQuery q(db);
        if (!q.prepare(sql))
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("Préparation DELETE a échoué: %1").arg(dbErrText(q));
            return -1;
        }
        binder(q);
        if (!q.exec())
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("DELETE a échoué: %1").arg(dbErrText(q));
            return -1;
        }
        const int affected = q.numRowsAffected();
        if (affected <= 0)
        {
            if (useTx) db.rollback();
            if (outErr)
                *outErr = tr("DELETE: aucune ligne supprimée (ID introuvable ?)");
            return 0;
        }
        if (useTx && !db.commit())
        {
            if (outErr)
                *outErr = tr("Commit a échoué: %1").arg(db.lastError().text());
            return -1;
        }
        return affected;
    };

    // 1) Prefer positional parameter; QODBC/Oracle often behaves better with '?' than with named ':id'.
    QString err1;
    {
        const QString sql = QStringLiteral("DELETE FROM QUAI WHERE %1 = ?").arg(m_cols.id);
        const int res = tryDelete(sql, [&](QSqlQuery &q) { q.addBindValue(idVal); }, &err1);
        if (res > 0)
            return true;
        if (res == 0)
        {
            if (error)
                *error = err1;
            return false;
        }
    }

    // 2) Fallback: named bind.
    QString err2;
    {
        const QString sql = QStringLiteral("DELETE FROM QUAI WHERE %1 = :id").arg(m_cols.id);
        const int res = tryDelete(sql, [&](QSqlQuery &q) { q.bindValue(QStringLiteral(":id"), idVal); }, &err2);
        if (res > 0)
            return true;
        if (res == 0)
        {
            if (error)
                *error = err2;
            return false;
        }
    }

    // 3) Last resort: literal SQL (no binds) to bypass buggy parameter handling.
    QString err3;
    {
        QString idSql;
        if (m_cols.idIsNumeric)
        {
            bool ok = false;
            const qlonglong ll = idVal.toLongLong(&ok);
            if (ok)
                idSql = QString::number(ll);
            else
                idSql = sqlNumber(idVal.toDouble(&ok));
        }
        else
        {
            idSql = sqlQuote(idVal.toString());
        }

        const QString sql = QStringLiteral("DELETE FROM QUAI WHERE %1 = %2").arg(m_cols.id, idSql);
        if (useTx)
            db.transaction();
        QSqlQuery qLit(db);
        if (!qLit.exec(sql))
        {
            if (useTx) db.rollback();
            err3 = tr("DELETE (fallback littéral) a échoué: %1").arg(dbErrText(qLit));
        }
        else if (qLit.numRowsAffected() <= 0)
        {
            if (useTx) db.rollback();
            err3 = tr("DELETE: aucune ligne supprimée (ID introuvable ?)");
            if (error)
                *error = err3;
            return false;
        }
        else if (useTx && !db.commit())
        {
            err3 = tr("Commit a échoué: %1").arg(db.lastError().text());
        }
        else
        {
            return true;
        }
    }

    if (error)
        *error = err1 + QStringLiteral("\n") + err2 + QStringLiteral("\n") + err3;
    return false;
}
