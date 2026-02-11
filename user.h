#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

struct User {
    QString cin;
    QString nom;
    QString prenom;
    QString email;
    QString login;
    QString password;
    QString role;
    QString statut;
    double heures;
    QDateTime dateCreation;
};

#endif // USER_H
