#pragma once

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QPushButton;
class QTableView;

class QuaiModel;
class QuaiSortFilterProxyModel;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void buildUi();
    void applyTheme();
    void wireSignals();

    void refreshStats();
    void loadSelectedRowToForm();

    void onAdd();
    void onUpdate();
    void onDelete();
    void onClearForm();
    void onExportPdf();

    // Form
    QLineEdit *m_idQuai = nullptr;
    QLineEdit *m_matricule = nullptr;
    QDateTimeEdit *m_arrivee = nullptr;
    QDateTimeEdit *m_depart = nullptr;
    QComboBox *m_etat = nullptr;
    QDoubleSpinBox *m_prix = nullptr;

    QPushButton *m_btnAdd = nullptr;
    QPushButton *m_btnUpdate = nullptr;
    QPushButton *m_btnDelete = nullptr;
    QPushButton *m_btnClear = nullptr;

    // Search
    QLineEdit *m_searchMatricule = nullptr;
    QLineEdit *m_searchDate = nullptr;
    QLineEdit *m_searchQuai = nullptr;
    QPushButton *m_btnPdf = nullptr;

    // Table
    QTableView *m_table = nullptr;

    // Stats labels
    QLabel *m_statQuaisOcc = nullptr;
    QLabel *m_statRetards = nullptr;
    QLabel *m_statArrivees = nullptr;
    QLabel *m_statTopQuai = nullptr;

    // Models
    QuaiModel *m_model = nullptr;
    QuaiSortFilterProxyModel *m_proxy = nullptr;
};
