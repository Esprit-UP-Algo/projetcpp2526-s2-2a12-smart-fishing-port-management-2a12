cmake_minimum_required(VERSION 3.16)
project(ArriveesAccostage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport)

qt_add_executable(appArriveesAccostage
    src/main.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/quai.h
    src/quaimodel.h
    src/quaimodel.cpp
    src/quaisortfilterproxymodel.h
    src/quaisortfilterproxymodel.cpp
    src/pdfexporter.h
    src/pdfexporter.cpp
    resources/resources.qrc
)

target_link_libraries(appArriveesAccostage PRIVATE Qt6::Widgets Qt6::PrintSupport)
