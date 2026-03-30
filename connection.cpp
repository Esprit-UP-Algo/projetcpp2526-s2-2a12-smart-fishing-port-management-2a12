
#include "connection.h"

#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>

Connection::Connection()
{
	// Création (ou récupération) d'une connexion nommée pour éviter les doublons.
	const QString connName = QStringLiteral("transaction_db");
	if (QSqlDatabase::contains(connName))
		db = QSqlDatabase::database(connName);
	else
		db = QSqlDatabase();
}

Connection::~Connection()
{
	closeConnection();
}

Connection& Connection::createInstance()
{
	static Connection instance;
	return instance;
}

QString Connection::lastError() const
{
	return m_lastError;
}

QSqlDatabase Connection::database() const
{
	return db;
}

void Connection::closeConnection()
{
	if (db.isValid() && db.isOpen())
		db.close();

	const QString connName = QStringLiteral("transaction_db");
	if (QSqlDatabase::contains(connName)) {
		// Il faut libérer le handle avant removeDatabase
		const_cast<Connection*>(this)->db = QSqlDatabase();
		QSqlDatabase::removeDatabase(connName);
	}
}

bool Connection::createConnection()
{
	m_lastError.clear();

	const QString connName = QStringLiteral("transaction_db");
	if (QSqlDatabase::contains(connName)) {
		db = QSqlDatabase::database(connName);
		if (db.isOpen())
			return true;
	}

	// Aide à la découverte des plugins SQL si tu lances depuis un dossier déployé.
	const QString appDir = QCoreApplication::applicationDirPath();
	if (!appDir.isEmpty()) {
		QCoreApplication::addLibraryPath(appDir);
		QCoreApplication::addLibraryPath(appDir + QStringLiteral("/plugins"));
	}

	const QString dbName = QStringLiteral("source_projet2A");
	const QString user = QStringLiteral("mouradd");
	// Mot de passe fourni par l'utilisateur
	const QString pass = QStringLiteral("mourad123");

	// Settings that can be adapted to your environment. Prefer to move
	// credentials and host info to environment variables or a config file.
	const QString host = QStringLiteral("localhost");
	const QString port = QStringLiteral("1521");
	const QString service = dbName; // Oracle service / SID

	auto tryOpen = [&](const QString &driver, const QString &databaseName) -> bool {
		QString lastConnInfo;
		if (!QSqlDatabase::isDriverAvailable(driver))
			return false;

		if (QSqlDatabase::contains(connName)) {
			db = QSqlDatabase::database(connName);
			db.close();
			db = QSqlDatabase();
			QSqlDatabase::removeDatabase(connName);
		}


		db = QSqlDatabase::addDatabase(driver, connName);

		// Build an appropriate connection string depending on driver.
		if (driver == QLatin1String("QOCI")) {
			// Try multiple service names: TNS alias, then common defaults (XE, ORCL), then host:port/service
			const QStringList services = { databaseName, QStringLiteral("XE"), QStringLiteral("ORCL") };
			bool opened = false;
			QString combinedErr;
			for (const QString &svc : services) {
				db.setDatabaseName(svc);
				db.setUserName(user);
				db.setPassword(pass);
				if (db.open()) { opened = true; break; }
				const QSqlError e = db.lastError();
				combinedErr += QStringLiteral("[%1] %2").arg(svc, e.text());
				if (!e.driverText().isEmpty()) combinedErr += QStringLiteral(" | ") + e.driverText();
				combinedErr += QStringLiteral(" ; ");
			}
			if (!opened) {
				// Final attempt with explicit EZCONNECT
				const QString connectString = QStringLiteral("//%1:%2/%3").arg(host, port, service);
				db.close();
				db.setDatabaseName(connectString);
				db.setUserName(user);
				db.setPassword(pass);
				if (!db.open()) {
					const QSqlError e = db.lastError();
					m_lastError = combinedErr + QStringLiteral("[EZCONNECT] %1").arg(e.text());
					if (!e.driverText().isEmpty()) m_lastError += QStringLiteral(" | ") + e.driverText();
					return false;
				}
			}
		} else if (driver == QLatin1String("QODBC")) {
			// For QODBC we try a DSN-less connection string including an ODBC
			// driver name. On Windows the ODBC manager may treat a plain
			// Dbq=... string as a DSN name (too long) — prefixing with
			// "DRIVER={...};" forces a true connection string.
			const QString odbcDriver = qEnvironmentVariable("ODBC_ORACLE_DRIVER",
				QStringLiteral("Oracle in XE"));
			// Prefer EZCONNECT (//host:port/service) when no TNS alias is present.
			const QStringList services = { databaseName, QStringLiteral("XE"), QStringLiteral("ORCL") };
			bool opened = false;
			QString combinedErr;
			for (const QString &svc : services) {
				const QString ezconnect = QStringLiteral("//%1:%2/%3").arg(host, port, svc);
				const QString odbcConn = QStringLiteral("DRIVER={%1};Dbq=%2;Uid=%3;Pwd=%4;")
					.arg(odbcDriver, ezconnect, user, pass);
				// Store a redacted connection info (no password) for diagnostics
				lastConnInfo = QStringLiteral("DRIVER={%1};Dbq=%2;Uid=%3").arg(odbcDriver, ezconnect, user);
				db.setDatabaseName(odbcConn);
				// Also set user/pass for drivers that use them separately.
				db.setUserName(user);
				db.setPassword(pass);
				if (db.open()) { opened = true; break; }
				const QSqlError e = db.lastError();
				combinedErr += QStringLiteral("[%1] %2").arg(ezconnect, e.text());
				if (!e.driverText().isEmpty()) combinedErr += QStringLiteral(" | ") + e.driverText();
				combinedErr += QStringLiteral(" ; ");
				db.close();
			}
			if (!opened) {
				m_lastError = combinedErr;
				if (!lastConnInfo.isEmpty()) m_lastError += QStringLiteral(" || conn=") + lastConnInfo;
				return false;
			}
		} else {
			// Fallback: try using databaseName directly
			db.setDatabaseName(databaseName);
			db.setUserName(user);
			db.setPassword(pass);
		}

		if (!db.open()) {
			// Provide both user-friendly and driver-level error details.
			const QSqlError err = db.lastError();
			m_lastError = err.text();
			if (!err.driverText().isEmpty())
				m_lastError += QStringLiteral(" | ") + err.driverText();
			// Append the last connection info (redacted) if available to help debugging
			if (!lastConnInfo.isEmpty())
				m_lastError += QStringLiteral(" || conn=") + lastConnInfo;
			return false;
		}

        // Requête test (Oracle)
        QSqlQuery q(db);
        if (!q.exec(QStringLiteral("SELECT 1 FROM dual"))) {
            m_lastError = q.lastError().text();
            db.close();
            return false;
        }
        return true;
    };

    // --- STRATEGIE DE CONNEXION ---
    // 1) QOCI (Oracle natif) : recommandé si le client Oracle est installé.
    // 2) QODBC (ODBC) : utilisé souvent dans l'Atelier Connexion.
    // L'ordre importe peu si un seul est configuré, mais QOCI est mieux.
    // On essaie les deux.
    
    if (tryOpen(QStringLiteral("QOCI"), dbName))
         return true;

    if (tryOpen(QStringLiteral("QODBC"), dbName))
         return true;

    if (m_lastError.isEmpty()) {
        m_lastError = QStringLiteral("Aucun driver (QOCI ou QODBC) n'a pu se connecter. Verifiez votre DSN (ODBC 64-bit) ou l'installation Oracle.");
    }
    return false;
}

