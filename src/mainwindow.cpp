#include "mainwindow.h"

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

#include <QPixmap>

#include "chambrefroidemodel.h"
#include "chambrefroidesortfilterproxymodel.h"
#include "pdfexporter.h"
#include "stockagelotmodel.h"
#include "stockagelotsortfilterproxymodel.h"
#include "transactionmodel.h"
#include "transactionsortfilterproxymodel.h"
#include <QStandardItemModel>
#include <QMap>
#include <QHash>
#include <algorithm>

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

    m_transModel = new TransactionModel(this);
    m_transProxy = new TransactionSortFilterProxyModel(this);
    m_transProxy->setSourceModel(m_transModel);

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
    // ensure logo has a white circular background
    logo->setStyleSheet("background-color: white; border-radius: 39px;");

    QPixmap pm(":/vision_sight.png");
    if (!pm.isNull())
        logo->setPixmap(pm.scaled(58, 58, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        logo->setText("VS");

    auto *brand1 = new QLabel(tr("VISION SIGHT"), brandBox);
    brand1->setObjectName("BrandTitle");
    brand1->setAlignment(Qt::AlignHCenter);

    brandBoxLayout->addWidget(logo, 0, Qt::AlignHCenter);
    brandBoxLayout->addWidget(brand1, 0, Qt::AlignHCenter);

    sideLayout->addWidget(brandBox);
    sideLayout->addSpacing(12);

    const QStringList items = {tr("Tableau de bord"), tr("Chambres"), tr("Lots"), tr("Statistiques"), tr("Paramètres")};
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

    // Top charts row (placeholders to match the picture repartition)
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(12);
    chartsRow->addWidget(makeChartPlaceholder(tr("Volume par espèce"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Poids total journalier"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Zones de pêche fréquentes"), page), 1);
    pageLayout->addLayout(chartsRow);

    // Middle row removed: Storage (Chambres/Lots) UI removed per request.

    // 'Lots stockés' UI removed per user request.

    // Transactions card (Vente / Achat)
    auto *transCard = makeCard(page, "Card");
    auto *transLayout = new QVBoxLayout(transCard);
    transLayout->setContentsMargins(14, 12, 14, 12);
    transLayout->setSpacing(10);

    auto *transTitle = new QLabel(tr("Ventes / Achats"), transCard);
    transTitle->setObjectName("SectionTitle");
    transLayout->addWidget(transTitle);

    auto *transFormGrid = new QFormLayout();
    transFormGrid->setLabelAlignment(Qt::AlignLeft);
    transFormGrid->setFormAlignment(Qt::AlignTop);
    transFormGrid->setHorizontalSpacing(14);
    transFormGrid->setVerticalSpacing(4);

    m_idVente = new QLineEdit(transCard);
    m_idVente->setObjectName("Input");
    m_idVente->setPlaceholderText("VTE-100");

    m_client = new QLineEdit(transCard);
    m_client->setObjectName("Input");
    m_client->setPlaceholderText(tr("Client"));

    m_lotTrans = new QComboBox(transCard);
    m_lotTrans->setObjectName("Input");

    m_prixKg = new QDoubleSpinBox(transCard);
    m_prixKg->setObjectName("Input");
    m_prixKg->setRange(0.0, 1e9);
    m_prixKg->setDecimals(2);

    m_quantiteTrans = new QDoubleSpinBox(transCard);
    m_quantiteTrans->setObjectName("Input");
    m_quantiteTrans->setRange(0.0, 1e9);
    m_quantiteTrans->setDecimals(3);

    m_totalTrans = new QLabel("0.00", transCard);
    m_totalTrans->setObjectName("StatValue");

    m_dateTrans = new QDateTimeEdit(QDateTime::currentDateTime(), transCard);
    m_dateTrans->setObjectName("Input");
    m_dateTrans->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_dateTrans->setCalendarPopup(true);

    m_modePaiement = new QComboBox(transCard);
    m_modePaiement->setObjectName("Input");
    m_modePaiement->addItems({tr("Espèces"), tr("Carte"), tr("Virement")});

    m_statutPaiement = new QComboBox(transCard);
    m_statutPaiement->setObjectName("Input");
    m_statutPaiement->addItems({tr("Payé"), tr("En attente"), tr("Annulé")});

    auto *labIdV = new QLabel(tr("ID Vente"), transCard); labIdV->setObjectName("Muted");
    auto *labClient = new QLabel(tr("Client"), transCard); labClient->setObjectName("Muted");
    auto *labLotT = new QLabel(tr("Lot"), transCard); labLotT->setObjectName("Muted");
    auto *labPrix = new QLabel(tr("Prix/kg"), transCard); labPrix->setObjectName("Muted");
    auto *labQTrans = new QLabel(tr("Quantité"), transCard); labQTrans->setObjectName("Muted");
    auto *labTotal = new QLabel(tr("Total"), transCard); labTotal->setObjectName("Muted");
    auto *labDateT = new QLabel(tr("Date"), transCard); labDateT->setObjectName("Muted");
    auto *labMode = new QLabel(tr("Mode paiement"), transCard); labMode->setObjectName("Muted");
    auto *labStat = new QLabel(tr("Statut paiement"), transCard); labStat->setObjectName("Muted");

    transFormGrid->addRow(labIdV, m_idVente);
    transFormGrid->addRow(labClient, m_client);
    transFormGrid->addRow(labLotT, m_lotTrans);
    transFormGrid->addRow(labPrix, m_prixKg);
    transFormGrid->addRow(labQTrans, m_quantiteTrans);
    transFormGrid->addRow(labTotal, m_totalTrans);
    transFormGrid->addRow(labDateT, m_dateTrans);
    transFormGrid->addRow(labMode, m_modePaiement);
    transFormGrid->addRow(labStat, m_statutPaiement);

    transLayout->addLayout(transFormGrid);

    auto *transBtnRow = new QHBoxLayout();
    transBtnRow->setSpacing(10);

    m_btnAddTrans = new QPushButton(tr("Ajouter"), transCard);
    m_btnAddTrans->setObjectName("PrimaryButton");
    m_btnAddTrans->setCursor(Qt::PointingHandCursor);

    m_btnUpdateTrans = new QPushButton(tr("Modifier"), transCard);
    m_btnUpdateTrans->setObjectName("SecondaryButton");
    m_btnUpdateTrans->setCursor(Qt::PointingHandCursor);

    m_btnDeleteTrans = new QPushButton(tr("Supprimer"), transCard);
    m_btnDeleteTrans->setObjectName("DangerButton");
    m_btnDeleteTrans->setCursor(Qt::PointingHandCursor);

    m_btnClearTrans = new QPushButton(tr("Réinitialiser"), transCard);
    m_btnClearTrans->setObjectName("GhostButton");
    m_btnClearTrans->setCursor(Qt::PointingHandCursor);

    transBtnRow->addWidget(m_btnAddTrans);
    transBtnRow->addWidget(m_btnUpdateTrans);
    transBtnRow->addWidget(m_btnDeleteTrans);
    transBtnRow->addWidget(m_btnClearTrans);
    transBtnRow->addStretch();

    transLayout->addLayout(transBtnRow);

    auto *transFilterCard = makeCard(transCard, "CardSmall");
    transFilterCard->setFixedHeight(78);
    auto *transFilterLayout = new QHBoxLayout(transFilterCard);
    transFilterLayout->setContentsMargins(16, 14, 16, 14);
    transFilterLayout->setSpacing(10);

    m_searchTransClient = new QLineEdit(transFilterCard);
    m_searchTransClient->setObjectName("Input");
    m_searchTransClient->setPlaceholderText(tr("Recherche client"));

    m_searchTransLot = new QLineEdit(transFilterCard);
    m_searchTransLot->setObjectName("Input");
    m_searchTransLot->setPlaceholderText(tr("Recherche lot"));

    m_searchTransFrom = new QDateTimeEdit(transFilterCard);
    m_searchTransFrom->setObjectName("Input");
    m_searchTransFrom->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_searchTransFrom->setCalendarPopup(true);

    m_searchTransTo = new QDateTimeEdit(transFilterCard);
    m_searchTransTo->setObjectName("Input");
    m_searchTransTo->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_searchTransTo->setCalendarPopup(true);

    m_btnExportTransPdf = new QPushButton(tr("Exporter PDF"), transFilterCard);
    m_btnExportTransPdf->setObjectName("AltPrimaryButton");
    m_btnExportTransPdf->setCursor(Qt::PointingHandCursor);

    transFilterLayout->addWidget(m_searchTransClient, 1);
    transFilterLayout->addWidget(m_searchTransLot, 1);
    transFilterLayout->addWidget(m_searchTransFrom);
    transFilterLayout->addWidget(m_searchTransTo);
    transFilterLayout->addWidget(m_btnExportTransPdf);

    transLayout->addWidget(transFilterCard);

    m_tableTrans = new QTableView(transCard);
    m_tableTrans->setObjectName("Table");
    m_tableTrans->setModel(m_transProxy);
    m_tableTrans->setSortingEnabled(true);
    m_tableTrans->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableTrans->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableTrans->setShowGrid(false);
    m_tableTrans->verticalHeader()->setVisible(false);
    m_tableTrans->horizontalHeader()->setStretchLastSection(true);
    m_tableTrans->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    transLayout->addWidget(m_tableTrans, 1);

    // Transaction stats card
    auto *transStatsCard = makeCard(page, "Card");
    auto *tsLayout = new QVBoxLayout(transStatsCard);
    tsLayout->setContentsMargins(14, 12, 14, 12);
    tsLayout->setSpacing(10);

    auto *tsTitle = new QLabel(tr("Statistiques Ventes"), transStatsCard);
    tsTitle->setObjectName("SectionTitle");
    tsLayout->addWidget(tsTitle);

    auto makeTransStatLine = [&](const QString &labelText, QLabel **out) {
        auto *row = new QHBoxLayout();
        row->setSpacing(10);

        auto *lab = new QLabel(labelText, transStatsCard);
        lab->setObjectName("Muted");

        auto *val = new QLabel("—", transStatsCard);
        val->setObjectName("StatValue");
        QFont f = val->font();
        f.setPixelSize(18);
        f.setBold(true);
        val->setFont(f);
        val->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        row->addWidget(lab, 1);
        row->addWidget(val);
        tsLayout->addLayout(row);

        *out = val;
    };

    makeTransStatLine(tr("Chiffre d'affaires"), &m_statRevenue);
    makeTransStatLine(tr("Espèces les plus vendues"), &m_statTopSpecies);
    makeTransStatLine(tr("Meilleurs clients"), &m_statBestClients);

    pageLayout->addWidget(transCard);
    pageLayout->addWidget(transStatsCard);
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
    // Storage (Chambres / Lots) UI and signals removed per request.

    

    // Transactions signals
    connect(m_btnAddTrans, &QPushButton::clicked, this, &MainWindow::onAddTransaction);
    connect(m_btnUpdateTrans, &QPushButton::clicked, this, &MainWindow::onUpdateTransaction);
    connect(m_btnDeleteTrans, &QPushButton::clicked, this, &MainWindow::onDeleteTransaction);
    connect(m_btnClearTrans, &QPushButton::clicked, this, &MainWindow::onClearTransactionForm);
    connect(m_btnExportTransPdf, &QPushButton::clicked, this, &MainWindow::onExportTransactionsPdf);

    connect(m_searchTransClient, &QLineEdit::textChanged, m_transProxy, &TransactionSortFilterProxyModel::setSearchClient);
    connect(m_searchTransLot, &QLineEdit::textChanged, m_transProxy, &TransactionSortFilterProxyModel::setSearchLot);
    connect(m_searchTransFrom, &QDateTimeEdit::dateTimeChanged, m_transProxy, &TransactionSortFilterProxyModel::setDateFrom);
    connect(m_searchTransTo, &QDateTimeEdit::dateTimeChanged, m_transProxy, &TransactionSortFilterProxyModel::setDateTo);

    connect(m_tableTrans->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::loadSelectedTransactionToForm);

    auto refreshTransAll = [this]() { refreshTransactionStats(); };
    connect(m_transModel, &QAbstractItemModel::rowsInserted, this, refreshTransAll);
    connect(m_transModel, &QAbstractItemModel::rowsRemoved, this, refreshTransAll);
    connect(m_transModel, &QAbstractItemModel::modelReset, this, refreshTransAll);
    connect(m_transModel, &QAbstractItemModel::dataChanged, this, refreshTransAll);

    connect(m_sortieRenseignee, &QCheckBox::toggled, this, [this](bool on) {
        m_sortie->setEnabled(on);
        if (!on)
            m_sortie->setDateTime(QDateTime::currentDateTime());
    });

    // Refresh stats automatically
    auto refreshAll = [this]() {
        refreshChambreCombo();
        refreshStats();
    };

    connect(m_chambreModel, &QAbstractItemModel::rowsInserted, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::rowsRemoved, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::modelReset, this, refreshAll);
    connect(m_chambreModel, &QAbstractItemModel::dataChanged, this, refreshAll);

    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, refreshAll);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, refreshAll);

    // Refresh transaction lot combobox when lot model changes
    connect(m_lotModel, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshTransLotCombo);
    connect(m_lotModel, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshTransLotCombo);
    connect(m_lotModel, &QAbstractItemModel::modelReset, this, &MainWindow::refreshTransLotCombo);
    connect(m_lotModel, &QAbstractItemModel::dataChanged, this, &MainWindow::refreshTransLotCombo);

    // Update total when price or qty change
    connect(m_prixKg, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double){
        const double total = m_prixKg->value() * m_quantiteTrans->value();
        m_totalTrans->setText(QLocale().toString(total, 'f', 2));
    });
    connect(m_quantiteTrans, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this](double){
        const double total = m_prixKg->value() * m_quantiteTrans->value();
        m_totalTrans->setText(QLocale().toString(total, 'f', 2));
    });

    // Seed lot combobox now
    refreshTransLotCombo();
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
    if (!m_chambreLot)
        return;

    const QString previous = m_chambreLot->currentText();

    m_chambreLot->blockSignals(true);
    m_chambreLot->clear();
    for (const auto &c : m_chambreModel->items())
        m_chambreLot->addItem(c.idChambre);

    const int idx = m_chambreLot->findText(previous);
    if (idx >= 0)
        m_chambreLot->setCurrentIndex(idx);

    m_chambreLot->blockSignals(false);
}

void MainWindow::refreshTransLotCombo()
{
    if (!m_lotTrans)
        return;

    const QString previous = m_lotTrans->currentText();
    m_lotTrans->blockSignals(true);
    m_lotTrans->clear();
    for (const auto &l : m_lotModel->items())
        m_lotTrans->addItem(l.idLot);
    const int idx = m_lotTrans->findText(previous);
    if (idx >= 0)
        m_lotTrans->setCurrentIndex(idx);
    m_lotTrans->blockSignals(false);
}

void MainWindow::refreshStats()
{
    const QLocale loc;

    const int cap = totalCapacity();
    const int used = totalUsed();

    const double occ = (cap > 0) ? (double(used) * 100.0 / double(cap)) : 0.0;
    const double avgDays = averageDurationDays();

    if (m_statOccupancy)
        m_statOccupancy->setText(loc.toString(occ, 'f', 1) + "%");
    if (m_statTotalVolume)
        m_statTotalVolume->setText(loc.toString(used));
    if (m_statAvgDuration)
        m_statAvgDuration->setText(loc.toString(avgDays, 'f', 1));
}

void MainWindow::refreshTransactionStats()
{
    const QLocale loc;

    double revenue = 0.0;
    QHash<QString, double> qtyBySpecies; // espece -> qty
    QHash<QString, double> revenueByClient;
    QHash<QString, double> revenueByMonth; // YYYY-MM -> revenue

    // Build lotId -> espece map
    QHash<QString, QString> lotToSpecies;
    for (const auto &l : m_lotModel->items())
        lotToSpecies.insert(l.idLot, l.espece);

    for (const auto &t : m_transModel->items())
    {
        revenue += t.total();
        const QString species = lotToSpecies.value(t.lot, QStringLiteral("?"));
        qtyBySpecies[species] += t.quantite;
        revenueByClient[t.client] += t.total();
        const QString month = t.date.isValid() ? t.date.toString("yyyy-MM") : QStringLiteral("?");
        revenueByMonth[month] += t.total();
    }

    // Top species by qty
    QString topSpeciesText = "—";
    if (!qtyBySpecies.isEmpty()) {
        QList<QPair<QString,double>> list;
        for (auto it = qtyBySpecies.constBegin(); it != qtyBySpecies.constEnd(); ++it)
            list.append(qMakePair(it.key(), it.value()));
        std::sort(list.begin(), list.end(), [](auto &a, auto &b){ return a.second > b.second; });
        QStringList parts;
        const int N = qMin(3, list.size());
        for (int i=0;i<N;++i)
            parts << QStringLiteral("%1 (%2)").arg(list[i].first).arg(QLocale().toString(list[i].second,'f',1));
        topSpeciesText = parts.join(", ");
    }

    // Best clients
    QString bestClientsText = "—";
    if (!revenueByClient.isEmpty()) {
        QList<QPair<QString,double>> list;
        for (auto it = revenueByClient.constBegin(); it != revenueByClient.constEnd(); ++it)
            list.append(qMakePair(it.key(), it.value()));
        std::sort(list.begin(), list.end(), [](auto &a, auto &b){ return a.second > b.second; });
        QStringList parts;
        const int N = qMin(3, list.size());
        for (int i=0;i<N;++i)
            parts << QStringLiteral("%1 (%2)").arg(list[i].first).arg(QLocale().toString(list[i].second,'f',2));
        bestClientsText = parts.join(", ");
    }

    if (m_statRevenue)
        m_statRevenue->setText(loc.toString(revenue, 'f', 2));
    if (m_statTopSpecies)
        m_statTopSpecies->setText(topSpeciesText);
    if (m_statBestClients)
        m_statBestClients->setText(bestClientsText);
}

void MainWindow::loadSelectedTransactionToForm()
{
    const int row = selectedSourceRow(m_tableTrans, m_transProxy);
    if (row < 0 || row >= m_transModel->rowCount())
        return;

    const auto &t = m_transModel->items().at(row);
    m_idVente->setText(t.idVente);
    m_client->setText(t.client);
    const int idx = m_lotTrans->findText(t.lot);
    if (idx >= 0) m_lotTrans->setCurrentIndex(idx);
    m_prixKg->setValue(t.prixKg);
    m_quantiteTrans->setValue(t.quantite);
    m_totalTrans->setText(QLocale().toString(t.total(), 'f', 2));
    m_dateTrans->setDateTime(t.date.isValid() ? t.date : QDateTime::currentDateTime());
    const int mIdx = m_modePaiement->findText(t.modePaiement);
    if (mIdx >= 0) m_modePaiement->setCurrentIndex(mIdx);
    const int sIdx = m_statutPaiement->findText(t.statutPaiement);
    if (sIdx >= 0) m_statutPaiement->setCurrentIndex(sIdx);
}

void MainWindow::onAddTransaction()
{
    if (m_lotModel->items().isEmpty()) {
        QMessageBox::warning(this, tr("Ajouter transaction"), tr("Ajoutez d'abord au moins un lot."));
        return;
    }

    Transaction t;
    t.idVente = m_idVente->text().trimmed();
    t.client = m_client->text().trimmed();
    t.lot = m_lotTrans->currentText().trimmed();
    t.prixKg = m_prixKg->value();
    t.quantite = m_quantiteTrans->value();
    t.date = m_dateTrans->dateTime();
    t.modePaiement = m_modePaiement->currentText();
    t.statutPaiement = m_statutPaiement->currentText();

    QString error;
    if (!m_transModel->addTransaction(t, &error)) {
        QMessageBox::warning(this, tr("Ajouter transaction"), error);
        return;
    }

    onClearTransactionForm();
}

void MainWindow::onUpdateTransaction()
{
    const int row = selectedSourceRow(m_tableTrans, m_transProxy);
    if (row < 0) {
        QMessageBox::information(this, tr("Modifier transaction"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    Transaction t;
    t.idVente = m_idVente->text().trimmed();
    t.client = m_client->text().trimmed();
    t.lot = m_lotTrans->currentText().trimmed();
    t.prixKg = m_prixKg->value();
    t.quantite = m_quantiteTrans->value();
    t.date = m_dateTrans->dateTime();
    t.modePaiement = m_modePaiement->currentText();
    t.statutPaiement = m_statutPaiement->currentText();

    QString error;
    if (!m_transModel->updateTransaction(row, t, &error))
        QMessageBox::warning(this, tr("Modifier transaction"), error);
}

void MainWindow::onDeleteTransaction()
{
    const int row = selectedSourceRow(m_tableTrans, m_transProxy);
    if (row < 0) {
        QMessageBox::information(this, tr("Supprimer transaction"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (QMessageBox::question(this, tr("Supprimer transaction"), tr("Supprimer la transaction sélectionnée ?")) != QMessageBox::Yes)
        return;

    QString error;
    if (!m_transModel->removeTransaction(row, &error))
        QMessageBox::warning(this, tr("Supprimer transaction"), error);

    onClearTransactionForm();
}

void MainWindow::onClearTransactionForm()
{
    m_idVente->clear();
    m_client->clear();
    m_lotTrans->setCurrentIndex(0);
    m_prixKg->setValue(0.0);
    m_quantiteTrans->setValue(0.0);
    m_totalTrans->setText("0.00");
    m_dateTrans->setDateTime(QDateTime::currentDateTime());
    m_modePaiement->setCurrentIndex(0);
    m_statutPaiement->setCurrentIndex(0);
}

void MainWindow::onExportTransactionsPdf()
{
    const int row = selectedSourceRow(m_tableTrans, m_transProxy);
    QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), "transactions.pdf", tr("PDF (*.pdf)"));
    if (filePath.isEmpty()) return;

    // If a row is selected, export single invoice-like table, else export whole table
    if (row >= 0) {
        const auto &t = m_transModel->items().at(row);
        QStandardItemModel tmp(0, 2);
        tmp.setHeaderData(0, Qt::Horizontal, tr("Champ"));
        tmp.setHeaderData(1, Qt::Horizontal, tr("Valeur"));
        QList<QList<QString>> rows = {
            {tr("ID Vente"), t.idVente},
            {tr("Client"), t.client},
            {tr("Lot"), t.lot},
            {tr("Prix/kg"), QLocale().toString(t.prixKg, 'f', 2)},
            {tr("Quantité"), QLocale().toString(t.quantite, 'f', 3)},
            {tr("Total"), QLocale().toString(t.total(), 'f', 2)},
            {tr("Date"), t.date.toString("yyyy-MM-dd HH:mm")},
            {tr("Mode paiement"), t.modePaiement},
            {tr("Statut paiement"), t.statutPaiement}
        };
        for (const auto &r : rows) {
            const int rIdx = tmp.rowCount();
            tmp.insertRow(rIdx);
            tmp.setData(tmp.index(rIdx,0), r[0]);
            tmp.setData(tmp.index(rIdx,1), r[1]);
        }

        QString error;
        if (!PdfExporter::exportTableToPdf(filePath, tr("FACTURE - %1").arg(t.idVente), &tmp, &error)) {
            QMessageBox::warning(this, tr("Exporter PDF"), error);
            return;
        }
    }
    else {
        QString error;
        if (!PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT — Transactions"), m_transProxy, &error)) {
            QMessageBox::warning(this, tr("Exporter PDF"), error);
            return;
        }
    }

    QMessageBox::information(this, tr("Exporter PDF"), tr("PDF exporté: %1").arg(filePath));
}

void MainWindow::loadSelectedChambreToForm()
{
    const int row = selectedSourceRow(m_tableChambres, m_chambreProxy);
    if (row < 0 || row >= m_chambreModel->rowCount())
        return;

    const auto &c = m_chambreModel->items().at(row);
    m_idChambre->setText(c.idChambre);
    m_capacite->setValue(c.capacite);
    m_tempConsigne->setValue(c.tempConsigne);
    m_emplacement->setText(c.emplacement);
    m_temperature->setValue(c.temperature);
}

void MainWindow::loadSelectedLotToForm()
{
    const int row = selectedSourceRow(m_tableLots, m_lotProxy);
    if (row < 0 || row >= m_lotModel->rowCount())
        return;

    const auto &l = m_lotModel->items().at(row);
    m_idLot->setText(l.idLot);
    m_espece->setText(l.espece);
    m_quantite->setValue(l.quantite);
    m_entree->setDateTime(l.dateEntree.isValid() ? l.dateEntree : QDateTime::currentDateTime());

    const bool hasSortie = l.dateSortie.isValid();
    m_sortieRenseignee->setChecked(hasSortie);
    m_sortie->setEnabled(hasSortie);
    m_sortie->setDateTime(hasSortie ? l.dateSortie : QDateTime::currentDateTime());

    const int idx = m_chambreLot->findText(l.idChambre);
    if (idx >= 0)
        m_chambreLot->setCurrentIndex(idx);
}

void MainWindow::onAddChambre()
{
    ChambreFroide c;
    c.idChambre = m_idChambre->text().trimmed();
    c.capacite = m_capacite->value();
    c.tempConsigne = m_tempConsigne->value();
    c.emplacement = m_emplacement->text().trimmed();
    c.temperature = m_temperature->value();

    QString error;
    if (!m_chambreModel->addChambre(c, &error))
    {
        QMessageBox::warning(this, tr("Ajouter chambre"), error);
        return;
    }

    onClearChambreForm();
}

void MainWindow::onUpdateChambre()
{
    const int row = selectedSourceRow(m_tableChambres, m_chambreProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Modifier chambre"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    ChambreFroide c;
    c.idChambre = m_idChambre->text().trimmed();
    c.capacite = m_capacite->value();
    c.tempConsigne = m_tempConsigne->value();
    c.emplacement = m_emplacement->text().trimmed();
    c.temperature = m_temperature->value();

    const int used = usedCapacityForChambre(m_chambreModel->items().at(row).idChambre);
    if (c.capacite < used)
    {
        QMessageBox::warning(this,
                             tr("Modifier chambre"),
                             tr("Capacité insuffisante: %1 déjà utilisé dans cette chambre.").arg(used));
        return;
    }

    QString error;
    if (!m_chambreModel->updateChambre(row, c, &error))
    {
        QMessageBox::warning(this, tr("Modifier chambre"), error);
        return;
    }
}

void MainWindow::onDeleteChambre()
{
    const int row = selectedSourceRow(m_tableChambres, m_chambreProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Supprimer chambre"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    const QString id = m_chambreModel->items().at(row).idChambre;
    for (const auto &l : m_lotModel->items())
    {
        if (l.idChambre.compare(id, Qt::CaseInsensitive) == 0)
        {
            QMessageBox::warning(this,
                                 tr("Supprimer chambre"),
                                 tr("Impossible: des lots référencent cette chambre."));
            return;
        }
    }

    if (QMessageBox::question(this, tr("Supprimer chambre"), tr("Supprimer la chambre sélectionnée ?")) != QMessageBox::Yes)
        return;

    QString error;
    if (!m_chambreModel->removeChambre(row, &error))
        QMessageBox::warning(this, tr("Supprimer chambre"), error);

    onClearChambreForm();
}

void MainWindow::onClearChambreForm()
{
    m_idChambre->clear();
    m_capacite->setValue(1);
    m_tempConsigne->setValue(0.0);
    m_emplacement->clear();
    m_temperature->setValue(0.0);
}

void MainWindow::onAddLot()
{
    if (m_chambreModel->items().isEmpty())
    {
        QMessageBox::warning(this, tr("Ajouter lot"), tr("Ajoutez d'abord une chambre froide."));
        return;
    }

    StockageLot l;
    l.idLot = m_idLot->text().trimmed();
    l.espece = m_espece->text().trimmed();
    l.quantite = m_quantite->value();
    l.dateEntree = m_entree->dateTime();
    l.dateSortie = m_sortieRenseignee->isChecked() ? m_sortie->dateTime() : QDateTime();
    l.idChambre = m_chambreLot->currentText().trimmed();

    // Capacity check
    const int chamberRow = m_chambreModel->indexOfId(l.idChambre);
    if (chamberRow < 0)
    {
        QMessageBox::warning(this, tr("Ajouter lot"), tr("Chambre invalide."));
        return;
    }

    const int used = usedCapacityForChambre(l.idChambre);
    const int cap = m_chambreModel->items().at(chamberRow).capacite;
    if (used + l.quantite > cap)
    {
        QMessageBox::warning(this,
                             tr("Ajouter lot"),
                             tr("Capacité dépassée (%1/%2).").arg(used + l.quantite).arg(cap));
        return;
    }

    QString error;
    if (!m_lotModel->addLot(l, &error))
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

    StockageLot l;
    l.idLot = m_idLot->text().trimmed();
    l.espece = m_espece->text().trimmed();
    l.quantite = m_quantite->value();
    l.dateEntree = m_entree->dateTime();
    l.dateSortie = m_sortieRenseignee->isChecked() ? m_sortie->dateTime() : QDateTime();
    l.idChambre = m_chambreLot->currentText().trimmed();

    const int chamberRow = m_chambreModel->indexOfId(l.idChambre);
    if (chamberRow < 0)
    {
        QMessageBox::warning(this, tr("Modifier lot"), tr("Chambre invalide."));
        return;
    }

    const int used = usedCapacityForChambre(l.idChambre, row);
    const int cap = m_chambreModel->items().at(chamberRow).capacite;
    if (used + l.quantite > cap)
    {
        QMessageBox::warning(this,
                             tr("Modifier lot"),
                             tr("Capacité dépassée (%1/%2)." ).arg(used + l.quantite).arg(cap));
        return;
    }

    QString error;
    if (!m_lotModel->updateLot(row, l, &error))
        QMessageBox::warning(this, tr("Modifier lot"), error);
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
    m_quantite->setValue(1);
    m_entree->setDateTime(QDateTime::currentDateTime());
    m_sortieRenseignee->setChecked(false);
    m_sortie->setDateTime(QDateTime::currentDateTime());
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
