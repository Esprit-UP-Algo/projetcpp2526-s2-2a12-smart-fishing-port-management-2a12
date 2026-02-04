#ifndef CAPTURELOT_H
#define CAPTURELOT_H

#include <QString>
#include <QDateTime>
#include <QVariant>

struct CaptureLot
{
    QString idLot;              // ID unique du lot
    QString espece;             // Espèce de poisson
    int poids;                  // Poids en kg
    QString zonePeche;          // Zone de pêche
    QDateTime dateCapture;      // Date de capture
    QString navire;             // Navire de pêche
    QString qualite;            // Qualité (Premium, Standard, etc.)
    QDateTime dateEntreeStock;  // Date d'entrée en stock

    // Constructeur par défaut
    CaptureLot() 
        : poids(0), 
          dateCapture(QDateTime::currentDateTime()),
          dateEntreeStock(QDateTime::currentDateTime()) {}

    // Constructeur avec paramètres
    CaptureLot(const QString &id, const QString &esp, int p, const QString &zone, 
               const QDateTime &capture, const QString &nav, const QString &qual, 
               const QDateTime &entree)
        : idLot(id), espece(esp), poids(p), zonePeche(zone), 
          dateCapture(capture), navire(nav), qualite(qual), 
          dateEntreeStock(entree) {}

    // Retourne la durée en stock en secondes
    int dureeSecondes() const
    {
        if (!dateEntreeStock.isValid())
            return 0;
        return dateEntreeStock.secsTo(QDateTime::currentDateTime());
    }

    // Retourne la durée en jours (avec décimales)
    double dureeDays() const
    {
        return double(dureeSecondes()) / double(24 * 3600);
    }

    // Validation
    bool isValid() const
    {
        return !idLot.trimmed().isEmpty() && 
               !espece.trimmed().isEmpty() && 
               poids > 0 &&
               !zonePeche.trimmed().isEmpty() &&
               dateCapture.isValid() &&
               !navire.trimmed().isEmpty() &&
               !qualite.trimmed().isEmpty() &&
               dateEntreeStock.isValid();
    }
};

Q_DECLARE_METATYPE(CaptureLot)

#endif // CAPTURELOT_H
