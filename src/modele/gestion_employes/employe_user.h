#ifndef EMPLOYE_USER_H
#define EMPLOYE_USER_H

#include <QString>

namespace employes {

struct EmployeUser {
    QString cin;
    QString nom;
    QString prenom;
    QString email;
    QString login;
    QString password;
    QString role;
    QString statut;
    double heures;
};

} // namespace employes

#endif // EMPLOYE_USER_H
