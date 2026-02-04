#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QTableView>
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include "models/capturelot.h"
#include "models/capturelotmodel.h"
#include "models/capturelotproxymodel.h"
#include "src/statcircle.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    // Models
    CaptureLotModel *m_lotModel;
    CaptureLotProxyModel *m_lotProxy;

    // Form fields - Lots
    QLineEdit *m_idLot;
    QLineEdit *m_espece;
    QSpinBox *m_poids;
    QLineEdit *m_zonePeche;
    QDateTimeEdit *m_dateCapture;
    QLineEdit *m_navire;
    QComboBox *m_qualite;
    QDateTimeEdit *m_dateEntreeStock;

    // Buttons
    QPushButton *m_btnAddLot;
    QPushButton *m_btnUpdateLot;
    QPushButton *m_btnDeleteLot;
    QPushButton *m_btnClearLot;
    QPushButton *m_btnExportPdf;
    QPushButton *m_btnExportExcel;

    // Search/Filter
    QLineEdit *m_searchLot;
    QLineEdit *m_searchEspece;
    QLineEdit *m_searchNavire;
    QLineEdit *m_searchZone;

    // Table
    QTableView *m_tableLots;

    // Statistics
    StatCircleWidget *m_captureCircle;
    QLabel *m_statQuantiteEspece;
    QLabel *m_statZonePeche;
    QLabel *m_statAvgPoids;

    // UI
    void buildUi();
    void applyTheme();
    void wireSignals();

    // Helpers
    int selectedSourceRow(QTableView *view, const QSortFilterProxyModel *proxy) const;
    void refreshStats();
    void loadSelectedLotToForm();

private slots:
    // CRUD operations
    void onAddLot();
    void onUpdateLot();
    void onDeleteLot();
    void onClearLotForm();
    void onExportPdf();
    void onExportExcel();
};

#endif // MAINWINDOW_H
