@echo off
REM Script de compilation pour Windows avec Qt 7.6.3
REM Assurez-vous que Qt 7.6.3 est installé et que qmake est dans le PATH

echo ============================================
echo COMPILATION - VISION SIGHT POISSONS
echo ============================================
echo.

REM Vérifier si qmake est disponible
where qmake >nul 2>nul
if %errorlevel% neq 0 (
    echo ERREUR: qmake non trouvé dans le PATH
    echo Assurez-vous que Qt 7.6.3 est installé et configuré
    pause
    exit /b 1
)

echo Nettoyage des fichiers de build...
if exist build rmdir /s /q build
if exist release rmdir /s /q release
if exist debug rmdir /s /q debug
if exist Makefile del Makefile

echo.
echo Exécution de qmake...
qmake VisionSightPoissons.pro -spec win32-msvc

if %errorlevel% neq 0 (
    echo ERREUR: qmake a échoué
    pause
    exit /b 1
)

echo.
echo Compilation du projet (utilisant nmake)...
REM Pour MinGW, remplacer nmake par make

REM Vérifier si nmake est disponible (Visual Studio)
where nmake >nul 2>nul
if %errorlevel% equ 0 (
    echo Utilisation de nmake (Visual Studio)
    nmake
) else (
    echo Utilisation de make (MinGW)
    where make >nul 2>nul
    if %errorlevel% neq 0 (
        echo ERREUR: Ni nmake ni make ne sont trouvés
        pause
        exit /b 1
    )
    make
)

if %errorlevel% neq 0 (
    echo ERREUR: La compilation a échoué
    pause
    exit /b 1
)

echo.
echo ============================================
echo COMPILATION RÉUSSIE
echo ============================================
echo.

REM Chercher l'exécutable généré
if exist release\VisionSightPoissons.exe (
    echo Exécutable trouvé: release\VisionSightPoissons.exe
    echo.
    echo Démarrage de l'application...
    start release\VisionSightPoissons.exe
) else if exist debug\VisionSightPoissons.exe (
    echo Exécutable trouvé: debug\VisionSightPoissons.exe
    echo.
    echo Démarrage de l'application...
    start debug\VisionSightPoissons.exe
) else (
    echo AVERTISSEMENT: Exécutable non trouvé
    echo Cherchez VisionSightPoissons.exe manuellement
)

pause
