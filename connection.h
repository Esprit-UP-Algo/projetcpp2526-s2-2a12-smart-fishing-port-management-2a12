
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QString>

class Connection
{
public:
	// Même principe que l'atelier : une seule instance.
	static Connection& createInstance();

	bool createConnection();
	void closeConnection();

	QString lastError() const;
	QSqlDatabase database() const;

private:
	Connection();
	~Connection();

	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

private:
	QSqlDatabase db;
	QString m_lastError;
};

#endif // CONNECTION_H

