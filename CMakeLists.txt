cmake_minimum_required(VERSION 3.16)
project(integration_transaction_quais LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport)

qt_add_executable(integration_transaction_quais
    src/main.cpp
    src/host_mainwindow.h
    src/host_mainwindow.cpp
    src/host_mainwindow.ui

    src/quais/mainwindow.h
    src/quais/mainwindow.cpp
    src/quais/quais_mainwindow.ui
    src/quais/widgets/circularstatwidget.h
    src/quais/widgets/circularstatwidget.cpp
    src/quais/widgets/minimapwidget.h
    src/quais/widgets/minimapwidget.cpp
    src/quais/quai.h
    src/quais/quaidialogs.h
    src/quais/quaidialogs.cpp
    src/quais/quaimodel.h
    src/quais/quaimodel.cpp
    src/quais/quaisearchfilterproxymodel.h
    src/quais/quaisearchfilterproxymodel.cpp
    src/quais/quaisortproxymodel.h
    src/quais/quaisortproxymodel.cpp
    src/quais/pdfexporter.h
    src/quais/pdfexporter.cpp

    src/transaction/mainwindow.h
    src/transaction/mainwindow.cpp
    src/transaction/transaction_mainwindow.ui
    src/transaction/transaction.h
    src/transaction/transactiondialog.h
    src/transaction/transactiondialog.cpp
    src/transaction/transactiondialog.ui

    resources/resources.qrc
)

target_include_directories(integration_transaction_quais PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/quais
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transaction
)

target_link_libraries(integration_transaction_quais PRIVATE Qt6::Widgets Qt6::PrintSupport)

target_compile_definitions(integration_transaction_quais PRIVATE INTEGRATION_APP)
