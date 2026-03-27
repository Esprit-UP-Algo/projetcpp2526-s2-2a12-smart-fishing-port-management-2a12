# Modèle - Logique Métier

Ce dossier contient toute la **logique métier** et l'**accès aux données**.

## 📦 Structure

### `db/` - Module de Base de Données
- `connection.h/.cpp` - Singleton pour la connexion Oracle
- `README.md` - Documentation du module
- `USAGE_EXAMPLE.cpp` - Exemples d'utilisation

### `gestion_employes/` - Logique Métier des Employés
- `employe_user.h` - Structure de données
- `employe_dao.h/.cpp` - Data Access Object (CRUD)
- `employe_service.h/.cpp` - Service métier (filtrage, tri, validation)
- `employe_payslip.h/.cpp` - Logique des fiches de paie

## ✅ Principes

### Pas de Dépendance Qt Widgets
```cpp
// ✅ AUTORISÉ
#include <QString>
#include <QVector>
#include <QObject>    // Seulement pour les signaux/slots

// ❌ INTERDIT
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
```

### Séparation DAO / Service
- **DAO** : Opérations CRUD sécurisées, validation de base
- **Service** : Filtrage, tri, gestion d'état, signaux

## 🔄 Flux Typique

1. Vue appelle `EmployeeService::createEmployee()`
2. Service valide les données via `EmployeDAO::validateEmploye()`
3. Service appelle `EmployeDAO::ajouter()`
4. DAO utilise `Connection::createInstance()` pour la BD
5. Service notifie la Vue via signaux `operationSucceeded()`

## 📝 Exemples

### Ajouter un employé
```cpp
EmployeUser employe;
employe.cin = "12345678";
employe.nom = "Dupont";
employe.prenom = "Jean";
employe.role = "Technicien";
// ... remplir autres champs

// Via le service (recommandé)
QString error = EmployeeService::validateEmployee(employe);
if (error.isEmpty()) {
    QString dbError = EmployeDAO::ajouter(employe);
}
```

### Filtrer les employés
```cpp
EmployeeService service;
service.setSearchFilter("Dupont", "");
service.setRoleFilter("Technicien");
service.setSortCriteria(0, true);  // Tri par CIN

auto filtered = service.getFilteredEmployees();
connect(&service, &EmployeeService::filteredEmployeesChanged,
        this, &MyClass::onDataChanged);
```

