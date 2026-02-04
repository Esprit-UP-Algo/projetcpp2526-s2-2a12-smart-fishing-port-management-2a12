# VISION SIGHT - Spécifications Fonctionnelles

## Résumé

Application Qt C++ pour la gestion complète des lots de poissons capturés, avec interface moderne, CRUD, recherche avancée, tri et statistiques.

---

## 1. GESTION DES LOTS (CRUD)

### 1.1 Créer un Lot
- **Formulaire**: Tous les champs obligatoires
- **Validation**: ID unique requis
- **Action**: Cliquer "Ajouter"
- **Résultat**: Lot ajouté à la table, formulaire vidé

### 1.2 Lire/Afficher Lots
- **Table interactive**: Affiche tous les lots
- **Sélection**: Cliquer sur une ligne charge les données dans le formulaire
- **Pagination**: Supportée par QTableView
- **Tri**: Cliquer sur les en-têtes

### 1.3 Modifier un Lot
- **Processus**:
  1. Sélectionner une ligne dans la table
  2. Les champs se remplissent automatiquement
  3. Modifier les valeurs
  4. Cliquer "Modifier"
- **Validation**: Contrôle de l'ID (pas de doublon)

### 1.4 Supprimer un Lot
- **Confirmation**: Dialog de confirmation
- **Processus**: Sélectionner → Supprimer → Confirmer
- **Résultat**: Lot supprimé, tableau mis à jour, formulaire réinitialisé

---

## 2. RECHERCHE ET FILTRAGE

### 2.1 Recherche par ID Lot
- **Champ**: "Recherche lot"
- **Type**: Filtrage en temps réel
- **Sensibilité**: Insensible à la casse

### 2.2 Recherche par Espèce
- **Champ**: "Recherche espèce"
- **Type**: Filtrage en temps réel
- **Exemples**: Sardine, Anchois, Thon, etc.

### 2.3 Recherche par Navire
- **Champ**: "Recherche navire"
- **Type**: Filtrage en temps réel
- **Utilité**: Identifier les lots d'un bateau spécifique

### 2.4 Recherche par Zone de Pêche
- **Champ**: "Recherche zone"
- **Type**: Filtrage en temps réel
- **Exemples**: Atlantique, Méditerranée, Pacifique, etc.

### 2.5 Comportement
- Les 4 filtres fonctionnent ensemble (AND)
- Mise à jour instantanée lors de la saisie
- Table se réajuste automatiquement

---

## 3. TRI AVANCÉ

### 3.1 Tri par Colonne
- **Colonnes triables**:
  - ID Lot
  - Espèce
  - Poids (numérique)
  - Zone Pêche
  - Date Capture (chronologique)
  - Navire
  - Qualité
  - Date Entrée Stock

### 3.2 Ordre de Tri
- **Cliquer une fois**: Ordre croissant (↑)
- **Cliquer deux fois**: Ordre décroissant (↓)
- **Cliquer trois fois**: Ordre par défaut

### 3.3 Tri Personnalisé
- Tri par poids permet de voir les plus gros lots
- Tri par date permet une chronologie
- Tri par espèce pour le regroupement

---

## 4. CHAMPS DE DONNÉES

### 4.1 ID Lot
- **Type**: Texte
- **Obligatoire**: Oui
- **Unique**: Oui
- **Format**: Ex: LOT-001, LOT-002
- **Validation**: Non vide, pas de doublon

### 4.2 Espèce
- **Type**: Texte
- **Obligatoire**: Oui
- **Exemples**: Sardine, Anchois, Thon, Morue, Saumon
- **Validation**: Non vide

### 4.3 Poids
- **Type**: Nombre entier
- **Unité**: Kilogrammes (kg)
- **Obligatoire**: Oui
- **Minimum**: 1 kg
- **Maximum**: 100 000 kg
- **Suffixe**: " kg"

### 4.4 Zone de Pêche
- **Type**: Texte
- **Obligatoire**: Oui
- **Exemples**: Atlantique, Méditerranée, Pacifique, Manche
- **Validation**: Non vide

### 4.5 Date de Capture
- **Type**: Date + Heure
- **Format**: YYYY-MM-DD HH:MM
- **Obligatoire**: Oui
- **Calendrier**: Popup disponible
- **Par défaut**: Maintenant

### 4.6 Navire
- **Type**: Texte
- **Obligatoire**: Oui
- **Exemple**: "Chalutier Atlantique", "Stella Maris"
- **Validation**: Non vide

### 4.7 Qualité
- **Type**: Liste déroulante
- **Obligatoire**: Oui
- **Valeurs**:
  - Premium (meilleure qualité)
  - Standard (qualité normale)
  - Économique (qualité acceptable)
  - Déclassé (qualité inférieure)
- **Par défaut**: Premium

### 4.8 Date Entrée Stock
- **Type**: Date + Heure
- **Format**: YYYY-MM-DD HH:MM
- **Obligatoire**: Oui
- **Calendrier**: Popup disponible
- **Par défaut**: Maintenant
- **Note**: Peut être antérieure à la date de capture

---

## 5. STATISTIQUES

### 5.1 Espèce Dominante
- **Calcul**: Compte les lots par espèce
- **Affichage**: Espèce avec le plus de lots
- **Résultat**: Texte (ex: "Sardine")
- **Mise à jour**: Automatique après chaque modification

### 5.2 Zone de Pêche Fréquente
- **Calcul**: Compte les lots par zone
- **Affichage**: Zone avec le plus de lots
- **Résultat**: Texte (ex: "Atlantique")
- **Mise à jour**: Automatique

### 5.3 Poids Moyen
- **Calcul**: Somme des poids / nombre de lots
- **Affichage**: Valeur décimale avec 1 chiffre après virgule
- **Format**: "150.5 kg"
- **Mise à jour**: Automatique

### 5.4 Diagramme Circulaire (Taux de Remplissage)
- **Type**: Widget personnalisé `StatCircleWidget`
- **Calcul**: nombre de lots / 100 (capacité max)
- **Affichage**: 
  - Cercle avec progression colorée (bleu)
  - Pourcentage au centre (ex: "45.2%")
  - Label "Captures" en bas
- **Couleurs**:
  - Cercle de fond: #101c2c (bleu foncé)
  - Bordure: #23374e
  - Progression: #39c0fa (cyan)
  - Texte: #e6eef6
- **Mise à jour**: Automatique

---

## 6. EXPORT DE DONNÉES

### 6.1 Export PDF
- **Bouton**: "PDF" (cyan/bleu)
- **Statut**: À implémenter
- **Données**: Tous les lots filtrés actuellement visibles
- **Format**: Tableau avec formatage professionnel
- **Contenu**: En-têtes de colonnes + données

### 6.2 Export Excel
- **Bouton**: "Excel" (cyan/bleu)
- **Statut**: À implémenter
- **Données**: Tous les lots filtrés
- **Format**: Fichier .xlsx
- **Contenu**: Feuille unique avec tous les lots

### 6.3 Fonctionnement
1. Appliquer les filtres souhaités
2. Cliquer "PDF" ou "Excel"
3. Dialog "Enregistrer sous..."
4. Choisir l'emplacement et le nom
5. Cliquer "Enregistrer"
6. Fichier créé avec les données filtrées

---

## 7. INTERFACE UTILISATEUR

### 7.1 Zones Principales

**Barre latérale** (gauche):
- Logo + "VISION SIGHT"
- Boutons de navigation (Tableau de bord, Lots, Statistiques, Paramètres)
- Couleur: Bleu sombre (#101c2c)

**En-tête** (haut):
- Titre "VISION SIGHT" (28px, bold)
- Sous-titre: "Module: Gestion des Lots de Poissons"
- Bouton menu (3 traits)

**Graphiques** (premier rang):
- 3 cartes placeholder pour futurs graphiques
- "Quantité par espèce"
- "Répartition par zone"
- "Poids moyen"

**Formulaire** (gauche, moitié):
- 8 champs d'entrée
- 4 boutons (Ajouter, Modifier, Supprimer, Réinitialiser)
- Barre de recherche (4 filtres)
- Table interactive

**Statistiques** (droite):
- Diagramme circulaire "Captures"
- 3 lignes de stats (Espèce, Zone, Poids moyen)
- Label "Mise à jour automatique"

### 7.2 Palette de Couleurs

- **Bleu foncé**: #132437 (arrière-plan principal)
- **Bleu plus foncé**: #101c2c (cartes, entrées)
- **Bleu moyen**: #0f2638, #0c1a29 (accents)
- **Cyan/Bleu clair**: #1e8de0, #39c0fa (boutons, accents)
- **Gris texte**: #e6eef6 (texte principal)
- **Gris clair**: #9bb0c3 (labels, hints)
- **Bordures**: #23374e

### 7.3 Styles des Éléments

**Boutons**:
- PrimaryButton: Gris foncé avec border
- AltPrimaryButton: Cyan lumineux
- SecondaryButton: Gris foncé
- DangerButton: Cyan (suppression)
- GhostButton: Transparent avec border

**Cartes**:
- Fond: #132437
- Border: 1px #23374e
- Border-radius: 16px
- Ombre: 24px blur, offset 0-8

**Entrées**:
- Fond: #101c2c
- Border: 1px #23374e
- Border-radius: 8px
- Couleur du texte: #e6eef6

---

## 8. MESSAGES ET VALIDATIONS

### 8.1 Messages de Succès
- "Lot ajouté avec succès"
- "Lot modifié avec succès"
- "Lot supprimé avec succès"
- "Export réussi: [chemin du fichier]"

### 8.2 Messages d'Erreur
- "Données invalides" - champs non remplis
- "Un lot avec cet ID existe déjà" - doublon d'ID
- "Sélectionnez une ligne dans le tableau" - pas de sélection
- "Poids invalide" - poids < 1
- Les messages s'affichent dans des dialogs d'avertissement

### 8.3 Messages de Confirmation
- "Supprimer le lot sélectionné ?" - Oui/Non requis
- "Oui" = suppression confirmée
- "Non" = opération annulée

---

## 9. ACTIONS UTILISATEUR

| Action | Déclencheur | Résultat |
|--------|-------------|----------|
| Ajouter lot | Clic "Ajouter" | Nouveau lot en table, formulaire réinitialisé |
| Modifier lot | Clic "Modifier" | Lot mis à jour, table rafraîchie |
| Supprimer lot | Clic "Supprimer" + Confirmation | Lot supprimé, formulaire réinitialisé |
| Réinitialiser | Clic "Réinitialiser" | Tous les champs vidés |
| Sélectionner | Clic sur ligne table | Champs remplis automatiquement |
| Rechercher | Saisie dans champ recherche | Table filtrée en temps réel |
| Trier | Clic en-tête colonne | Table triée (alternance asc/desc) |
| Export PDF | Clic "PDF" + Enregistrer | Fichier PDF créé |
| Export Excel | Clic "Excel" + Enregistrer | Fichier Excel créé |

---

## 10. DONNÉES EXEMPLES

### Lots de Démonstration

| ID Lot | Espèce | Poids | Zone | Date Capture | Navire | Qualité | Date Entrée |
|--------|--------|-------|------|--------------|--------|---------|-------------|
| LOT-001 | Sardine | 250 | Atlantique | 2024-01-15 08:30 | Stella Maris | Premium | 2024-01-15 14:00 |
| LOT-002 | Anchois | 180 | Méditerranée | 2024-01-14 10:15 | Marie-Jeanne | Standard | 2024-01-15 09:30 |
| LOT-003 | Thon | 500 | Atlantique | 2024-01-15 12:00 | Neptune | Premium | 2024-01-15 18:00 |
| LOT-004 | Morue | 320 | Manche | 2024-01-13 06:45 | Chalutier Breton | Économique | 2024-01-14 11:00 |

---

## 11. FONCTIONNALITÉS FUTURES

- [ ] Persistance base de données (SQLite/PostgreSQL)
- [ ] Graphiques dynamiques (Qt Charts)
- [ ] Import de fichiers Excel/CSV
- [ ] Historique des modifications
- [ ] Gestion des utilisateurs et login
- [ ] Permissions d'accès
- [ ] Notifications/Alertes
- [ ] Rapports périodiques
- [ ] Intégration API web
- [ ] Synchronisation cloud

---

## 12. NOTES TECHNIQUES

### Architecture

```
Qt Application (QMainWindow)
├── CentralWidget (QWidget)
│   └── RootLayout (QHBoxLayout)
│       ├── Sidebar (QFrame)
│       │   └── Navigation Buttons
│       └── ScrollArea (main content)
│           └── Page (QWidget)
│               ├── Header
│               ├── Charts Row
│               ├── Main Row
│               │   ├── Lots Form + Table
│               │   └── Stats Card
│               └── Stretch
```

### Patterns

- **MVC**: CaptureLotModel + QTableView + CaptureLotProxyModel
- **Signal/Slot**: Connexions Qt
- **Observer**: Auto-refresh stats
- **Factory**: makeCard(), makeTitle()

### Performance

- Table supporte 1000+ lignes
- Recherche O(n) acceptable pour < 10 000 lots
- Stats recalculées dynamiquement
- Pas de caching (données en mémoire)

---

## 13. COMPATIBILITÉ

- **Qt Version**: 6.x, 7.6.3
- **C++ Standard**: C++17
- **Plateformes**: Windows, Linux, macOS
- **Compilateurs**: MSVC 2022, MinGW, Clang
- **Qt Creator**: 13.x+

---

Dernière mise à jour: Janvier 2026
