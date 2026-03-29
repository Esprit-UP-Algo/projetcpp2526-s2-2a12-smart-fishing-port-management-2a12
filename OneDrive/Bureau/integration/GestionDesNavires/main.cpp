#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include "logindialog.h"
#include "securepassword.h"

#include <QCoreApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName(QStringLiteral("GestionDesNavires"));
    QCoreApplication::setApplicationName(QStringLiteral("GestionDesNavires"));

    // Tentative de connexion automatique (mot de passe chiffré DPAPI dans QSettings).
    {
        QSettings s;
        s.beginGroup(QStringLiteral("db"));
        const QString dsn = s.value(QStringLiteral("dsn"), QStringLiteral("source_projet2A")).toString();
        const QString user = s.value(QStringLiteral("user"), QStringLiteral("mouradd")).toString();
        const QString protectedPass = s.value(QStringLiteral("pass_protected")).toString();
        s.endGroup();

        QString decryptErr;
        const QString pass = unprotectSecret(protectedPass, &decryptErr);
        QString connErr;

        if (!pass.isEmpty() && Connection::instance()->createConnect(dsn, user, pass, &connErr)) {
            MainWindow w;
            w.show();
            return a.exec();
        }
    }

    // Fallback: afficher le dialogue pour (re)saisir le mot de passe.
    LoginDialog login;
    if (login.exec() != QDialog::Accepted)
        return 0;

    MainWindow w;
    w.show();

    return a.exec();
}
