#include <QApplication>
#include <QMessageBox>

#include "modele/db/connection.h"
#include "view/gestion_employes/employe_logindialog.h"
#include "view/gestion_employes/employe_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "\n========================================";
    qDebug() << "  VISION SIGHT - Gestion Des Employés";
    qDebug() << "========================================\n";

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
        qDebug() << "Application closed: Database connection failed";
        qDebug() << "Error details:" << errorDetails;
        return -1;
    }

    QMessageBox::information(nullptr,
                             "✅ Connexion Réussie",
                             "La connexion à la base de données Oracle a été établie avec succès.\n\n"
                             "═══════════════════════════════════════\n"
                             "INFORMATIONS DE CONNEXION:\n"
                             "═══════════════════════════════════════\n"
                             "🔹 Statut: Connecté ✓\n"
                             "🔹 DSN: Source_Projet2A\n"
                             "🔹 Utilisateur: mouradd\n"
                             "🔹 Driver: QODBC\n"
                             "🔹 Base de données: Oracle\n\n"
                             "═══════════════════════════════════════\n"
                             "L'application démarre...\n"
                             "═══════════════════════════════════════");

    qDebug() << "Database connection successful!\n";

    employes::EmployeLoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted)
    {
        qDebug() << "Login cancelled by user";
        return 0;
    }

    qDebug() << "Launching main window...\n";
    employes::EmployeMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}