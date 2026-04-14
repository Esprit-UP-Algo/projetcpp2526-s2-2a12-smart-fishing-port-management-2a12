@echo off
REM ============================================
REM Ferdawes - Build Script (Windows)
REM Employee Management Application
REM ============================================

setlocal enabledelayedexpansion

echo.
echo ============================================
echo  FERDAWES - Gestion des Employes
echo  Build & Setup Script
echo ============================================
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
    echo ERROR: CMake not found in PATH
    echo Please install CMake 3.16 or higher
    echo Download: https://cmake.org/download/
    pause
    exit /b 1
)

REM Check if Qt is installed (simple check)
where qmake >nul 2>&1
if errorlevel 1 (
    echo WARNING: qmake not found in PATH
    echo Ensure Qt 6.7 is installed and added to PATH
    echo Qt Download: https://www.qt.io/download
)

echo [1/5] Cleaning previous builds...
if exist build (
    rmdir /s /q build
    echo   - Removed build directory
)
if exist CMakeCache.txt (
    del CMakeCache.txt
    echo   - Removed CMake cache
)

echo.
echo [2/5] Creating build directory...
mkdir build
cd build

echo.
echo [3/5] Configuring CMake...
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release

if errorlevel 1 (
    echo.
    echo ERROR: CMake configuration failed
    echo Try setting CMAKE_PREFIX_PATH to your Qt installation
    cd ..
    pause
    exit /b 1
)

echo.
echo [4/5] Building AppGestionEmploye...
cmake --build . --config Release --parallel 4

if errorlevel 1 (
    echo.
    echo ERROR: Build failed
    cd ..
    pause
    exit /b 1
)

echo.
echo [5/5] Build Complete!
echo.
echo ============================================
echo  BUILD SUCCESSFUL
echo ============================================
echo.
echo Executable location:
echo   .\Release\AppGestionEmploye.exe
echo.
echo To run the application:
echo   .\Release\AppGestionEmploye.exe
echo.
echo Optional: Deploy with dependencies
echo   windeployqt .\Release\AppGestionEmploye.exe
echo.

cd ..

:end_script
echo.
echo Setup complete! Press any key to exit...
pause >nul
