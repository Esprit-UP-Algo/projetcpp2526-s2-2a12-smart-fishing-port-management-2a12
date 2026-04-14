#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDateTime>
#include <QVector>

struct Transaction {
    QString numFacture;
    QString pecheur;
    QString reference;  // Numéro de référence saisi par l'utilisateur
    // `lot` column removed from DB schema; keep field for UI but allow empty
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
