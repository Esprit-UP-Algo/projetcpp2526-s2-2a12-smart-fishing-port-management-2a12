QT += core gui sql printsupport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = VisionSightPoissons
TEMPLATE = app

# Activer les avertissements
QMAKE_CXXFLAGS += -Wall -Wextra

# Chemin pour les fichiers objets et mocs
MOC_DIR = build/moc
OBJECTS_DIR = build/obj
RCC_DIR = build/rcc

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/statcircle.cpp \
    src/documentexporter.cpp \
    src/lotdialog.cpp \
    src/chambrefroidedialog.cpp \
    src/sidebarwidget.cpp \
    models/capturelotmodel.cpp \
    models/capturelotproxymodel.cpp

HEADERS += \
    src/mainwindow.h \
    src/statcircle.h \
    src/documentexporter.h \
    src/lotdialog.h \
    src/chambrefroidedialog.h \
    src/sidebarwidget.h \
    models/capturelot.h \
    models/capturelotmodel.h \
    models/capturelotproxymodel.h

# Installation de ressources (optionnel)
RESOURCES += resources/resources.qrc

# Installation
target.path = .
INSTALLS += target
