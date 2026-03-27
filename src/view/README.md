# Vue - Interface Utilisateur

Ce dossier contient l'**interface utilisateur** et la **présentation**.

## 📦 Structure

### Fichiers Principaux
- `main.cpp` - Point d'entrée de l'application, initialise la BD et la fenêtre principale

### `gestion_employes/` - Fênentres et Dialogues
- `employe_mainwindow.*` - Fenêtre principale avec gestion des employés
- `employe_logindialog.*` - Dialogue de connexion
- `employe_userdialog.*` - Dialogue de création/modification d'employés
- `employe_payslipdialog.ui` - Dialogue des fiches de paie
- `employe_resources.qrc` - Ressources (images, icônes)
- `employe_transaction_fr_DZ.ts` - Fichier de traductions

## ✅ Principes

### Appel du Modèle
```cpp
// ✅ CORRECT - Appel au DAO/Service de manière décentralisée
#include "../../modele/gestion_employes/employe_dao.h"

QVector<EmployeUser> users = EmployeDAO::afficher();
```

### Signaux/Slots pour la Mise à Jour
```cpp
// ✅ MIEUX - Utiliser les signaux du Service
EmployeeService* service = new EmployeeService();
connect(service, &EmployeeService::filteredEmployeesChanged,
        this, &TabWidget::onEmployeesChanged);
service->setSearchFilter("Dupont", "");
```

### Interface UI Intuitive
```cpp
// ✅ PRINCIPES
// - Utiliser Qt Designer pour les .ui
// - Séparation des préoccupations: logique / présentation
// - Affichage immédiat du feedback utilisateur
// - Messages d'erreur clairs
```

## 🔄 Flux Typique

1. Utilisateur clique sur "Créer Employé"
2. Vue ouvre `EmployeUserDialog`
3. Utilisateur remplit les champs et clique OK
4. Vue récupère les données via `dialog.user()`
5. Vue appelle `EmployeDAO::ajouter()`
6. Vue affiche un message de succès/erreur
7. Vue rafraîchit l'affichage via `refreshUserTable()`

## 📝 Exemple: Ajouter un Employé

```cpp
// Dans EmployeMainWindow::onCreerUser()
EmployeUserDialog dlg(this, "Nouvel Employé");

if (dlg.exec() != QDialog::Accepted)
    return;

EmployeUser employe = dlg.user();
employe.dateCreation = QDateTime::currentDateTime();
employe.statut = "Actif";

// Valider
QString error = EmployeDAO::validateEmploye(employe);
if (!error.isEmpty()) {
    QMessageBox::warning(this, "Erreur", error);
    return;
}

// Ajouter à la BD
QString dbError = EmployeDAO::ajouter(employe);
if (!dbError.isEmpty()) {
    QMessageBox::critical(this, "Erreur BD", dbError);
    return;
}

// Succès
QMessageBox::information(this, "Succès", 
    QString("Employé %1 %2 ajouté!").arg(employe.prenom, employe.nom));

// Rafraîchir l'affichage
refreshUserTable();
```

## 🎨 Includes Corrects

```cpp
// main.cpp
#include "gestion_employes/employe_mainwindow.h"
#include "../modele/db/connection.h"

// employe_mainwindow.cpp
#include "../../modele/gestion_employes/employe_dao.h"
#include "../../modele/gestion_employes/employe_service.h"
#include "ui_employe_mainwindow.h"

// employe_userdialog.h
#include "../../modele/gestion_employes/employe_user.h"
```

## 📋 Fichiers Qt

### `.ui` (Interface Designer)
- Éditable avec Qt Designer
- Génère les fichiers `ui_*.h`
- Contient la disposition des widgets

### `.qrc` (Resource Collection)
- Déclare les ressources (images, icônes, stylesheets)
- Compilé en code C++ par `rcc`

### `.ts` (Translation Source)
- Fichiers de traductions
- Éditable avec Qt Linguist

