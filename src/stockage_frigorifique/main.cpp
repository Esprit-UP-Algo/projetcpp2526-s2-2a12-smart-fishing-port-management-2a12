#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    stockage::MainWindow w;
    w.show();

    return app.exec();
}
