cmake_minimum_required(VERSION 3.16)
project(ArriveesAccostage LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport Sql Network)

qt_add_executable(appArriveesAccostage
    src/main.cpp
    src/connection.h
    src/connection.cpp
    src/mainwindow.h
    src/mainwindow.cpp
    src/mainwindow.ui
    src/widgets/circularstatwidget.h
    src/widgets/circularstatwidget.cpp
    src/widgets/minimapwidget.h
    src/widgets/minimapwidget.cpp
    src/quai.h
    src/quaidialogs.h
    src/quaidialogs.cpp
    src/quaimodel.h
    src/quaimodel.cpp
    src/quaisearchfilterproxymodel.h
    src/quaisearchfilterproxymodel.cpp
    src/quaisortproxymodel.h
    src/quaisortproxymodel.cpp
    src/pdfexporter.h
    src/pdfexporter.cpp
    src/pdfshareserver.h
    src/pdfshareserver.cpp
    resources/resources.qrc
)

target_include_directories(appArriveesAccostage PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(appArriveesAccostage PRIVATE Qt6::Widgets Qt6::PrintSupport Qt6::Sql Qt6::Network)
