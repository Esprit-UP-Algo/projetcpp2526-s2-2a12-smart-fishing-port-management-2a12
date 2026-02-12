#include <QApplication>
#include "host_mainwindow.h"
#include "gestion_employes/employe_logindialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    employes::EmployeLoginDialog login;
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    HostMainWindow w;
    w.show();

    return app.exec();
}
