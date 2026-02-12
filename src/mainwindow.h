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
#include <QtCharts>

// IMPORTANT: Do NOT use 'using namespace QtCharts;' locally
// Instead, use proper namespacing or macro if needed.
// However, standard Qt6 syntax is usually straightforward.

#include "models/capturelot.h"
#include "models/capturelotmodel.h"
#include "models/capturelotproxymodel.h"
#include "statcircle.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    // Models
    CaptureLotModel *m_lotModel;
    CaptureLotProxyModel *m_lotProxy;

    // Buttons
    QPushButton *m_btnAddLot;
    QPushButton *m_btnUpdateLot;
    QPushButton *m_btnDeleteLot;
    QCheckBox *m_chkSelectAll;
    QPushButton *m_btnTraceLot;
    QPushButton *m_btnIceYield; // Nouveau bouton Analyse Glace
    QPushButton *m_btnExportPdf;
    QPushButton *m_btnExportExcel;

    // Search/Filter
    QComboBox *m_sortCombo;
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
    
    // Charts
    QChartView *m_chartEspece;
    QChartView *m_chartZone;
    QChartView *m_chartPoids;

    // UI
    void buildUi();
    void applyTheme();
    void wireSignals();

    // Helpers
    int selectedSourceRow(QTableView *view, const QSortFilterProxyModel *proxy) const;
    void refreshStats();

private slots:
    // CRUD operations
    void onAddLot();
    void onUpdateLot();
    void onDeleteLot();
    void onTraceLot();
    void onIceYieldAnalysis();
    void onSortChanged(int index);
    void onExportPdf();
    void onExportExcel();
};

#endif // MAINWINDOW_H
