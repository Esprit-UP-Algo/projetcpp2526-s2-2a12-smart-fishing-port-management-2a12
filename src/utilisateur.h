#pragma once

#include <QDateTime>
#include <QString>

struct Utilisateur
{
    QString idUtilisateur;
    QString nom;
    QString prenom;
    QString email;
    QString login;
    QString motDePasse;
    QString role;
    QString statut; // e.g. "Actif" / "Inactif"
    QDateTime dateCreation;
};
