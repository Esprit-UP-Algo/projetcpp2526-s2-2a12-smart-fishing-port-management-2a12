#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDateTime>
#include <QVector>

struct Transaction {
    QString numFacture;
    QString pecheur;
    QString lot;
    double  prixKg     = 0.0;
    double  quantite   = 0.0;
    double  total      = 0.0;
    QDateTime dateTransaction;
    QString modePaiement;   // "Espèces", "Chèque", "Virement"
    QString statutPaiement; // "Payé", "En attente", "Annulé"

    void calcTotal() { total = prixKg * quantite; }
};

#endif // TRANSACTION_H
