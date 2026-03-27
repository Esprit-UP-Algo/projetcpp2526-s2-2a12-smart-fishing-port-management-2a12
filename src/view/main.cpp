#include "gestion_employes/employe_mainwindow.h"
#include "gestion_employes/employe_logindialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QDebug>

// Include database connection
#include "../modele/db/connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "transaction_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Connection& dbConnection = Connection::createInstance();

    if (!dbConnection.createConnect())
    {
        QString errorDetails = dbConnection.getLastError();
        QMessageBox::critical(nullptr,
                              "❌ Erreur de Connexion à la Base de Données",
                              "La connexion à la base de données Oracle a échoué.\n\n"
                              "═══════════════════════════════════════\n"
                              "INFORMATIONS DE CONNEXION:\n"
                              "═══════════════════════════════════════\n"
                              "🔹 DSN: Source_Projet2A\n"
                              "🔹 Utilisateur: mouradd\n"
                              "🔹 Driver: QODBC\n\n"
                              "═══════════════════════════════════════\n"
                              "DÉTAILS DE L'ERREUR:\n"
                              "═══════════════════════════════════════\n" +
                              errorDetails + "\n\n"
                              "═══════════════════════════════════════\n"
                              "SOLUTIONS:\n"
                              "═══════════════════════════════════════\n"
                              "✓ Vérifiez que la source ODBC 'Source_Projet2A'\n"
                              "  est configurée dans le Gestionnaire ODBC\n"
                              "✓ Vérifiez les identifiants (mouradd/mourad123)\n"
                              "✓ Vérifiez que le serveur Oracle est accessible\n"
                              "✓ Vérifiez que le driver Oracle ODBC est installé");
        qDebug() << "✗ Application closed: Database connection failed";
        qDebug() << "Erreur détaillée:" << errorDetails;
        return -1;
    }

    qDebug() << "Database connection successful!\n";

    employes::EmployeLoginDialog login;
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    employes::EmployeMainWindow w;
    w.show();
    return a.exec();
}
