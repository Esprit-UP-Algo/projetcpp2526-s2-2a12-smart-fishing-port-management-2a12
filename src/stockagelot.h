#pragma once

#include <algorithm>

#include <QDateTime>
#include <QString>

struct StockageLot
{
    QString idLot;
    QString espece;
    int quantite = 0;
    QDateTime dateEntree;
    QDateTime dateSortie; // invalid => non sortie
    QString idChambre;

    bool isSorti() const { return dateSortie.isValid(); }

    qint64 dureeSecondes(const QDateTime &now = QDateTime::currentDateTime()) const
    {
        const QDateTime end = dateSortie.isValid() ? dateSortie : now;
        if (!dateEntree.isValid() || !end.isValid())
            return 0;
        return std::max<qint64>(0, dateEntree.secsTo(end));
    }
};
