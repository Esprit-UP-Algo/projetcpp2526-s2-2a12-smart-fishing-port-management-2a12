#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>

#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Session {
    QString utilisateur;
    QString dateConnexion;
    QString heure;
    QString ip;
    QString action;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Sidebar
    void setActiveSidebarButton(QPushButton *btn);

private slots:
    // ── Employee slots ──
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



    // Employee helpers
private:
    void refreshUserTable();
    void refreshUserStats();
    void refreshSessions();
    void populateUserRow(int row, const User &u);
    void updatePayrollCombo();
    QVector<User> filteredAndSortedUsers() const;
    bool matchesUserSearch(const User &u) const;

    Ui::MainWindow *ui;

    QVector<User> m_users;
    QVector<Session> m_sessions;
};

#endif // MAINWINDOW_H
