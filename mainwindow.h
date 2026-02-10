#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Sidebar
    void setActiveSidebarButton(QPushButton *btn);

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

    Ui::MainWindow *ui;
    QVector<Transaction> m_transactions;
};

#endif // MAINWINDOW_H
