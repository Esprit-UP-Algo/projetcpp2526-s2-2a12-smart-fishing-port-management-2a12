#ifndef EMPLOYE_MAINWINDOW_H
#define EMPLOYE_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QFuture>

class QAction;
class QActionGroup;
class QMenu;
class QWidget;

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
    enum class EmployeesSection {
        Affichage = 0,
        Statistique = 1,
        FicheDePaie = 2
    };

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
    void setupEmployeesDropdown();
    void setEmployeesSubmenuExpanded(bool expanded);
    void setEmployeesSection(EmployeesSection section);
    void setActiveEmployeesSubButton(QAction *action);
    void sendPayslipEmailAsync(const employes::EmployeUser &user,
                               const QString &month,
                               const QString &pdfPath,
                               const QString &subject,
                               const QString &body);

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

    QMenu *m_empDropdownMenu = nullptr;
    QActionGroup *m_empDropdownGroup = nullptr;
    QAction *m_empActionAffichage = nullptr;
    QAction *m_empActionStatistique = nullptr;
    QAction *m_empActionFichePaie = nullptr;

    QWidget *m_empStatsPage = nullptr;
    QWidget *m_empPayslipPage = nullptr;

    bool m_empDropdownExpanded = false;
};

} // namespace employes

#endif // EMPLOYE_MAINWINDOW_H
