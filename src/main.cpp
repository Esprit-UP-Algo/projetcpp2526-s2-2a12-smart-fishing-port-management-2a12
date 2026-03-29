#include <QApplication>
#include <QMessageBox>

#include "modele/db/connection.h"
#include "view/gestion_employes/employe_logindialog.h"
#include "view/gestion_employes/employe_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Connection& dbConnection = Connection::createInstance();

    if (!dbConnection.createConnect())
    {
        QString errorDetails = dbConnection.getLastError();
        QMessageBox::critical(nullptr,
                              "Erreur Connexion Base de Donnees",
                              "La connexion a echoue.\n\n" + errorDetails);
        return -1;
    }

    QMessageBox::information(nullptr,
                             "Connexion OK",
                             "Connexion a la base de donnees etablie.");

    employes::EmployeLoginDialog loginDialog;
    if (loginDialog.exec() != QDialog::Accepted)
    {
        return 0;
    }

    employes::EmployeMainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}