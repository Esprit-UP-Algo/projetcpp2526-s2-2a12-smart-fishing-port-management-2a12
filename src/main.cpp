#include "view/MainWindow.h"

#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Shared settings namespace expected by embedded modules.
    QCoreApplication::setOrganizationName(QStringLiteral("VisionSight"));
    QCoreApplication::setApplicationName(QStringLiteral("IntegrationSuite"));

    MainWindow w;
    w.show();

    return app.exec();
}
