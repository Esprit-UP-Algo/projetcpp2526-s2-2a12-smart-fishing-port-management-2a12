# Quick Start Guide - VISION SIGHT

## 🚀 Démarrage Rapide (5 minutes)

### 1️⃣ Ouvrir Qt Creator

```
Démarrer Qt Creator 13.x+
```

### 2️⃣ Ouvrir le Projet

```
File → Open File or Project
Sélectionner: VisionSightPoissons.pro
Cliquer: Configure Project
```

### 3️⃣ Compiler

```
Ctrl + B    (ou Build → Build Project)
```

### 4️⃣ Exécuter

```
Ctrl + R    (ou Build → Run)
```

✅ **C'est tout !** L'application démarre.

---

## 📱 Interface en 10 secondes

```
┌─────────────────────────────────────────────────────┐
│ ≡  VISION SIGHT                                    ≡ │
│ Module: Gestion des Lots de Poissons               │
├────────────────┬────────────────┬────────────────────┤
│ VISION SIGHT   │  [Graphique]   │ [Graphique]  [...] │
│ Tableau bord   │                │                    │
│ Lots        ■  │ ┌─────────────────────────────────┐ │
│ Statistiques   │ │ GESTION DES LOTS                │ │
│ Paramètres     │ │ ID: ______  Espèce: ______     │ │
│                │ │ Poids: __   Zone: ______        │ │
│                │ │ Date Capture: ______            │ │
│                │ │ Navire: ____  Qualité: ▼       │ │
│                │ │                                 │ │
│                │ │ [Ajouter] [Modifier] [Suppr]   │ │
│                │ │                                 │ │
│                │ │ Recherche: ___ ___ ___ ___     │ │
│                │ │ ┌─────────────────────────────┐ │ │
│                │ │ │ ID │ Espèce │ Poids │ ... │ │ │
│                │ │ ├─────────────────────────────┤ │ │
│                │ │ │LOT-001│Sardine│250kg│ ... │ │ │
│                │ │ │LOT-002│Anchois│180kg│ ... │ │ │
│                │ │ └─────────────────────────────┘ │ │
│                │ └─────────────────────────────────┘ │ │
│                │ ┌─────────────┐                     │ │
│                │ │ Statistiques│                     │ │
│                │ │ [Diagramme] Espèce: Sardine     │ │
│                │ │             Zone: Atlantique    │ │
│                │ │             Poids: 240.5 kg     │ │
│                │ │ Mise à jour automatique         │ │
│                │ └─────────────┘                     │ │
└────────────────┴────────────────┴────────────────────┘
```

---

## 📝 Workflow Typique

### Ajouter un Lot

```
1. Remplir les champs:
   - ID Lot: LOT-001
   - Espèce: Sardine
   - Poids: 250
   - Zone Pêche: Atlantique
   - Date Capture: 2024-01-15 08:30
   - Navire: Stella Maris
   - Qualité: Premium
   - Date Entrée Stock: 2024-01-15 14:00

2. Cliquer "Ajouter"

3. Résultat: 
   ✓ Lot visible dans la table
   ✓ Formulaire réinitialisé
   ✓ Statistiques mises à jour
```

### Rechercher un Lot

```
1. Saisir dans "Recherche lot": "001"
   → Table affiche LOT-001 uniquement

2. Saisir dans "Recherche espèce": "sardine"
   → Filtre cumulatif

3. Appliquer d'autres filtres...
   → Les filtres se combinent
```

### Modifier un Lot

```
1. Cliquer sur la ligne LOT-001 dans la table

2. Les champs se remplissent automatiquement

3. Modifier une valeur (ex: Poids 250 → 300)

4. Cliquer "Modifier"

5. Résultat: ✓ Lot mis à jour
```

### Supprimer un Lot

```
1. Cliquer sur la ligne

2. Cliquer "Supprimer"

3. Confirmer "Supprimer le lot sélectionné ?"

4. Résultat: ✓ Lot supprimé
```

---

## 🔧 Fichiers Clés

| Fichier | Rôle |
|---------|------|
| `main.cpp` | Point d'entrée application |
| `mainwindow.h/cpp` | Interface principale + CRUD |
| `statcircle.h/cpp` | Diagramme circulaire |
| `capturelot.h` | Structure de données |
| `capturelotmodel.h/cpp` | Modèle table Qt |
| `capturelotproxymodel.h/cpp` | Filtre/tri |

---

## ⚙️ Configuration Minimale

✅ **Installée?**
- [ ] Qt 6.x ou 7.6.3
- [ ] Qt Creator 13.x+
- [ ] Compilateur C++ (MSVC/MinGW)

✅ **Chemin corrigé?**
```
Vérifier que Qt bin est dans le PATH Windows:
C:\Qt\6.7\msvc2022_64\bin
```

---

## 🐛 Problèmes Courants

### Erreur "qmake not found"
→ Redémarrer Qt Creator

### Erreur de compilation
→ `Build → Clean All` puis relancer la compilation

### La table est vide
→ Ajouter un lot avec le bouton "Ajouter"

### Les statistiques ne se mettent pas à jour
→ Vérifier que les modèles sont connectés (voir wireSignals())

---

## 💡 Conseils d'Usage

1. **Organiser les données**:
   - Utiliser des IDs lisibles (LOT-001, LOT-002...)
   - Standardiser les noms d'espèces

2. **Recherche efficace**:
   - Recherche par zone pour voir d'où viennent les captures
   - Trier par poids pour identifier les plus gros lots

3. **Statistiques**:
   - Observer l'espèce dominante
   - Surveiller le taux de remplissage

4. **Performance**:
   - L'application supporte 1000+ lots facilement
   - Laisser le système chercher avant de filtrer plusieurs fois

---

## 📚 Ressources

- **Qt Docs**: https://doc.qt.io/qt-6/
- **Qt Creator Guide**: https://doc.qt.io/qtcreator/
- **Fichier complet**: `SPECIFICATIONS.md`
- **Installation détaillée**: `INSTALLATION.md`

---

## ✨ Fonctionnalités Principales

✅ **CRUD Complet**
- Ajouter, modifier, supprimer des lots

✅ **Recherche Avancée**
- 4 filtres cumulatifs en temps réel

✅ **Tri Intelligent**
- Sur 8 colonnes différentes

✅ **Statistiques Live**
- Espèce dominante, zone fréquente, poids moyen
- Diagramme circulaire animé

✅ **Interface Moderne**
- Design bleu sombre professionnel
- Responsive et fluide

⏳ **À Venir**
- Export PDF/Excel
- Persistance base de données
- Graphiques dynamiques

---

## 🎯 Objectifs de Conception

La structure du code suit les **meilleures pratiques Qt**:

```
✓ Pattern MVC (Model-View-Controller)
✓ Signal/Slot (Paradigme Qt)
✓ Validation des données
✓ Messages d'erreur clairs
✓ Interface responsive
✓ Code modulaire et extensible
```

---

## 📞 Besoin d'Aide?

1. **Vérifier les sorties build** dans Qt Creator
2. **Lire les messages d'erreur** (Build Output)
3. **Consulter INSTALLATION.md** pour configuration détaillée
4. **Voir SPECIFICATIONS.md** pour tous les détails

---

**Bon développement! 🚀**
