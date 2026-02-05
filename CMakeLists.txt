cmake_minimum_required(VERSION 3.16)
project(StockageFrigorifique LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport)

qt_add_executable(appStockageFrigorifique
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp

    src/pdfexporter.h
    src/pdfexporter.cpp

    src/chambrefroide.h
    src/chambrefroidemodel.h
    src/chambrefroidemodel.cpp
    src/chambrefroidesortfilterproxymodel.h
    src/chambrefroidesortfilterproxymodel.cpp


    src/utilisateur.h
    src/utilisateurmodel.h
    src/utilisateurmodel.cpp
    src/utilisateursortfilterproxymodel.h
    src/utilisateursortfilterproxymodel.cpp

    resources/resources.qrc
)

target_link_libraries(appStockageFrigorifique PRIVATE Qt6::Widgets Qt6::PrintSupport)
