#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QVector>
#include <QTextEdit>
#include <QLineEdit>
#include "transaction.h"
#include "fishpricingdialog.h"
#include "transactionchatbot.h"

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

    // Ventes dropdown
    void onToggleVentesMenu();
    void onVentesAffichage();
    void onVentesStatistique();
    void onVentesAvancees();

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
    
    // Chatbot
    void onSendChatMessage();
    void setupChatBotUI();
    
    // Price Estimation
    void onPrixEstimeButtonClicked();
    void loadFishPricesFromFile();



private:
    void setActiveVentesOption(QPushButton *btn);
    void setVentesMenuExpanded(bool expanded, bool animated = true);
    void scrollToSectionTop(QWidget *section);
    void showOnlyVentesSection(QWidget *section);
    void showVentesAffichageBlock();

    void refreshTable();
    void loadDataFromDb();
    void refreshStats();
    void refreshAnomalies();
    void refreshPrevisions();
    void populateRow(int row, const Transaction &t);
    int resolvePecheurId(const QString &input, bool &ok) const;
    QString userNameForId(int id) const;
    QString colName(const QString &logical) const;
    QStringList allowedValuesForCheckConstraint(const QString &table, const QString &column) const;

    QVector<Transaction> filteredAndSorted() const;
    bool matchesSearch(const Transaction &t) const;
    QPixmap generateQRCode(const QString &text, int size);



    Ui::MainWindow *ui;
    QVector<Transaction> m_transactions;
    bool m_hasLotColumn = false;
    bool m_pecheurIsNumeric = false;
    QMap<QString, QString> m_colMap; // normalized logical -> actual column name
    QString m_tableName = QStringLiteral("TRANSACTION");

    bool m_ventesMenuExpanded = false;
    QPropertyAnimation *m_ventesMenuAnim = nullptr;
    
    // Chatbot
    TransactionChatBot *m_chatBot = nullptr;
    QTextEdit *m_chatDisplay = nullptr;
    QLineEdit *m_chatInput = nullptr;
    
    // Price estimation - use alias for FishPriceInfo from chatbot
    using FishPriceInfo = TransactionChatBot::FishPriceInfo;
    QMap<QString, FishPriceInfo> m_fishPriceEstimates;
    
    // Error tracking
    QString m_lastReference;


};

#endif // MAINWINDOW_H
