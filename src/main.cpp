#include <QApplication>

#include "host_mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    HostMainWindow w;
    w.show();

    return app.exec();
}
