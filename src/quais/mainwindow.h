#pragma once

#include <QMainWindow>
#include <QPushButton>

class CircularStatWidget;
class QTableView;

QT_BEGIN_NAMESPACE
namespace Ui { class QuaisMainWindow; }
QT_END_NAMESPACE

class QStandardItemModel;
class QuaiModel;
class QuaiSearchFilterProxyModel;
class QuaiSortProxyModel;

namespace quais {

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setSidebarActiveQuais();

signals:
    void requestShowTransactions();
    void requestShowNavires();
    void requestShowCaptures();
    void requestShowStockage();

private:
    void setActiveSidebarButton(QPushButton *btn);
    void applyStyleNames();
    void setupCardEffects();
    void setupLogo();
    void setupMenuIcon();
    void setupFonts();
    void setupModels();
    void setupTable();
    void setupMiniMaps();
    void setupStatWidgets();
    void applyTheme();
    void wireSignals();

    void refreshStats();
    void refreshMiniMap();
    void refreshTraceability();

    void onCreate();
    void onUpdate();
    void onDelete();
    void onExportPdf();

#ifdef INTEGRATION_APP
    void onSidebarVentes();
    void onSidebarNavires();
    void onSidebarCaptures();
    void onSidebarStockage();
#endif

    Ui::QuaisMainWindow *ui;

    QuaiModel *m_model = nullptr;
    QuaiSearchFilterProxyModel *m_searchProxy = nullptr;
    QuaiSortProxyModel *m_sortProxy = nullptr;

    QStandardItemModel *m_revByQuaiModel = nullptr;
    QStandardItemModel *m_occByQuaiModel = nullptr;
    QStandardItemModel *m_occHistoryModel = nullptr;
    QStandardItemModel *m_revHistoryModel = nullptr;

    bool m_internalTableSelectionChange = false;
    bool m_internalSelectAllChange = false;
    bool m_selectAllWanted = false;

    CircularStatWidget *m_statEtat = nullptr;
    CircularStatWidget *m_statTauxGlobal = nullptr;
    CircularStatWidget *m_statRevenuPeriode = nullptr;
    CircularStatWidget *m_statRevenuMoyen = nullptr;
    CircularStatWidget *m_statPlusRentable = nullptr;
    CircularStatWidget *m_statMoinsUtilise = nullptr;

    QTableView *m_table = nullptr;
    QTableView *m_revByQuaiTable = nullptr;
    QTableView *m_occByQuaiTable = nullptr;
    QTableView *m_occHistoryTable = nullptr;
    QTableView *m_revHistoryTable = nullptr;
};

} // namespace quais
