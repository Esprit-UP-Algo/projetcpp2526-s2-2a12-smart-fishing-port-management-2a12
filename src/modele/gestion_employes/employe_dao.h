#ifndef EMPLOYE_DAO_H
#define EMPLOYE_DAO_H

#include <QString>
#include <QVector>
#include "employe_user.h"

namespace employes {

class EmployeDAO
{
public:
    EmployeDAO();
    ~EmployeDAO();

    static QString validateCIN(const QString &cin);

    static QString validateCINUnique(const QString &cin, const QString &excludeCIN = "");

    static QString validateEmail(const QString &email);

    static QString validateNotEmpty(const QString &field, const QString &fieldName);

    static QString validateID(const QString &idStr);

    static QString validatePassword(const QString &password);

    static QString validateEmploye(const EmployeUser &employe);

    static QString ajouter(const EmployeUser &employe);

    static QVector<EmployeUser> afficher();

    static QString modifier(const EmployeUser &employe);

    static QString supprimer(const QString &cin);
    static EmployeUser findByLogin(const QString &login);



private:
    static bool isValidEmail(const QString &email);
    static bool isAllDigits(const QString &str);
    static bool cinExists(const QString &cin, const QString &excludeCIN = "");
};

} // namespace employes

#endif // EMPLOYE_DAO_H
