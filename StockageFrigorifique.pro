QT += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = appStockageFrigorifique
TEMPLATE = app

QMAKE_CXXFLAGS += -Wall -Wextra

MOC_DIR = build_stockage/moc
OBJECTS_DIR = build_stockage/obj
RCC_DIR = build_stockage/rcc

INCLUDEPATH += src/stockage_frigorifique

SOURCES += \
    src/stockage_frigorifique/main.cpp \
    src/stockage_frigorifique/mainwindow.cpp \
    src/stockage_frigorifique/pdfexporter.cpp \
    src/stockage_frigorifique/statcircle.cpp \
    src/stockage_frigorifique/chambrefroidemodel.cpp \
    src/stockage_frigorifique/chambrefroidesortfilterproxymodel.cpp \
    src/stockage_frigorifique/addchambrefroidedialog.cpp \
    src/stockage_frigorifique/updatechambrefroidedialog.cpp \
    src/stockage_frigorifique/stockagelotmodel.cpp \
    src/stockage_frigorifique/stockagelotsortfilterproxymodel.cpp \
    src/stockage_frigorifique/addstockagelotdialog.cpp \
    src/stockage_frigorifique/updatestockagelotdialog.cpp

HEADERS += \
    src/stockage_frigorifique/mainwindow.h \
    src/stockage_frigorifique/pdfexporter.h \
    src/stockage_frigorifique/statcircle.h \
    src/stockage_frigorifique/chambrefroide.h \
    src/stockage_frigorifique/chambrefroidemodel.h \
    src/stockage_frigorifique/chambrefroidesortfilterproxymodel.h \
    src/stockage_frigorifique/addchambrefroidedialog.h \
    src/stockage_frigorifique/updatechambrefroidedialog.h \
    src/stockage_frigorifique/stockagelot.h \
    src/stockage_frigorifique/stockagelotmodel.h \
    src/stockage_frigorifique/stockagelotsortfilterproxymodel.h \
    src/stockage_frigorifique/addstockagelotdialog.h \
    src/stockage_frigorifique/updatestockagelotdialog.h

FORMS += \
    src/stockage_frigorifique/mainwindow.ui

RESOURCES += \
    src/stockage_frigorifique/resources/resources.qrc

# Installation
target.path = .
INSTALLS += target
