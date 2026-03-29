cmake_minimum_required(VERSION 3.16)

# When building standalone, set up project + Qt.
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ArriveesAccostage LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport Sql)
endif()

qt_add_library(quai_module STATIC
    src/quaiconnection.h
    src/quaiconnection.cpp
    src/quaiwindow.h
    src/quaiwindow.cpp
    src/quaiwindow.ui
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
    src/quaisortfilterproxymodel.h
    src/quaisortfilterproxymodel.cpp
    src/pdfexporter.h
    src/pdfexporter.cpp
    resources/quai_resources.qrc
)

target_include_directories(quai_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(quai_module PUBLIC Qt6::Widgets Qt6::PrintSupport Qt6::Sql)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    qt_add_executable(appArriveesAccostage
        src/main.cpp
    )

    target_link_libraries(appArriveesAccostage PRIVATE quai_module)
endif()
