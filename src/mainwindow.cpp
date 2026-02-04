#include "mainwindow.h"
#include "documentexporter.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHeaderView>
#include <QHBoxLayout>
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
#include <QPixmap>
#include <QMap>

// Fonctions utilitaires pour l'UI
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
    setWindowTitle(tr("VISION SIGHT - Gestion des Lots de Poissons"));
    resize(1366, 768);

    m_lotModel = new CaptureLotModel(this);
    m_lotProxy = new CaptureLotProxyModel(this);
    m_lotProxy->setSourceModel(m_lotModel);

    buildUi();
    applyTheme();
    wireSignals();
    refreshStats();
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ========== SIDEBAR ==========
    auto *sidebar = new QFrame(central);
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(210);

    auto *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(18, 18, 18, 18);
    sideLayout->setSpacing(10);

    auto *brandBox = new QWidget(sidebar);
    auto *brandBoxLayout = new QVBoxLayout(brandBox);
    brandBoxLayout->setContentsMargins(0, 0, 0, 0);
    brandBoxLayout->setSpacing(8);

    auto *logo = new QLabel(brandBox);
    logo->setFixedSize(78, 78);
    logo->setAlignment(Qt::AlignCenter);
    logo->setObjectName("Logo");
    
    // Charger le logo automatiquement
    QPixmap logoPixmap(":/logo.png");
    if (!logoPixmap.isNull())
    {
        logo->setPixmap(logoPixmap.scaled(58, 58, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        logo->setText("VS");  // Fallback si logo absent
    }

    auto *brand1 = new QLabel(tr("VISION SIGHT"), brandBox);
    brand1->setObjectName("BrandTitle");
    brand1->setAlignment(Qt::AlignHCenter);

    brandBoxLayout->addWidget(logo, 0, Qt::AlignHCenter);
    brandBoxLayout->addWidget(brand1, 0, Qt::AlignHCenter);

    sideLayout->addWidget(brandBox);
    sideLayout->addSpacing(12);

    const QStringList items = {tr("Tableau de bord"), tr("Lots"), tr("Statistiques"), tr("Paramètres")};
    for (const QString &t : items)
    {
        auto *btn = new QPushButton(t, sidebar);
        btn->setObjectName("SideButton");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(42);
        sideLayout->addWidget(btn);
    }
    sideLayout->addStretch();

    rootLayout->addWidget(sidebar);

    // ========== MAIN CONTENT ==========
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
    auto *h2 = new QLabel(tr("Module: Gestion des suivie des captures · Entité: Capture-Lot"), page);
    h2->setObjectName("HeaderMuted");

    titleCol->addWidget(h1);
    titleCol->addWidget(h2);

    headerRow->addLayout(titleCol);
    headerRow->addStretch();

    auto *menuBtn = new QToolButton(page);
    menuBtn->setText("≡");
    menuBtn->setObjectName("MenuButton");
    menuBtn->setFixedSize(36, 36);
    headerRow->addWidget(menuBtn);

    pageLayout->addLayout(headerRow);

    // Top charts row
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(12);
    chartsRow->addWidget(makeChartPlaceholder(tr("Quantité par espèce"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Répartition par zone"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Poids moyen"), page), 1);
    pageLayout->addLayout(chartsRow);

    // ========== LOTS FORM CARD ==========
    auto *lotCard = makeCard(page, "Card");
    auto *lotLayout = new QVBoxLayout(lotCard);
    lotLayout->setContentsMargins(14, 12, 14, 12);
    lotLayout->setSpacing(10);

    auto *lotTitle = new QLabel(tr("Gestion des Lots de Poissons"), lotCard);
    lotTitle->setObjectName("SectionTitle");
    lotLayout->addWidget(lotTitle);

    // Form grid
    auto *lotFormGrid = new QFormLayout();
    lotFormGrid->setLabelAlignment(Qt::AlignLeft);
    lotFormGrid->setFormAlignment(Qt::AlignTop);
    lotFormGrid->setHorizontalSpacing(14);
    lotFormGrid->setVerticalSpacing(4);

    m_idLot = new QLineEdit(lotCard);
    m_idLot->setObjectName("Input");
    m_idLot->setPlaceholderText("LOT-001");

    m_espece = new QLineEdit(lotCard);
    m_espece->setObjectName("Input");
    m_espece->setPlaceholderText(tr("Sardine, Anchois..."));

    m_poids = new QSpinBox(lotCard);
    m_poids->setObjectName("Input");
    m_poids->setRange(1, 100000);
    m_poids->setSuffix(" kg");

    m_zonePeche = new QLineEdit(lotCard);
    m_zonePeche->setObjectName("Input");
    m_zonePeche->setPlaceholderText(tr("Atlantique, Méditerranée..."));

    m_dateCapture = new QDateTimeEdit(QDateTime::currentDateTime(), lotCard);
    m_dateCapture->setObjectName("Input");
    m_dateCapture->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_dateCapture->setCalendarPopup(true);

    m_navire = new QLineEdit(lotCard);
    m_navire->setObjectName("Input");
    m_navire->setPlaceholderText(tr("Nom du navire"));

    m_qualite = new QComboBox(lotCard);
    m_qualite->setObjectName("Input");
    m_qualite->addItems({tr("Premium"), tr("Standard"), tr("Économique"), tr("Déclassé")});

    m_dateEntreeStock = new QDateTimeEdit(QDateTime::currentDateTime(), lotCard);
    m_dateEntreeStock->setObjectName("Input");
    m_dateEntreeStock->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_dateEntreeStock->setCalendarPopup(true);

    auto *labId = new QLabel(tr("ID Lot"), lotCard);
    labId->setObjectName("Muted");
    auto *labEsp = new QLabel(tr("Espèce"), lotCard);
    labEsp->setObjectName("Muted");
    auto *labPoid = new QLabel(tr("Poids"), lotCard);
    labPoid->setObjectName("Muted");
    auto *labZone = new QLabel(tr("Zone Pêche"), lotCard);
    labZone->setObjectName("Muted");
    auto *labCap = new QLabel(tr("Date Capture"), lotCard);
    labCap->setObjectName("Muted");
    auto *labNav = new QLabel(tr("Navire"), lotCard);
    labNav->setObjectName("Muted");
    auto *labQual = new QLabel(tr("Qualité"), lotCard);
    labQual->setObjectName("Muted");
    auto *labEnt = new QLabel(tr("Date Entrée Stock"), lotCard);
    labEnt->setObjectName("Muted");

    lotFormGrid->addRow(labId, m_idLot);
    lotFormGrid->addRow(labEsp, m_espece);
    lotFormGrid->addRow(labPoid, m_poids);
    lotFormGrid->addRow(labZone, m_zonePeche);
    lotFormGrid->addRow(labCap, m_dateCapture);
    lotFormGrid->addRow(labNav, m_navire);
    lotFormGrid->addRow(labQual, m_qualite);
    lotFormGrid->addRow(labEnt, m_dateEntreeStock);

    lotLayout->addLayout(lotFormGrid);

    // Buttons row
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

    lotLayout->addLayout(lotBtnRow);

    // Filter card
    auto *lotFilterCard = makeCard(lotCard, "CardSmall");
    lotFilterCard->setFixedHeight(78);
    auto *lotFilterLayout = new QHBoxLayout(lotFilterCard);
    lotFilterLayout->setContentsMargins(16, 14, 16, 14);
    lotFilterLayout->setSpacing(10);

    m_searchLot = new QLineEdit(lotFilterCard);
    m_searchLot->setObjectName("Input");
    m_searchLot->setPlaceholderText(tr("Recherche lot"));

    m_searchEspece = new QLineEdit(lotFilterCard);
    m_searchEspece->setObjectName("Input");
    m_searchEspece->setPlaceholderText(tr("Recherche espèce"));

    m_searchNavire = new QLineEdit(lotFilterCard);
    m_searchNavire->setObjectName("Input");
    m_searchNavire->setPlaceholderText(tr("Recherche navire"));

    m_searchZone = new QLineEdit(lotFilterCard);
    m_searchZone->setObjectName("Input");
    m_searchZone->setPlaceholderText(tr("Recherche zone"));

    m_btnExportPdf = new QPushButton(tr("CSV"), lotFilterCard);
    m_btnExportPdf->setObjectName("AltPrimaryButton");
    m_btnExportPdf->setCursor(Qt::PointingHandCursor);
    m_btnExportPdf->setMaximumWidth(60);

    m_btnExportExcel = new QPushButton(tr("Excel"), lotFilterCard);
    m_btnExportExcel->setObjectName("AltPrimaryButton");
    m_btnExportExcel->setCursor(Qt::PointingHandCursor);
    m_btnExportExcel->setMaximumWidth(60);

    lotFilterLayout->addWidget(m_searchLot, 1);
    lotFilterLayout->addWidget(m_searchEspece, 1);
    lotFilterLayout->addWidget(m_searchNavire, 1);
    lotFilterLayout->addWidget(m_searchZone, 1);
    lotFilterLayout->addWidget(m_btnExportPdf);
    lotFilterLayout->addWidget(m_btnExportExcel);

    lotLayout->addWidget(lotFilterCard);

    // Table
    m_tableLots = new QTableView(lotCard);
    m_tableLots->setObjectName("Table");
    m_tableLots->setModel(m_lotProxy);
    m_tableLots->setSortingEnabled(true);
    m_tableLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableLots->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableLots->setShowGrid(false);
    m_tableLots->verticalHeader()->setVisible(false);
    m_tableLots->horizontalHeader()->setStretchLastSection(true);
    m_tableLots->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_tableLots->verticalHeader()->setDefaultSectionSize(28);
    m_tableLots->setAlternatingRowColors(true);
    m_tableLots->resizeColumnsToContents();
    m_tableLots->setMinimumHeight(200);
    m_tableLots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableLots->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    lotLayout->addWidget(m_tableLots, 1);

    // Middle row: Stats
    auto *midRow = new QHBoxLayout();
    midRow->setSpacing(12);
    midRow->addWidget(lotCard, 2);

    // Stats card
    auto *statsCard = makeCard(page, "Card");
    auto *stLayout = new QVBoxLayout(statsCard);
    stLayout->setContentsMargins(14, 12, 14, 12);
    stLayout->setSpacing(10);

    auto *stTitle = new QLabel(tr("Statistiques"), statsCard);
    stTitle->setObjectName("SectionTitle");
    stLayout->addWidget(stTitle);

    auto *statRow = new QHBoxLayout();
    statRow->setSpacing(12);

    m_captureCircle = new StatCircleWidget(statsCard);
    m_captureCircle->setMinimumSize(140, 160);
    m_captureCircle->setLabel(tr("Captures"));
    statRow->addWidget(m_captureCircle, 0, Qt::AlignLeft);

    auto *statValuesWidget = new QWidget(statsCard);
    auto *statValuesLayout = new QVBoxLayout(statValuesWidget);
    statValuesLayout->setContentsMargins(0, 0, 0, 0);
    statValuesLayout->setSpacing(8);

    auto makeStatLine = [&](const QString &labelText, QLabel **out) {
        auto *row = new QHBoxLayout();
        row->setSpacing(10);

        auto *lab = new QLabel(labelText, statsCard);
        lab->setObjectName("Muted");

        auto *val = new QLabel("—", statsCard);
        val->setObjectName("StatValue");
        QFont f = val->font();
        f.setPixelSize(18);
        f.setBold(true);
        val->setFont(f);
        val->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        row->addWidget(lab, 1);
        row->addWidget(val);
        statValuesLayout->addLayout(row);

        *out = val;
    };

    makeStatLine(tr("Espèce dominante"), &m_statQuantiteEspece);
    makeStatLine(tr("Zone fréquente"), &m_statZonePeche);
    makeStatLine(tr("Poids moyen"), &m_statAvgPoids);

    statRow->addWidget(statValuesWidget, 1);
    stLayout->addLayout(statRow);

    auto *stHint = new QLabel(tr("Mise à jour automatique"), statsCard);
    stHint->setObjectName("AccentText");
    stLayout->addWidget(stHint);
    stLayout->addStretch();

    midRow->addWidget(statsCard, 1);

    pageLayout->addLayout(midRow);
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
        QLabel#Logo { background: #0f2638; color: #e6eef6; border: 2px solid #1e8de0; border-radius: 39px; font-weight: 800; }
        QLabel#BrandTitle { color: #e6eef6; font-size: 16px; font-weight: 800; letter-spacing: 1px; }

        QPushButton#SideButton {
            text-align: left;
            padding-left: 12px;
            color: #e6eef6;
            background: transparent;
            border: 1px solid transparent;
            border-radius: 10px;
            font-size: 14px;
        }
        QPushButton#SideButton:hover { background: #18304a; border-color: #23374e; }

        QLabel#HeaderH1 { color: #132437; }
        QLabel#HeaderMuted { color: #132437; }

        QLabel#Muted { color: #9bb0c3; }
        QLabel#SectionTitle { color: #39c0fa; font-size: 18px; font-weight: 700; }
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

        QLineEdit#Input, QComboBox#Input, QDateTimeEdit#Input, QSpinBox#Input {
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
        QSpinBox#Input { padding-right: 30px; }

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

    setStyleSheet(qss);
}

void MainWindow::wireSignals()
{
    // CRUD
    connect(m_btnAddLot, &QPushButton::clicked, this, &MainWindow::onAddLot);
    connect(m_btnUpdateLot, &QPushButton::clicked, this, &MainWindow::onUpdateLot);
    connect(m_btnDeleteLot, &QPushButton::clicked, this, &MainWindow::onDeleteLot);
    connect(m_btnClearLot, &QPushButton::clicked, this, &MainWindow::onClearLotForm);

    // Export
    connect(m_btnExportPdf, &QPushButton::clicked, this, &MainWindow::onExportPdf);
    connect(m_btnExportExcel, &QPushButton::clicked, this, &MainWindow::onExportExcel);

    // Search/Filter
    connect(m_searchLot, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchLot);
    connect(m_searchEspece, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchEspece);
    connect(m_searchNavire, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchNavire);
    connect(m_searchZone, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchZone);

    // Table selection
    connect(m_tableLots->selectionModel(), &QItemSelectionModel::selectionChanged, 
            this, &MainWindow::loadSelectedLotToForm);

    // Auto-refresh stats
    auto refreshAll = [this]() {
        refreshStats();
    };

    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, refreshAll);
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

void MainWindow::refreshStats()
{
    const auto &lots = m_lotModel->items();
    if (lots.isEmpty())
    {
        if (m_statQuantiteEspece)
            m_statQuantiteEspece->setText("—");
        if (m_statZonePeche)
            m_statZonePeche->setText("—");
        if (m_statAvgPoids)
            m_statAvgPoids->setText("—");
        if (m_captureCircle)
            m_captureCircle->setValue(0.0);
        return;
    }

    // Espèce dominante
    QMap<QString, int> especeCount;
    int totalPoids = 0;

    for (const auto &lot : lots)
    {
        especeCount[lot.espece]++;
        totalPoids += lot.poids;
    }

    QString dominantEspece = "—";
    int maxCount = 0;
    for (auto it = especeCount.begin(); it != especeCount.end(); ++it)
    {
        if (it.value() > maxCount)
        {
            maxCount = it.value();
            dominantEspece = it.key();
        }
    }

    if (m_statQuantiteEspece)
        m_statQuantiteEspece->setText(dominantEspece);

    // Zone fréquente
    QMap<QString, int> zoneCount;
    for (const auto &lot : lots)
        zoneCount[lot.zonePeche]++;

    QString frequentZone = "—";
    maxCount = 0;
    for (auto it = zoneCount.begin(); it != zoneCount.end(); ++it)
    {
        if (it.value() > maxCount)
        {
            maxCount = it.value();
            frequentZone = it.key();
        }
    }

    if (m_statZonePeche)
        m_statZonePeche->setText(frequentZone);

    // Poids moyen
    const double avgPoids = double(totalPoids) / double(lots.size());
    const QLocale loc;
    if (m_statAvgPoids)
        m_statAvgPoids->setText(loc.toString(avgPoids, 'f', 1) + " kg");

    // Circle: nombre de lots / capacité théorique (100 lots max)
    const double occupancy = qMin(1.0, double(lots.size()) / 100.0);
    if (m_captureCircle)
        m_captureCircle->setValue(occupancy);
}

void MainWindow::loadSelectedLotToForm()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0 || row >= m_lotModel->rowCount())
        return;

    const auto &lot = m_lotModel->items().at(row);
    m_idLot->setText(lot.idLot);
    m_espece->setText(lot.espece);
    m_poids->setValue(lot.poids);
    m_zonePeche->setText(lot.zonePeche);
    m_dateCapture->setDateTime(lot.dateCapture);
    m_navire->setText(lot.navire);
    m_qualite->setCurrentText(lot.qualite);
    m_dateEntreeStock->setDateTime(lot.dateEntreeStock);
}

void MainWindow::onAddLot()
{
    CaptureLot lot;
    lot.idLot = m_idLot->text().trimmed();
    lot.espece = m_espece->text().trimmed();
    lot.poids = m_poids->value();
    lot.zonePeche = m_zonePeche->text().trimmed();
    lot.dateCapture = m_dateCapture->dateTime();
    lot.navire = m_navire->text().trimmed();
    lot.qualite = m_qualite->currentText();
    lot.dateEntreeStock = m_dateEntreeStock->dateTime();

    QString error;
    if (!m_lotModel->addLot(lot, &error))
    {
        QMessageBox::warning(this, tr("Ajouter lot"), error);
        return;
    }

    onClearLotForm();
}

void MainWindow::onUpdateLot()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Modifier lot"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    CaptureLot lot;
    lot.idLot = m_idLot->text().trimmed();
    lot.espece = m_espece->text().trimmed();
    lot.poids = m_poids->value();
    lot.zonePeche = m_zonePeche->text().trimmed();
    lot.dateCapture = m_dateCapture->dateTime();
    lot.navire = m_navire->text().trimmed();
    lot.qualite = m_qualite->currentText();
    lot.dateEntreeStock = m_dateEntreeStock->dateTime();

    QString error;
    if (!m_lotModel->updateLot(row, lot, &error))
    {
        QMessageBox::warning(this, tr("Modifier lot"), error);
        return;
    }
}

void MainWindow::onDeleteLot()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Supprimer lot"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (QMessageBox::question(this, tr("Supprimer lot"), tr("Supprimer le lot sélectionné ?")) != QMessageBox::Yes)
        return;

    QString error;
    if (!m_lotModel->removeLot(row, &error))
        QMessageBox::warning(this, tr("Supprimer lot"), error);

    onClearLotForm();
}

void MainWindow::onClearLotForm()
{
    m_idLot->clear();
    m_espece->clear();
    m_poids->setValue(1);
    m_zonePeche->clear();
    m_dateCapture->setDateTime(QDateTime::currentDateTime());
    m_navire->clear();
    m_qualite->setCurrentIndex(0);
    m_dateEntreeStock->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::onExportPdf()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter en CSV"), "lots_poissons.csv", tr("Fichiers CSV (*.csv)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!DocumentExporter::exportTableToPdf(filePath, tr("VISION SIGHT — Lots de Poissons"), m_lotProxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter"), tr("Données exportées avec succès:\n%1\n\nVous pouvez ouvrir ce fichier dans Excel et l'imprimer en PDF.").arg(filePath));
}

void MainWindow::onExportExcel()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter Excel"), "lots_poissons.csv", tr("Excel (*.csv);;Fichiers Excel (*.xlsx)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!DocumentExporter::exportTableToExcel(filePath, tr("VISION SIGHT — Lots de Poissons"), m_lotProxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter Excel"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter Excel"), tr("Données exportées avec succès:\n%1").arg(filePath));
}
