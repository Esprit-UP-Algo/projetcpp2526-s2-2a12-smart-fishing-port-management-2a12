#include "employe_mainwindow.h"
#include "employe_logindialog.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

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

    employes::EmployeLoginDialog login;
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    employes::EmployeMainWindow w;
    w.show();
    return a.exec();
}
