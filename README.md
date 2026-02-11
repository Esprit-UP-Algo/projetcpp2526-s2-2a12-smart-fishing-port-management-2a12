# VISION SIGHT - Gestion des Lots de Poissons

Une application Qt 7.6.3 pour la gestion complète des lots de poissons avec CRUD, recherche avancée et statistiques.

## Fonctionnalités

- **CRUD Complet**: Ajouter, modifier, supprimer des lots de poissons
- **Recherche Avancée**: Filtrer par :
  - ID du lot
  - Espèce de poisson
  - Navire de pêche
  - Zone de pêche
- **Tri Intelligent**: Tri par poids, date de capture, qualité, etc.
- **Statistiques**:
  - Espèce dominante
  - Zone de pêche fréquente
  - Poids moyen par lot
  - Taux de remplissage (diagramme circulaire)
- **Export de Données**:
  - PDF (à implémenter)
  - Excel (à implémenter)

## Caractéristiques des Lots

Chaque lot de poissons contient:
- **ID Lot**: Identifiant unique (ex: LOT-001)
- **Espèce**: Type de poisson (Sardine, Anchois, Thon, etc.)
- **Poids**: Poids total en kg
- **Zone Pêche**: Zone géographique de pêche
- **Date Capture**: Date et heure de la capture
- **Navire**: Nom du navire de pêche
- **Qualité**: Niveau de qualité (Premium, Standard, Économique, Déclassé)
- **Date Entrée Stock**: Date et heure d'entrée en stock

## Structure du Projet

```
interface arnoub/
├── src/
│   ├── main.cpp
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   ├── statcircle.h
│   └── statcircle.cpp
├── models/
│   ├── capturelot.h
│   ├── capturelotmodel.h
│   ├── capturelotmodel.cpp
│   ├── capturelotproxymodel.h
│   └── capturelotproxymodel.cpp
├── resources/
│   └── resources.qrc
├── VisionSightPoissons.pro
└── README.md
```

## Compilation et Exécution

### Prérequis
- Qt 7.6.3
- Qt Creator 13.x
- Compilateur C++ (MSVC ou MinGW pour Windows)

### Étapes de compilation

1. **Ouvrir le projet dans Qt Creator**:
   - Fichier → Ouvrir un fichier ou un projet
   - Sélectionner `VisionSightPoissons.pro`

2. **Configurer le kit Qt**:
   - Sélectionner la version Qt 7.6.3 appropriée
   - Choisir le compilateur C++17 compatible

3. **Compiler**:
   - Ctrl + B ou Build → Build Project

4. **Exécuter**:
   - Ctrl + R ou Build → Run

### Compilation en ligne de commande (Windows)

```bash
# Naviguer vers le répertoire du projet
cd "c:\Users\GIGABYTE\OneDrive - ESPRIT\Bureau\interface arnoub"

# Exécuter qmake
qmake VisionSightPoissons.pro

# Compiler avec make ou nmake
nmake  # ou make si vous utilisez MinGW

# Exécuter l'application
release\VisionSightPoissons.exe
```

## Utilisation

1. **Ajouter un lot**: Remplir le formulaire et cliquer sur "Ajouter"
2. **Modifier un lot**: Sélectionner une ligne, modifier les données, cliquer sur "Modifier"
3. **Supprimer un lot**: Sélectionner une ligne et cliquer sur "Supprimer"
4. **Rechercher**: Utiliser les champs de recherche pour filtrer les lots
5. **Trier**: Cliquer sur les en-têtes de colonnes pour trier
6. **Exporter**: Cliquer sur "PDF" ou "Excel" pour exporter les données

## Design UI

L'interface suit un design moderne et épuré :
- **Thème sombre** avec couleurs de base bleues (#1e8de0, #39c0fa)
- **Barre latérale** de navigation avec logo personnalisé
- **Cartes** avec ombres douces
- **Composant statistique** avec diagramme circulaire animé
- **Tableau** avec sélection et tri intégrés

## Développement Futur

- [ ] Implémentation de l'export PDF
- [ ] Implémentation de l'export Excel
- [ ] Persistance des données (SQLite/PostgreSQL)
- [ ] Graphiques animés pour les statistiques
- [ ] Historique des modifications
- [ ] Gestion utilisateurs et permissions

## Licence

Ce projet est fourni à titre informatif pour usage pédagogique.

## Support

Pour toute question ou problème, veuillez consulter la documentation Qt:
- https://doc.qt.io/qt-6/
- https://doc.qt.io/qt-creator/
