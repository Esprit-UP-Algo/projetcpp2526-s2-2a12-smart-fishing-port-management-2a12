cmake_minimum_required(VERSION 3.16)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(GestionDesNavires LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets Network PrintSupport Sql)
endif()

add_executable(appGestionDesNavires
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    dialognavire.h
    dialognavire.cpp
    dialogmouvement.h
    dialogmouvement.cpp
    logindialog.h
    logindialog.cpp
    securepassword.h
    securepassword.cpp
    connection.h
    connection.cpp
    navire.h
    qrcodegen.hpp
    qrcodegen.cpp
    resources.qrc
    vision_sight.jpg
)

target_include_directories(appGestionDesNavires PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(appGestionDesNavires PRIVATE
    Qt6::Widgets
    Qt6::Network
    Qt6::PrintSupport
    Qt6::Sql
)

if (WIN32)
    target_link_libraries(appGestionDesNavires PRIVATE Crypt32)
endif()
