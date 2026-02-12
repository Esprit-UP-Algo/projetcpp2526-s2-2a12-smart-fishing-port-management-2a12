#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <algorithm>

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QPixmap>

#include "statcircle.h"
#include "chambrefroidemodel.h"
#include "chambrefroidesortfilterproxymodel.h"
#include "pdfexporter.h"
#include "stockagelotmodel.h"
#include "stockagelotsortfilterproxymodel.h"
#include "addchambrefroidedialog.h"
#include "addstockagelotdialog.h"
#include "updatechambrefroidedialog.h"
#include "updatestockagelotdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    setupModels();
    loadLogoImage();
    setupStatCircles();
    setupConnections();
    
    refreshChambreCombo();
    refreshStats();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModels()
{
    // Chambres model and proxy
    m_chambreModel = new ChambreFroideModel(this);
    m_chambreProxy = new ChambreFroideSortFilterProxyModel(this);
    m_chambreProxy->setSourceModel(m_chambreModel);
    ui->tableChambres->setModel(m_chambreProxy);
    ui->tableChambres->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableChambres->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableChambres->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableChambres->horizontalHeader()->setSectionResizeMode(ui->tableChambres->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    // Lots model and proxy
    m_lotModel = new StockageLotModel(this);
    m_lotProxy = new StockageLotSortFilterProxyModel(this);
    m_lotProxy->setSourceModel(m_lotModel);
    ui->tableLots->setModel(m_lotProxy);
    ui->tableLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableLots->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableLots->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableLots->horizontalHeader()->setSectionResizeMode(ui->tableLots->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    // Unassigned lots proxy
    m_unassignedLotsProxy = new StockageLotSortFilterProxyModel(this);
    m_unassignedLotsProxy->setSourceModel(m_lotModel);
    m_unassignedLotsProxy->setShowOnlyUnassigned(true);
    ui->tableUnassignedLots->setModel(m_unassignedLotsProxy);
    ui->tableUnassignedLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableUnassignedLots->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableUnassignedLots->setColumnHidden(StockageLotModel::ColIdChambre, true);
    ui->tableUnassignedLots->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableUnassignedLots->horizontalHeader()->setSectionResizeMode(ui->tableUnassignedLots->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    // Assigned lots proxy
    m_assignedLotsProxy = new StockageLotSortFilterProxyModel(this);
    m_assignedLotsProxy->setSourceModel(m_lotModel);
    m_assignedLotsProxy->setShowOnlyAssigned(true);
    ui->tableAssignments->setModel(m_assignedLotsProxy);
    ui->tableAssignments->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableAssignments->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableAssignments->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableAssignments->horizontalHeader()->setSectionResizeMode(ui->tableAssignments->horizontalHeader()->count() - 1, QHeaderView::Stretch);
}

void MainWindow::loadLogoImage()
{
    QPixmap logoPixmap(":/vision_sight.png");
    if (!logoPixmap.isNull())
    {
        ui->logoLabel->setPixmap(logoPixmap.scaled(58, 58, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::setupStatCircles()
{
    // Create stat circle widgets
    m_occupancyCircle = new StatCircleWidget(this);
    m_occupancyCircle->setLabel(tr("Taux d'occupation"));
    m_occupancyCircle->setColor(QColor("#39c0fa"));
    
    m_volumeCircle = new StatCircleWidget(this);
    m_volumeCircle->setLabel(tr("Volume total (kg)"));
    m_volumeCircle->setColor(QColor("#5f27cd"));
    
    m_durationCircle = new StatCircleWidget(this);
    m_durationCircle->setLabel(tr("Durée moy. (j)"));
    m_durationCircle->setColor(QColor("#00d2d3"));

    // Add circles to the stats layout
    ui->statsCirclesLayout->addWidget(m_occupancyCircle);
    ui->statsCirclesLayout->addWidget(m_volumeCircle);
    ui->statsCirclesLayout->addWidget(m_durationCircle);
}

void MainWindow::setupConnections()
{
    // Chambres buttons
    connect(ui->btnAddChambre, &QPushButton::clicked, this, &MainWindow::onShowAddChambreDialog);
    connect(ui->btnUpdateChambre, &QPushButton::clicked, this, &MainWindow::onShowUpdateChambreDialog);
    connect(ui->btnDeleteChambre, &QPushButton::clicked, this, &MainWindow::onDeleteChambre);
    connect(ui->btnClearChambre, &QPushButton::clicked, this, [this]() {
        ui->tableChambres->clearSelection();
    });
    connect(ui->checkSelectAllChambre, &QCheckBox::toggled, this, &MainWindow::onSelectAllChambres);

    // Lots buttons
    connect(ui->btnAddLot, &QPushButton::clicked, this, &MainWindow::onShowAddLotDialog);
    connect(ui->btnUpdateLot, &QPushButton::clicked, this, &MainWindow::onShowUpdateLotDialog);
    connect(ui->btnDeleteLot, &QPushButton::clicked, this, &MainWindow::onDeleteLot);
    connect(ui->btnClearLot, &QPushButton::clicked, this, [this]() {
        ui->tableLots->clearSelection();
    });
    connect(ui->checkSelectAllLot, &QCheckBox::toggled, this, &MainWindow::onSelectAllLots);

    // Export buttons
    connect(ui->btnExportChambrePdf, &QPushButton::clicked, this, &MainWindow::onExportChambreFacturePdf);
    connect(ui->btnExportPdf, &QPushButton::clicked, this, &MainWindow::onExportLotsPdf);

    // Tri
    connect(ui->triChambre, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTriChambreChanged);

    // Search filters for chambres
    connect(ui->searchChambreId, &QLineEdit::textChanged, this, [this](const QString &text) {
        m_chambreProxy->setSearchId(text);
    });
    connect(ui->searchChambreEmp, &QLineEdit::textChanged, this, [this](const QString &text) {
        m_chambreProxy->setSearchEmplacement(text);
    });

    // Search filters for lots
    connect(ui->searchLot, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchId);
    connect(ui->searchLotChambre, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchChambre);
    connect(ui->searchLotEspece, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchEspece);

    // Sync checkbox state with table selection (Chambres)
    connect(ui->tableChambres->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        ui->checkSelectAllChambre->blockSignals(true);
        const int totalRows = m_chambreProxy->rowCount();
        const int selectedRows = ui->tableChambres->selectionModel()->selectedRows().count();
        ui->checkSelectAllChambre->setChecked(totalRows > 0 && selectedRows == totalRows);
        ui->checkSelectAllChambre->blockSignals(false);
    });

    // Sync checkbox state with table selection (Lots)
    connect(ui->tableLots->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        ui->checkSelectAllLot->blockSignals(true);
        const int totalRows = m_lotProxy->rowCount();
        const int selectedRows = ui->tableLots->selectionModel()->selectedRows().count();
        ui->checkSelectAllLot->setChecked(totalRows > 0 && selectedRows == totalRows);
        ui->checkSelectAllLot->blockSignals(false);
    });

    // Refresh stats automatically
    auto refreshAll = [this]() {
        refreshChambreCombo();
        refreshStats();
        if (ui->tableChambres) ui->tableChambres->resizeColumnsToContents();
        if (ui->tableLots) ui->tableLots->resizeColumnsToContents();
    };

    connect(m_chambreModel, &QAbstractItemModel::rowsInserted, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::rowsRemoved, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::modelReset, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::dataChanged, this, refreshAll);

    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, refreshAll);

    // Refresh affectation filters when data changes
    auto refreshAffectationFilters = [this]() {
        if (m_unassignedLotsProxy)
            m_unassignedLotsProxy->refresh();
        if (m_assignedLotsProxy)
            m_assignedLotsProxy->refresh();
        if (ui->tableUnassignedLots) ui->tableUnassignedLots->resizeColumnsToContents();
        if (ui->tableAssignments) ui->tableAssignments->resizeColumnsToContents();
    };
    
    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, refreshAffectationFilters);

    // Affectation
    connect(ui->btnAssignLot, &QPushButton::clicked, this, &MainWindow::onAssignLotToChambre);
    connect(ui->btnUnassignLot, &QPushButton::clicked, this, &MainWindow::onUnassignLot);

    // Stats period filter
    connect(ui->statsPeriodFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this]() {
        refreshStats();
    });
}

int MainWindow::selectedSourceRow(QTableView *view, const QSortFilterProxyModel *proxy) const
{
    if (!view || !proxy)
        return -1;

    const QModelIndex proxyIndex = view->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    return proxy->mapToSource(proxyIndex).row();
}

int MainWindow::usedCapacityForChambre(const QString &idChambre, int excludeRow) const
{
    int used = 0;
    const auto &lots = m_lotModel->items();
    for (int i = 0; i < lots.size(); ++i)
    {
        if (i == excludeRow)
            continue;
        if (lots.at(i).idChambre.compare(idChambre, Qt::CaseInsensitive) == 0)
            used += lots.at(i).quantite;
    }
    return used;
}

int MainWindow::totalCapacity() const
{
    int sum = 0;
    for (const auto &c : m_chambreModel->items())
        sum += c.capacite;
    return sum;
}

int MainWindow::totalUsed() const
{
    int sum = 0;
    const auto &lots = m_lotModel->items();
    for (const auto &lot : lots)
        sum += lot.quantite;
    return sum;
}

double MainWindow::averageDurationDays() const
{
    const auto &lots = m_lotModel->items();
    if (lots.isEmpty())
        return 0.0;

    double totalDays = 0.0;
    int count = 0;

    for (const auto &lot : lots)
    {
        if (lot.entree.isValid())
        {
            const auto endDt = lot.sortie.isValid() ? lot.sortie : QDateTime::currentDateTime();
            const qint64 secs = lot.entree.secsTo(endDt);
            const double days = secs / 86400.0;
            totalDays += days;
            ++count;
        }
    }

    return (count > 0) ? (totalDays / count) : 0.0;
}

void MainWindow::refreshChambreCombo()
{
    ui->assignChambreCombo->clear();
    ui->assignChambreCombo->addItem(tr("-- Sélectionner --"), QVariant());

    for (const ChambreFroide &ch : m_chambreModel->items())
    {
        const int used = usedCapacityForChambre(ch.idChambre);
        const int remaining = ch.capacite - used;
        const QString label = QString("%1 (Capacité: %2, Restant: %3 kg)")
                                  .arg(ch.idChambre)
                                  .arg(ch.capacite)
                                  .arg(remaining);
        ui->assignChambreCombo->addItem(label, ch.idChambre);
    }
}

void MainWindow::refreshStats()
{
    const int totCap = totalCapacity();
    const int used = totalUsed();
    double occupancy = (totCap > 0) ? (100.0 * used / totCap) : 0.0;
    if (occupancy > 100.0)
        occupancy = 100.0;

    const double avgDays = averageDurationDays();

    // Update circle widgets
    if (m_occupancyCircle)
    {
        m_occupancyCircle->setValue(static_cast<int>(std::round(occupancy)));
        m_occupancyCircle->setBottomText(QString("%1%").arg(static_cast<int>(std::round(occupancy))));
    }

    if (m_volumeCircle)
    {
        const int pct = (totCap > 0) ? static_cast<int>(std::round(100.0 * used / totCap)) : 0;
        m_volumeCircle->setValue(pct);
        m_volumeCircle->setBottomText(QString("%1 kg").arg(used));
    }

    if (m_durationCircle)
    {
        // Show average duration as a percentage (scale 0-30 days -> 0-100%)
        const int pct = static_cast<int>(std::round(std::min(100.0, (avgDays / 30.0) * 100.0)));
        m_durationCircle->setValue(pct);
        m_durationCircle->setBottomText(QString("%1 j").arg(avgDays, 0, 'f', 1));
    }
}

void MainWindow::onShowAddChambreDialog()
{
    AddChambreFroideDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        ChambreFroide ch;
        ch.idChambre = dlg.idChambre();
        ch.capacite = dlg.capacite();
        ch.tempConsigne = dlg.tempConsigne();
        ch.emplacement = dlg.emplacement();
        ch.temperature = dlg.temperature();

        m_chambreModel->addItem(ch);
    }
}

void MainWindow::onShowUpdateChambreDialog()
{
    const int row = selectedSourceRow(ui->tableChambres, m_chambreProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner une chambre."));
        return;
    }

    ChambreFroide ch = m_chambreModel->items().at(row);

    UpdateChambreFroideDialog dlg(this);
    dlg.setIdChambre(ch.idChambre);
    dlg.setCapacite(ch.capacite);
    dlg.setTempConsigne(ch.tempConsigne);
    dlg.setEmplacement(ch.emplacement);
    dlg.setTemperature(ch.temperature);

    if (dlg.exec() == QDialog::Accepted)
    {
        ch.capacite = dlg.capacite();
        ch.tempConsigne = dlg.tempConsigne();
        ch.emplacement = dlg.emplacement();
        ch.temperature = dlg.temperature();

        m_chambreModel->updateItem(row, ch);
    }
}

void MainWindow::onDeleteChambre()
{
    const QModelIndexList sel = ui->tableChambres->selectionModel()->selectedRows();
    if (sel.isEmpty())
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner au moins une chambre."));
        return;
    }

    QList<int> rows;
    for (const QModelIndex &idx : sel)
        rows.append(m_chambreProxy->mapToSource(idx).row());

    std::sort(rows.begin(), rows.end(), std::greater<int>());

    for (int r : rows)
        m_chambreModel->removeItem(r);
}

void MainWindow::onSelectAllChambres()
{
    if (ui->checkSelectAllChambre->isChecked())
        ui->tableChambres->selectAll();
    else
        ui->tableChambres->clearSelection();
}

void MainWindow::onShowAddLotDialog()
{
    AddStockageLotDialog dlg(this);
    dlg.setChambreModel(m_chambreModel);
    if (dlg.exec() == QDialog::Accepted)
    {
        StockageLot lot;
        lot.idLot = dlg.idLot();
        lot.espece = dlg.espece();
        lot.quantite = dlg.quantite();
        lot.entree = dlg.dateEntree();
        lot.sortie = dlg.dateSortie();
        lot.idChambre = dlg.idChambre();

        m_lotModel->addItem(lot);
    }
}

void MainWindow::onShowUpdateLotDialog()
{
    const int row = selectedSourceRow(ui->tableLots, m_lotProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner un lot."));
        return;
    }

    StockageLot lot = m_lotModel->items().at(row);

    UpdateStockageLotDialog dlg(this);
    dlg.setChambreModel(m_chambreModel);
    dlg.setIdLot(lot.idLot);
    dlg.setEspece(lot.espece);
    dlg.setQuantite(lot.quantite);
    dlg.setDateEntree(lot.entree);
    dlg.setDateSortie(lot.sortie);
    dlg.setIdChambre(lot.idChambre);

    if (dlg.exec() == QDialog::Accepted)
    {
        lot.espece = dlg.espece();
        lot.quantite = dlg.quantite();
        lot.entree = dlg.dateEntree();
        lot.sortie = dlg.dateSortie();
        lot.idChambre = dlg.idChambre();

        m_lotModel->updateItem(row, lot);
    }
}

void MainWindow::onDeleteLot()
{
    const QModelIndexList sel = ui->tableLots->selectionModel()->selectedRows();
    if (sel.isEmpty())
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner au moins un lot."));
        return;
    }

    QList<int> rows;
    for (const QModelIndex &idx : sel)
        rows.append(m_lotProxy->mapToSource(idx).row());

    std::sort(rows.begin(), rows.end(), std::greater<int>());

    for (int r : rows)
        m_lotModel->removeItem(r);
}

void MainWindow::onSelectAllLots()
{
    if (ui->checkSelectAllLot->isChecked())
        ui->tableLots->selectAll();
    else
        ui->tableLots->clearSelection();
}

void MainWindow::onExportLotsPdf()
{
    const QString filename = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), QString(), tr("PDF Files (*.pdf)"));
    if (filename.isEmpty())
        return;

    PdfExporter::exportLotsToPdf(filename, m_lotModel->items());
    QMessageBox::information(this, tr("Succès"), tr("Export PDF terminé."));
}

void MainWindow::onExportChambreFacturePdf()
{
    const QString filename = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), QString(), tr("PDF Files (*.pdf)"));
    if (filename.isEmpty())
        return;

    PdfExporter::exportChambresToPdf(filename, m_chambreModel->items());
    QMessageBox::information(this, tr("Succès"), tr("Export PDF terminé."));
}

void MainWindow::onTriChambreChanged(int index)
{
    if (index == 0)
    {
        m_chambreProxy->sort(-1);
    }
    else if (index == 1)
    {
        m_chambreProxy->sort(ChambreFroideModel::ColCapacite, Qt::AscendingOrder);
    }
    else if (index == 2)
    {
        m_chambreProxy->sort(ChambreFroideModel::ColTempConsigne, Qt::AscendingOrder);
    }
}

void MainWindow::onAssignLotToChambre()
{
    const int row = selectedSourceRow(ui->tableUnassignedLots, m_unassignedLotsProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner un lot à affecter."));
        return;
    }

    const QString chamberId = ui->assignChambreCombo->currentData().toString();
    if (chamberId.isEmpty())
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez choisir une chambre."));
        return;
    }

    StockageLot lot = m_lotModel->items()[row];
    lot.idChambre = chamberId;
    m_lotModel->updateItem(row, lot);

    m_unassignedLotsProxy->refresh();
    m_assignedLotsProxy->refresh();

    refreshStats();
}

void MainWindow::onUnassignLot()
{
    const int row = selectedSourceRow(ui->tableAssignments, m_assignedLotsProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Info"), tr("Veuillez sélectionner un lot affecté."));
        return;
    }

    StockageLot lot = m_lotModel->items()[row];
    lot.idChambre.clear();
    m_lotModel->updateItem(row, lot);

    m_unassignedLotsProxy->refresh();
    m_assignedLotsProxy->refresh();

    refreshStats();
}

// Add missing search slots references
void MainWindow::onAddChambre() {}
void MainWindow::onUpdateChambre() {}
void MainWindow::onAddLot() {}
void MainWindow::onUpdateLot() {}
