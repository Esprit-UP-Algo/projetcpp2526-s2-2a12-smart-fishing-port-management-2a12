cmake_minimum_required(VERSION 3.16)

project(IntegrationAll LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Put all executables in one place so the interfaces can launch each other
# via QCoreApplication::applicationDirPath().
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt6 6.7 REQUIRED COMPONENTS Widgets Network PrintSupport Sql)

# Keep GestionDesNavires first so IDEs that pick the first runnable target
# use it as default launch target.
add_subdirectory(GestionDesNavires)

# Visual Studio generator hint (harmless for Ninja/Qt Creator).
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT appGestionDesNavires)
