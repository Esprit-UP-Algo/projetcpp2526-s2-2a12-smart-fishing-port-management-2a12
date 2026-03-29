#ifndef NAVIRE_H
#define NAVIRE_H

#include <QString>
#include <QDateTime>
#include <QList>

struct Mouvement {
    QString navireNom;
    QDateTime heureDepart;
    QDateTime heureRetour;
    QString cargaisonRetour;
    double distanceKm;
};

struct Navire {
    int id;
    QString nom;
    QString matricule;
    QString type;
    int capacite;
    QString statut;
    QString position;
    int nbSorties;

    Navire() : id(0), capacite(0), nbSorties(0) {}

    Navire(int id, const QString &nom, const QString &matricule,
           const QString &type, int capacite, const QString &statut,
           const QString &position, int nbSorties = 0)
        : id(id), nom(nom), matricule(matricule), type(type),
          capacite(capacite), statut(statut), position(position),
          nbSorties(nbSorties) {}
};

#endif // NAVIRE_H
