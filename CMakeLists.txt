cmake_minimum_required(VERSION 3.16)
project(integration_transaction_quais LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets PrintSupport Network Sql Charts)

qt_add_executable(integration_transaction_quais
    src/main.cpp
    
    # Capture Module (SuiviDesCaptures)
    src/mainwindow.cpp
    src/mainwindow.h
    src/statcircle.cpp
    src/statcircle.h
    models/capturelot.h
    models/capturelotmodel.cpp
    models/capturelotmodel.h
    models/capturelotproxymodel.cpp
    models/capturelotproxymodel.h
    src/documentexporter.cpp
    src/documentexporter.h
    src/lotdialog.cpp
    src/lotdialog.h
    src/chambrefroidedialog.cpp
    src/chambrefroidedialog.h
    src/gestioncapture.ui

    # Integration - Host
    src/host_mainwindow.h
    src/host_mainwindow.cpp
    src/host_mainwindow.ui

    # Quais Module
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

    # Transaction Module
    src/transaction/mainwindow.h
    src/transaction/mainwindow.cpp
    src/transaction/transaction_mainwindow.ui
    src/transaction/transaction.h
    src/transaction/transactiondialog.h
    src/transaction/transactiondialog.cpp
    src/transaction/transactiondialog.ui

    src/navires/mainwindow.h
    src/navires/mainwindow.cpp
    src/navires/mainwindow.ui
    src/navires/navire.h
    src/navires/dialognavire.h
    src/navires/dialognavire.cpp
    src/navires/dialogmouvement.h
    src/navires/dialogmouvement.cpp
    src/navires/resources.qrc

    # Suivi des captures (integrated)
    src/suivi_des_captures/mainwindow.h
    src/suivi_des_captures/mainwindow.cpp
    src/suivi_des_captures/sidebarwidget.h
    src/suivi_des_captures/sidebarwidget.cpp
    src/suivi_des_captures/statcircle.h
    src/suivi_des_captures/statcircle.cpp
    src/suivi_des_captures/documentexporter.h
    src/suivi_des_captures/documentexporter.cpp
    src/suivi_des_captures/lotdialog.h
    src/suivi_des_captures/lotdialog.cpp
    src/suivi_des_captures/chambrefroidedialog.h
    src/suivi_des_captures/chambrefroidedialog.cpp
    src/suivi_des_captures/models/capturelot.h
    src/suivi_des_captures/models/capturelotmodel.h
    src/suivi_des_captures/models/capturelotmodel.cpp
    src/suivi_des_captures/models/capturelotproxymodel.h
    src/suivi_des_captures/models/capturelotproxymodel.cpp
    src/suivi_des_captures/resources/resources.qrc

    resources/resources.qrc
)

target_include_directories(integration_transaction_quais PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/quais
    ${CMAKE_CURRENT_SOURCE_DIR}/src/transaction
    ${CMAKE_CURRENT_SOURCE_DIR}/src/navires
    ${CMAKE_CURRENT_SOURCE_DIR}/src/suivi_des_captures
    ${CMAKE_CURRENT_SOURCE_DIR}/src/suivi_des_captures/models
)

target_link_libraries(integration_transaction_quais PRIVATE Qt6::Widgets Qt6::PrintSupport Qt6::Network Qt6::Sql Qt6::Charts)

target_compile_definitions(integration_transaction_quais PRIVATE INTEGRATION_APP)

# --- Suivi des captures (integrated from suivi_des_captures4.0) ---
find_package(Qt6 6.7 REQUIRED COMPONENTS Core Gui Widgets Sql PrintSupport Charts)

qt_add_executable(VisionSightPoissons
    src/suivi_des_captures/main.cpp
    src/suivi_des_captures/mainwindow.cpp
    src/suivi_des_captures/mainwindow.h
    src/suivi_des_captures/sidebarwidget.cpp
    src/suivi_des_captures/sidebarwidget.h
    src/suivi_des_captures/statcircle.cpp
    src/suivi_des_captures/statcircle.h
    src/suivi_des_captures/documentexporter.cpp
    src/suivi_des_captures/documentexporter.h
    src/suivi_des_captures/lotdialog.cpp
    src/suivi_des_captures/lotdialog.h
    src/suivi_des_captures/chambrefroidedialog.cpp
    src/suivi_des_captures/chambrefroidedialog.h
    src/suivi_des_captures/models/capturelot.h
    src/suivi_des_captures/models/capturelotmodel.cpp
    src/suivi_des_captures/models/capturelotmodel.h
    src/suivi_des_captures/models/capturelotproxymodel.cpp
    src/suivi_des_captures/models/capturelotproxymodel.h
    src/suivi_des_captures/gestioncapture.ui
    src/suivi_des_captures/resources/resources.qrc
)

target_include_directories(VisionSightPoissons PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/suivi_des_captures
    ${CMAKE_CURRENT_SOURCE_DIR}/src/suivi_des_captures/models
)

target_link_libraries(VisionSightPoissons PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
    Qt6::Charts
)
