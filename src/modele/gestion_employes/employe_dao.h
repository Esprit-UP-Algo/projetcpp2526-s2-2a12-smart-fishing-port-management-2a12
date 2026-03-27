#ifndef EMPLOYE_DAO_H
#define EMPLOYE_DAO_H

#include <QString>
#include <QVector>
#include "employe_user.h"

namespace employes {

/**
 * @class EmployeDAO
 * @brief Data Access Object pour la gestion des employés
 * 
 * Responsabilités:
 * - Valider les données de saisie (CIN, EMAIL, champs vides, etc.)
 * - Effectuer les opérations CRUD (ajouter, afficher, modifier, supprimer)
 * - Utiliser QSqlQuery::prepare() et bindValue() pour sécuriser les requêtes
 */
class EmployeDAO
{
public:
    EmployeDAO();
    ~EmployeDAO();

    // ─── VALIDATION MÉTIER ──────────────────────────────
    
    /**
     * @brief Valide un CIN (exactement 8 chiffres)
     * @param cin Le CIN à valider
     * @return QString vide si valide, sinon message d'erreur
     */
    static QString validateCIN(const QString &cin);

    /**
     * @brief Valide un EMAIL (contient '@' et '.')
     * @param email L'email à valider
     * @return QString vide si valide, sinon message d'erreur
     */
    static QString validateEmail(const QString &email);

    /**
     * @brief Valide un champ texte (non vide)
     * @param field Le contenu du champ
     * @param fieldName Le nom du champ (pour le message d'erreur)
     * @return QString vide si valide, sinon message d'erreur
     */
    static QString validateNotEmpty(const QString &field, const QString &fieldName);

    /**
     * @brief Valide un ID (nombre positif)
     * @param idStr L'ID à valider
     * @return QString vide si valide, sinon message d'erreur
     */
    static QString validateID(const QString &idStr);

    /**
     * @brief Valide un mot de passe (minimum 6 caractères)
     * @param password Le mot de passe à valider
     * @return QString vide si valide, sinon message d'erreur
     */
    static QString validatePassword(const QString &password);

    /**
     * @brief Valide TOUS les champs d'un employé
     * @param employe L'employé à valider
     * @return QString vide si tous valides, sinon premier message d'erreur
     */
    static QString validateEmploye(const EmployeUser &employe);

    // ─── OPÉRATIONS CRUD ────────────────────────────────

    /**
     * @brief Ajoute un nouvel employé à la base de données
     * @param employe L'employé à ajouter
     * @return QString vide si succès, sinon message d'erreur
     * 
     * Pré-condition: validateEmploye() doit être appelé avant
     */
    static QString ajouter(const EmployeUser &employe);

    /**
     * @brief Affiche tous les employés
     * @return QVector d'employés
     */
    static QVector<EmployeUser> afficher();

    /**
     * @brief Modifie un employé existant
     * @param employe L'employé avec données mises à jour
     * @return QString vide si succès, sinon message d'erreur
     * 
     * Note: L'ID de l'employé doit être valide
     */
    static QString modifier(const EmployeUser &employe);

    /**
     * @brief Supprime un employé
     * @param cin Le CIN de l'employé à supprimer
     * @return QString vide si succès, sinon message d'erreur
     */
    static QString supprimer(const QString &cin);

private:
    // Fonctions utilitaires privées
    static bool isValidEmail(const QString &email);
    static bool isAllDigits(const QString &str);
};

} // namespace employes

#endif // EMPLOYE_DAO_H
