cmake_minimum_required(VERSION 3.16)
project(VisionSightPoissons LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Gui Widgets Sql REQUIRED)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/statcircle.cpp
    src/statcircle.h
    models/capturelot.h
    models/capturelotmodel.cpp
    models/capturelotmodel.h
    models/capturelotproxymodel.cpp
    models/capturelotproxymodel.h
)

set(RESOURCES
    resources/resources.qrc
)

add_executable(VisionSightPoissons
    ${SOURCES}
    ${RESOURCES}
)

target_link_libraries(VisionSightPoissons
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)
