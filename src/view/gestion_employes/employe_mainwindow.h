#ifndef EMPLOYE_MAINWINDOW_H
#define EMPLOYE_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>

#include "../../modele/gestion_employes/employe_user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class EmployeMainWindow; }
QT_END_NAMESPACE

namespace employes {

struct EmployeSession {
    QString utilisateur;
    QString dateConnexion;
    QString heure;
    QString ip;
    QString action;
};

class EmployeMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    EmployeMainWindow(QWidget *parent = nullptr);
    ~EmployeMainWindow();

    void setSidebarActiveEmployees();
    void pulseSidebarButton(QWidget *widget);

signals:
    void requestShowQuais();
    void requestShowTransactions();
    void requestShowNavires();
    void requestShowCaptures();
    void requestShowStockage();

private slots:
    // Sidebar
    void setActiveSidebarButton(QPushButton *btn);

#ifdef INTEGRATION_APP
    void onSidebarQuais();
    void onSidebarNavires();
    void onSidebarCaptures();
    void onSidebarStockage();
    void onSidebarVentes();
#endif

    void onCreerUser();
    void onModifierUser();
    void onSupprimerUser();
    void onActiverDesactiverUser();
    void onRechercherUser();
    void onTriUserChanged();
    void onPayslipPDF();
    void onPayslipExcel();
    void onPayslipEmail();
    void onTerminerSession();

private:
    void refreshUserTable();
    void refreshUserStats();
    void refreshSessions();
    void populateUserRow(int row, const EmployeUser &u);
    void updatePayrollCombo();
    QVector<EmployeUser> filteredAndSortedUsers() const;
    bool matchesUserSearch(const EmployeUser &u) const;
    void setupLogo();

    Ui::EmployeMainWindow *ui;

    QVector<EmployeUser> m_users;
    QVector<EmployeSession> m_sessions;
};

} // namespace employes

#endif // EMPLOYE_MAINWINDOW_H
