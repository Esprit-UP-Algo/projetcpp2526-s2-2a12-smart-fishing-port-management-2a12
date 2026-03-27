#ifndef EMPLOYE_SERVICE_H
#define EMPLOYE_SERVICE_H

#include <QString>
#include <QVector>
#include <QObject>
#include "employe_user.h"

namespace employes {

class EmployeeService : public QObject
{
    Q_OBJECT

public:
    explicit EmployeeService(QObject *parent = nullptr);
    ~EmployeeService();

    QVector<EmployeUser> getFilteredEmployees() const;
    QVector<EmployeUser> getAllEmployees() const;
    int getEmployeeCount() const;

    void setSearchFilter(const QString &nom, const QString &email);
    void setRoleFilter(const QString &role);
    void setStatutFilter(const QString &statut);
    void setSortCriteria(int sortColumn, bool ascending);
    void clearAllFilters();

    QString createEmployee(const EmployeUser &employee);
    QString updateEmployee(const EmployeUser &employee);
    QString deleteEmployee(const QString &cin);
    QString toggleEmployeeStatus(const QString &cin);
    
    QString refreshEmployeesFromDatabase();

    static QString validateEmployee(const EmployeUser &employee);

signals:
    void filteredEmployeesChanged(const QVector<EmployeUser> &employees);
    
    void allEmployeesChanged(const QVector<EmployeUser> &employees);
    
    void operationFailed(const QString &errorMessage);
    
    void operationSucceeded(const QString &successMessage);

private slots:
    void applyFiltersAndSort();

private:
    QVector<EmployeUser> m_allEmployees;
    QVector<EmployeUser> m_filteredEmployees;
    
    QString m_filterNom;
    QString m_filterEmail;
    QString m_filterRole;
    QString m_filterStatut;
    
    int m_sortColumn;
    bool m_sortAscending;
    
    bool matchesFilters(const EmployeUser &employee) const;
    void sortEmployees(QVector<EmployeUser> &employees) const;
};

} // namespace employes

#endif // EMPLOYE_SERVICE_H
