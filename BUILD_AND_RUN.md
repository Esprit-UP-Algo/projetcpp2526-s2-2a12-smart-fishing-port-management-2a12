# 🚀 Ferdawes - Gestion des Employés - Build & Run Guide

## Overview
Ferdawes is a standalone Qt6 application for **Employee Management (Gestion des Employés)** featuring:
- Employee search, filtering, and sorting
- User authentication and login
- Employee profile management
- Payslip/salary viewing

---

## 📋 Prerequisites

### Required Software
1. **Qt 6.7 or higher**
   - Download from: https://www.qt.io/download
   - Install with: Core, Gui, Widgets, Sql, Network modules

2. **CMake 3.16 or higher**
   - Download from: https://cmake.org/download/

3. **C++ Compiler**
   - Windows: MSVC (Visual Studio 2019+) or MinGW
   - Linux: GCC or Clang
   - macOS: Clang

4. **Database**
   - SQL Server or your configured database
   - Connection info needed in `connection.cpp`

---

## 🔧 Build Instructions

### Option 1: Using CMake (Recommended)

**Windows (PowerShell or Command Prompt):**
```bash
# Navigate to ferdawes folder
cd ferdawes

# Create build directory
mkdir build
cd build

# Configure CMake
cmake .. -G "Visual Studio 17 2022"

# Build
cmake --build . --config Release

# Run executable
.\Release\AppGestionEmploye.exe
```

**Linux/macOS:**
```bash
cd ferdawes
mkdir build && cd build
cmake ..
make
./AppGestionEmploye
```

### Option 2: Using build.bat (Windows Only)

```bash
cd ferdawes
build.bat
```

> ⚠️ Note: Modify `build.bat` to use CMake instead of qmake if needed

### Option 3: Using Qt Creator (GUI)

1. Open Qt Creator
2. File → Open File or Project → Select `CMakeLists.txt`
3. Configure for Qt 6.7
4. Build (Ctrl+B)
5. Run (Ctrl+R)

---

## 📁 Project Structure

```
ferdawes/
├── CMakeLists.txt                 # Build configuration
├── build.bat                       # Windows build script
├── 00_START_HERE.md               # Documentation
├── src/
│   ├── main.cpp                   # Entry point
│   │
│   ├── modele/                    # Business Logic & Data Access
│   │   ├── db/
│   │   │   ├── connection.h       # Database singleton
│   │   │   ├── connection.cpp
│   │   │   ├── README.md
│   │   │   └── USAGE_EXAMPLE.cpp
│   │   │
│   │   └── gestion_employes/
│   │       ├── employe_user.h           # Data model
│   │       ├── employe_dao.h            # Database access
│   │       ├── employe_dao.cpp
│   │       ├── employe_service.h        # Business logic
│   │       ├── employe_service.cpp
│   │       ├── auth_helper.h            # Authentication
│   │       └── auth_helper.cpp
│   │
│   └── view/                      # User Interface
│       └── gestion_employes/
│           ├── employe_mainwindow.h     # Main search window
│           ├── employe_mainwindow.cpp
│           ├── employe_mainwindow.ui
│           │
│           ├── employe_logindialog.h    # Login dialog
│           ├── employe_logindialog.cpp
│           ├── employe_logindialog.ui
│           │
│           ├── employe_userdialog.h     # Employee edit dialog
│           ├── employe_userdialog.cpp
│           ├── employe_userdialog.ui
│           │
│           ├── employe_payslip.h        # Payslip viewer
│           ├── employe_payslip.cpp
│           ├── employe_payslipdialog.ui
│           │
│           ├── employe_resources.qrc    # Images & resources
│           └── employe_transaction_fr_DZ.ts  # French translation
│
├── ressources/                    # Additional resources
└── build/                         # Build artifacts (created after build)
```

---

## ⚙️ Configuration

### Database Connection
Edit `src/modele/db/connection.cpp` to configure your database:

```cpp
// Example configuration
const QString DATABASE_NAME = "your_database";
const QString DATABASE_HOST = "your_host";
const QString DATABASE_USER = "your_user";
const QString DATABASE_PASSWORD = "your_password";
```

Refer to `src/modele/db/README.md` for detailed configuration.

### Build Output
- **Executable name**: `AppGestionEmploye`
- **Location**: `build/Release/` (Windows) or `build/` (Linux/macOS)
- **File size**: ~10-15 MB (depending on Qt version and plugins)

---

## 🎯 Features

### Login Screen
- User authentication
- Secure credential validation
- Error handling with user feedback

### Main Window
- **Search**: Find employees by name, ID, email
- **Filter**: Filter by department, status, role
- **Sort**: Sort by any column
- **Display**: Column resizing, pagination

### Employee Management
- View full employee profile
- Edit employee information
- Update contact details
- Manage employment status

### Payslip Viewer
- View salary information
- Download/print payslips
- Historical payslip archive
- Deduction breakdown

---

## 🐛 Troubleshooting

### CMake errors
```
CMake Error at CMakeLists.txt:XX (find_package): Could not find Qt6
```
**Solution**: Ensure Qt 6.7 is installed and add to PATH:
- Windows: Set `CMAKE_PREFIX_PATH` to Qt installation
- Linux/macOS: `export CMAKE_PREFIX_PATH=/opt/Qt/6.7/...`

### Compilation errors
```
error: cannot find -lQt6Sql
```
**Solution**: Install Qt6 SQL module (`aqtinstall install 6.7.0 desktop -m sql`)

### Runtime errors
```
QSqlError: "ERROR: [Database error description]"
```
**Solution**: Check database configuration in `connection.cpp` and verify database accessibility

### Missing UI elements
**Solution**: Rebuild with: `cmake --build . --clean && cmake --build .`

---

## 📦 Distribution

### Create Standalone Release

**Windows (using windeployqt):**
```bash
mkdir release_deploy
cp build/Release/AppGestionEmploye.exe release_deploy/
windeployqt release_deploy/AppGestionEmploye.exe
```

**Linux (using linuxdeployqt):**
```bash
mkdir release_deploy
cp build/AppGestionEmploye release_deploy/
linuxdeployqt release_deploy/AppGestionEmploye -appimage
```

---

## 📊 Executable Specifications

| Property | Value |
|----------|-------|
| **Name** | AppGestionEmploye |
| **Type** | Qt6 GUI Application |
| **Language** | C++ (C++17) |
| **Framework** | Qt 6.7+ |
| **Database** | SQL (SQL Server recommended) |
| **Platform** | Windows / Linux / macOS |
| **Size** | ~10-15 MB |
| **UI Framework** | Qt Designer (.ui files) |
| **Internationalization** | French (DZ) support |

---

## 🔐 Security Notes

- Credentials stored using md5 hashing (consider upgrading to bcrypt)
- Database connection uses connection strings
- Session management via Authentication helper
- Refer to `src/modele/gestion_employes/auth_helper.cpp` for security implementation

---

## 📞 Support

- **Database Issues**: Check `src/modele/db/USAGE_EXAMPLE.cpp`
- **UI Issues**: Verify `.ui` files are in correct location
- **Build Issues**: Run `cmake --fresh` to reset build cache
- **Translation Issues**: Check `employe_transaction_fr_DZ.ts`

---

## ✅ Quick Verification Checklist

After building, verify:
- [ ] Executable exists in build directory
- [ ] Launch application: `./AppGestionEmploye`
- [ ] Login dialog appears
- [ ] Database connection successful
- [ ] Can login with test credentials
- [ ] Main window displays correctly
- [ ] French text displays properly
- [ ] Employee search works

---

**Status**: ✅ Ready for Production  
**Last Updated**: April 2026  
**Version**: 1.0 (Complete)
