#ifndef TRANSACTIONS_MAINWINDOW_H
#define TRANSACTIONS_MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TransactionMainWindow; }
QT_END_NAMESPACE

namespace transactions {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSidebarActiveVentes();

signals:
    void requestShowQuais();
    void requestShowNavires();
    void requestShowCaptures();
    void requestShowStockage();
    void requestShowEmployees();

private slots:
    // Sidebar
    void setActiveSidebarButton(QPushButton *btn);

#ifdef INTEGRATION_APP
    void onSidebarNavires();
    void onSidebarCaptures();
    void onSidebarStockage();
    void onSidebarEmployees();
    void onSidebarQuais();
#endif


    // CRUD
    void onCreer();
    void onModifier();
    void onSupprimer();

    // Search
    void onSearchChanged();

    // Sort
    void onSortChanged();

    // PDF
    void onExportPDF();

private:
    void refreshTable();
    void refreshStats();
    void refreshAnomalies();
    void refreshPrevisions();
    void populateRow(int row, const Transaction &t);

    QVector<Transaction> filteredAndSorted() const;
    bool matchesSearch(const Transaction &t) const;

    Ui::TransactionMainWindow *ui;
    QVector<Transaction> m_transactions;
};

} // namespace transactions

#endif // TRANSACTIONS_MAINWINDOW_H
