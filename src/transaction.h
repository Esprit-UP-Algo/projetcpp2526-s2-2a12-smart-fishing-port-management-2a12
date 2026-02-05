#pragma once

#include <QString>
#include <QDateTime>

struct Transaction
{
    QString idVente;
    QString client;
    QString lot;
    double prixKg = 0.0;
    double quantite = 0.0;
    double total() const { return prixKg * quantite; }
    QDateTime date;
    QString modePaiement;
    QString statutPaiement;
};
