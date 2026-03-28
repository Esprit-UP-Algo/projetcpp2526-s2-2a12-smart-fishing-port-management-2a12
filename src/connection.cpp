#include "connection.h"

#include <QDebug>
#include <QSqlError>

static constexpr const char *kDsn = "Source_Projet2A";
static constexpr const char *kUser = "mouradd";
static constexpr const char *kPass = "mourad123";

Connection &Connection::createInstance()
{
    static Connection instance;
    return instance;
}

Connection::Connection()
    : m_connectionName(QStringLiteral("Source_Projet2A_conn"))
{
    // Create a named connection so we don't collide with other DB users.
    if (QSqlDatabase::contains(m_connectionName))
        m_db = QSqlDatabase::database(m_connectionName);
}

Connection::~Connection()
{
    closeConnection();
}

bool Connection::createConnection()
{
    // Recreate connection cleanly if needed.
    if (m_db.isValid())
        closeConnection();

    const QStringList drivers = QSqlDatabase::drivers();
    qDebug() << "Qt SQL drivers:" << drivers;

    // We follow the atelier principle: use ODBC and a DSN named source_projet2A.
    const QString driver = drivers.contains(QStringLiteral("QODBC")) ? QStringLiteral("QODBC")
                                                                    : (drivers.contains(QStringLiteral("QODBC3")) ? QStringLiteral("QODBC3")
                                                                                                                 : QString());

    if (driver.isEmpty())
    {
        qDebug() << "No ODBC driver available in Qt (QODBC).";
        return false;
    }

    m_db = QSqlDatabase::addDatabase(driver, m_connectionName);
    m_db.setUserName(QString::fromLatin1(kUser));
    m_db.setPassword(QString::fromLatin1(kPass));

    // Try the DSN directly first.
    m_db.setDatabaseName(QString::fromLatin1(kDsn));
    if (m_db.open())
    {
        qDebug() << "Connected successful (DSN direct):" << m_db.databaseName();
        return true;
    }

    const QString err1 = m_db.lastError().text();
    qDebug() << "Open failed (DSN direct):" << err1;

    // Some ODBC setups prefer an explicit DSN=... form.
    m_db.close();
    m_db.setDatabaseName(QStringLiteral("DSN=%1").arg(QString::fromLatin1(kDsn)));
    if (m_db.open())
    {
        qDebug() << "Connected successful (DSN=...):" << m_db.databaseName();
        return true;
    }

    qDebug() << "Open failed (DSN=...):" << m_db.lastError().text();
    return false;
}

void Connection::closeConnection()
{
    if (m_db.isOpen())
        m_db.close();

    // Drop the connection from Qt's pool.
    m_db = QSqlDatabase();
    if (QSqlDatabase::contains(m_connectionName))
        QSqlDatabase::removeDatabase(m_connectionName);
}

QSqlDatabase Connection::database() const
{
    return m_db;
}
