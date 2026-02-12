#include "mainwindow.h"

#include <algorithm>

#include <QAbstractItemView>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QIcon>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSortFilterProxyModel>
#include <QSpinBox>
#include <QTableView>
#include <QToolButton>
#include <QVBoxLayout>
#include <QSizePolicy>

#include "statcircle.h"

#include <QPixmap>

#include "chambrefroidemodel.h"
#include "chambrefroidesortfilterproxymodel.h"
#include "pdfexporter.h"
#include "stockagelotmodel.h"
#include "stockagelotsortfilterproxymodel.h"
#include "addchambrefroidedialog.h"
#include "addstockagelotdialog.h"
#include "updatechambrefroidedialog.h"
#include "updatestockagelotdialog.h"

namespace stockage {

static QFrame *makeCard(QWidget *parent, const char *objectName)
{
    auto *f = new QFrame(parent);
    f->setObjectName(objectName);
    f->setFrameShape(QFrame::NoFrame);

    auto *shadow = new QGraphicsDropShadowEffect(f);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 8);
    shadow->setColor(QColor(0, 0, 0, 90));
    f->setGraphicsEffect(shadow);

    return f;
}

static QLabel *makeTitle(const QString &text, int px, bool bold, QWidget *parent)
{
    auto *l = new QLabel(text, parent);
    QFont ft = l->font();
    ft.setPixelSize(px);
    ft.setBold(bold);
    l->setFont(ft);
    return l;
}

static QWidget *makeChartPlaceholder(const QString &title, QWidget *parent)
{
    auto *card = makeCard(parent, "Card");
    auto *vl = new QVBoxLayout(card);
    vl->setContentsMargins(14, 12, 14, 12);
    vl->setSpacing(10);

    auto *t = new QLabel(title, card);
    t->setObjectName("SectionTitle");

    auto *ph = new QLabel(card);
    ph->setObjectName("ChartPlaceholder");
    ph->setMinimumHeight(140);
    ph->setAlignment(Qt::AlignCenter);
    ph->setText(QObject::tr("[Graphique]"));

    vl->addWidget(t);
    vl->addWidget(ph, 1);

    return card;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("VISION SIGHT"));
    resize(1366, 768);

    m_chambreModel = new ChambreFroideModel(this);
    m_chambreProxy = new ChambreFroideSortFilterProxyModel(this);
    m_chambreProxy->setSourceModel(m_chambreModel);

    m_lotModel = new StockageLotModel(this);
    m_lotProxy = new StockageLotSortFilterProxyModel(this);
    m_lotProxy->setSourceModel(m_lotModel);

    m_unassignedLotsProxy = new StockageLotSortFilterProxyModel(this);
    m_unassignedLotsProxy->setSourceModel(m_lotModel);
    m_unassignedLotsProxy->setShowOnlyUnassigned(true);

    m_assignedLotsProxy = new StockageLotSortFilterProxyModel(this);
    m_assignedLotsProxy->setSourceModel(m_lotModel);
    m_assignedLotsProxy->setShowOnlyAssigned(true);

    buildUi();
    applyTheme();
    wireSignals();

    refreshChambreCombo();
    refreshStats();
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Sidebar
    auto *sidebar = new QFrame(central);
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(270);

    auto *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(18, 18, 18, 18);
    sideLayout->setSpacing(10);

    auto *brandBox = new QWidget(sidebar);
    auto *brandBoxLayout = new QVBoxLayout(brandBox);
    brandBoxLayout->setContentsMargins(0, 0, 0, 0);
    brandBoxLayout->setSpacing(8);

    auto *logo = new QLabel(brandBox);
    logo->setFixedSize(96, 96);
    logo->setAlignment(Qt::AlignCenter);
    logo->setObjectName("Logo");

    QPixmap pm(":/vision_sight.png");
    if (!pm.isNull())
        logo->setPixmap(pm.scaled(72, 72, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        logo->setText("VS");

    auto *brand1 = new QLabel(tr("VISION SIGHT"), brandBox);
    brand1->setObjectName("BrandTitle");
    brand1->setAlignment(Qt::AlignHCenter);

    brandBoxLayout->addWidget(logo, 0, Qt::AlignHCenter);
    brandBoxLayout->addWidget(brand1, 0, Qt::AlignHCenter);

    sideLayout->addWidget(brandBox);
    sideLayout->addSpacing(12);

    const QStringList items = {tr("Gestion Des employés"), tr("Gestion Des Navires"), tr("Gestion Des Quais"), tr("Gestion Des Captures"), tr("Stockage Frigorifique"), tr("Gestion Des Ventes")};
    for (int i = 0; i < items.size(); ++i)
    {
        auto *btn = new QPushButton(items.at(i), sidebar);
        btn->setObjectName("SideButton");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(42);
        btn->setCheckable(true);
        btn->setChecked(false);
        sideLayout->addWidget(btn);

        m_sidebarButtons.push_back(btn);

        connect(btn, &QPushButton::clicked, this, [this, i]() {
            if (i >= 0 && i < m_sidebarButtons.size())
                setActiveSidebarButton(m_sidebarButtons.at(i));

            switch (i)
            {
            case 1:
                emit requestShowNavires();
                break;
            case 2:
                emit requestShowQuais();
                break;
            case 3:
                emit requestShowCaptures();
                break;
            case 5:
                emit requestShowTransactions();
                break;
            default:
                break;
            }
        });
    }

    // Default active entry: Stockage Frigorifique
    if (m_sidebarButtons.size() >= 5)
        setActiveSidebarButton(m_sidebarButtons.at(4));
    sideLayout->addStretch();

    rootLayout->addWidget(sidebar);

    // Main scrollable content
    auto *scroll = new QScrollArea(central);
    scroll->setObjectName("MainScroll");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);

    auto *page = new QWidget(scroll);
    page->setObjectName("Page");
    scroll->setWidget(page);

    auto *pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(24, 18, 24, 24);
    pageLayout->setSpacing(14);

    // Header row
    auto *headerRow = new QHBoxLayout();
    auto *titleCol = new QVBoxLayout();
    titleCol->setSpacing(2);

    auto *h1 = makeTitle(tr("VISION SIGHT"), 28, true, page);
    h1->setObjectName("HeaderH1");

    titleCol->addWidget(h1);

    headerRow->addLayout(titleCol);
    headerRow->addStretch();

    auto *menuBtn = new QToolButton(page);
    menuBtn->setText("≡");
    menuBtn->setObjectName("MenuButton");
    menuBtn->setFixedSize(36, 36);
    headerRow->addWidget(menuBtn);

    pageLayout->addLayout(headerRow);

    // Middle row: Chambre card + Stats card
    auto *midRow = new QHBoxLayout();
    midRow->setSpacing(12);

    // Chambre card
    auto *chCard = makeCard(page, "Card");
    auto *chLayout = new QVBoxLayout(chCard);
    chLayout->setContentsMargins(14, 12, 14, 12);
    chLayout->setSpacing(10);

    auto *chTitle = new QLabel(tr("Chambres froides"), chCard);
    chTitle->setObjectName("SectionTitle");
    chLayout->addWidget(chTitle);

    auto *chBtnMainLayout = new QVBoxLayout();
    chBtnMainLayout->setSpacing(8);
    chBtnMainLayout->setContentsMargins(0, 0, 0, 0);

    auto *chBtnRow = new QHBoxLayout();
    chBtnRow->setSpacing(10);

    m_btnAddChambre = new QPushButton(tr("Ajouter"), chCard);
    m_btnAddChambre->setObjectName("PrimaryButton");
    m_btnAddChambre->setCursor(Qt::PointingHandCursor);

    m_btnUpdateChambre = new QPushButton(tr("Modifier"), chCard);
    m_btnUpdateChambre->setObjectName("SecondaryButton");
    m_btnUpdateChambre->setCursor(Qt::PointingHandCursor);

    m_btnDeleteChambre = new QPushButton(tr("Supprimer"), chCard);
    m_btnDeleteChambre->setObjectName("DangerButton");
    m_btnDeleteChambre->setCursor(Qt::PointingHandCursor);

    m_btnClearChambre = new QPushButton(tr("Réinitialiser"), chCard);
    m_btnClearChambre->setObjectName("GhostButton");
    m_btnClearChambre->setCursor(Qt::PointingHandCursor);

    chBtnRow->addWidget(m_btnAddChambre);
    chBtnRow->addWidget(m_btnUpdateChambre);
    chBtnRow->addWidget(m_btnDeleteChambre);
    chBtnRow->addWidget(m_btnClearChambre);
    chBtnRow->addStretch();

    chBtnMainLayout->addLayout(chBtnRow);

    auto *chCheckRow = new QHBoxLayout();
    chCheckRow->setSpacing(0);
    chCheckRow->setContentsMargins(0, 0, 0, 0);

    m_checkSelectAllChambre = new QCheckBox(tr("Tout sélectionner"), chCard);
    m_checkSelectAllChambre->setObjectName("SelectAllCheck");
    m_checkSelectAllChambre->setCursor(Qt::PointingHandCursor);
    m_checkSelectAllChambre->setMaximumWidth(140);

    chCheckRow->addWidget(m_checkSelectAllChambre);
    chCheckRow->addStretch();

    chBtnMainLayout->addLayout(chCheckRow);
    chLayout->addLayout(chBtnMainLayout);

    // Carte Recherche / Filtres
    auto *chFilterCard = makeCard(chCard, "CardSmall");
    auto *chFilterCardLayout = new QVBoxLayout(chFilterCard);
    chFilterCardLayout->setContentsMargins(16, 14, 16, 14);
    chFilterCardLayout->setSpacing(12);

    auto *filterTitle = new QLabel(tr("Recherche / Filtres"), chFilterCard);
    filterTitle->setObjectName("SectionTitle");
    chFilterCardLayout->addWidget(filterTitle);

    auto *chFilterGrid = new QGridLayout();
    chFilterGrid->setHorizontalSpacing(14);
    chFilterGrid->setVerticalSpacing(10);

    // Row 1
    auto *lblChambreId = new QLabel(tr("Recherche chambre"), chFilterCard);
    lblChambreId->setObjectName("Muted");
    m_searchChambreId = new QLineEdit(chFilterCard);
    m_searchChambreId->setObjectName("Input");
    m_searchChambreId->setPlaceholderText(tr("Recherche chambre"));

    auto *lblEmplacement = new QLabel(tr("Recherche emplacement"), chFilterCard);
    lblEmplacement->setObjectName("Muted");
    m_searchChambreEmpl = new QLineEdit(chFilterCard);
    m_searchChambreEmpl->setObjectName("Input");
    m_searchChambreEmpl->setPlaceholderText(tr("Recherche emplacement"));

    chFilterGrid->addWidget(lblChambreId, 0, 0);
    chFilterGrid->addWidget(m_searchChambreId, 1, 0);
    chFilterGrid->addWidget(lblEmplacement, 0, 1);
    chFilterGrid->addWidget(m_searchChambreEmpl, 1, 1);

    chFilterCardLayout->addLayout(chFilterGrid);
    chLayout->addWidget(chFilterCard);

    m_tableChambres = new QTableView(chCard);
    m_tableChambres->setObjectName("Table");
    m_tableChambres->setModel(m_chambreProxy);
    m_tableChambres->setSortingEnabled(true);
    m_tableChambres->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableChambres->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tableChambres->setShowGrid(false);
    m_tableChambres->verticalHeader()->setVisible(false);
    m_tableChambres->horizontalHeader()->setStretchLastSection(true);
    m_tableChambres->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    // Ensure rows are visible: set a sensible default row height,
    // enable alternating row colors, size columns to contents and
    // make the view expand vertically inside its layout.
    m_tableChambres->verticalHeader()->setDefaultSectionSize(28);
    m_tableChambres->setAlternatingRowColors(true);
    m_tableChambres->setMinimumHeight(180);
    m_tableChambres->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableChambres->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableChambres->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableChambres->horizontalHeader()->setSectionResizeMode(m_tableChambres->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    chLayout->addWidget(m_tableChambres, 1);

    midRow->addWidget(chCard, 2);

    // Tri et Export row
    auto *triExportRow = new QHBoxLayout();
    triExportRow->setSpacing(14);

    // Tri card
    auto *triCard = makeCard(page, "CardSmall");
    auto *triLayout = new QVBoxLayout(triCard);
    triLayout->setContentsMargins(14, 12, 14, 12);
    triLayout->setSpacing(10);

    auto *triTitle = new QLabel(tr("Tri"), triCard);
    triTitle->setObjectName("SectionTitle");
    triLayout->addWidget(triTitle);

    auto *triSubtitle = new QLabel(tr("Tri par capacité chambre"), triCard);
    triSubtitle->setObjectName("Muted");
    triLayout->addWidget(triSubtitle);

    m_triChambre = new QComboBox(triCard);
    m_triChambre->setObjectName("Input");
    m_triChambre->addItem(tr("-- Sélectionner --"));
    m_triChambre->addItem(tr("Par capacité"));
    m_triChambre->addItem(tr("Par température"));
    triLayout->addWidget(m_triChambre);
    triLayout->addStretch();

    triExportRow->addWidget(triCard, 1);

    // Export card
    auto *exportCard = makeCard(page, "CardSmall");
    auto *exportLayout = new QVBoxLayout(exportCard);
    exportLayout->setContentsMargins(14, 12, 14, 12);
    exportLayout->setSpacing(10);

    auto *exportTitle = new QLabel(tr("Export"), exportCard);
    exportTitle->setObjectName("SectionTitle");
    exportLayout->addWidget(exportTitle);

    auto *exportSubtitle = new QLabel(tr("Rapport chambres froides"), exportCard);
    exportSubtitle->setObjectName("Muted");
    exportLayout->addWidget(exportSubtitle);

    m_btnExportChambrePdf = new QPushButton(tr("Exporter PDF"), exportCard);
    m_btnExportChambrePdf->setObjectName("AltPrimaryButton");
    m_btnExportChambrePdf->setCursor(Qt::PointingHandCursor);
    m_btnExportChambrePdf->setMinimumHeight(36);
    exportLayout->addWidget(m_btnExportChambrePdf);
    exportLayout->addStretch();

    triExportRow->addWidget(exportCard, 1);

    pageLayout->addLayout(triExportRow);

    // Stats card
    auto *statsCard = makeCard(page, "Card");
    auto *stLayout = new QVBoxLayout(statsCard);
    stLayout->setContentsMargins(14, 12, 14, 12);
    stLayout->setSpacing(10);

    auto *stTitle = new QLabel(tr("Statistiques"), statsCard);
    stTitle->setObjectName("SectionTitle");
    stLayout->addWidget(stTitle);

    // Period filter row
    auto *periodFilterRow = new QHBoxLayout();
    periodFilterRow->setSpacing(10);
    
    auto *periodLabel = new QLabel(tr("Période:"), statsCard);
    periodLabel->setObjectName("Muted");
    periodFilterRow->addWidget(periodLabel);
    
    m_statsPeriodFilter = new QComboBox(statsCard);
    m_statsPeriodFilter->setObjectName("Input");
    m_statsPeriodFilter->addItem(tr("Aujourd'hui"), "day");
    m_statsPeriodFilter->addItem(tr("Cette semaine"), "week");
    m_statsPeriodFilter->addItem(tr("Dernières 24 heures"), "hour");
    m_statsPeriodFilter->addItem(tr("Toute la période"), "all");
    m_statsPeriodFilter->setCurrentIndex(3); // Default: all
    periodFilterRow->addWidget(m_statsPeriodFilter);

    periodFilterRow->addStretch();

    m_btnOptimizeStorage = new QPushButton(tr("Optimiser"), statsCard);
    m_btnOptimizeStorage->setObjectName("NeonButton");
    m_btnOptimizeStorage->setCursor(Qt::PointingHandCursor);
    m_btnOptimizeStorage->setMinimumHeight(32);
    m_btnOptimizeStorage->setIcon(QIcon::fromTheme("magic-wand"));
    periodFilterRow->addWidget(m_btnOptimizeStorage);
    
    stLayout->addLayout(periodFilterRow);

    // Four circles in a 2x2 grid
    auto *circlesGrid = new QGridLayout();
    circlesGrid->setHorizontalSpacing(20);
    circlesGrid->setVerticalSpacing(20);

    // Circle 1: Taux d'occupation
    m_occupancyCircle = new StatCircleWidget(statsCard);
    m_occupancyCircle->setMinimumSize(140, 140);
    m_occupancyCircle->setMaximumSize(140, 140);
    m_occupancyCircle->setLabel(tr("Taux d'occupation"));
    circlesGrid->addWidget(m_occupancyCircle, 0, 0, Qt::AlignCenter);

    // Circle 2: Volumes stockés
    m_volumeCircle = new StatCircleWidget(statsCard);
    m_volumeCircle->setMinimumSize(140, 140);
    m_volumeCircle->setMaximumSize(140, 140);
    m_volumeCircle->setLabel(tr("Volumes stockés"));
    circlesGrid->addWidget(m_volumeCircle, 0, 1, Qt::AlignCenter);

    // Circle 3: Durée moyenne
    m_durationCircle = new StatCircleWidget(statsCard);
    m_durationCircle->setMinimumSize(140, 140);
    m_durationCircle->setMaximumSize(140, 140);
    m_durationCircle->setLabel(tr("Durée moyenne (jours)"));
    circlesGrid->addWidget(m_durationCircle, 1, 0, Qt::AlignCenter);

    // Circle 4: Efficacité énergétique
    m_energyEfficiencyCircle = new StatCircleWidget(statsCard);
    m_energyEfficiencyCircle->setMinimumSize(140, 140);
    m_energyEfficiencyCircle->setMaximumSize(140, 140);
    m_energyEfficiencyCircle->setLabel(tr("Efficacite energetique"));
    m_energyEfficiencyCircle->setColor(QColor(0, 149, 255));
    circlesGrid->addWidget(m_energyEfficiencyCircle, 1, 1, Qt::AlignCenter);

    stLayout->addLayout(circlesGrid);

    auto *stHint = new QLabel(tr("Mise à jour automatique"), statsCard);
    stHint->setObjectName("AccentText");
    stLayout->addWidget(stHint);
    stLayout->addStretch();

    midRow->addWidget(statsCard, 1);

    pageLayout->addLayout(midRow);

    // Affectation Card - Middle section
    auto *affectCard = makeCard(page, "Card");
    auto *affectLayout = new QVBoxLayout(affectCard);
    affectLayout->setContentsMargins(14, 12, 14, 12);
    affectLayout->setSpacing(10);

    auto *affectTitle = new QLabel(tr("Affectation de lots aux chambres"), affectCard);
    affectTitle->setObjectName("SectionTitle");
    affectLayout->addWidget(affectTitle);

    auto *affectContentLayout = new QHBoxLayout();
    affectContentLayout->setSpacing(10);

    // Left side: Unassigned lots
    auto *unassignedLayout = new QVBoxLayout();
    unassignedLayout->setSpacing(8);

    auto *unassignedLabel = new QLabel(tr("Lots disponibles"), affectCard);
    unassignedLabel->setObjectName("Muted");
    unassignedLayout->addWidget(unassignedLabel);

    m_tableUnassignedLots = new QTableView(affectCard);
    m_tableUnassignedLots->setObjectName("Table");
    m_tableUnassignedLots->setModel(m_unassignedLotsProxy);
    m_tableUnassignedLots->setSortingEnabled(true);
    m_tableUnassignedLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableUnassignedLots->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableUnassignedLots->setShowGrid(false);
    m_tableUnassignedLots->verticalHeader()->setVisible(false);
    m_tableUnassignedLots->horizontalHeader()->setStretchLastSection(true);
    m_tableUnassignedLots->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_tableUnassignedLots->verticalHeader()->setDefaultSectionSize(28);
    m_tableUnassignedLots->setAlternatingRowColors(true);
    m_tableUnassignedLots->setMinimumHeight(150);
    m_tableUnassignedLots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableUnassignedLots->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableUnassignedLots->setColumnHidden(StockageLotModel::ColIdChambre, true);
    m_tableUnassignedLots->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableUnassignedLots->horizontalHeader()->setSectionResizeMode(m_tableUnassignedLots->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    unassignedLayout->addWidget(m_tableUnassignedLots);

    affectContentLayout->addLayout(unassignedLayout, 1);

    // Middle: Control buttons
    auto *affectBtnLayout = new QVBoxLayout();
    affectBtnLayout->setSpacing(8);
    affectBtnLayout->addStretch();

    auto *chambreLabel = new QLabel(tr("Chambre:"), affectCard);
    chambreLabel->setObjectName("Muted");
    affectBtnLayout->addWidget(chambreLabel);

    m_assignChambreCombo = new QComboBox(affectCard);
    m_assignChambreCombo->setObjectName("Input");
    affectBtnLayout->addWidget(m_assignChambreCombo);

    m_btnAssignLot = new QPushButton(tr("Affecter →"), affectCard);
    m_btnAssignLot->setObjectName("PrimaryButton");
    m_btnAssignLot->setCursor(Qt::PointingHandCursor);
    m_btnAssignLot->setMinimumHeight(36);
    affectBtnLayout->addWidget(m_btnAssignLot);

    m_btnUnassignLot = new QPushButton(tr("← Désaffecter"), affectCard);
    m_btnUnassignLot->setObjectName("SecondaryButton");
    m_btnUnassignLot->setCursor(Qt::PointingHandCursor);
    m_btnUnassignLot->setMinimumHeight(36);
    affectBtnLayout->addWidget(m_btnUnassignLot);

    affectBtnLayout->addStretch();

    affectContentLayout->addLayout(affectBtnLayout, 0);

    // Right side: Assignments
    auto *assignmentsLayout = new QVBoxLayout();
    assignmentsLayout->setSpacing(8);

    auto *assignmentsLabel = new QLabel(tr("Affectations actuelles"), affectCard);
    assignmentsLabel->setObjectName("Muted");
    assignmentsLayout->addWidget(assignmentsLabel);

    m_tableAssignments = new QTableView(affectCard);
    m_tableAssignments->setObjectName("Table");
    m_tableAssignments->setModel(m_assignedLotsProxy);
    m_tableAssignments->setSortingEnabled(true);
    m_tableAssignments->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableAssignments->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableAssignments->setShowGrid(false);
    m_tableAssignments->verticalHeader()->setVisible(false);
    m_tableAssignments->horizontalHeader()->setStretchLastSection(true);
    m_tableAssignments->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_tableAssignments->verticalHeader()->setDefaultSectionSize(28);
    m_tableAssignments->setAlternatingRowColors(true);
    m_tableAssignments->setMinimumHeight(150);
    m_tableAssignments->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableAssignments->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableAssignments->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableAssignments->horizontalHeader()->setSectionResizeMode(m_tableAssignments->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    assignmentsLayout->addWidget(m_tableAssignments);

    affectContentLayout->addLayout(assignmentsLayout, 1);

    affectLayout->addLayout(affectContentLayout);

    pageLayout->addWidget(affectCard);
    auto *lotCard = makeCard(page, "Card");
    auto *lotLayout = new QVBoxLayout(lotCard);
    lotLayout->setContentsMargins(14, 12, 14, 12);
    lotLayout->setSpacing(10);

    auto *lotTitle = new QLabel(tr("Lots stockés"), lotCard);
    lotTitle->setObjectName("SectionTitle");
    lotLayout->addWidget(lotTitle);

    auto *lotBtnMainLayout = new QVBoxLayout();
    lotBtnMainLayout->setSpacing(8);
    lotBtnMainLayout->setContentsMargins(0, 0, 0, 0);

    auto *lotBtnRow = new QHBoxLayout();
    lotBtnRow->setSpacing(10);

    m_btnAddLot = new QPushButton(tr("Ajouter"), lotCard);
    m_btnAddLot->setObjectName("PrimaryButton");
    m_btnAddLot->setCursor(Qt::PointingHandCursor);

    m_btnUpdateLot = new QPushButton(tr("Modifier"), lotCard);
    m_btnUpdateLot->setObjectName("SecondaryButton");
    m_btnUpdateLot->setCursor(Qt::PointingHandCursor);

    m_btnDeleteLot = new QPushButton(tr("Supprimer"), lotCard);
    m_btnDeleteLot->setObjectName("DangerButton");
    m_btnDeleteLot->setCursor(Qt::PointingHandCursor);

    m_btnClearLot = new QPushButton(tr("Réinitialiser"), lotCard);
    m_btnClearLot->setObjectName("GhostButton");
    m_btnClearLot->setCursor(Qt::PointingHandCursor);

    lotBtnRow->addWidget(m_btnAddLot);
    lotBtnRow->addWidget(m_btnUpdateLot);
    lotBtnRow->addWidget(m_btnDeleteLot);
    lotBtnRow->addWidget(m_btnClearLot);
    lotBtnRow->addStretch();

    lotBtnMainLayout->addLayout(lotBtnRow);

    auto *lotCheckRow = new QHBoxLayout();
    lotCheckRow->setSpacing(0);
    lotCheckRow->setContentsMargins(0, 0, 0, 0);

    m_checkSelectAllLot = new QCheckBox(tr("Tout sélectionner"), lotCard);
    m_checkSelectAllLot->setObjectName("SelectAllCheck");
    m_checkSelectAllLot->setCursor(Qt::PointingHandCursor);
    m_checkSelectAllLot->setMaximumWidth(140);

    lotCheckRow->addWidget(m_checkSelectAllLot);
    lotCheckRow->addStretch();

    lotBtnMainLayout->addLayout(lotCheckRow);
    lotLayout->addLayout(lotBtnMainLayout);

    // Carte Recherche / Filtres
    auto *lotFilterCard = makeCard(lotCard, "CardSmall");
    auto *lotFilterCardLayout = new QVBoxLayout(lotFilterCard);
    lotFilterCardLayout->setContentsMargins(16, 14, 16, 14);
    lotFilterCardLayout->setSpacing(12);

    auto *lotFilterTitle = new QLabel(tr("Recherche / Filtres"), lotFilterCard);
    lotFilterTitle->setObjectName("SectionTitle");
    lotFilterCardLayout->addWidget(lotFilterTitle);

    auto *lotFilterGrid = new QGridLayout();
    lotFilterGrid->setHorizontalSpacing(14);
    lotFilterGrid->setVerticalSpacing(10);

    // Row 1
    auto *lblLotId = new QLabel(tr("Recherche lot"), lotFilterCard);
    lblLotId->setObjectName("Muted");
    m_searchLot = new QLineEdit(lotFilterCard);
    m_searchLot->setObjectName("Input");
    m_searchLot->setPlaceholderText(tr("Recherche lot"));

    auto *lblLotChambre = new QLabel(tr("Recherche chambre"), lotFilterCard);
    lblLotChambre->setObjectName("Muted");
    m_searchLotChambre = new QLineEdit(lotFilterCard);
    m_searchLotChambre->setObjectName("Input");
    m_searchLotChambre->setPlaceholderText(tr("Recherche chambre"));

    // Row 2
    auto *lblLotEspece = new QLabel(tr("Recherche espèce"), lotFilterCard);
    lblLotEspece->setObjectName("Muted");
    m_searchLotEspece = new QLineEdit(lotFilterCard);
    m_searchLotEspece->setObjectName("Input");
    m_searchLotEspece->setPlaceholderText(tr("Recherche espèce"));

    m_btnExportPdf = new QPushButton(tr("Exporter PDF"), lotFilterCard);
    m_btnExportPdf->setObjectName("AltPrimaryButton");
    m_btnExportPdf->setCursor(Qt::PointingHandCursor);
    m_btnExportPdf->setMinimumHeight(36);

    lotFilterGrid->addWidget(lblLotId, 0, 0);
    lotFilterGrid->addWidget(m_searchLot, 1, 0);
    lotFilterGrid->addWidget(lblLotChambre, 0, 1);
    lotFilterGrid->addWidget(m_searchLotChambre, 1, 1);
    lotFilterGrid->addWidget(lblLotEspece, 2, 0);
    lotFilterGrid->addWidget(m_searchLotEspece, 3, 0);
    lotFilterGrid->addWidget(m_btnExportPdf, 3, 1, Qt::AlignBottom);

    lotFilterCardLayout->addLayout(lotFilterGrid);
    lotLayout->addWidget(lotFilterCard);

    m_tableLots = new QTableView(lotCard);
    m_tableLots->setObjectName("Table");
    m_tableLots->setModel(m_lotProxy);
    m_tableLots->setSortingEnabled(true);
    m_tableLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableLots->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_tableLots->setShowGrid(false);
    m_tableLots->verticalHeader()->setVisible(false);
    m_tableLots->horizontalHeader()->setStretchLastSection(true);
    // Ensure rows are visible and the view expands vertically.
    m_tableLots->verticalHeader()->setDefaultSectionSize(28);
    m_tableLots->setAlternatingRowColors(true);
    m_tableLots->setMinimumHeight(200);
    m_tableLots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableLots->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableLots->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_tableLots->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_tableLots->horizontalHeader()->setSectionResizeMode(m_tableLots->horizontalHeader()->count() - 1, QHeaderView::Stretch);

    lotLayout->addWidget(m_tableLots, 1);

    pageLayout->addWidget(lotCard);
    pageLayout->addStretch();

    rootLayout->addWidget(scroll);
}

void MainWindow::applyTheme()
{
    const QString qss = R"(
        QMainWindow {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff,
                stop:1 #ADD8E6
            );
        }

        QDialog {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #ffffff,
                stop:1 #ADD8E6
            );
        }

        QScrollArea#MainScroll,
        QScrollArea#MainScroll QWidget#qt_scrollarea_viewport {
            background: transparent;
        }

        QWidget#Page { background: transparent; }

        QFrame#Sidebar {
            background: #101c2c;
            border-top-right-radius: 16px;
            border-bottom-right-radius: 16px;
        }
        QLabel#Logo { background: #ffffff; color: #132437; border: 2px solid #ffffff; border-radius: 48px; padding: 0px; }
        QLabel#BrandTitle { color: #e6eef6; font-size: 16px; font-weight: 800; }


        QPushButton#SideButton {
            text-align: left;
            padding-left: 12px;
            color: #e6eef6;
            background: transparent;
            border: 1px solid transparent;
            border-radius: 10px;
            font-size: 16px;
            font-weight: 600;
        }
        QPushButton#SideButton:hover {
            background: #18304a;
            border-color: #23374e;
        }
        QPushButton#SideButton:checked {
            color: #39c0fa;
            background: rgba(57,192,250,22);
            border-color: rgba(57,192,250,90);
        }
        QPushButton#SideButton:pressed {
            background: #1a3552;
        }

        QLabel#HeaderH1 { color: #132437; }
        QLabel#HeaderMuted { color: #132437; }

        QLabel#Muted { color: #9bb0c3; font-size: 12px; }
        QLabel#SectionTitle { color: #39c0fa; font-size: 16px; font-weight: 700; }
        QLabel#AccentText { color: #39c0fa; }
        QLabel#StatValue { color: #e6eef6; }

        QLabel#ChartPlaceholder {
            background: #101c2c;
            border: 1px dashed #23374e;
            border-radius: 12px;
            color: #9bb0c3;
        }

        QToolButton#MenuButton {
            color: #e6eef6;
            background: #132437;
            border: 1px solid #23374e;
            border-radius: 10px;
            font-size: 18px;
        }
        QToolButton#MenuButton:hover { background: #18304a; }

        QFrame#Card, QFrame#CardSmall {
            background: #132437;
            border: 1px solid #23374e;
            border-radius: 16px;
        }
        QFrame#CardSmall { border-radius: 12px; }

        QLineEdit#Input, QComboBox#Input, QDateTimeEdit#Input, QDoubleSpinBox#Input, QSpinBox#Input {
            background: #101c2c;
            border: 1px solid #23374e;
            border-radius: 8px;
            min-height: 22px;
            font-size: 11px;
            padding: 4px 10px;
            color: #e6eef6;
            selection-background-color: #1e8de0;
        }
        QComboBox#Input::drop-down { border: 0px; width: 20px; }
        QDateTimeEdit#Input { padding-right: 28px; }
        QDoubleSpinBox#Input { padding-right: 30px; }
        QSpinBox#Input { padding-right: 30px; }

        QCheckBox#Check { color: #e6eef6; }

        QCheckBox#SelectAllCheck {
            background: #132437;
            color: #e6eef6;
            border: 1px solid #23374e;
            border-radius: 8px;
            padding: 8px 12px;
            font-weight: 600;
            font-size: 13px;
            spacing: 8px;
        }
        QCheckBox#SelectAllCheck::indicator {
            width: 18px;
            height: 18px;
            border: 2px solid #39c0fa;
            border-radius: 4px;
            background: #101c2c;
        }
        QCheckBox#SelectAllCheck::indicator:checked {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, 
                                        stop:0 #39c0fa, stop:1 #2a99ec);
            border-color: #39c0fa;
        }
        QCheckBox#SelectAllCheck:hover {
            background: #18304a;
            border-color: #2b4561;
        }

        QPushButton#PrimaryButton {
            background: #23374e;
            color: #e6eef6;
            border: 1px solid #2b4561;
            border-radius: 10px;
            padding: 9px 14px;
            font-weight: 700;
        }
        QPushButton#PrimaryButton:hover { background: #2b4561; }

        QPushButton#AltPrimaryButton {
            background: #1e8de0;
            color: white;
            border: 0px;
            border-radius: 10px;
            padding: 9px 14px;
            font-weight: 700;
        }
        QPushButton#AltPrimaryButton:hover { background: #2a99ec; }

        QPushButton#NeonButton {
            background: #0095ff;
            color: #101924;
            border: 0px;
            border-radius: 10px;
            padding: 6px 12px;
            font-weight: 700;
        }
        QPushButton#NeonButton:hover { background: #2aa7ff; }

        QPushButton#SecondaryButton {
            background: #23374e;
            color: #e6eef6;
            border: 1px solid #2b4561;
            border-radius: 10px;
            padding: 9px 14px;
        }
        QPushButton#SecondaryButton:hover { background: #2b4561; }

        QPushButton#DangerButton {
            background: #39c0fa;
            color: #0c1a29;
            border: 0px;
            border-radius: 10px;
            padding: 9px 14px;
            font-weight: 700;
        }
        QPushButton#DangerButton:hover { background: #56cdfc; }

        QPushButton#GhostButton {
            background: transparent;
            color: #e6eef6;
            border: 1px solid #23374e;
            border-radius: 10px;
            padding: 9px 14px;
        }
        QPushButton#GhostButton:hover { background: #18304a; }

        QTableView#Table {
            background: transparent;
            color: #e6eef6;
            border: 0px;
            selection-background-color: #1e3c5c;
            selection-color: #e6eef6;
        }
        QHeaderView::section {
            background: #101c2c;
            color: #9bb0c3;
            border: 0px;
            padding: 8px 10px;
            font-weight: 700;
        }
        QTableView::item {
            padding: 6px 10px;
            border-radius: 6px;
        }
    )";

    qApp->setStyleSheet(qss);
}
void MainWindow::setActiveSidebarButton(QPushButton *activeBtn)
{
    for (auto *btn : std::as_const(m_sidebarButtons))
    {
        const bool active = (btn == activeBtn);
        btn->setChecked(active);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
}

void MainWindow::wireSignals()
{
    // Chambre
    connect(m_btnAddChambre, &QPushButton::clicked, this, &MainWindow::onShowAddChambreDialog);
    connect(m_btnUpdateChambre, &QPushButton::clicked, this, &MainWindow::onShowUpdateChambreDialog);
    connect(m_btnDeleteChambre, &QPushButton::clicked, this, &MainWindow::onDeleteChambre);
    connect(m_checkSelectAllChambre, &QCheckBox::toggled, this, &MainWindow::onSelectAllChambres);

    connect(m_searchChambreId, &QLineEdit::textChanged, m_chambreProxy, &ChambreFroideSortFilterProxyModel::setSearchIdChambre);
    connect(m_searchChambreEmpl, &QLineEdit::textChanged, m_chambreProxy, &ChambreFroideSortFilterProxyModel::setSearchEmplacement);
    
    // Sync checkbox state with table selection
    connect(m_tableChambres->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        m_checkSelectAllChambre->blockSignals(true);
        const int totalRows = m_chambreProxy->rowCount();
        const int selectedRows = m_tableChambres->selectionModel()->selectedRows().count();
        m_checkSelectAllChambre->setChecked(totalRows > 0 && selectedRows == totalRows);
        m_checkSelectAllChambre->blockSignals(false);
    });

    connect(m_triChambre, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTriChambreChanged);
    connect(m_btnExportChambrePdf, &QPushButton::clicked, this, &MainWindow::onExportChambreFacturePdf);

    if (m_btnOptimizeStorage)
        connect(m_btnOptimizeStorage, &QPushButton::clicked, this, &MainWindow::on_btnOptimizeStorage_clicked);

    // Lots
    connect(m_btnAddLot, &QPushButton::clicked, this, &MainWindow::onShowAddLotDialog);
    connect(m_btnUpdateLot, &QPushButton::clicked, this, &MainWindow::onShowUpdateLotDialog);
    connect(m_btnDeleteLot, &QPushButton::clicked, this, &MainWindow::onDeleteLot);
    connect(m_checkSelectAllLot, &QCheckBox::toggled, this, &MainWindow::onSelectAllLots);
    connect(m_btnExportPdf, &QPushButton::clicked, this, &MainWindow::onExportLotsPdf);

    connect(m_searchLot, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchLot);
    connect(m_searchLotChambre, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchChambre);
    connect(m_searchLotEspece, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchEspece);
    
    // Sync checkbox state with table selection
    connect(m_tableLots->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {
        m_checkSelectAllLot->blockSignals(true);
        const int totalRows = m_lotProxy->rowCount();
        const int selectedRows = m_tableLots->selectionModel()->selectedRows().count();
        m_checkSelectAllLot->setChecked(totalRows > 0 && selectedRows == totalRows);
        m_checkSelectAllLot->blockSignals(false);
    });

    // Refresh stats automatically
    auto refreshAll = [this]() {
        refreshChambreCombo();
        refreshStats();
        if (m_tableChambres) m_tableChambres->resizeColumnsToContents();
        if (m_tableLots) m_tableLots->resizeColumnsToContents();
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
        if (m_tableUnassignedLots) m_tableUnassignedLots->resizeColumnsToContents();
        if (m_tableAssignments) m_tableAssignments->resizeColumnsToContents();
    };
    
    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, refreshAffectationFilters);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, refreshAffectationFilters);

    // Affectation
    connect(m_btnAssignLot, &QPushButton::clicked, this, &MainWindow::onAssignLotToChambre);
    connect(m_btnUnassignLot, &QPushButton::clicked, this, &MainWindow::onUnassignLot);
}

int MainWindow::selectedSourceRow(QTableView *view, const QSortFilterProxyModel *proxy) const
{
    if (!view || !proxy)
        return -1;

    const auto selected = view->selectionModel()->selectedRows();
    if (!selected.isEmpty())
        return proxy->mapToSource(selected.first()).row();

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
    for (const auto &l : m_lotModel->items())
        sum += l.quantite;
    return sum;
}

double MainWindow::averageDurationDays() const
{
    const auto &lots = m_lotModel->items();
    if (lots.isEmpty())
        return 0.0;

    double sumDays = 0.0;
    for (const auto &l : lots)
        sumDays += double(l.dureeSecondes()) / double(24 * 3600);
    return sumDays / double(lots.size());
}

void MainWindow::refreshChambreCombo()
{
    if (!m_assignChambreCombo)
        return;

    const QString previous = m_assignChambreCombo->currentText();

    m_assignChambreCombo->blockSignals(true);
    m_assignChambreCombo->clear();
    m_assignChambreCombo->addItem(tr("-- Sélectionner --"));
    for (const auto &c : m_chambreModel->items())
        m_assignChambreCombo->addItem(c.idChambre);

    const int idx = m_assignChambreCombo->findText(previous);
    if (idx >= 0)
        m_assignChambreCombo->setCurrentIndex(idx);

    m_assignChambreCombo->blockSignals(false);
}

void MainWindow::refreshStats()
{
    const QLocale loc;

    const int cap = totalCapacity();
    
    // Get selected period from combo
    QString periodType = "all";
    if (m_statsPeriodFilter)
    {
        periodType = m_statsPeriodFilter->currentData().toString();
    }
    
    // Calculate date range based on period
    QDateTime startDate;
    QDateTime endDate = QDateTime::currentDateTime();
    
    if (periodType == "day")
    {
        // Aujourd'hui: depuis minuit
        startDate = QDateTime(QDate::currentDate(), QTime(0, 0, 0));
    }
    else if (periodType == "week")
    {
        // Cette semaine: depuis lundi
        QDate today = QDate::currentDate();
        int dayOfWeek = today.dayOfWeek(); // 1 = Monday, 7 = Sunday
        startDate = QDateTime(today.addDays(1 - dayOfWeek), QTime(0, 0, 0));
    }
    else if (periodType == "hour")
    {
        // Dernières 24 heures
        startDate = endDate.addSecs(-24 * 3600);
    }
    // else "all" - no filtering
    
    // Filter lots by period
    int used = 0;
    double sumDays = 0.0;
    int lotCount = 0;
    
    for (const auto &l : m_lotModel->items())
    {
        bool include = (periodType == "all");
        
        if (!include && l.dateEntree.isValid())
        {
            // Include lot if it was active during the period
            // (entered before end and either not exited or exited after start)
            if (l.dateEntree <= endDate)
            {
                if (!l.isSorti() || l.dateSortie >= startDate)
                {
                    // Check if lot entered during the period or was already active
                    if (l.dateEntree >= startDate || !l.isSorti())
                    {
                        include = true;
                    }
                }
            }
        }
        
        if (include)
        {
            used += l.quantite;
            sumDays += double(l.dureeSecondes()) / double(24 * 3600);
            lotCount++;
        }
    }
    
    const double occ = (cap > 0) ? (double(used) * 100.0 / double(cap)) : 0.0;
    const double avgDays = (lotCount > 0) ? (sumDays / double(lotCount)) : 0.0;

    // Update circle 1: Taux d'occupation (0-100%)
    if (m_occupancyCircle)
    {
        m_occupancyCircle->setValue(occ / 100.0);
        m_occupancyCircle->setLabel(tr("Taux d'occupation") + "\n" + loc.toString(occ, 'f', 1) + "%");
    }

    // Update circle 2: Volumes stockés (as percentage of total capacity)
    if (m_volumeCircle)
    {
        const double volumePercent = (cap > 0) ? (double(used) / double(cap)) : 0.0;
        m_volumeCircle->setValue(volumePercent);
        m_volumeCircle->setLabel(tr("Volumes stockés") + "\n" + loc.toString(used));
    }

    // Update circle 3: Durée moyenne (normalize to 0-1, max 30 days)
    if (m_durationCircle)
    {
        const double maxDays = 30.0;
        const double durationPercent = qMin(1.0, avgDays / maxDays);
        m_durationCircle->setValue(durationPercent);
        m_durationCircle->setLabel(tr("Durée moyenne (jours)") + "\n" + loc.toString(avgDays, 'f', 1));
    }

    // Keep old labels for compatibility
    if (m_statOccupancy)
        m_statOccupancy->setText(loc.toString(occ, 'f', 1) + "%");
    if (m_statTotalVolume)
        m_statTotalVolume->setText(loc.toString(used));
    if (m_statAvgDuration)
        m_statAvgDuration->setText(loc.toString(avgDays, 'f', 1));
}

void MainWindow::on_btnOptimizeStorage_clicked()
{
    if (!m_chambreModel || !m_lotModel)
        return;

    QHash<QString, int> lotsByChambre;
    int totalQuantite = 0;
    for (const auto &l : m_lotModel->items())
    {
        lotsByChambre[l.idChambre] += l.quantite;
        totalQuantite += l.quantite;
    }

    QStringList alertes;
    double consommation = 0.0;

    struct ChambreInfo
    {
        QString id;
        double tempConsigne = 0.0;
        int capacite = 0;
    };
    QVector<ChambreInfo> chambres;

    for (const auto &c : m_chambreModel->items())
    {
        const double delta = c.temperature - c.tempConsigne;
        if (delta > 2.0)
        {
            alertes.append(tr("%1 (T: %2, Consigne: %3)")
                               .arg(c.idChambre)
                               .arg(c.temperature, 0, 'f', 1)
                               .arg(c.tempConsigne, 0, 'f', 1));
        }
        consommation += qAbs(delta) + 1.0;

        ChambreInfo info;
        info.id = c.idChambre;
        info.tempConsigne = c.tempConsigne;
        info.capacite = c.capacite;
        chambres.push_back(info);
    }

    QStringList suggestions;
    for (int i = 0; i < chambres.size(); ++i)
    {
        for (int j = i + 1; j < chambres.size(); ++j)
        {
            const auto &a = chambres.at(i);
            const auto &b = chambres.at(j);
            if (qFuzzyCompare(a.tempConsigne + 1.0, b.tempConsigne + 1.0))
            {
                const int qa = lotsByChambre.value(a.id, 0);
                const int qb = lotsByChambre.value(b.id, 0);
                const int total = qa + qb;
                const int cap = qMax(a.capacite, b.capacite);
                if (total > 0 && total <= cap)
                {
                    suggestions.append(tr("Fusion possible: %1 + %2 (Charge %3/%4)")
                                           .arg(a.id, b.id)
                                           .arg(total)
                                           .arg(cap));
                }
            }
        }
    }

    if (consommation <= 0.0)
        consommation = 1.0;

    const double score = double(totalQuantite) / consommation;
    if (m_energyEfficiencyCircle)
    {
        const double normalized = qMin(1.0, score / 100.0);
        m_energyEfficiencyCircle->setValue(normalized);
        m_energyEfficiencyCircle->setLabel(tr("Efficacite energ.") + "\n" + QString::number(score, 'f', 1));
    }

    QStringList messages;
    if (!alertes.isEmpty())
        messages.append(tr("Alertes temperature (T > Consigne + 2):\n%1").arg(alertes.join("\n")));
    if (!suggestions.isEmpty())
        messages.append(tr("Regroupement propose:\n%1").arg(suggestions.join("\n")));

    if (messages.isEmpty())
    {
        QMessageBox::information(this, tr("Optimisation"), tr("Aucune anomalie detectee. Score: %1").arg(score, 0, 'f', 1));
        return;
    }

    const QString msg = messages.join("\n\n");
    if (!alertes.isEmpty())
        QMessageBox::warning(this, tr("Optimisation"), msg);
    else
        QMessageBox::information(this, tr("Optimisation"), msg);
}

void MainWindow::onAddChambre()
{
    // Placeholder - form has been removed, use dialog instead
}

void MainWindow::onShowAddChambreDialog()
{
    auto *dialog = new AddChambreFroideDialog(this);
    
    // Set callback for "Ajouter" button
    dialog->setChambreAddedCallback([this](const ChambreFroide &c) {
        QString error;
        if (!m_chambreModel->addChambre(c, &error))
        {
            QMessageBox::warning(this, tr("Ajouter chambre"), error);
            return;
        }
        refreshChambreCombo();
        refreshStats();
    });
    
    if (dialog->exec() == QDialog::Accepted)
    {
        const ChambreFroide c = dialog->getChambreFroide();
        
        // Processing final submission with "Enregistrer" button
        QString error;
        if (!c.idChambre.isEmpty() && !m_chambreModel->addChambre(c, &error))
        {
            QMessageBox::warning(this, tr("Ajouter chambre"), error);
        }
        else if (!c.idChambre.isEmpty())
        {
            refreshChambreCombo();
            refreshStats();
        }
    }
    delete dialog;
}

void MainWindow::onShowAddLotDialog()
{
    if (m_chambreModel->items().isEmpty())
    {
        QMessageBox::warning(this, tr("Ajouter lot"), tr("Ajoutez d'abord une chambre froide."));
        return;
    }

    auto *dialog = new AddStockageLotDialog(m_chambreModel, this);
    
    // Set callback for "Ajouter" button
    dialog->setLotAddedCallback([this](const StockageLot &l) {
        // Capacity check
        const int chamberRow = m_chambreModel->indexOfId(l.idChambre);
        if (chamberRow < 0)
        {
            QMessageBox::warning(this, tr("Ajouter lot"), tr("Chambre invalide."));
            return false;
        }

        const int used = usedCapacityForChambre(l.idChambre);
        const int cap = m_chambreModel->items().at(chamberRow).capacite;
        if (used + l.quantite > cap)
        {
            QMessageBox::warning(this,
                                 tr("Ajouter lot"),
                                 tr("Capacité dépassée (%1/%2).").arg(used + l.quantite).arg(cap));
            return false;
        }

        QString error;
        if (!m_lotModel->addLot(l, &error))
        {
            QMessageBox::warning(this, tr("Ajouter lot"), error);
            return false;
        }

        refreshStats();
        return true;
    });
    
    if (dialog->exec() == QDialog::Accepted)
    {
        const StockageLot l = dialog->getStockageLot();
        
        if (l.idLot.isEmpty())
        {
            delete dialog;
            return;
        }
        
        // Processing final submission with "Enregistrer" button
        const int chamberRow = m_chambreModel->indexOfId(l.idChambre);
        if (chamberRow < 0)
        {
            QMessageBox::warning(this, tr("Ajouter lot"), tr("Chambre invalide."));
        }
        else
        {
            const int used = usedCapacityForChambre(l.idChambre);
            const int cap = m_chambreModel->items().at(chamberRow).capacite;
            if (used + l.quantite > cap)
            {
                QMessageBox::warning(this,
                                     tr("Ajouter lot"),
                                     tr("Capacité dépassée (%1/%2).").arg(used + l.quantite).arg(cap));
            }
            else
            {
                QString error;
                if (!m_lotModel->addLot(l, &error))
                {
                    QMessageBox::warning(this, tr("Ajouter lot"), error);
                }
                else
                {
                    refreshStats();
                }
            }
        }
    }
    delete dialog;
}

void MainWindow::onShowUpdateChambreDialog()
{
    const int row = selectedSourceRow(m_tableChambres, m_chambreProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Modifier chambre"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    const ChambreFroide originalChambre = m_chambreModel->items().at(row);
    auto *dialog = new UpdateChambreFroideDialog(originalChambre, this);
    
    if (dialog->exec() == QDialog::Accepted)
    {
        const ChambreFroide c = dialog->getChambreFroide();

        const int used = usedCapacityForChambre(originalChambre.idChambre, row);
        if (c.capacite < used)
        {
            QMessageBox::warning(this,
                                 tr("Modifier chambre"),
                                 tr("Capacité insuffisante: %1 déjà utilisé dans cette chambre.").arg(used));
            delete dialog;
            return;
        }

        QString error;
        if (!m_chambreModel->updateChambre(row, c, &error))
        {
            QMessageBox::warning(this, tr("Modifier chambre"), error);
        }
        else
        {
            refreshChambreCombo();
            refreshStats();
        }
    }
    delete dialog;
}

void MainWindow::onUpdateChambre()
{
    // Placeholder for backward compatibility
}

void MainWindow::onDeleteChambre()
{
    // Get all selected rows from the proxy model
    QModelIndexList selected = m_tableChambres->selectionModel()->selectedRows();
    
    if (selected.isEmpty())
    {
        QMessageBox::information(this, tr("Supprimer chambre"), tr("Sélectionnez au moins une ligne dans le tableau."));
        return;
    }

    // Map proxy indexes to source rows and collect IDs
    QList<int> sourceRows;
    QStringList chambreIds;
    for (const QModelIndex &proxyIdx : selected)
    {
        const QModelIndex sourceIdx = m_chambreProxy->mapToSource(proxyIdx);
        const int row = sourceIdx.row();
        if (row >= 0 && row < m_chambreModel->rowCount())
        {
            sourceRows.append(row);
            chambreIds.append(m_chambreModel->items().at(row).idChambre);
        }
    }

    // Check if any chamber is referenced by lots
    QStringList chambresAvecLots;
    for (const QString &id : chambreIds)
    {
        for (const auto &l : m_lotModel->items())
        {
            if (l.idChambre.compare(id, Qt::CaseInsensitive) == 0)
            {
                if (!chambresAvecLots.contains(id))
                    chambresAvecLots.append(id);
                break;
            }
        }
    }

    // If some chambers have lots, show which ones
    if (!chambresAvecLots.isEmpty())
    {
        QString msg = tr("Impossible de supprimer les chambres suivantes (des lots y sont assignés):\n\n");
        msg += chambresAvecLots.join(", ");
        QMessageBox::warning(this, tr("Supprimer chambre"), msg);
        return;
    }

    QString msg = (sourceRows.size() == 1) 
        ? tr("Supprimer la chambre sélectionnée ?")
        : tr("Supprimer les %1 chambres sélectionnées ?").arg(sourceRows.size());
        
    if (QMessageBox::question(this, tr("Supprimer chambre"), msg) != QMessageBox::Yes)
        return;

    // Sort in descending order to remove from end to begin
    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    QString error;
    for (int row : sourceRows)
    {
        if (!m_chambreModel->removeChambre(row, &error))
        {
            QMessageBox::warning(this, tr("Supprimer chambre"), error);
            break;
        }
    }

    refreshChambreCombo();
    m_checkSelectAllChambre->setChecked(false);
}

void MainWindow::onSelectAllChambres()
{
    if (m_checkSelectAllChambre->isChecked())
    {
        m_tableChambres->selectAll();
    }
    else
    {
        m_tableChambres->clearSelection();
    }
}

void MainWindow::onAddLot()
{
    // Placeholder for backward compatibility
}

void MainWindow::onShowUpdateLotDialog()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Modifier lot"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    const StockageLot originalLot = m_lotModel->items().at(row);
    auto *dialog = new UpdateStockageLotDialog(originalLot, m_chambreModel, this);
    
    if (dialog->exec() == QDialog::Accepted)
    {
        const StockageLot l = dialog->getStockageLot();
        
        // Capacity check
        const int chamberRow = m_chambreModel->indexOfId(l.idChambre);
        if (chamberRow < 0)
        {
            QMessageBox::warning(this, tr("Modifier lot"), tr("Chambre invalide."));
            delete dialog;
            return;
        }

        const int used = usedCapacityForChambre(l.idChambre, row);
        const int cap = m_chambreModel->items().at(chamberRow).capacite;
        if (used + l.quantite > cap)
        {
            QMessageBox::warning(this,
                                 tr("Modifier lot"),
                                 tr("Capacité dépassée (%1/%2).").arg(used + l.quantite).arg(cap));
            delete dialog;
            return;
        }

        QString error;
        if (!m_lotModel->updateLot(row, l, &error))
        {
            QMessageBox::warning(this, tr("Modifier lot"), error);
        }
        else
        {
            refreshStats();
        }
    }
    delete dialog;
}

void MainWindow::onUpdateLot()
{
    // Placeholder for backward compatibility
}

void MainWindow::onDeleteLot()
{
    // Get all selected rows from the proxy model
    QModelIndexList selected = m_tableLots->selectionModel()->selectedRows();
    
    if (selected.isEmpty())
    {
        QMessageBox::information(this, tr("Supprimer lot"), tr("Sélectionnez au moins une ligne dans le tableau."));
        return;
    }

    // Map proxy indexes to source rows
    QList<int> sourceRows;
    for (const QModelIndex &proxyIdx : selected)
    {
        const QModelIndex sourceIdx = m_lotProxy->mapToSource(proxyIdx);
        const int row = sourceIdx.row();
        if (row >= 0 && row < m_lotModel->rowCount())
        {
            sourceRows.append(row);
        }
    }

    QString msg = (sourceRows.size() == 1) 
        ? tr("Supprimer le lot sélectionné ?")
        : tr("Supprimer les %1 lots sélectionnés ?").arg(sourceRows.size());
        
    if (QMessageBox::question(this, tr("Supprimer lot"), msg) != QMessageBox::Yes)
        return;

    // Sort in descending order to remove from end to begin
    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    QString error;
    for (int row : sourceRows)
    {
        if (!m_lotModel->removeLot(row, &error))
        {
            QMessageBox::warning(this, tr("Supprimer lot"), error);
            break;
        }
    }

    refreshStats();
    m_checkSelectAllLot->setChecked(false);
}

void MainWindow::onSelectAllLots()
{
    if (m_checkSelectAllLot->isChecked())
    {
        m_tableLots->selectAll();
    }
    else
    {
        m_tableLots->clearSelection();
    }
}

void MainWindow::onExportLotsPdf()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), "lots.pdf", tr("PDF (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT — Lots stockés"), m_lotProxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter PDF"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter PDF"), tr("PDF exporté: %1").arg(filePath));
}

void MainWindow::onTriChambreChanged(int index)
{
    if (index == 0)
    {
        // Pas de tri
        m_tableChambres->sortByColumn(-1, Qt::AscendingOrder);
        return;
    }

    if (index == 1)
    {
        // Tri par capacité
        m_tableChambres->sortByColumn(1, Qt::DescendingOrder);
    }
    else if (index == 2)
    {
        // Tri par température
        m_tableChambres->sortByColumn(4, Qt::AscendingOrder);
    }
}

void MainWindow::onExportChambreFacturePdf()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter Facture PDF"), "facture_location_chambre.pdf", tr("PDF (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT — Facture de Location de Chambre Froide"), m_chambreProxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter Facture PDF"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter Facture PDF"), tr("PDF exporté: %1").arg(filePath));
}

void MainWindow::onAssignLotToChambre()
{
    const int row = selectedSourceRow(m_tableUnassignedLots, m_unassignedLotsProxy);
    if (row < 0 || row >= m_lotModel->rowCount())
    {
        QMessageBox::information(this, tr("Affecter lot"), tr("Sélectionnez un lot dans la table."));
        return;
    }

    const QString chambreId = m_assignChambreCombo->currentText();
    if (chambreId.isEmpty() || chambreId == tr("-- Sélectionner --"))
    {
        QMessageBox::information(this, tr("Affecter lot"), tr("Sélectionnez une chambre."));
        return;
    }

    StockageLot lot = m_lotModel->items()[row];
    lot.idChambre = chambreId;

    QString error;
    if (!m_lotModel->updateLot(row, lot, &error))
    {
        QMessageBox::warning(this, tr("Affecter lot"), error);
        return;
    }

    QMessageBox::information(this, tr("Affecter lot"), tr("Lot \"%1\" affecté à la chambre \"%2\"").arg(lot.idLot, chambreId));
    
    // Refresh filters and views
    m_unassignedLotsProxy->refresh();
    m_assignedLotsProxy->refresh();
    m_tableUnassignedLots->resizeColumnsToContents();
    m_tableAssignments->resizeColumnsToContents();
    refreshStats();
}

void MainWindow::onUnassignLot()
{
    const int row = selectedSourceRow(m_tableAssignments, m_assignedLotsProxy);
    if (row < 0 || row >= m_lotModel->rowCount())
    {
        QMessageBox::information(this, tr("Désaffecter lot"), tr("Sélectionnez une affectation dans la table."));
        return;
    }

    StockageLot lot = m_lotModel->items()[row];
    const QString previousChambre = lot.idChambre;
    lot.idChambre = "";

    QString error;
    if (!m_lotModel->updateLot(row, lot, &error))
    {
        QMessageBox::warning(this, tr("Désaffecter lot"), error);
        return;
    }

    QMessageBox::information(this, tr("Désaffecter lot"), tr("Lot \"%1\" désaffecté de la chambre \"%2\"").arg(lot.idLot, previousChambre));
    
    // Refresh filters and views
    m_unassignedLotsProxy->refresh();
    m_assignedLotsProxy->refresh();
    m_tableUnassignedLots->resizeColumnsToContents();
    m_tableAssignments->resizeColumnsToContents();
    refreshStats();
}

} // namespace stockage