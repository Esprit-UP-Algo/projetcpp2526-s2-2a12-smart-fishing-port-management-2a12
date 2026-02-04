# Guide d'Installation et Configuration - Qt Creator 7.6.3

## 1. Installation de Qt 7.6.3

### Sur Windows

1. Télécharger Qt Open Source depuis: https://www.qt.io/download
2. Exécuter l'installateur Qt
3. Lors de l'installation:
   - Sélectionner **Qt 7.6.3** (ou 6.x si 7.6.3 n'est pas disponible)
   - Sélectionner les composants:
     - ✓ Desktop (MSVC 2022 64-bit ou MinGW)
     - ✓ Qt Creator
     - ✓ Documentation
4. Compléter l'installation

### Installation de Visual Studio Build Tools (recommandé)

Pour MSVC compiler:
1. Télécharger: https://visualstudio.microsoft.com/visual-cpp-build-tools/
2. Installer les composants C++
3. Redémarrer après installation

## 2. Configuration du Projet dans Qt Creator

### Méthode 1: Via Qt Creator GUI

1. Ouvrir Qt Creator
2. **File → Open File or Project**
3. Naviguer et sélectionner: `VisionSightPoissons.pro`
4. Qt Creator détecte automatiquement:
   - Kit Qt 7.6.3 disponible
   - Répertoires de build
5. Cliquer sur **Configure Project**
6. Sélectionner:
   - ✓ Debug
   - ✓ Release
7. Cliquer **Configure**

### Méthode 2: Via Ligne de Commande

```batch
cd "c:\Users\GIGABYTE\OneDrive - ESPRIT\Bureau\interface arnoub"

# Générer les fichiers de build
qmake VisionSightPoissons.pro -spec win32-msvc

# Compiler (choix 1: Visual Studio)
nmake

# OU compiler (choix 2: MinGW)
mingw32-make
```

## 3. Compilation et Exécution

### Via Qt Creator (recommandé)

1. **Compiler le projet**: 
   - Shortcut: `Ctrl + B`
   - Menu: Build → Build Project "VisionSightPoissons"

2. **Exécuter l'application**:
   - Shortcut: `Ctrl + R`
   - Menu: Build → Run

3. **Mode Debug/Release**:
   - En bas à gauche: Sélectionner "Debug" ou "Release"

### Via Ligne de Commande

```batch
# Avec Script Windows (facile)
build.bat

# Ou manuellement avec nmake
qmake VisionSightPoissons.pro -spec win32-msvc
nmake

# Lancer l'exécutable
release\VisionSightPoissons.exe
```

## 4. Structure des Fichiers

```
interface arnoub/
│
├── src/                          # Code source principal
│   ├── main.cpp                 # Point d'entrée de l'application
│   ├── mainwindow.h/cpp         # Fenêtre principale et UI
│   ├── statcircle.h/cpp         # Composant du diagramme circulaire
│
├── models/                       # Modèles de données (MVC)
│   ├── capturelot.h             # Structure de données pour un lot
│   ├── capturelotmodel.h/cpp    # Modèle Qt pour la table
│   ├── capturelotproxymodel.h/cpp  # Filtre/Tri
│
├── resources/
│   └── resources.qrc            # Ressources (images, styles, etc.)
│
├── VisionSightPoissons.pro      # Fichier de configuration qmake
├── CMakeLists.txt               # Fichier de configuration CMake (alternatif)
├── build.bat                    # Script de compilation Windows
├── README.md                    # Documentation
└── INSTALLATION.md              # Ce fichier
```

## 5. Architecture du Code

### Patterns Utilisés

1. **Model-View Pattern (Qt)**:
   - `CaptureLotModel`: Gère les données
   - `CaptureLotProxyModel`: Filtre et tri
   - `QTableView`: Affichage

2. **CRUD Operations**:
   - Create: `onAddLot()`
   - Read: `loadSelectedLotToForm()`
   - Update: `onUpdateLot()`
   - Delete: `onDeleteLot()`

3. **Signal/Slot** (Qt paradigm):
   - Connexion automatique des événements UI
   - Rafraîchissement des statistiques

### Structure de Données (CaptureLot)

```cpp
struct CaptureLot {
    QString idLot;           // Identifiant unique
    QString espece;          // Type de poisson
    int poids;               // Poids en kg
    QString zonePeche;       // Zone géographique
    QDateTime dateCapture;   // Date/heure de capture
    QString navire;          // Nom du bateau
    QString qualite;         // Niveau de qualité
    QDateTime dateEntreeStock; // Entrée en stock
};
```

## 6. Utilisation de l'Application

### Opérations Principales

1. **Ajouter un lot**:
   - Remplir tous les champs du formulaire
   - Cliquer "Ajouter"
   - Le lot apparaît dans le tableau

2. **Modifier un lot**:
   - Cliquer sur une ligne du tableau
   - Les données se chargent dans le formulaire
   - Modifier les valeurs
   - Cliquer "Modifier"

3. **Supprimer un lot**:
   - Sélectionner une ligne
   - Cliquer "Supprimer"
   - Confirmer la suppression

4. **Rechercher/Filtrer**:
   - Utiliser les 4 champs de recherche:
     - Recherche lot (ID)
     - Recherche espèce
     - Recherche navire
     - Recherche zone
   - Le tableau se met à jour en temps réel

5. **Trier**:
   - Cliquer sur les en-têtes des colonnes
   - Cliquer deux fois pour inverser l'ordre

6. **Exporter**:
   - Boutons "PDF" et "Excel" (à implémenter)

### Statistiques

Le panneau droit affiche:
- **Espèce dominante**: La plus capturée
- **Zone fréquente**: La zone la plus pêchée
- **Poids moyen**: Moyenne des poids
- **Taux de remplissage**: Diagramme circulaire (0% à 100%)

## 7. Dépannage

### Erreur: "qmake not found"
- Solution: Ajouter Qt bin au PATH Windows
- Chemin: `C:\Qt\<version>\msvc2022_64\bin`

### Erreur: "MSVC compiler not found"
- Solution: Installer Visual Studio Build Tools
- Ou utiliser MinGW à la place

### Erreur de compilation "unknown module"
- Vérifier le fichier `.pro`:
  - `QT += core gui sql` doit être présent
  - Tous les fichiers sources listés

### Fenêtre vide après démarrage
- Vérifier que `buildUi()` est appelée dans le constructeur
- Vérifier que `applyTheme()` charge le style

### Pas de données dans le tableau
- Utiliser "Ajouter" pour créer des lots
- Vérifier que `CaptureLotModel` est connectée à la vue

## 8. Personnalisation

### Modifier le Style

Le style est défini dans `mainwindow.cpp`, fonction `applyTheme()`:
- Modifier les couleurs hex
- Ajouter des propriétés CSS
- Charger depuis un fichier `.qss` externe

### Ajouter de Nouveaux Champs

1. Ajouter dans `capturelot.h`:
```cpp
struct CaptureLot {
    // ... champs existants ...
    QString nouveauChamp;  // Nouveau champ
};
```

2. Ajouter dans `mainwindow.h`:
```cpp
QLineEdit *m_nouveauChamp;
```

3. Ajouter dans `buildUi()` et `wireSignals()`

### Connecter une Base de Données

Remplacer les modèles de données par:
```cpp
// Utiliser QSqlTableModel à la place de QAbstractTableModel
QSqlTableModel *m_lotModel = new QSqlTableModel(this);
m_lotModel->setTable("lots");
m_lotModel->select();
```

## 9. Ressources Utiles

- **Documentation Qt 7.6**: https://doc.qt.io/qt-6/
- **Qt Creator Guide**: https://doc.qt.io/qtcreator/
- **Qt SQL Module**: https://doc.qt.io/qt-6/qtsql-index.html
- **Qt Widgets**: https://doc.qt.io/qt-6/qtwidgets-index.html

## 10. Support et Questions

Pour des problèmes spécifiques:
1. Vérifier la console d'erreurs dans Qt Creator
2. Lire le fichier Build Output (Build → Show Build Output)
3. Recompiler en nettoyant d'abord (Clean All + Build)

## Notes Importantes

⚠️ **Droits d'accès**: Exécutez Qt Creator avec les droits administrateur si vous rencontrez des erreurs de permissions

⚠️ **Antivirus**: Certains antivirus ralentissent la compilation. Excluez le dossier du projet des scans en temps réel.

✓ **Conseil**: Utilisez Visual Studio Code avec l'extension C++ pour une meilleure expérience si Qt Creator pose problème.
