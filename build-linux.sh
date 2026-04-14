#!/bin/bash
# ============================================
# Ferdawes - Build Script (Linux/macOS)
# Employee Management Application
# ============================================

set -e  # Exit on any error

echo ""
echo "============================================"
echo "  FERDAWES - Gestion des Employes"
echo "  Build & Setup Script"
echo "============================================"
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "ERROR: CMake not found"
    echo "Please install CMake 3.16 or higher"
    echo "macOS: brew install cmake"
    echo "Linux: apt-get install cmake"
    exit 1
fi

# Check if Qt is installed
if ! command -v qmake &> /dev/null; then
    echo "WARNING: qmake not found in PATH"
    echo "Ensure Qt 6.7 is installed"
    echo "macOS: brew install qt6"
    echo "Linux: apt-get install qt6-base-dev"
fi

echo "[1/5] Cleaning previous builds..."
if [ -d "build" ]; then
    rm -rf build
    echo "   - Removed build directory"
fi

echo ""
echo "[2/5] Creating build directory..."
mkdir -p build
cd build

echo ""
echo "[3/5] Configuring CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: CMake configuration failed"
    echo "Set CMAKE_PREFIX_PATH to your Qt installation:"
    echo "  export CMAKE_PREFIX_PATH=/path/to/Qt6"
    cd ..
    exit 1
fi

echo ""
echo "[4/5] Building AppGestionEmploye..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo ""
    echo "ERROR: Build failed"
    cd ..
    exit 1
fi

echo ""
echo "[5/5] Build Complete!"
echo ""
echo "============================================"
echo "  BUILD SUCCESSFUL"
echo "============================================"
echo ""
echo "Executable location:"
echo "   ./AppGestionEmploye"
echo ""
echo "To run the application:"
echo "   ./AppGestionEmploye"
echo ""

cd ..

echo ""
echo "Setup complete!"
