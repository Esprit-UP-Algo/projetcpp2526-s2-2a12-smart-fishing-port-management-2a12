#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class Connection
{
public:
    static Connection &createInstance();

    bool createConnection();
    void closeConnection();

    QSqlDatabase database() const;

private:
    Connection();
    ~Connection();

    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;

    QString m_connectionName;
    QSqlDatabase m_db;
};

#endif // CONNECTION_H
