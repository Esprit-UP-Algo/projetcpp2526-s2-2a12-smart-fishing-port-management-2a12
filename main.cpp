#include "mainwindow.h"
#include "connection.h"

#include <QApplication>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection &c = Connection::createInstance();
    const bool test = c.createConnection();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "transaction_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    if (test) {
        QMessageBox::information(nullptr,
                                 QObject::tr("Base de données"),
                                 QObject::tr("Connexion réussie à source_projet2A."));
    } else {
        QMessageBox::critical(nullptr,
                              QObject::tr("Base de données"),
                              QObject::tr("Échec de connexion à source_projet2A.\n\nDétails: %1")
                                  .arg(c.lastError()));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
