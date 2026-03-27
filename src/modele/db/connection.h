#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

/**
 * @class Connection
 * @brief Singleton Pattern for Oracle Database Connection
 * 
 * This class manages the connection to the Oracle database using the Meyers Singleton
 * pattern (thread-safe, lazy initialization). It uses the QODBC driver to connect
 * to the Source_Projet2A data source.
 * 
 * Design Pattern: Meyers Singleton (Static Local Variable)
 * - Private constructor
 * - Static method createInstance() returns const reference to unique instance
 * - Copy constructor and assignment operator are deleted
 */
class Connection
{
public:
    /**
     * @brief Gets the unique instance of Connection (Meyers Singleton)
     * @return const reference to the static instance
     */
    static Connection& createInstance();

    /**
     * @brief Establishes connection to the Oracle database
     * @return true if connection is successful, false otherwise
     */
    bool createConnect();

    /**
     * @brief Gets the QSqlDatabase object
     * @return reference to the database object
     */
    QSqlDatabase& getDatabase();

    /**
     * @brief Gets the last error message
     * @return QString containing the last error
     */
    QString getLastError() const;

    /**
     * @brief Checks if the database is currently connected
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

    // Delete copy constructor and assignment operator (enforce uniqueness)
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    /**
     * @brief Private constructor (Singleton pattern)
     */
    Connection();

    // Database object
    QSqlDatabase db;

    // Error message storage
    QString lastError;
};

#endif // CONNECTION_H
