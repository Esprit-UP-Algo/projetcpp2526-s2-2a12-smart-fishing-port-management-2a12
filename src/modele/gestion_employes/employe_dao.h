#ifndef EMPLOYE_DAO_H
#define EMPLOYE_DAO_H

#include <QString>
#include <QVector>
#include "employe_user.h"

namespace employes {

class EmployeDAO
{
public:
    struct Salaire {
        float brut = 0.0f;
        float cnss = 0.0f;
        float impot = 0.0f;
        float net = 0.0f;
    };

    EmployeDAO();
    ~EmployeDAO();

    static QString validateCIN(const QString &cin);

    static QString validateCINUnique(const QString &cin, const QString &excludeCIN = "");

    static QString validateEmail(const QString &email);

    static QString validateNotEmpty(const QString &field, const QString &fieldName);

    static QString validateID(const QString &idStr);

    static QString validatePassword(const QString &password);

    static QString validateEmploye(const EmployeUser &employe, bool checkCINUnique = true, const QString &excludeCIN = "");

    static QString ajouter(const EmployeUser &employe);

    static QVector<EmployeUser> afficher();

    static QString modifier(const EmployeUser &employe);

    static QString supprimer(const QString &cin);
    static EmployeUser findByLogin(const QString &login);

    // Calcul salarial (regles simplifiees inspirees de la legislation tunisienne)
    static float getTauxHoraire(const QString &role);
    static float calculerSalaireBrut(float heures, const QString &role);
    static float calculerCNSS(float brut);
    static float calculerImpot(float brut);
    static float calculerSalaireNet(float brut);
    static Salaire calculerSalaireComplet(float heures, const QString &role);



private:
    static bool isValidEmail(const QString &email);
    static bool isAllDigits(const QString &str);
    static bool cinExists(const QString &cin, const QString &excludeCIN = "");
};

} // namespace employes

#endif // EMPLOYE_DAO_H
