#ifndef EMPLOYE_SERVICE_H
#define EMPLOYE_SERVICE_H

#include <QString>
#include <QVector>
#include <QObject>
#include "employe_user.h"

namespace employes {

/**
 * @class EmployeeService
 * @brief Service métier pour la gestion des employés
 * 
 * Cette classe hérite de QObject UNIQUEMENT pour utiliser signals/slots.
 * Elle NE dépend d'AUCUN widget Qt.
 * 
 * Responsabilités:
 * - Encapsuler la logique métier (filtrage, tri, recherche)
 * - Gérer les données m_allEmployees et m_filteredEmployees
 * - Valider les données
 * - Accéder au DAO (base de données)
 * - Émettre des signaux lors de changements de données
 * 
 * Pattern: Service Locator + Observable (signaux/slots)
 */
class EmployeeService : public QObject
{
    Q_OBJECT

public:
    explicit EmployeeService(QObject *parent = nullptr);
    ~EmployeeService();

    // ═══ ACCESSEURS (Données) ═══
    QVector<EmployeUser> getFilteredEmployees() const;
    QVector<EmployeUser> getAllEmployees() const;
    int getEmployeeCount() const;

    // ═══ FILTRAGE ET TRI ═══
    void setSearchFilter(const QString &nom, const QString &email);
    void setRoleFilter(const QString &role);
    void setStatutFilter(const QString &statut);
    void setSortCriteria(int sortColumn, bool ascending);
    void clearAllFilters();

    // ═══ OPÉRATIONS CRUD ═══
    QString createEmployee(const EmployeUser &employee);
    QString updateEmployee(const EmployeUser &employee);
    QString deleteEmployee(const QString &cin);
    QString toggleEmployeeStatus(const QString &cin);
    
    // ═══ RECHARGEMENT DONNÉES ═══
    QString refreshEmployeesFromDatabase();

    // ═══ VALIDATION MÉTIER ═══
    static QString validateEmployee(const EmployeUser &employee);

signals:
    // Émis quand la liste filtrée change (après filtrage/tri)
    void filteredEmployeesChanged(const QVector<EmployeUser> &employees);
    
    // Émis quand la liste totale change (après CRUD op)
    void allEmployeesChanged(const QVector<EmployeUser> &employees);
    
    // Émis quand une opération a échoué
    void operationFailed(const QString &errorMessage);
    
    // Émis quand une opération a réussi
    void operationSucceeded(const QString &successMessage);

private slots:
    // Slot interne pour mettre à jour après un changement
    void applyFiltersAndSort();

private:
    // ═══ DONNÉES ═══
    QVector<EmployeUser> m_allEmployees;        // Toutes les données
    QVector<EmployeUser> m_filteredEmployees;   // Données filtrées/triées
    
    // ═══ CRITÈRES DE FILTRAGE ═══
    QString m_filterNom;
    QString m_filterEmail;
    QString m_filterRole;
    QString m_filterStatut;
    
    // ═══ CRITÈRES DE TRI ═══
    int m_sortColumn;      // 0=CIN, 1=Rôle, 2=Date
    bool m_sortAscending;
    
    // ═══ HELPER FUNCTIONS ═══
    bool matchesFilters(const EmployeUser &employee) const;
    void sortEmployees(QVector<EmployeUser> &employees) const;
};

} // namespace employes

#endif // EMPLOYE_SERVICE_H
