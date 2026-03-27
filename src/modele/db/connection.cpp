#include "connection.h"
#include <QSqlError>
#include <QDebug>

/**
 * @brief Private constructor - Initializes the QODBC driver
 * 
 * The constructor is private to enforce the Singleton pattern.
 * It initializes the QSqlDatabase object with the QODBC driver.
 */
Connection::Connection()
{
    // Initialize database with QODBC driver
    db = QSqlDatabase::addDatabase("QODBC", "Oracle_Connection");
}

/**
 * @brief Meyers Singleton Implementation
 * 
 * This is a thread-safe implementation using a static local variable.
 * The instance is created only once on first call and destroyed at program exit.
 * 
 * @return Connection& Reference to the unique static instance
 */
Connection& Connection::createInstance()
{
    static Connection instance;
    return instance;
}

/**
 * @brief Establishes connection to the Oracle database
 * 
 * Connection details:
 * - Driver: QODBC
 * - DSN: Source_Projet2A
 * - Username: mouradd
 * - Password: mourad123
 * 
 * @return bool true if connection successful, false otherwise
 */
bool Connection::createConnect()
{
    // If already connected, return success
    if (db.isOpen())
    {
        qDebug() << "✓ Database already connected";
        return true;
    }

    // Set connection parameters
    db.setDatabaseName("Source_Projet2A");  // DSN name
    db.setUserName("mouradd");              // Username
    db.setPassword("mourad123");            // Password

    qDebug() << "📡 Attempting to connect to Oracle database...";
    qDebug() << "   DSN: Source_Projet2A";
    qDebug() << "   User: mouradd";
    qDebug() << "   Driver: " << db.driverName();

    // Attempt to open the connection
    if (!db.open())
    {
        //lastError = db.lastError().text();
        QSqlError error = db.lastError();
        lastError = error.text();
        
        qDebug() << "✗ ERROR: Connection failed!";
        qDebug() << "  Error type:" << error.type();
        qDebug() << "  Error text:" << error.text();
        qDebug() << "  Database error:" << error.databaseText();
        qDebug() << "  Driver error:" << error.driverText();
        
        // Additional diagnostic info
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

/**
 * @brief Gets the QSqlDatabase object for executing queries
 * 
 * @return QSqlDatabase& Reference to the database object
 */
QSqlDatabase& Connection::getDatabase()
{
    return db;
}

/**
 * @brief Gets the last error message
 * 
 * @return QString containing the error message
 */
QString Connection::getLastError() const
{
    return lastError;
}

/**
 * @brief Checks if the database connection is active
 * 
 * @return bool true if connection is open, false otherwise
 */
bool Connection::isConnected() const
{
    return db.isOpen();
}
