#include "employe_service.h"
#include "employe_dao.h"
#include <algorithm>
#include <QDebug>

namespace employes {

EmployeeService::EmployeeService(QObject *parent)
    : QObject(parent)
    , m_sortColumn(0)
    , m_sortAscending(true)
    , m_filterRole("Tous les rôles")
    , m_filterStatut("Tous les statuts")
{
    refreshEmployeesFromDatabase();
}

EmployeeService::~EmployeeService()
{
}

QVector<EmployeUser> EmployeeService::getFilteredEmployees() const
{
    return m_filteredEmployees;
}

QVector<EmployeUser> EmployeeService::getAllEmployees() const
{
    return m_allEmployees;
}

int EmployeeService::getEmployeeCount() const
{
    return m_allEmployees.size();
}

void EmployeeService::setSearchFilter(const QString &nom, const QString &email)
{
    m_filterNom = nom.trimmed();
    m_filterEmail = email.trimmed();
    applyFiltersAndSort();
}

void EmployeeService::setRoleFilter(const QString &role)
{
    m_filterRole = role;
    applyFiltersAndSort();
}

void EmployeeService::setStatutFilter(const QString &statut)
{
    m_filterStatut = statut;
    applyFiltersAndSort();
}

void EmployeeService::setSortCriteria(int sortColumn, bool ascending)
{
    m_sortColumn = sortColumn;
    m_sortAscending = ascending;
    applyFiltersAndSort();
}

void EmployeeService::clearAllFilters()
{
    m_filterNom.clear();
    m_filterEmail.clear();
    m_filterRole = "Tous les rôles";
    m_filterStatut = "Tous les statuts";
    m_sortColumn = 0;
    m_sortAscending = true;
    applyFiltersAndSort();
}

QString EmployeeService::createEmployee(const EmployeUser &employee)
{
    QString validationError = validateEmployee(employee);
    if (!validationError.isEmpty()) {
        emit operationFailed(validationError);
        return validationError;
    }
    
    QString dbError = EmployeDAO::ajouter(employee);
    if (!dbError.isEmpty()) {
        emit operationFailed(dbError);
        return dbError;
    }
    
    refreshEmployeesFromDatabase();
    emit operationSucceeded("Employé créé avec succès");
    return "";
}

QString EmployeeService::updateEmployee(const EmployeUser &employee)
{
    QString validationError = validateEmployee(employee);
    if (!validationError.isEmpty()) {
        emit operationFailed(validationError);
        return validationError;
    }
    
    QString dbError = EmployeDAO::modifier(employee);
    if (!dbError.isEmpty()) {
        emit operationFailed(dbError);
        return dbError;
    }
    
    refreshEmployeesFromDatabase();
    emit operationSucceeded("Employé modifié avec succès");
    return "";
}

QString EmployeeService::deleteEmployee(const QString &cin)
{
    QString validationError = EmployeDAO::validateCIN(cin);
    if (!validationError.isEmpty()) {
        emit operationFailed(validationError);
        return validationError;
    }
    
    QString dbError = EmployeDAO::supprimer(cin);
    if (!dbError.isEmpty()) {
        emit operationFailed(dbError);
        return dbError;
    }
    
    refreshEmployeesFromDatabase();
    emit operationSucceeded("Employé supprimé avec succès");
    return "";
}

QString EmployeeService::toggleEmployeeStatus(const QString &cin)
{
    auto it = std::find_if(m_allEmployees.begin(), m_allEmployees.end(),
        [&cin](const EmployeUser &e) { return e.cin == cin; });
    
    if (it == m_allEmployees.end()) {
        QString error = QString("Employe avec CIN '%1' non trouve").arg(cin);
        emit operationFailed(error);
        return error;
    }
    
    EmployeUser employee = *it;
    employee.statut = (employee.statut == "Actif") ? "Inactif" : "Actif";
    
    return updateEmployee(employee);
}

QString EmployeeService::refreshEmployeesFromDatabase()
{
    m_allEmployees = EmployeDAO::afficher();
    applyFiltersAndSort();
    return "";
}


QString EmployeeService::validateEmployee(const EmployeUser &employee)
{
    // Redélégguer au DAO (qui contient la logique de validation)
    // À terme, cette logique devrait être dans ce SERVICE pour plus de clarté
    
    QString errorCIN = EmployeDAO::validateCIN(employee.cin);
    if (!errorCIN.isEmpty()) return errorCIN;
    
    QString errorNom = EmployeDAO::validateNotEmpty(employee.nom, "Nom");
    if (!errorNom.isEmpty()) return errorNom;
    
    QString errorPrenom = EmployeDAO::validateNotEmpty(employee.prenom, "Prénom");
    if (!errorPrenom.isEmpty()) return errorPrenom;
    
    QString errorEmail = EmployeDAO::validateEmail(employee.email);
    if (!errorEmail.isEmpty()) return errorEmail;
    
    QString errorLogin = EmployeDAO::validateNotEmpty(employee.login, "Login");
    if (!errorLogin.isEmpty()) return errorLogin;
    
    QString errorPassword = EmployeDAO::validatePassword(employee.password);
    if (!errorPassword.isEmpty()) return errorPassword;
    
    QString errorRole = EmployeDAO::validateNotEmpty(employee.role, "Rôle");
    if (!errorRole.isEmpty()) return errorRole;
    
    return "";
}

// ═══════════════════════════════════════════════════════════════════════════
// ═══ SLOTS PRIVÉS (Logique Interne) ════════════════════════════════════════
// ═══════════════════════════════════════════════════════════════════════════

void EmployeeService::applyFiltersAndSort()
{
    m_filteredEmployees.clear();
    for (const auto &employee : m_allEmployees) {
        if (matchesFilters(employee)) {
            m_filteredEmployees.append(employee);
        }
    }
    
    sortEmployees(m_filteredEmployees);
    emit filteredEmployeesChanged(m_filteredEmployees);
}

// ═══════════════════════════════════════════════════════════════════════════
// ═══ HELPER FUNCTIONS ══════════════════════════════════════════════════════
// ═══════════════════════════════════════════════════════════════════════════

bool EmployeeService::matchesFilters(const EmployeUser &employee) const
{
    // Filtrer par nom/prénom
    if (!m_filterNom.isEmpty()) {
        bool nomMatch = employee.nom.contains(m_filterNom, Qt::CaseInsensitive);
        bool prenomMatch = employee.prenom.contains(m_filterNom, Qt::CaseInsensitive);
        if (!nomMatch && !prenomMatch)
            return false;
    }
    
    // Filtrer par email
    if (!m_filterEmail.isEmpty()) {
        if (!employee.email.contains(m_filterEmail, Qt::CaseInsensitive))
            return false;
    }
    
    // Filtrer par rôle
    if (m_filterRole != "Tous les rôles" && employee.role != m_filterRole)
        return false;
    
    // Filtrer par statut
    if (m_filterStatut != "Tous les statuts" && employee.statut != m_filterStatut)
        return false;
    
    return true;
}

void EmployeeService::sortEmployees(QVector<EmployeUser> &employees) const
{
    std::sort(employees.begin(), employees.end(),
        [this](const EmployeUser &a, const EmployeUser &b) {
        
        int cmp = 0;
        
        switch(m_sortColumn) {
        case 0: // CIN
            cmp = a.cin.compare(b.cin, Qt::CaseInsensitive);
            break;
        case 1: // Rôle
            cmp = a.role.compare(b.role, Qt::CaseInsensitive);
            break;
        default:
            cmp = 0;
        }
        
        return m_sortAscending ? (cmp < 0) : (cmp > 0);
    });
}

} // namespace employes

