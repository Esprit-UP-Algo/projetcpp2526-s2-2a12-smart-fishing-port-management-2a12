#pragma once

#include <QMainWindow>

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

class ChambreFroideModel;
class ChambreFroideSortFilterProxyModel;
class StockageLotModel;
class StockageLotSortFilterProxyModel;

class StatCircleWidget;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
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

    void loadSelectedChambreToForm();
    void loadSelectedLotToForm();

    void onAddChambre();
    void onUpdateChambre();
    void onDeleteChambre();
    void onClearChambreForm();

    void onAddLot();
    void onUpdateLot();
    void onDeleteLot();
    void onClearLotForm();

    void onExportLotsPdf();

    // Chambre froide form
    QLineEdit *m_idChambre = nullptr;
    QSpinBox *m_capacite = nullptr;
    QDoubleSpinBox *m_tempConsigne = nullptr;
    QLineEdit *m_emplacement = nullptr;
    QDoubleSpinBox *m_temperature = nullptr;

    QPushButton *m_btnAddChambre = nullptr;
    QPushButton *m_btnUpdateChambre = nullptr;
    QPushButton *m_btnDeleteChambre = nullptr;
    QPushButton *m_btnClearChambre = nullptr;

    QLineEdit *m_searchChambreId = nullptr;
    QLineEdit *m_searchChambreEmpl = nullptr;

    QTableView *m_tableChambres = nullptr;

    // Lots form
    QLineEdit *m_idLot = nullptr;
    QLineEdit *m_espece = nullptr;
    QSpinBox *m_quantite = nullptr;
    QDateTimeEdit *m_entree = nullptr;
    QDateTimeEdit *m_sortie = nullptr;
    QCheckBox *m_sortieRenseignee = nullptr;
    QComboBox *m_chambreLot = nullptr;

    QPushButton *m_btnAddLot = nullptr;
    QPushButton *m_btnUpdateLot = nullptr;
    QPushButton *m_btnDeleteLot = nullptr;
    QPushButton *m_btnClearLot = nullptr;

    QLineEdit *m_searchLot = nullptr;
    QLineEdit *m_searchLotChambre = nullptr;
    QLineEdit *m_searchLotEspece = nullptr;
    QPushButton *m_btnExportPdf = nullptr;

    QTableView *m_tableLots = nullptr;

    // Stats
    QLabel *m_statOccupancy = nullptr;
    QLabel *m_statTotalVolume = nullptr;
    QLabel *m_statAvgDuration = nullptr;
    // Circular stat widget for occupancy
    StatCircleWidget *m_occupancyCircle = nullptr;

    // Models
    ChambreFroideModel *m_chambreModel = nullptr;
    ChambreFroideSortFilterProxyModel *m_chambreProxy = nullptr;

    StockageLotModel *m_lotModel = nullptr;
    StockageLotSortFilterProxyModel *m_lotProxy = nullptr;
};
