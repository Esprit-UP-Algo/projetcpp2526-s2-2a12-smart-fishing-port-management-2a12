#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Définir le style par défaut
    app.setStyle("Fusion");

    captures::MainWindow window;
    window.show();

    return app.exec();
}
