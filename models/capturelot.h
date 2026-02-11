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
    QString etat;               // Etat du lot (En stock, Vendu, etc.)
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
        CaptureLot(const QString &id, const QString &esp, int p, const QString &zone, 
                             const QDateTime &capture, const QString &etatLot, const QString &nav, const QString &qual, 
                             const QDateTime &entree)
                : idLot(id), espece(esp), poids(p), zonePeche(zone), 
                    dateCapture(capture), etat(etatLot), navire(nav), qualite(qual), 
                    dateEntreeStock(entree) {}

    // Détermine si c'est un "Poisson Bleu" (Sardine, Maquereau, Thon...)
    bool isPoissonBleu() const
    {
        const QStringList bleu = { 
            "Sardine", "Anchois", "Maquereau", "Thon", "Thonine", 
            "Saurel", "Allache", "Bonite", "Espadon", "Sériole" 
        };
        // Vérifie si l'espèce contient un des mots-clés (insensible à la casse)
        for (const QString &b : bleu) {
            if (espece.contains(b, Qt::CaseInsensitive))
                return true;
        }
        return false;
    }

    // Retourne la quantité de glace recommandée (kg)
    // Règle métier : 0.8 kg de glace par kg de poisson bleu (exemple)
    double glaceRecommandee() const
    {
        if (isPoissonBleu())
            return poids * 0.8; 
        return poids * 0.3; // Moins pour les espèces nobles (gestion différente)
    }

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
               !etat.trimmed().isEmpty() &&
               !navire.trimmed().isEmpty() &&
               !qualite.trimmed().isEmpty() &&
               dateEntreeStock.isValid();
    }
};

Q_DECLARE_METATYPE(CaptureLot)

#endif // CAPTURELOT_H
