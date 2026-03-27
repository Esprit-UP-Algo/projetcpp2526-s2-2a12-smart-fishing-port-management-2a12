#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class Connection
{
public:
    static Connection& createInstance();

    bool createConnect();

    QSqlDatabase& getDatabase();

    QString getLastError() const;

    bool isConnected() const;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    Connection();

    QSqlDatabase db;
    QString lastError;
};

#endif // CONNECTION_H
