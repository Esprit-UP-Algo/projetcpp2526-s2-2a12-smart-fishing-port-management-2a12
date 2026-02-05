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
// StockageLot removed
class UtilisateurModel;
class UtilisateurSortFilterProxyModel;

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
    void loadSelectedUserToForm();

    void onAddChambre();
    void onUpdateChambre();
    void onDeleteChambre();
    void onClearChambreForm();


    void onAddUser();
    void onUpdateUser();
    void onDeleteUser();
    void onClearUserForm();

    void onExportUsersPdf();

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

    // Lots removed

    // Stats
    QLabel *m_statOccupancy = nullptr;
    QLabel *m_statTotalVolume = nullptr;
    QLabel *m_statAvgDuration = nullptr;

    // Utilisateurs form
    QLineEdit *m_userId = nullptr;
    QLineEdit *m_userNom = nullptr;
    QLineEdit *m_userPrenom = nullptr;
    QLineEdit *m_userEmail = nullptr;
    QLineEdit *m_userLogin = nullptr;
    QLineEdit *m_userMotDePasse = nullptr;
    QComboBox *m_userRole = nullptr;
    QComboBox *m_userStatut = nullptr;
    QDateTimeEdit *m_userDateCreation = nullptr;

    QPushButton *m_btnAddUser = nullptr;
    QPushButton *m_btnUpdateUser = nullptr;
    QPushButton *m_btnDeleteUser = nullptr;
    QPushButton *m_btnClearUser = nullptr;

    QLineEdit *m_searchUserNom = nullptr;
    QLineEdit *m_searchUserEmail = nullptr;
    QLineEdit *m_searchUserRole = nullptr;
    QLineEdit *m_searchUserStatut = nullptr;
    QPushButton *m_btnExportUsersPdf = nullptr;

    QTableView *m_tableUsers = nullptr;

    // Models
    ChambreFroideModel *m_chambreModel = nullptr;
    ChambreFroideSortFilterProxyModel *m_chambreProxy = nullptr;


    UtilisateurModel *m_userModel = nullptr;
    UtilisateurSortFilterProxyModel *m_userProxy = nullptr;
};
