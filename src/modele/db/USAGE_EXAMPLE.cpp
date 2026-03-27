/**
 * @file USAGE_EXAMPLE.cpp
 * @brief Exemples d'utilisation de la classe Connection (Singleton Pattern)
 * 
 * Ce fichier montre comment utiliser le Singleton Connection dans d'autres
 * parties de l'application pour accéder à la base de données Oracle.
 */

#include "db/connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

// ==========================================
// EXEMPLE 1: Accéder à l'instance unique
// ==========================================
void exemple1_AccederALInstance()
{
    // La première même obtient l'instance (crée si première fois)
    Connection& db = Connection::createInstance();

    // On peut la récupérer plusieurs fois - toujours la même instance!
    Connection& db2 = Connection::createInstance();

    // db et db2 désignent exactement le même objet
    // (même adresse mémoire)
}

// ==========================================
// EXEMPLE 2: Exécuter une requête simple
// ==========================================
void exemple2_ExecuterRequete()
{
    Connection& db = Connection::createInstance();

    // Vérifier que la connexion est active
    if (!db.isConnected())
    {
        qDebug() << "Erreur: Base de données non connectée!";
        return;
    }

    // Créer une requête SQL
    QSqlQuery query(db.getDatabase());
    bool success = query.exec("SELECT * FROM employes");

    if (!success)
    {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return;
    }

    // Parcourir les résultats
    while (query.next())
    {
        QString nom = query.value("nom").toString();
        QString prenom = query.value("prenom").toString();
        qDebug() << "Employé:" << nom << prenom;
    }
}

// ==========================================
// EXEMPLE 3: Insérer des données
// ==========================================
void exemple3_InsererDonnees()
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    QSqlQuery query(db.getDatabase());

    // Préparer la requête avec des paramètres
    query.prepare("INSERT INTO employes (cin, nom, prenom, email, role) "
                  "VALUES (?, ?, ?, ?, ?)");

    // Binder les valeurs
    query.addBindValue("123456789");      // cin
    query.addBindValue("Dupont");         // nom
    query.addBindValue("Jean");           // prenom
    query.addBindValue("jean@example.com"); // email
    query.addBindValue("Technicien");     // role

    // Exécuter
    if (!query.exec())
    {
        qDebug() << "Erreur insertion:" << query.lastError().text();
        return;
    }

    qDebug() << "✓ Employé inséré avec succès!";
}

// ==========================================
// EXEMPLE 4: Mettre à jour des données
// ==========================================
void exemple4_MettreAJourDonnees(const QString& cin, const QString& nouveauRole)
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    QSqlQuery query(db.getDatabase());

    query.prepare("UPDATE employes SET role = ? WHERE cin = ?");
    query.addBindValue(nouveauRole);
    query.addBindValue(cin);

    if (!query.exec())
    {
        qDebug() << "Erreur mise à jour:" << query.lastError().text();
        return;
    }

    qDebug() << "✓ Employé mIS à jour!";
}

// ==========================================
// EXEMPLE 5: Supprimer des données
// ==========================================
void exemple5_SupprimerDonnees(const QString& cin)
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    QSqlQuery query(db.getDatabase());

    query.prepare("DELETE FROM employes WHERE cin = ?");
    query.addBindValue(cin);

    if (!query.exec())
    {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        return;
    }

    qDebug() << "✓ Employé supprimé!";
}

// ==========================================
// EXEMPLE 6: Transaction (commit/rollback)
// ==========================================
void exemple6_Transaction()
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    QSqlDatabase database = db.getDatabase();

    // Démarrer une transaction
    if (!database.transaction())
    {
        qDebug() << "Erreur: Impossible de démarrer une transaction!";
        return;
    }

    try
    {
        QSqlQuery query1(database);
        query1.prepare("INSERT INTO employes (cin, nom) VALUES (?, ?)");
        query1.addBindValue("111111");
        query1.addBindValue("Martin");

        if (!query1.exec())
            throw std::runtime_error("Erreur insertion 1");

        QSqlQuery query2(database);
        query2.prepare("INSERT INTO employes (cin, nom) VALUES (?, ?)");
        query2.addBindValue("222222");
        query2.addBindValue("Durand");

        if (!query2.exec())
            throw std::runtime_error("Erreur insertion 2");

        // Tout s'est bien passé - valider la transaction
        if (database.commit())
        {
            qDebug() << "✓ Transaction validée avec succès!";
        }
        else
        {
            throw std::runtime_error("Erreur commit");
        }
    }
    catch (const std::exception& e)
    {
        // Erreur - annuler tous les changements
        qDebug() << "✗ Erreur:" << e.what() << "- ROLLBACK effectué";
        database.rollback();
    }
}

// ==========================================
// EXEMPLE 7: Compter les enregistrements
// ==========================================
int exemple7_CompterEnregistrements()
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return -1;
    }

    QSqlQuery query(db.getDatabase());

    if (!query.exec("SELECT COUNT(*) FROM employes"))
    {
        qDebug() << "Erreur:" << query.lastError().text();
        return -1;
    }

    if (query.next())
    {
        int count = query.value(0).toInt();
        qDebug() << "Nombre d'employés:" << count;
        return count;
    }

    return -1;
}

// ==========================================
// EXEMPLE 8: Rechercher un employé
// ==========================================
void exemple8_RechercherEmploye(const QString& nomRecherche)
{
    Connection& db = Connection::createInstance();

    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    QSqlQuery query(db.getDatabase());

    // Utiliser LIKE pour la recherche flexible
    query.prepare("SELECT cin, nom, prenom, email, role FROM employes "
                  "WHERE nom LIKE ?");
    query.addBindValue("%" + nomRecherche + "%");

    if (!query.exec())
    {
        qDebug() << "Erreur recherche:" << query.lastError().text();
        return;
    }

    qDebug() << "Résultats de recherche pour:" << nomRecherche;

    if (!query.isActive())
    {
        qDebug() << "Aucun résultat trouvé";
        return;
    }

    while (query.next())
    {
        qDebug() << "  - CIN:" << query.value("cin").toString()
                 << "Nom:" << query.value("nom").toString()
                 << "Rôle:" << query.value("role").toString();
    }
}

// ==========================================
// AVANTAGES DU SINGLETON PATTERN
// ==========================================

/*
✅ SINGLE RESPONSIBILITY:
   - Une seule instance gère la connexion
   - Pas de confusion avec plusieurs connexions

✅ LAZY INITIALIZATION:
   - La connexion est créée à la première utilisation
   - Pas de surcharge au démarrage

✅ THREAD-SAFE (C++11+):
   - L'initialisation statique locale est thread-safe
   - Aucun besoin de mutex manuel

✅ DESTRUCTION AUTOMATIQUE:
   - La connexion est fermée à la fin du programme
   - Pas de fuite mémoire

✅ ACCÈS GLOBAL:
   - Connection::createInstance() accessible partout
   - Pas besoin de passer l'instance en paramètre

✅ UNICITÉ GARANTIE:
   - = delete sur le constructeur de copie
   - = delete sur l'opérateur d'affectation
   - Impossible de dupliquer l'instance
*/

// ==========================================
// UTILISATION RECOMMANDÉE DANS LE CODE
// ==========================================

/*
// Dans n'importe quel fichier .cpp:

#include "db/connection.h"
#include <QSqlQuery>

void uneFonction()
{
    // Récupérer l'instance (thread-safe, lazy init)
    Connection& db = Connection::createInstance();

    // Vérifier la connexion
    if (!db.isConnected())
    {
        qDebug() << "Base de données non connectée!";
        return;
    }

    // Exécuter une requête
    QSqlQuery query(db.getDatabase());
    query.exec("SELECT * FROM employes");

    // ... usage normal ...
}
*/
