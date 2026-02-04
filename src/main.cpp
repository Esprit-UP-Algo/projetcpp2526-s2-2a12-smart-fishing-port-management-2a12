#include <QApplication>
#include "src/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Définir le style par défaut
    app.setStyle("Fusion");

    MainWindow window;
    window.show();

    return app.exec();
}
