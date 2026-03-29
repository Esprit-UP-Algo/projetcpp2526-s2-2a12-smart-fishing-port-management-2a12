#ifndef QUAI_CONNECTION_H
#define QUAI_CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class QuaiConnection
{
public:
    static QuaiConnection &createInstance();

    bool createConnection();
    void closeConnection();

    QSqlDatabase database() const;

private:
    QuaiConnection();
    ~QuaiConnection();

    QuaiConnection(const QuaiConnection &) = delete;
    QuaiConnection &operator=(const QuaiConnection &) = delete;

    QString m_connectionName;
    QSqlDatabase m_db;
};

#endif // QUAI_CONNECTION_H
