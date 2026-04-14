cmake_minimum_required(VERSION 3.16)
project(AppGestionEmploye LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Core Gui Widgets Sql Network Concurrent)

# ============================================
# Main Application Executable
# ============================================
qt_add_executable(AppGestionEmploye
    # Entry point
    src/main.cpp

    # ========== MODEL LAYER ==========
    
    # Database Connection (Singleton Pattern)
    src/modele/db/connection.h
    src/modele/db/connection.cpp
    
    # Gestion Employes - Data & Business Logic
    src/modele/gestion_employes/employe_user.h
    src/modele/gestion_employes/employe_dao.h
    src/modele/gestion_employes/employe_dao.cpp
    src/modele/gestion_employes/employe_service.h
    src/modele/gestion_employes/employe_service.cpp
    src/modele/gestion_employes/auth_helper.h
    src/modele/gestion_employes/auth_helper.cpp
    src/modele/gestion_employes/input_validator.h
    src/modele/gestion_employes/input_validator.cpp

    # ========== VIEW LAYER ==========
    
    # Main Windows & Dialogs
    src/view/gestion_employes/employe_mainwindow.h
    src/view/gestion_employes/employe_mainwindow.cpp
    src/view/gestion_employes/employe_mainwindow.ui
    
    src/view/gestion_employes/employe_logindialog.h
    src/view/gestion_employes/employe_logindialog.cpp
    src/view/gestion_employes/employe_logindialog.ui
    
    src/view/gestion_employes/employe_userdialog.h
    src/view/gestion_employes/employe_userdialog.cpp
    src/view/gestion_employes/employe_userdialog.ui
    
    src/view/gestion_employes/employe_payslip.h
    src/view/gestion_employes/employe_payslip.cpp
    src/view/gestion_employes/employe_payslipdialog.ui
    src/view/gestion_employes/payslip_pdf_exporter.h
    src/view/gestion_employes/payslip_pdf_exporter.cpp
    src/view/gestion_employes/config_manager.h
    src/view/gestion_employes/config_manager.cpp
    src/view/gestion_employes/smtp_mailer.h
    src/view/gestion_employes/smtp_mailer.cpp
    src/view/gestion_employes/smtp_settingsdialog.h
    src/view/gestion_employes/smtp_settingsdialog.cpp
    
    # Resources & Translations
    src/view/gestion_employes/employe_resources.qrc
    src/view/gestion_employes/employe_transaction_fr_DZ.ts
)

# ============================================
# Link Qt Libraries
# ============================================
target_link_libraries(AppGestionEmploye PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    Qt6::Concurrent
)
