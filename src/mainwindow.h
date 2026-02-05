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

class TransactionModel;
class TransactionSortFilterProxyModel;

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
    void refreshTransLotCombo();
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

    void loadSelectedTransactionToForm();
    void onAddTransaction();
    void onUpdateTransaction();
    void onDeleteTransaction();
    void onClearTransactionForm();
    void onExportTransactionsPdf();
    void refreshTransactionStats();

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

    // Transactions (Vente / Achat)
    QLineEdit *m_idVente = nullptr;
    QLineEdit *m_client = nullptr;
    QComboBox *m_lotTrans = nullptr;
    QDoubleSpinBox *m_prixKg = nullptr;
    QDoubleSpinBox *m_quantiteTrans = nullptr;
    QLabel *m_totalTrans = nullptr;
    QDateTimeEdit *m_dateTrans = nullptr;
    QComboBox *m_modePaiement = nullptr;
    QComboBox *m_statutPaiement = nullptr;

    QPushButton *m_btnAddTrans = nullptr;
    QPushButton *m_btnUpdateTrans = nullptr;
    QPushButton *m_btnDeleteTrans = nullptr;
    QPushButton *m_btnClearTrans = nullptr;
    QPushButton *m_btnExportTransPdf = nullptr;

    QLineEdit *m_searchTransClient = nullptr;
    QLineEdit *m_searchTransLot = nullptr;
    QDateTimeEdit *m_searchTransFrom = nullptr;
    QDateTimeEdit *m_searchTransTo = nullptr;

    QTableView *m_tableTrans = nullptr;

    // Transaction stats
    QLabel *m_statRevenue = nullptr;
    QLabel *m_statTopSpecies = nullptr;
    QLabel *m_statBestClients = nullptr;

    // Stats
    QLabel *m_statOccupancy = nullptr;
    QLabel *m_statTotalVolume = nullptr;
    QLabel *m_statAvgDuration = nullptr;

    // Models
    ChambreFroideModel *m_chambreModel = nullptr;
    ChambreFroideSortFilterProxyModel *m_chambreProxy = nullptr;

    StockageLotModel *m_lotModel = nullptr;
    StockageLotSortFilterProxyModel *m_lotProxy = nullptr;

    TransactionModel *m_transModel = nullptr;
    TransactionSortFilterProxyModel *m_transProxy = nullptr;
};
