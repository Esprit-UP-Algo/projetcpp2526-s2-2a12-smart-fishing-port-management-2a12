#include "employe_dao.h"
#include "../db/connection.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QRegularExpression>

using namespace employes;

EmployeDAO::EmployeDAO()
{
}

EmployeDAO::~EmployeDAO()
{
}

QString EmployeDAO::validateCIN(const QString &cin)
{
    if (cin.isEmpty()) {
        return "Le CIN est obligatoire.";
    }
    
    if (!isAllDigits(cin)) {
        return "Le CIN doit contenir uniquement des chiffres.";
    }
    
    if (cin.length() != 8) {
        return "Le CIN doit contenir exactement 8 chiffres.";
    }
    
    return "";
}(const QString &email)(const QString &email)
{
    if (email.isEmpty()) {
        return "L'email est obligatoire.";
    }
    
    if (!isValidEmail(email)) {
        return "L'email n'est pas valide (doit contenir '@' et '.').";
    }
    
    return "";
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

QString EmployeDAO::validateEmploye(const EmployeUser &employe)
{
    QString cinError = validateCIN(employe.cin);
    if (!cinError.isEmpty()) return cinError;
    
    QString emailError = validateEmail(employe.email);
    if (!emailError.isEmpty()) return emailError;
    
    QString nomError = validateNotEmpty(employe.nom, "Le nom");
    if (!nomError.isEmpty()) return nomError;
    
    QString prenomError = validateNotEmpty(employe.prenom, "Le prénom");
    if (!prenomError.isEmpty()) return prenomError;
    
    QString passwordError = validatePassword(employe.password);
    if (!passwordError.isEmpty()) return passwordError;
    
    QString roleError = validateNotEmpty(employe.role, "Le rôle");
    if (!roleError.isEmpty()) return roleError;

    return "";
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
            qDebug() << "EmployeDAO::ajouter - SQL Error:" << errorMsg;
            return "Erreur lors de l'ajout de l'employé: " + errorMsg;
        }
        
        qDebug() << "EmployeDAO::ajouter - SUCCESS: Employee" << employe.prenom << employe.nom << "added";
        return "";
    }
    catch (const std::exception& e) {
        qDebug() << "EmployeDAO::ajouter - Exception:" << e.what();
        return "Erreur d'exception: " + QString(e.what());
    }
}

QVector<EmployeUser> EmployeDAO::afficher()
{
    QVector<EmployeUser> employes;
    
    try {
        Connection& dbConnection = Connection::createInstance();
        QSqlDatabase db = dbConnection.getDatabase();
        
        if (!db.isOpen()) {
            qDebug() << "EmployeDAO::afficher - Database not open";
            return employes;
        }
        
        QSqlQuery query(db);
        query.prepare("SELECT ID_EMPLOYE, CIN, NOM, PRENOM, EMAIL, LOGIN, MOTDEPASSE, ROLE, STATUT, HEUREDETRAVAIL FROM EMPLOYES");
        
        if (!query.exec()) {
            qDebug() << "EmployeDAO::afficher - SQL Error:" << query.lastError().text();
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
        
        qDebug() << "EmployeDAO::afficher - Retrieved" << employes.count() << "employees";
        return employes;
    }
    catch (const std::exception& e) {
        qDebug() << "EmployeDAO::afficher - Exception:" << e.what();
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
            qDebug() << "EmployeDAO::modifier - SQL Error:" << errorMsg;
            return "Erreur lors de la modification de l'employé: " + errorMsg;
        }
        
        if (query.numRowsAffected() == 0) {
            qDebug() << "EmployeDAO::modifier - Employee not found:" << employe.cin;
            return "Aucun employé trouvé avec le CIN: " + employe.cin;
        }
        
        qDebug() << "EmployeDAO::modifier - SUCCESS: Employee" << employe.prenom << employe.nom << "modified";
        return "";
    }
    catch (const std::exception& e) {
        qDebug() << "EmployeDAO::modifier - Exception:" << e.what();
        return "Erreur d'exception: " + QString(e.what());
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
            qDebug() << "EmployeDAO::supprimer - SQL Error:" << errorMsg;
            return "Erreur lors de la suppression de l'employé: " + errorMsg;
        }
        
        if (query.numRowsAffected() == 0) {
            qDebug() << "EmployeDAO::supprimer - Employee not found:" << cin;
            return "Aucun employé trouvé avec le CIN: " + cin;
        }
        
        qDebug() << "EmployeDAO::supprimer - SUCCESS: Employee" << cin << "deleted";
        return "";
    }
    catch (const std::exception& e) {
        qDebug() << "EmployeDAO::supprimer - Exception:" << e.what();
        return "Erreur d'exception: " + QString(e.what());
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
