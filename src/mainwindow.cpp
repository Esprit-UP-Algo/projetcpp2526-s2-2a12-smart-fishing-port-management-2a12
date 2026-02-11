#include "mainwindow.h"
#include "documentexporter.h"
#include "lotdialog.h"

// Include explicit headers for models and widgets to satisfy Intellisense
#include "../models/capturelot.h"
#include "../models/capturelotmodel.h"
#include "../models/capturelotproxymodel.h"
#include "statcircle.h"

#include <QEvent>

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
#include <QStringList>
#include <QList>

// Fonctions utilitaires pour l'UI
static QFrame *makeCard(QWidget *parent, const char *objectName)
{
    QFrame *f = new QFrame(parent);
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

static QString autoQualiteFromDate(const QDateTime &capture)
{
    if (!capture.isValid())
        return QObject::tr("Déclassé");

    // Calcul précis en heures pour une meilleure évaluation
    qint64 hoursdiff = capture.secsTo(QDateTime::currentDateTime()) / 3600;

    // Moins de 24h = Premium
    if (hoursdiff <= 24)
        return QObject::tr("Premium");
    // Moins de 72h (3 jours) = Standard
    if (hoursdiff <= 72)
        return QObject::tr("Standard");
    // Moins de 168h (7 jours) = Économique
    if (hoursdiff <= 168)
        return QObject::tr("Économique");
        
    // Plus de 7 jours
    return QObject::tr("Déclassé");
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

    // Explicit QStringList initialization
    QStringList items;
    items << tr("Gestion Des employés")
          << tr("Gestion Des Navires")
          << tr("Gestion Des Quais")
          << tr("Gestion Des Captures")
          << tr("Stockage Frigorifique")
          << tr("Gestion Des Ventes");
    for (int i = 0; i < items.size(); ++i) {
        auto *btn = new QPushButton(items[i], sidebar);
        btn->setObjectName("SideButton");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(42);
        btn->setStyleSheet("background: transparent; color: #e6eef6; font-size: 14px; border: 1px solid transparent; border-radius: 10px;");
        sideLayout->addWidget(btn);
        btn->installEventFilter(this);
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
    auto *h2 = new QLabel(tr("Module: Gestion du suivi des captures · Entité: Capture-Lot"), page);
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

    m_chkSelectAll = new QCheckBox(tr("Tout Sélectionner"), lotCard);
    m_chkSelectAll->setCursor(Qt::PointingHandCursor);
    m_chkSelectAll->setStyleSheet("color: #e6eef6; font-weight: bold; margin-left: 10px;");

    m_btnTraceLot = new QPushButton(tr("Traçabilité"), lotCard);
    m_btnTraceLot->setObjectName("SecondaryButton");
    m_btnTraceLot->setCursor(Qt::PointingHandCursor);

    m_btnIceYield = new QPushButton(tr("Analyse Rendement"), lotCard);
    m_btnIceYield->setObjectName("SecondaryButton");
    m_btnIceYield->setCursor(Qt::PointingHandCursor);

    lotBtnRow->addWidget(m_btnAddLot);
    lotBtnRow->addWidget(m_btnUpdateLot);
    lotBtnRow->addWidget(m_btnDeleteLot);
    lotBtnRow->addWidget(m_chkSelectAll);
    lotBtnRow->addWidget(m_btnTraceLot);
    lotBtnRow->addWidget(m_btnIceYield);
    lotBtnRow->addStretch();

    lotLayout->addLayout(lotBtnRow);

    // Filter card
    auto *lotFilterCard = makeCard(lotCard, "CardSmall");
    lotFilterCard->setFixedHeight(78);
    auto *lotFilterLayout = new QHBoxLayout(lotFilterCard);
    lotFilterLayout->setContentsMargins(16, 14, 16, 14);
    lotFilterLayout->setSpacing(10);

    m_sortCombo = new QComboBox(lotFilterCard);
    m_sortCombo->setObjectName("Input");
    m_sortCombo->addItems({
        tr("Trier par..."),
        tr("Zone de pêche"),
        tr("Espèce"),
        tr("Qualité")
    });
    m_sortCombo->setMinimumWidth(130);

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

    m_btnExportPdf = new QPushButton(tr("PDF"), lotFilterCard);
    m_btnExportPdf->setObjectName("AltPrimaryButton");
    m_btnExportPdf->setCursor(Qt::PointingHandCursor);
    m_btnExportPdf->setMaximumWidth(60);

    m_btnExportExcel = new QPushButton(tr("Excel"), lotFilterCard);
    m_btnExportExcel->setObjectName("AltPrimaryButton");
    m_btnExportExcel->setCursor(Qt::PointingHandCursor);
    m_btnExportExcel->setMaximumWidth(60);

    lotFilterLayout->addWidget(m_sortCombo);
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
    m_tableLots->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
        QLabel#Logo { background: #ffffff; color: #e6eef6; border: 2px solid #1e8de0; border-radius: 39px; font-weight: 800; }
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
    
    connect(m_chkSelectAll, &QCheckBox::stateChanged, this, [this](int state) {
        if (state == Qt::Checked)
            m_tableLots->selectAll();
        else
            m_tableLots->clearSelection();
    });

    connect(m_btnTraceLot, &QPushButton::clicked, this, &MainWindow::onTraceLot);
    connect(m_btnIceYield, &QPushButton::clicked, this, &MainWindow::onIceYieldAnalysis);

    // Export
    connect(m_btnExportPdf, &QPushButton::clicked, this, &MainWindow::onExportPdf);
    connect(m_btnExportExcel, &QPushButton::clicked, this, &MainWindow::onExportExcel);

    // Search/Filter
    connect(m_searchLot, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchLot);
    connect(m_searchEspece, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchEspece);
    connect(m_searchNavire, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchNavire);
    connect(m_searchZone, &QLineEdit::textChanged, m_lotProxy, &CaptureLotProxyModel::setSearchZone);

    // Sort
    connect(m_sortCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::onSortChanged);

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

    // Quantité par espèce (poids total) et espèce dominante
    QMap<QString, int> especePoids;
    QMap<QString, int> especeCount;
    int totalPoids = 0;

    for (const auto &lot : lots)
    {
        especeCount[lot.espece]++;
        especePoids[lot.espece] += lot.poids;
        totalPoids += lot.poids;
    }

    QString dominantEspece = "—";
    int maxPoids = 0;
    for (auto it = especePoids.begin(); it != especePoids.end(); ++it)
    {
        if (it.value() > maxPoids)
        {
            maxPoids = it.value();
            dominantEspece = it.key();
        }
    }

    if (m_statQuantiteEspece)
    {
        const QLocale loc;
        const QString qty = loc.toString(maxPoids) + " kg";
        m_statQuantiteEspece->setText(dominantEspece == "—" ? dominantEspece : dominantEspece + " (" + qty + ")");
    }

    // Zone fréquente
    QMap<QString, int> zoneCount;
    for (const auto &lot : lots)
        zoneCount[lot.zonePeche]++;

    QString frequentZone = "—";
    int maxCount = 0;
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

void MainWindow::onAddLot()
{
    LotDialog dlg(LotDialog::Ajout, this);

    if (dlg.exec() == QDialog::Accepted) {
        CaptureLot lot;
        lot.idLot = dlg.idLot().trimmed();
        lot.espece = dlg.espece().trimmed();
        lot.poids = dlg.poids();
        lot.zonePeche = dlg.zonePeche().trimmed();
        lot.dateCapture = dlg.dateCapture();
        lot.etat = dlg.etat().trimmed();
        lot.navire = dlg.navire().trimmed();
        lot.qualite = dlg.qualite().trimmed();
        lot.dateEntreeStock = dlg.dateEntreeStock();

        if (lot.qualite == tr("Auto"))
            lot.qualite = autoQualiteFromDate(lot.dateCapture);

        QString error;
        if (!m_lotModel->addLot(lot, &error)) {
            QMessageBox::warning(this, tr("Ajouter lot"), error);
            return;
        }
    }
}

void MainWindow::onUpdateLot()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0) {
        QMessageBox::information(this, tr("Modifier lot"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }
    const CaptureLot &current = m_lotModel->items().at(row);
    LotDialog dlg(LotDialog::Modif, this);
    dlg.setIdLot(current.idLot);
    dlg.setEspece(current.espece);
    dlg.setPoids(current.poids);
    dlg.setZonePeche(current.zonePeche);
    dlg.setDateCapture(current.dateCapture);
    dlg.setEtat(current.etat);
    dlg.setNavire(current.navire);
    dlg.setQualite(current.qualite);
    dlg.setDateEntreeStock(current.dateEntreeStock);

    if (dlg.exec() == QDialog::Accepted) {
        CaptureLot lot;
        lot.idLot = dlg.idLot().trimmed();
        lot.espece = dlg.espece().trimmed();
        lot.poids = dlg.poids();
        lot.zonePeche = dlg.zonePeche().trimmed();
        lot.dateCapture = dlg.dateCapture();
        lot.etat = dlg.etat().trimmed();
        lot.navire = dlg.navire().trimmed();
        lot.qualite = dlg.qualite().trimmed();
        lot.dateEntreeStock = dlg.dateEntreeStock();

        if (lot.qualite == tr("Auto"))
            lot.qualite = autoQualiteFromDate(lot.dateCapture);

        QString error;
        if (!m_lotModel->updateLot(row, lot, &error)) {
            QMessageBox::warning(this, tr("Modifier lot"), error);
            return;
        }
    }
}

void MainWindow::onDeleteLot()
{
    const auto &selection = m_tableLots->selectionModel()->selectedRows();
    if (selection.isEmpty())
    {
        QMessageBox::information(this, tr("Supprimer lot"), tr("Sélectionnez au moins un lot dans le tableau."));
        return;
    }

    if (QMessageBox::question(this, tr("Supprimer lot"), 
            tr("Voulez-vous vraiment supprimer %n lot(s) sélectionné(s) ?", "", selection.count())) != QMessageBox::Yes)
        return;

    // Récupérer les index sources et les trier en ordre décroissant
    QList<int> sourceRows;
    for (const auto &index : selection)
    {
        sourceRows.append(m_lotProxy->mapToSource(index).row());
    }
    std::sort(sourceRows.begin(), sourceRows.end(), std::greater<int>());

    // Supprimer
    QString error;
    bool hasError = false;
    for (int row : sourceRows)
    {
        if (!m_lotModel->removeLot(row, &error))
            hasError = true;
    }

    if (hasError)
        QMessageBox::warning(this, tr("Supprimer lot"), tr("Certains lots n'ont pas pu être supprimés : %1").arg(error));
}

void MainWindow::onTraceLot()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Traçabilité"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    const CaptureLot &lot = m_lotModel->items().at(row);
    
    // --- Funct. Avancée: Traçabilité complète ---
    // Calcul de durées
    qint64 durationCaptureToStock = lot.dateCapture.secsTo(lot.dateEntreeStock);
    QString durationText;
    if (durationCaptureToStock < 0) 
        durationText = tr("Incohérence (Stock avant Capture)");
    else {
        double hours = durationCaptureToStock / 3600.0;
        durationText = tr("%1 heures").arg(QString::number(hours, 'f', 1));
    }

    // État de vente
    bool isSold = (lot.etat.compare(tr("Vendu"), Qt::CaseInsensitive) == 0);
    QString venteStatus = isSold ? tr("✅ VENTE TERMINÉE") : tr("⏳ EN STOCK / ATTENTE");

    // Construction du rapport détaillé (Modern Mobile UI) - No Emoji
    QString report = "<html><head><style>"
                     "body { font-family: 'Segoe UI', sans-serif; color: #1e293b; font-size: 13px; }"
                     ".header { font-size: 16px; font-weight: 700; color: #0f172a; margin-bottom: 2px; letter-spacing: -0.5px; }"
                     ".sub-header { font-size: 11px; color: #64748b; margin-bottom: 20px; text-transform: uppercase; letter-spacing: 0.5px; }"
                     ".timeline-step { padding-bottom: 20px; }"
                     ".card { background-color: #ffffff; padding: 15px; margin-bottom: 10px; border-left: 4px solid #3b82f6; }" 
                     ".card-title { font-size: 12px; font-weight: 700; color: #334155; margin-bottom: 6px; text-transform: uppercase; }"
                     ".label { color: #64748b; font-size: 11px; }"
                     ".value { color: #0f172a; font-weight: 600; font-size: 13px; }"
                     ".badge { background-color: #dbeafe; color: #1e40af; font-size: 10px; font-weight: 700; padding: 2px 6px; }"
                     ".badge-success { background-color: #dcfce7; color: #166534; }"
                     ".badge-orange { background-color: #ffedd5; color: #9a3412; }"
                     ".dot { color: #3b82f6; font-size: 18px; line-height: 10px; }"
                     ".line { color: #cbd5e1; font-size: 10px; }"
                     "</style></head><body>";

    report += tr("<div class='header'>Chronologie de Traçabilité</div>");
    report += tr("<div class='sub-header'>LOT Réf #%1</div>").arg(lot.idLot);
    
    // Timeline Structure
    report += "<table width='100%' cellspacing='0' cellpadding='0'>";
    
    // Step 1: Capture
    report += "<tr><td width='25' valign='top' align='center'><div class='dot'>●</div><div class='line'>│<br>│<br>│<br>│</div></td>";
    report += "<td><div class='timeline-step'><div class='card'>";
    report += tr("<div class='card-title'>CAPTURE & ORIGINE</div>");
    report += "<table width='100%'>";
    report += tr("<tr><td class='label'>Date</td><td class='value'>%1</td></tr>").arg(lot.dateCapture.toString("dd MMM yyyy, HH:mm"));
    report += tr("<tr><td class='label'>Navire</td><td class='value'>%1</td></tr>").arg(lot.navire);
    report += tr("<tr><td class='label'>Zone</td><td class='value'>%1</td></tr>").arg(lot.zonePeche);
    report += tr("<tr><td class='label'>Espèce</td><td class='value'>%1 <span class='badge'>%2 kg</span></td></tr>").arg(lot.espece).arg(lot.poids);
    report += "</table></div></div></td></tr>";

    // Transport (in between)
    report += tr("<tr><td width='25' valign='top' align='center'><div class='line'>│<br>│</div></td>");
    report += tr("<td style='padding-bottom:10px;'><div style='color:#64748b; font-size:11px; font-style:italic;'>Transport Logistique (%1)</div></td></tr>").arg(durationText);

    // Step 2: Stockage
    report += "<tr><td width='25' valign='top' align='center'><div class='dot' style='color:#eab308'>●</div><div class='line'>│<br>│<br>│<br>│</div></td>";
    report += "<td><div class='timeline-step'><div class='card' style='border-left-color: #eab308'>";
    report += tr("<div class='card-title'>STOCKAGE & QUALITÉ</div>");
    report += "<table width='100%'>";
    report += tr("<tr><td class='label'>Entrée</td><td class='value'>%1</td></tr>").arg(lot.dateEntreeStock.toString("dd MMM yyyy, HH:mm"));
    report += tr("<tr><td class='label'>Qualité</td><td class='value' style='color:#15803d'>%1</td></tr>").arg(lot.qualite);
    
    qint64 dureeStock = lot.dateEntreeStock.secsTo(QDateTime::currentDateTime());
    double joursStock = dureeStock / 86400.0;
    report += tr("<tr><td class='label'>Durée</td><td class='value'>%1 Jours</td></tr>").arg(QString::number(joursStock, 'f', 1));
    report += "</table></div></div></td></tr>";

    // Step 3: Vente
    QString dotColor = isSold ? "#22c55e" : "#f97316";
    QString badgeClass = isSold ? "badge-success" : "badge-orange";
    
    report += tr("<tr><td width='25' valign='top' align='center'><div class='dot' style='color:%1'>●</div></td>").arg(dotColor);
    report += tr("<td><div class='timeline-step'><div class='card' style='border-left-color: %1'>").arg(dotColor);
    report += tr("<div class='card-title'>STATUT COMMERCIAL</div>");
    report += tr("<div><span class='badge %1'>%2</span></div>").arg(badgeClass).arg(venteStatus);
    report += "</div></div></td></tr>";

    report += "</table></body></html>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Rapport de Traçabilité"));
    // Modern Light Theme Stylesheet for this Specific Dialog
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #f1f5f9; }" // Slate-100
        "QLabel { color: #334155; }"
        "QPushButton { "
        "  background-color: #0f172a; color: #ffffff; " // Slate-900 btn
        "  border: none; border-radius: 8px; padding: 8px 20px; font-weight: 600; font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #1e293b; }"
        "QPushButton:pressed { background-color: #334155; }"
    );
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(report);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::onIceYieldAnalysis()
{
    const auto &lots = m_lotModel->items();
    if (lots.isEmpty()) {
        QMessageBox::information(this, tr("Analyse Rendement"), tr("Aucun lot à analyser."));
        return;
    }

    double poidsBleu = 0;
    double poidsNoble = 0;
    int countBleu = 0;
    int countNoble = 0;
    QString portDepart = "Kelibia"; 

    for (const auto &lot : lots) {
        if (lot.isPoissonBleu()) {
            poidsBleu += lot.poids;
            countBleu++;
        } else {
            poidsNoble += lot.poids;
            countNoble++;
        }
    }

    double totalPoids = poidsBleu + poidsNoble;
    double ratioBleu = (totalPoids > 0) ? (poidsBleu / totalPoids) * 100.0 : 0.0;
    double ratioNoble = (totalPoids > 0) ? (poidsNoble / totalPoids) * 100.0 : 0.0;
    
    // Ice Calculation
    double glaceBleu = poidsBleu * 0.8;
    double glaceNoble = poidsNoble * 0.4;
    double totalGlace = glaceBleu + glaceNoble;

    QString report = "<html><head><style>"
                     "body { font-family: 'Segoe UI', sans-serif; color: #1e293b; font-size: 13px; }"
                     ".header { font-size: 18px; font-weight: 700; color: #0f172a; margin-bottom: 4px; }"
                     ".sub { font-size: 11px; color: #64748b; margin-bottom: 20px; }"
                     
                     // Cards
                     ".kpi-card { background-color: #ffffff; padding: 12px; border-radius: 6px; margin-bottom: 12px; border-bottom: 2px solid #e2e8f0; }"
                     
                     ".label { font-size: 10px; color: #64748b; text-transform: uppercase; font-weight: 600; margin-bottom: 4px; }"
                     ".value-lg { font-size: 20px; font-weight: 800; color: #0f172a; }"
                     ".value-md { font-size: 14px; font-weight: 700; color: #334155; }"
                     
                     // Progress Bar Simulation
                     ".progress-track { background-color: #e2e8f0; height: 6px; width: 100%; margin-top: 8px; }"
                     ".progress-fill { background-color: #3b82f6; height: 6px; }"
                     
                     ".icon { font-size: 16px; margin-right: 5px; }"
                     ".total-box { background-color: #eff6ff; border: 1px solid #bfdbfe; padding: 12px; border-radius: 6px; margin-top: 10px; }"
                     "</style></head><body>";
    
    report += tr("<div class='header'>Analyse Rendement & Glace</div>");
    report += tr("<div class='sub'>TABLEAU DE BORD LOGISTIQUE • %1 LOTS</div>").arg(lots.size());

    // --- KPI CARDS ROW ---
    report += "<table width='100%' cellspacing='0' cellpadding='0'><tr>";
    // Card 1: Volume
    report += "<td width='50%' style='padding-right:5px;'><div class='kpi-card'>";
    report += tr("<div class='label'>Volume Total</div>");
    report += tr("<div class='value-lg'>%1 <span style='font-size:12px; color:#64748b'>kg</span></div>").arg(totalPoids);
    report += "</div></td>";
    // Card 2: Ratio Noble
    report += "<td width='50%' style='padding-left:5px;'><div class='kpi-card'>";
    report += tr("<div class='label'>Part Nobles</div>");
    report += tr("<div class='value-lg' style='color:#3b82f6'>%1<span style='font-size:14px'>%</span></div>").arg(QString::number(ratioNoble, 'f', 1));
    report += "</div></td></tr></table>";

    // --- SPECIES DISTRIBUTION (Progress Bar) ---
    report += "<div class='kpi-card'>";
    report += tr("<div class='label'>Répartition des Espèces</div>");
    report += "<table width='100%'>";
    report += tr("<tr><td width='50%' class='value-md'>Bleu: %1%</td><td width='50%' align='right' class='value-md'>Noble: %2%</td></tr>").arg(QString::number(ratioBleu, 'f', 0)).arg(QString::number(ratioNoble, 'f', 0));
    report += "</table>";
    
    // Progress Bar (Visual)
    report += "<div class='progress-track'><table width='100%' cellspacing='0' cellpadding='0'><tr>";
    if (ratioNoble > 0)
        report += tr("<td width='%1%' class='progress-fill' style='background-color:#eab308;'></td>").arg(ratioNoble);
    if (ratioBleu > 0)
        report += tr("<td width='%1%' class='progress-fill' style='background-color:#3b82f6;'></td>").arg(ratioBleu);
    report += "</tr></table></div>";
    
    report += "<div style='font-size:10px; color:#94a3b8; margin-top:5px; text-align:center;'>Ratio calculé sur la base du poids net</div>";
    report += "</div>";

    // --- ICE ESTIMATION ---
    report += "<div class='kpi-card' style='border-bottom: 2px solid #3b82f6;'>";
    report += tr("<div class='label' style='color:#3b82f6'>Estimation Glace (Route Standard)</div>");
    
    report += "<table width='100%' style='margin-top:5px;'>";
    report += tr("<tr><td style='color:#64748b; font-size:11px;'>Poisson Bleu (0.8 kg/kg)</td><td align='right' style='font-weight:600;'>%1 kg</td></tr>").arg(glaceBleu);
    report += tr("<tr><td style='color:#64748b; font-size:11px;'>Espèces Nobles (0.4 kg/kg)</td><td align='right' style='font-weight:600;'>%1 kg</td></tr>").arg(glaceNoble);
    report += "</table>";
    
    report += tr("<div class='total-box'><table width='100%'><tr><td style='color:#1e40af; font-weight:700;'>TOTAL REQUIS</td><td align='right' style='color:#1e40af; font-size:16px; font-weight:800;'>%1 kg</td></tr></table></div>").arg(totalGlace);
    report += "</div>";

    report += "</body></html>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Analyse Rendement & Glace"));
    msgBox.setStyleSheet(
        "QMessageBox { background-color: #f1f5f9; }" // Light Grey Background
        "QLabel { color: #334155; }"
        "QPushButton { "
        "  background-color: #0f172a; color: #ffffff; " // Dark Button
        "  border: none; border-radius: 8px; padding: 10px 24px; font-weight: 700; font-size: 13px;"
        "}"
        "QPushButton:hover { background-color: #1e293b; }"
        "QPushButton:pressed { background-color: #334155; }"
    );
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(report);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::onSortChanged(int index)
{
    // Index mapping:
    // 0: Trier par... (Default)
    // 1: Zone de pêche
    // 2: Espèce
    // 3: Qualité

    switch (index)
    {
    case 1: // Zone de pêche (col 3)
        m_lotProxy->sort(3, Qt::AscendingOrder);
        break;
    case 2: // Espèce (col 1)
        m_lotProxy->sort(1, Qt::AscendingOrder);
        break;
    case 3: // Qualité (col 7)
        m_lotProxy->sort(7, Qt::AscendingOrder);
        break;
    default:
        // Default sort (e.g. by ID or Date)
        m_lotProxy->sort(0, Qt::AscendingOrder);
        break;
    }
}

void MainWindow::onExportPdf()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Exporter en PDF"),
        "lots_poissons.pdf",
        tr("PDF (*.pdf);;Fichiers CSV (*.csv)"));
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    auto *btn = qobject_cast<QPushButton*>(obj);
    if (btn && btn->objectName() == "SideButton") {
        if (event->type() == QEvent::Enter) {
            btn->setStyleSheet("background: #36b6ff; color: #101c2c; font-weight: bold; border-radius: 10px;");
            return true;
        } else if (event->type() == QEvent::Leave) {
            btn->setStyleSheet("background: transparent; color: #e6eef6; font-size: 14px; border: 1px solid transparent; border-radius: 10px;");
            return true;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
