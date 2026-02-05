#include "transactionmodel.h"

#include <QDateTime>
#include <QLocale>

static QString fmtDate(const QDateTime &dt)
{
    if (!dt.isValid())
        return QStringLiteral("—");
    return dt.toString("yyyy-MM-dd HH:mm");
}

TransactionModel::TransactionModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int TransactionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_items.size();
}

int TransactionModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColCount;
}

QVariant TransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const int r = index.row();
    const int c = index.column();
    if (r < 0 || r >= m_items.size())
        return {};

    const Transaction &t = m_items.at(r);

    if (role == Qt::DisplayRole) {
        switch (c) {
        case ColIdVente: return t.idVente;
        case ColClient: return t.client;
        case ColLot: return t.lot;
        case ColPrixKg: return QString::number(t.prixKg, 'f', 2);
        case ColQuantite: return QString::number(t.quantite, 'f', 3);
        case ColTotal: return QString::number(t.total(), 'f', 2);
        case ColDate: return fmtDate(t.date);
        case ColModePaiement: return t.modePaiement;
        case ColStatutPaiement: return t.statutPaiement;
        default: return {};
        }
    }

    if (role == SortValueRole) {
        switch (c) {
        case ColPrixKg: return t.prixKg;
        case ColQuantite: return t.quantite;
        case ColTotal: return t.total();
        case ColDate: return t.date;
        default: return data(index, Qt::DisplayRole);
        }
    }

    return {};
}

QVariant TransactionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QAbstractTableModel::headerData(section, orientation, role);

    switch (section) {
    case ColIdVente: return QStringLiteral("ID Vente");
    case ColClient: return QStringLiteral("Client");
    case ColLot: return QStringLiteral("Lot");
    case ColPrixKg: return QStringLiteral("Prix/kg");
    case ColQuantite: return QStringLiteral("Quantité");
    case ColTotal: return QStringLiteral("Total");
    case ColDate: return QStringLiteral("Date");
    case ColModePaiement: return QStringLiteral("Mode paiement");
    case ColStatutPaiement: return QStringLiteral("Statut paiement");
    default: return {};
    }
}

bool TransactionModel::addTransaction(const Transaction &t, QString *errorOut)
{
    QString err;
    if (!validate(t, &err)) {
        if (errorOut) *errorOut = err;
        return false;
    }

    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(t);
    endInsertRows();
    return true;
}

bool TransactionModel::updateTransaction(int row, const Transaction &t, QString *errorOut)
{
    if (row < 0 || row >= m_items.size()) {
        if (errorOut) *errorOut = QStringLiteral("Invalid row");
        return false;
    }

    QString err;
    if (!validate(t, &err)) {
        if (errorOut) *errorOut = err;
        return false;
    }

    m_items[row] = t;
    emit dataChanged(index(row, 0), index(row, ColCount - 1));
    return true;
}

bool TransactionModel::removeTransaction(int row, QString *errorOut)
{
    if (row < 0 || row >= m_items.size()) {
        if (errorOut) *errorOut = QStringLiteral("Invalid row");
        return false;
    }

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
    return true;
}

int TransactionModel::indexOfId(const QString &idVente) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items.at(i).idVente == idVente)
            return i;
    }
    return -1;
}

bool TransactionModel::validate(const Transaction &t, QString *errorOut) const
{
    if (t.idVente.trimmed().isEmpty()) {
        if (errorOut) *errorOut = QStringLiteral("ID Vente is required");
        return false;
    }
    if (t.client.trimmed().isEmpty()) {
        if (errorOut) *errorOut = QStringLiteral("Client is required");
        return false;
    }
    if (t.lot.trimmed().isEmpty()) {
        if (errorOut) *errorOut = QStringLiteral("Lot is required");
        return false;
    }
    if (t.prixKg < 0) {
        if (errorOut) *errorOut = QStringLiteral("Prix/kg must be >= 0");
        return false;
    }
    if (t.quantite <= 0) {
        if (errorOut) *errorOut = QStringLiteral("Quantité must be > 0");
        return false;
    }
    return true;
}
