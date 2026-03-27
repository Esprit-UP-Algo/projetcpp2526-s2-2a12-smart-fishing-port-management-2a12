# Database Connection Module - Singleton Pattern 🗄️

## Vue d'ensemble

Ce module implémente le **Pattern Singleton (Meyers)** pour gérer la connexion à la base de données Oracle en utilisant le driver QODBC.

## 📋 Fichiers du module

### 1. `connection.h` - Interface publique
- **Classe**: `Connection`
- **Pattern**: Meyers Singleton (thread-safe, initialisation lazy)
- **Constructeur**: Privé (enforce l'unicité)
- **Opérateurs supprimés**: Copie et affectation (`= delete`)

### 2. `connection.cpp` - Implémentation
- **Driver QODBC**: Configuration pour Oracle
- **Identifiants**:
  - Utilisateur: `mouradd`
  - Mot de passe: `mourad123`
  - DSN: `Source_Projet2A`
- **Méthodes**:
  - `createInstance()`: Retourne la référence à l'instance unique
  - `createConnect()`: Établit la connexion à Oracle
  - `getDatabase()`: Retourne la référence au QSqlDatabase
  - `getLastError()`: Récupère le dernier message d'erreur
  - `isConnected()`: Vérifie l'état de la connexion

---

## 🎯 Architecture du Pattern Singleton

### Meyers Singleton (C++11+)

```cpp
Connection& Connection::createInstance()
{
    static Connection instance;  // Créé une seule fois, détruit à la fin du programme
    return instance;
}
```

**Avantages**:
- ✅ Thread-safe en C++11+
- ✅ Pas de gestion manuelle d'allocation
- ✅ Initialisation à la première utilisation (lazy initialization)
- ✅ Destruction automatique

### Protections contre la duplication

```cpp
// Constructeur privé (pas d'instanciation externe)
Connection::Connection() { /* ... */ }

// Suppression du constructeur de copie
Connection(const Connection&) = delete;

// Suppression de l'opérateur d'affectation
Connection& operator=(const Connection&) = delete;
```

**Résultat**: Une seule instance existe dans toute l'application ✅

---

## 💡 Utilisation dans main.cpp

```cpp
#include "db/connection.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Récupérer l'instance unique (crée à la première utilisation)
    Connection& dbConnection = Connection::createInstance();

    // Essayer de se connecter
    if (!dbConnection.createConnect())
    {
        // Gérer l'erreur
        QString error = dbConnection.getLastError();
        QMessageBox::critical(nullptr, "Erreur", error);
        return -1;
    }

    // Utiliser la connexion ailleurs dans l'application
    // ...

    return app.exec();
}
```

---

## 🔌 Configuration ODBC (Source_Projet2A)

**Important**: La source ODBC doit être configurée sur votre système Windows:

### Étapes de configuration (Windows 10/11):

1. **Ouvrir le Gestionnaire ODBC**:
   - Panneau de Contrôle → Outils d'administration → Sources de données (ODBC)
   - Ou taper `odbcad32` dans Exécuter (Win+R)

2. **Créer une source de données**:
   - Onglet: "Sources de données utilisateur" (ou "Système")
   - Cliquer: "Ajouter"
   - Driver: Sélectionner le driver Oracle (ex: "Oracle in instantclient_19_8")
   - Nom de la source (DSN): **Source_Projet2A**
   - Description: "Base de données Oracle pour Projet 2A"
   - TNS Connection String: Configurer l'adresse du serveur Oracle
   - Cliquer: "Tester la connexion"
   - Username: **mouradd**
   - Password: **mourad123**

3. **Vérifier la connexion**:
   - Cliquer sur "Tester la connexion"
   - Si succès → "Connection successful"

---

## 🐛 Dépannage

### Erreur: "Driver not found"
```
✗ ERROR: Connection failed!
  Error: QODBC driver not available
```
**Solution**: Installer le driver Oracle ODBC sur votre machine
- Télécharger: Oracle ODBC Driver
- Ou utiliser: Oracle Instant Client avec ODBC

### Erreur: "DSN not found"
```
✗ ERROR: Connection failed!
  Error: [Microsoft][ODBC Driver Manager] Data source name not found
```
**Solution**: 
- Vérifier que la source ODBC "Source_Projet2A" existe
- Utiliser le Gestionnaire ODBC pour créer/vérifier

### Erreur: "Invalid username/password"
```
✗ ERROR: Connection failed!
  Error: ORA-01017: invalid username/password
```
**Solution**:
- Vérifier que les identifiants sont corrects
- Username: `mouradd`
- Password: `mourad123`

---

## 📊 Exemple de flux d'exécution

```
Application START
    ↓
main.cpp: Connection& dbConnection = Connection::createInstance()
    ↓
    [Première utilisation → Crée l'instance statique]
    ↓
main.cpp: dbConnection.createConnect()
    ↓
    └─→ db.setDatabaseName("Source_Projet2A")
    └─→ db.setUserName("mouradd")
    └─→ db.setPassword("mourad123")
    └─→ db.open() → true/false
    ↓
[Succès] → Afficher login dialog
    ↓
[Application continue...]
    ↓
Application EXIT
    ↓
    [Destruction automatique de l'instance statique]
```

---

## ✅ Checklist de déploiement

- [ ] DSN "Source_Projet2A" configuré dans le Gestionnaire ODBC
- [ ] Oracle ODBC Driver installé sur le système
- [ ] Authentifiants corrects (mouradd/mourad123)
- [ ] CMakeLists.txt inclut `src/db/connection.h` et `src/db/connection.cpp`
- [ ] main.cpp appelle `Connection::createInstance()` au démarrage
- [ ] Code compile sans erreur
- [ ] Message de succès s'affiche à la connexion ✓

---

**Design Pattern**: Meyers Singleton (Thread-safe, Lazy init)  
**Driver**: QODBC  
**Base de données**: Oracle 12c/19c  
**Date de création**: 27 Mars 2026
