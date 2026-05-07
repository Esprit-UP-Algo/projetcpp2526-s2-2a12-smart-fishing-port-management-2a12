cmake_minimum_required(VERSION 3.16)

project(IntegrationFinale LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Core Widgets Network PrintSupport Sql)

# Used by subdirectories to locate the shared plugin interface headers.
set(INTEGRATION_FINALE_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

add_executable(integration_finale
    src/main.cpp
    src/model/ModulePageInterface.h
    src/model/ModuleKey.h
    src/model/LoadedModule.h
    src/data/ModuleLoader.h
    src/data/ModuleLoader.cpp
    src/view/MainWindow.h
    src/view/MainWindow.cpp
)

target_include_directories(integration_finale PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src"
)

target_link_libraries(integration_finale PRIVATE Qt6::Core Qt6::Widgets)

# Ensure runtime data files used by the ventes module are available next to the executable.
add_custom_command(TARGET integration_finale POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/../transaction/poissons.txt"
        "$<TARGET_FILE_DIR:integration_finale>/poissons.txt"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/../transaction/prix_poissons.txt"
        "$<TARGET_FILE_DIR:integration_finale>/prix_poissons.txt"
)

# --- Build module plugins from existing code (kept in-place) ---
# We build them as separate DLL plugins so their Qt meta-object symbols and UI files don't collide.

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../transaction" "${CMAKE_BINARY_DIR}/_modules/transaction")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../integration/integration/GestionDesNavires" "${CMAKE_BINARY_DIR}/_modules/GestionDesNavires")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../integration/integration/interface raef" "${CMAKE_BINARY_DIR}/_modules/interface_raef")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/../ferdawes" "${CMAKE_BINARY_DIR}/_modules/ferdawes")

# Frigo project (captures + stockage) plugins.
add_subdirectory(
    "${CMAKE_CURRENT_LIST_DIR}/../doudi+arnouba/gestion_stock_frigo_capturee (2)/gestion_stock_frigo_capture/gestion_stock_frigo_capture"
    "${CMAKE_BINARY_DIR}/_modules/gestion_stock_frigo_capture"
)

# Copy/runtime colocate: ensure plugin DLLs land next to the main executable.
foreach(_tgt IN ITEMS module_ventes module_navires module_quais module_captures module_stockage module_employees)
    if(TARGET ${_tgt})
        add_dependencies(integration_finale ${_tgt})
        set_target_properties(${_tgt} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    endif()
endforeach()
