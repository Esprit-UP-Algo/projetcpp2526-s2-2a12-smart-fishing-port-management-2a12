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
        qDebug() << "✓ Database already connected";
        return true;
    }

    db.setDatabaseName("Source_Projet2A");
    db.setUserName("mouradd");
    db.setPassword("mourad123");

    qDebug() << "📡 Attempting to connect to Oracle database...";
    qDebug() << "   DSN: Source_Projet2A";
    qDebug() << "   User: mouradd";
    qDebug() << "   Driver: " << db.driverName();

    if (!db.open())
    {
        QSqlError error = db.lastError();
        lastError = error.text();
        
        qDebug() << "✗ ERROR: Connection failed!";
        qDebug() << "  Error type:" << error.type();
        qDebug() << "  Error text:" << error.text();
        qDebug() << "  Database error:" << error.databaseText();
        qDebug() << "  Driver error:" << error.driverText();
        
        qDebug() << "\n🔍 DIAGNOSTIC CHECKS:";
        qDebug() << "  - Verify ODBC DSN 'Source_Projet2A' exists in Windows ODBC Admin";
        qDebug() << "  - Check credentials (mouradd / mourad123)";
        qDebug() << "  - Verify Oracle server is accessible";
        qDebug() << "  - Check if QODBC driver is installed";
        qDebug() << "  - Available drivers:" << QSqlDatabase::drivers();
        
        return false;
    }

    qDebug() << "✓ SUCCESS: Connected to Oracle database!";
    qDebug() << "  Database Name:" << db.databaseName();
    qDebug() << "  User Name:" << db.userName();
    qDebug() << "  Host Name:" << db.hostName();
    qDebug() << "  Port:" << db.port();
    qDebug() << "  Driver:" << db.driverName();
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
