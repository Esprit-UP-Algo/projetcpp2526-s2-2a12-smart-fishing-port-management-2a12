#include "connection.h"
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC", "Oracle_Connection");
}

Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

bool Connection::createConnect()
{
    if (db.isOpen())
    {
        return true;
    }

    db.setDatabaseName("Source_Projet2A");
    db.setUserName("mouradd");
    db.setPassword("mourad123");

    if (!db.open())
    {
        QSqlError error = db.lastError();
        lastError = error.text();
        qDebug() << "Database connection failed:" << error.text();
        return false;
    }

    return true;
}

QSqlDatabase& Connection::getDatabase()
{
    return db;
}

QString Connection::getLastError() const
{
    return lastError;
}

bool Connection::isConnected() const
{
    return db.isOpen();
}
