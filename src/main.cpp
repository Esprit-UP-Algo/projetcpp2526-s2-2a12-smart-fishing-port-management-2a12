#include <QApplication>
#include <QMessageBox>

#include "connection.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!Connection::createInstance().createConnection())
    {
        QMessageBox::critical(nullptr,
                              QObject::tr("Connexion"),
                              QObject::tr("Échec de la connexion à la base de données.\n"
                                          "Vérifiez:\n"
                                          "- DSN ODBC: source_projet2A\n"
                                          "- User: raef\n"
                                          "- Driver Qt: QODBC"));
        return 1;
    }
    QMessageBox::information(nullptr, QObject::tr("Connexion"), QObject::tr("Connected successful"));

    MainWindow w;
    w.show();

    return app.exec();
}
