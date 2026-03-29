#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include "securepassword.h"

// Initialisation du pointeur d'instance
Connection* Connection::p_instance = nullptr;

// Constructeur privé
Connection::Connection()
{
    // Initialisation de la base de données
    db = QSqlDatabase::addDatabase("QODBC");
}

// Méthode statique pour obtenir l'instance unique
Connection* Connection::instance()
{
    if (p_instance == nullptr) {
        p_instance = new Connection();
    }
    return p_instance;
}

// Méthode pour établir la connexion
bool Connection::createConnect()
{
    // Compatibilité: on lit les derniers paramètres utilisés.
    QSettings s;
    s.beginGroup(QStringLiteral("db"));
    const QString dsn = s.value(QStringLiteral("dsn"), QStringLiteral("source_projet2A")).toString();
    const QString user = s.value(QStringLiteral("user"), QStringLiteral("mouradd")).toString();
    const QString protectedPass = s.value(QStringLiteral("pass_protected")).toString();
    s.endGroup();

    QString decryptErr;
    const QString pass = unprotectSecret(protectedPass, &decryptErr);
    QString err;
    return createConnect(dsn, user, pass, &err);
}

bool Connection::createConnect(const QString &dsn, const QString &user, const QString &password, QString *errorMessage)
{
    if (db.isOpen())
        db.close();

    db.setDatabaseName(dsn);
    db.setUserName(user);
    db.setPassword(password);

    if (db.open()) {
        qDebug() << "Connexion à la base de données réussie";
        return true;
    }

    const QString err = db.lastError().text();
    qDebug() << "Erreur de connexion:" << err;
    if (errorMessage)
        *errorMessage = err;
    return false;
}

// Fermer la connexion
void Connection::closeConnection()
{
    if (db.isOpen()) {
        db.close();
    }
}

// Destructeur privé
Connection::~Connection()
{
    closeConnection();
}
