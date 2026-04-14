#include "employe_dao.h"
#include "input_validator.h"
#include "../db/connection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QRegularExpression>
#include <stdexcept>

using namespace employes;

EmployeDAO::EmployeDAO()
{
}

EmployeDAO::~EmployeDAO()
{
}

QString EmployeDAO::validateCIN(const QString &cin)
{
    return InputValidator::validateCIN(cin, 8);
}

QString EmployeDAO::validateEmail(const QString &email)
{
    return InputValidator::validateEmail(email);
}

QString EmployeDAO::validateNotEmpty(const QString &field, const QString &fieldName)
{
    if (field.trimmed().isEmpty()) {
        return fieldName + " est obligatoire.";
    }
    
    return "";
}

QString EmployeDAO::validateID(const QString &idStr)
{
    if (idStr.isEmpty()) {
        return "L'ID est obligatoire.";
    }
    
    bool ok = false;
    int id = idStr.toInt(&ok);
    
    if (!ok || id <= 0) {
        return "L'ID doit être un nombre positif.";
    }
    
    return "";
}

QString EmployeDAO::validatePassword(const QString &password)
{
    if (password.isEmpty()) {
        return "Le mot de passe est obligatoire.";
    }
    
    if (password.length() < 6) {
        return "Le mot de passe doit contenir au minimum 6 caractères.";
    }
    
    return "";
}

QString EmployeDAO::validateEmploye(const EmployeUser &employe, bool checkCINUnique, const QString &excludeCIN)
{
    QString cinError = validateCIN(employe.cin);
    if (!cinError.isEmpty()) return cinError;

    if (checkCINUnique) {
        QString cinUniqueError = validateCINUnique(employe.cin, excludeCIN);
        if (!cinUniqueError.isEmpty()) return cinUniqueError;
    }

    QString emailError = validateEmail(employe.email);
    if (!emailError.isEmpty()) return emailError;

    QString nomError = InputValidator::validateName(employe.nom, "Le nom", true);
    if (!nomError.isEmpty()) return nomError;

    QString prenomError = InputValidator::validateName(employe.prenom, "Le prenom", true);
    if (!prenomError.isEmpty()) return prenomError;

    QString heuresError = InputValidator::validatePositiveDouble(employe.heures, "Les heures de travail");
    if (!heuresError.isEmpty()) return heuresError;

    QString passwordError = validatePassword(employe.password);
    if (!passwordError.isEmpty()) return passwordError;

    QString roleError = validateNotEmpty(employe.role, "Le rôle");
    if (!roleError.isEmpty()) return roleError;

    return "";
}

float EmployeDAO::getTauxHoraire(const QString &role)
{
    const QString normalizedRole = role.trimmed().toLower();

    if (normalizedRole == "ouvrier") return 11.0f;
    if (normalizedRole == "technicien") return 13.0f;
    if (normalizedRole == "agent") return 14.0f;
    if (normalizedRole == "chef_equipe") return 17.0f;
    if (normalizedRole == "superviseur") return 19.0f;
    if (normalizedRole == "admin") return 22.0f;

    throw std::invalid_argument("Role inconnu pour le calcul de salaire");
}

float EmployeDAO::calculerSalaireBrut(float heures, const QString &role)
{
    if (heures < 0.0f) {
        throw std::invalid_argument("Le nombre d'heures ne peut pas etre negatif");
    }

    const float taux = getTauxHoraire(role);

    const float heuresNormales = qMin(heures, 48.0f);
    const float heuresSup1 = qMax(qMin(heures - 48.0f, 12.0f), 0.0f);
    const float heuresSup2 = qMax(heures - 60.0f, 0.0f);

    return (heuresNormales * taux)
           + (heuresSup1 * taux * 1.25f)
           + (heuresSup2 * taux * 1.50f);
}

float EmployeDAO::calculerCNSS(float brut)
{
    if (brut < 0.0f) {
        throw std::invalid_argument("Le salaire brut ne peut pas etre negatif");
    }

    return brut * 0.0918f;
}

float EmployeDAO::calculerImpot(float brut)
{
    if (brut < 0.0f) {
        throw std::invalid_argument("Le salaire brut ne peut pas etre negatif");
    }

    if (brut <= 500.0f) return 0.0f;
    if (brut <= 1000.0f) return brut * 0.10f;
    if (brut <= 2000.0f) return brut * 0.20f;
    return brut * 0.30f;
}

float EmployeDAO::calculerSalaireNet(float brut)
{
    if (brut < 0.0f) {
        throw std::invalid_argument("Le salaire brut ne peut pas etre negatif");
    }

    return brut - calculerCNSS(brut) - calculerImpot(brut);
}

EmployeDAO::Salaire EmployeDAO::calculerSalaireComplet(float heures, const QString &role)
{
    const float brut = calculerSalaireBrut(heures, role);

    Salaire salaire;
    salaire.brut = brut;
    salaire.cnss = calculerCNSS(brut);
    salaire.impot = calculerImpot(brut);
    salaire.net = brut - salaire.cnss - salaire.impot;

    return salaire;
}

QString EmployeDAO::ajouter(const EmployeUser &employe)
{
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return "La connexion à la base de données n'est pas ouverte.";
        }
        
        QSqlQuery query(db);
        
        query.prepare("INSERT INTO EMPLOYES (CIN, NOM, PRENOM, EMAIL, LOGIN, MOTDEPASSE, ROLE, STATUT, HEUREDETRAVAIL) "
                      "VALUES (:cin, :nom, :prenom, :email, :login, :password, :role, :statut, :heures)");
        
        query.addBindValue(employe.cin);
        query.addBindValue(employe.nom);
        query.addBindValue(employe.prenom);
        query.addBindValue(employe.email);
        query.addBindValue(employe.login);
        query.addBindValue(employe.password);
        query.addBindValue(employe.role);
        query.addBindValue(employe.statut);
        query.addBindValue(employe.heures);
        
        if (!query.exec()) {
            QString errorMsg = query.lastError().text();
            return "Erreur lors de l'ajout de l'employé: " + errorMsg;
        }
        
        return "";
    }
    catch (const std::exception& e) {
        return "Erreur: " + QString(e.what());
    }
}

QVector<EmployeUser> EmployeDAO::afficher()
{
    QVector<EmployeUser> employes;
    
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return employes;
        }
        
        QSqlQuery query(db);
        query.prepare("SELECT ID_EMPLOYE, CIN, NOM, PRENOM, EMAIL, LOGIN, MOTDEPASSE, ROLE, STATUT, HEUREDETRAVAIL FROM EMPLOYES");
        
        if (!query.exec()) {
            return employes;
        }
        
        while (query.next()) {
            EmployeUser employe;
            employe.cin = query.value("CIN").toString();
            employe.nom = query.value("NOM").toString();
            employe.prenom = query.value("PRENOM").toString();
            employe.email = query.value("EMAIL").toString();
            employe.login = query.value("LOGIN").toString();
            employe.password = query.value("MOTDEPASSE").toString();
            employe.role = query.value("ROLE").toString();
            employe.statut = query.value("STATUT").toString();
            employe.heures = query.value("HEUREDETRAVAIL").toDouble();
            
            employes.append(employe);
        }
        
        return employes;
    }
    catch (const std::exception& e) {
        return employes;
    }
}

QString EmployeDAO::modifier(const EmployeUser &employe)
{
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return "La connexion à la base de données n'est pas ouverte.";
        }
        
        QSqlQuery query(db);
        
        query.prepare("UPDATE EMPLOYES SET "
                      "NOM = :nom, "
                      "PRENOM = :prenom, "
                      "EMAIL = :email, "
                      "LOGIN = :login, "
                      "MOTDEPASSE = :password, "
                      "ROLE = :role, "
                      "STATUT = :statut, "
                      "HEUREDETRAVAIL = :heures "
                      "WHERE CIN = :cin");
        
        query.addBindValue(employe.nom);
        query.addBindValue(employe.prenom);
        query.addBindValue(employe.email);
        query.addBindValue(employe.login);
        query.addBindValue(employe.password);
        query.addBindValue(employe.role);
        query.addBindValue(employe.statut);
        query.addBindValue(employe.heures);
        query.addBindValue(employe.cin);
        
        if (!query.exec()) {
            QString errorMsg = query.lastError().text();
            return "Erreur lors de la modification de l'employé: " + errorMsg;
        }
        
        if (query.numRowsAffected() == 0) {
            return "Aucun employé trouvé avec le CIN: " + employe.cin;
        }
        
        return "";
    }
    catch (const std::exception& e) {
        return "Erreur: " + QString(e.what());
    }
}

QString EmployeDAO::supprimer(const QString &cin)
{
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return "La connexion à la base de données n'est pas ouverte.";
        }
        
        QSqlQuery query(db);
        
        query.prepare("DELETE FROM EMPLOYES WHERE CIN = :cin");
        query.addBindValue(cin);
        
        if (!query.exec()) {
            QString errorMsg = query.lastError().text();
            return "Erreur lors de la suppression de l'employé: " + errorMsg;
        }
        
        if (query.numRowsAffected() == 0) {
            return "Aucun employé trouvé avec le CIN: " + cin;
        }
        
        return "";
    }
    catch (const std::exception& e) {
        return "Erreur: " + QString(e.what());
    }
}

bool EmployeDAO::isValidEmail(const QString &email)
{
    return email.contains('@') && email.contains('.');
}

bool EmployeDAO::isAllDigits(const QString &str)
{
    if (str.isEmpty()) return false;
    
    for (const QChar &c : str) {
        if (!c.isDigit()) {
            return false;
        }
    }
    
    return true;
}

EmployeUser EmployeDAO::findByLogin(const QString &login)
{
    EmployeUser employe;
    
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return employe;
        }
        
        QSqlQuery query(db);
        query.prepare("SELECT CIN, NOM, PRENOM, EMAIL, LOGIN, MOTDEPASSE, ROLE, STATUT, HEUREDETRAVAIL "
                      "FROM EMPLOYES WHERE LOGIN = :login");
        query.addBindValue(login);
        
        if (!query.exec()) {
            return employe;
        }
        
        if (query.next()) {
            employe.cin = query.value("CIN").toString();
            employe.nom = query.value("NOM").toString();
            employe.prenom = query.value("PRENOM").toString();
            employe.email = query.value("EMAIL").toString();
            employe.login = query.value("LOGIN").toString();
            employe.password = query.value("MOTDEPASSE").toString();
            employe.role = query.value("ROLE").toString();
            employe.statut = query.value("STATUT").toString();
            employe.heures = query.value("HEUREDETRAVAIL").toDouble();
        }
        
        return employe;
    }
    catch (const std::exception& e) {
        return employe;
    }
}

QString EmployeDAO::validateCINUnique(const QString &cin, const QString &excludeCIN)
{
    if (cin.isEmpty()) {
        return "";
    }
    
    if (cinExists(cin, excludeCIN)) {
        return "CIN déjà existant.";
    }
    
    return "";
}

bool EmployeDAO::cinExists(const QString &cin, const QString &excludeCIN)
{
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            return false;
        }
        
        QSqlQuery query(db);
        
        if (!excludeCIN.isEmpty() && excludeCIN == cin) {
            query.prepare("SELECT COUNT(*) as count FROM EMPLOYES WHERE CIN = :cin AND CIN != :excludeCIN");
            query.addBindValue(cin);
            query.addBindValue(excludeCIN);
        } else {
            query.prepare("SELECT COUNT(*) as count FROM EMPLOYES WHERE CIN = :cin");
            query.addBindValue(cin);
        }
        
        if (!query.exec()) {
            return false;
        }
        
        if (query.next()) {
            int count = query.value("count").toInt();
            return count > 0;
        }
        
        return false;
    }
    catch (const std::exception& e) {
        return false;
    }
}
