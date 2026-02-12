#pragma once

#include <QMainWindow>
#include <QList>

class QSortFilterProxyModel;

class QLabel;
class QLineEdit;
class QComboBox;
class QCheckBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QSpinBox;
class QPushButton;
class QTableView;

namespace stockage {
class ChambreFroideModel;
class ChambreFroideSortFilterProxyModel;
class StockageLotModel;
class StockageLotSortFilterProxyModel;
class StatCircleWidget;
}

namespace stockage {

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void requestShowQuais();
    void requestShowTransactions();
    void requestShowNavires();
    void requestShowCaptures();
    void requestShowEmployees();

private:
    void setActiveSidebarButton(QPushButton *activeBtn);
    void buildUi();
    void applyTheme();
    void wireSignals();

    void refreshChambreCombo();
    void refreshStats();

    int selectedSourceRow(QTableView *view, const QSortFilterProxyModel *proxy) const;

    int usedCapacityForChambre(const QString &idChambre, int excludeRow = -1) const;
    int totalCapacity() const;
    int totalUsed() const;
    double averageDurationDays() const;

    void onShowAddChambreDialog();
    void onShowAddLotDialog();
    void onShowUpdateChambreDialog();
    void onShowUpdateLotDialog();
    void onAddChambre();
    void onUpdateChambre();
    void onDeleteChambre();
    void onSelectAllChambres();

    void onAddLot();
    void onUpdateLot();
    void onDeleteLot();
    void onSelectAllLots();

    void onExportLotsPdf();
    void onExportChambreFacturePdf();
    void onTriChambreChanged(int index);

    void on_btnOptimizeStorage_clicked();

    void onAssignLotToChambre();
    void onUnassignLot();

    // Chambre froide form
    QPushButton *m_btnAddChambre = nullptr;
    QPushButton *m_btnUpdateChambre = nullptr;
    QPushButton *m_btnDeleteChambre = nullptr;
    QPushButton *m_btnClearChambre = nullptr;
    QCheckBox *m_checkSelectAllChambre = nullptr;

    QLineEdit *m_searchChambreId = nullptr;
    QLineEdit *m_searchChambreEmpl = nullptr;
    
    QComboBox *m_triChambre = nullptr;
    QPushButton *m_btnExportChambrePdf = nullptr;

    QTableView *m_tableChambres = nullptr;

    // Lots form
    QPushButton *m_btnAddLot = nullptr;
    QPushButton *m_btnUpdateLot = nullptr;
    QPushButton *m_btnDeleteLot = nullptr;
    QPushButton *m_btnClearLot = nullptr;
    QCheckBox *m_checkSelectAllLot = nullptr;

    QLineEdit *m_searchLot = nullptr;
    QLineEdit *m_searchLotChambre = nullptr;
    QLineEdit *m_searchLotEspece = nullptr;
    QPushButton *m_btnExportPdf = nullptr;

    QTableView *m_tableLots = nullptr;

    // Affectation
    QTableView *m_tableUnassignedLots = nullptr;
    QComboBox *m_assignChambreCombo = nullptr;
    QPushButton *m_btnAssignLot = nullptr;
    QPushButton *m_btnUnassignLot = nullptr;
    QTableView *m_tableAssignments = nullptr;
    StockageLotSortFilterProxyModel *m_unassignedLotsProxy = nullptr;
    StockageLotSortFilterProxyModel *m_assignedLotsProxy = nullptr;

    // Stats
    QLabel *m_statOccupancy = nullptr;
    QLabel *m_statTotalVolume = nullptr;
    QLabel *m_statAvgDuration = nullptr;
    // Circular stat widgets
    StatCircleWidget *m_occupancyCircle = nullptr;
    StatCircleWidget *m_volumeCircle = nullptr;
    StatCircleWidget *m_durationCircle = nullptr;
    StatCircleWidget *m_energyEfficiencyCircle = nullptr;
    // Period filter for stats
    QComboBox *m_statsPeriodFilter = nullptr;
    QPushButton *m_btnOptimizeStorage = nullptr;

    // Models
    ChambreFroideModel *m_chambreModel = nullptr;
    ChambreFroideSortFilterProxyModel *m_chambreProxy = nullptr;

    StockageLotModel *m_lotModel = nullptr;
    StockageLotSortFilterProxyModel *m_lotProxy = nullptr;

    QList<QPushButton *> m_sidebarButtons;
};

} // namespace stockage
