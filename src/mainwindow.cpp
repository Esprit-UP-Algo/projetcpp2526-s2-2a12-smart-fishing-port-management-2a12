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
#include <QSizePolicy>

#include "statcircle.h"

#include <QPixmap>

#include "chambrefroidemodel.h"
#include "chambrefroidesortfilterproxymodel.h"
#include "pdfexporter.h"
#include "stockagelotmodel.h"
#include "stockagelotsortfilterproxymodel.h"

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
    auto *h2 = new QLabel(tr("Module: Stockage Frigorifique · Entité: ChambreFroide"), page);
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

    // Top charts row (placeholders to match the picture repartition)
    auto *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(12);
    chartsRow->addWidget(makeChartPlaceholder(tr("Volume par espèce"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Poids total journalier"), page), 1);
    chartsRow->addWidget(makeChartPlaceholder(tr("Zones de pêche fréquentes"), page), 1);
    pageLayout->addLayout(chartsRow);

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

    auto *chFormGrid = new QFormLayout();
    chFormGrid->setLabelAlignment(Qt::AlignLeft);
    chFormGrid->setFormAlignment(Qt::AlignTop);
    chFormGrid->setHorizontalSpacing(14);
    chFormGrid->setVerticalSpacing(4);

    m_idChambre = new QLineEdit(chCard);
    m_idChambre->setObjectName("Input");
    m_idChambre->setPlaceholderText("CF-01");

    m_capacite = new QSpinBox(chCard);
    m_capacite->setObjectName("Input");
    m_capacite->setRange(1, 100000000);

    m_tempConsigne = new QDoubleSpinBox(chCard);
    m_tempConsigne->setObjectName("Input");
    m_tempConsigne->setRange(-100.0, 50.0);
    m_tempConsigne->setDecimals(1);
    m_tempConsigne->setSingleStep(0.5);

    m_emplacement = new QLineEdit(chCard);
    m_emplacement->setObjectName("Input");
    m_emplacement->setPlaceholderText("Zone A");

    m_temperature = new QDoubleSpinBox(chCard);
    m_temperature->setObjectName("Input");
    m_temperature->setRange(-100.0, 50.0);
    m_temperature->setDecimals(1);
    m_temperature->setSingleStep(0.5);

    auto *labId = new QLabel(tr("IDChambre"), chCard);
    labId->setObjectName("Muted");
    auto *labCap = new QLabel(tr("Capacité"), chCard);
    labCap->setObjectName("Muted");
    auto *labCons = new QLabel(tr("TempConsigne"), chCard);
    labCons->setObjectName("Muted");
    auto *labEmp = new QLabel(tr("Emplacement"), chCard);
    labEmp->setObjectName("Muted");
    auto *labTemp = new QLabel(tr("Température"), chCard);
    labTemp->setObjectName("Muted");

    chFormGrid->addRow(labId, m_idChambre);
    chFormGrid->addRow(labCap, m_capacite);
    chFormGrid->addRow(labCons, m_tempConsigne);
    chFormGrid->addRow(labEmp, m_emplacement);
    chFormGrid->addRow(labTemp, m_temperature);

    chLayout->addLayout(chFormGrid);

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

    chLayout->addLayout(chBtnRow);

    auto *chFilterRow = new QHBoxLayout();
    chFilterRow->setSpacing(10);
    m_searchChambreId = new QLineEdit(chCard);
    m_searchChambreId->setObjectName("Input");
    m_searchChambreId->setPlaceholderText(tr("Recherche chambre (ID)"));

    m_searchChambreEmpl = new QLineEdit(chCard);
    m_searchChambreEmpl->setObjectName("Input");
    m_searchChambreEmpl->setPlaceholderText(tr("Recherche emplacement"));

    chFilterRow->addWidget(m_searchChambreId, 1);
    chFilterRow->addWidget(m_searchChambreEmpl, 1);
    chLayout->addLayout(chFilterRow);

    m_tableChambres = new QTableView(chCard);
    m_tableChambres->setObjectName("Table");
    m_tableChambres->setModel(m_chambreProxy);
    m_tableChambres->setSortingEnabled(true);
    m_tableChambres->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableChambres->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableChambres->setShowGrid(false);
    m_tableChambres->verticalHeader()->setVisible(false);
    m_tableChambres->horizontalHeader()->setStretchLastSection(true);
    m_tableChambres->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    // Ensure rows are visible: set a sensible default row height,
    // enable alternating row colors, size columns to contents and
    // make the view expand vertically inside its layout.
    m_tableChambres->verticalHeader()->setDefaultSectionSize(28);
    m_tableChambres->setAlternatingRowColors(true);
    m_tableChambres->resizeColumnsToContents();
    m_tableChambres->setMinimumHeight(180);
    m_tableChambres->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableChambres->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    chLayout->addWidget(m_tableChambres, 1);

    midRow->addWidget(chCard, 2);

    // Stats card
    auto *statsCard = makeCard(page, "Card");
    auto *stLayout = new QVBoxLayout(statsCard);
    stLayout->setContentsMargins(14, 12, 14, 12);
    stLayout->setSpacing(10);

    auto *stTitle = new QLabel(tr("Statistiques"), statsCard);
    stTitle->setObjectName("SectionTitle");
    stLayout->addWidget(stTitle);

    // Layout: circle on left, stat lines on right
    auto *statRow = new QHBoxLayout();
    statRow->setSpacing(12);

    m_occupancyCircle = new StatCircleWidget(statsCard);
    m_occupancyCircle->setMinimumSize(120, 120);
    m_occupancyCircle->setLabel(tr("Occupation"));
    statRow->addWidget(m_occupancyCircle, 0, Qt::AlignLeft);

    auto *statValuesWidget = new QWidget(statsCard);
    auto *statValuesLayout = new QVBoxLayout(statValuesWidget);
    statValuesLayout->setContentsMargins(0, 0, 0, 0);
    statValuesLayout->setSpacing(8);

    auto makeStatLine = [&](const QString &labelText, QLabel **out, QBoxLayout *target = nullptr) {
        auto *row = new QHBoxLayout();
        row->setSpacing(10);

        auto *lab = new QLabel(labelText, statsCard);
        lab->setObjectName("Muted");

        auto *val = new QLabel("—", statsCard);
        val->setObjectName("StatValue");
        QFont f = val->font();
        f.setPixelSize(22);
        f.setBold(true);
        val->setFont(f);
        val->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        row->addWidget(lab, 1);
        row->addWidget(val);

        if (target)
            target->addLayout(row);
        else
            stLayout->addLayout(row);

        *out = val;
    };

    makeStatLine(tr("Taux d'occupation"), &m_statOccupancy, statValuesLayout);
    makeStatLine(tr("Volumes stockés"), &m_statTotalVolume, statValuesLayout);
    makeStatLine(tr("Durée moyenne (jours)"), &m_statAvgDuration, statValuesLayout);

    statRow->addWidget(statValuesWidget, 1);
    stLayout->addLayout(statRow);

    auto *stHint = new QLabel(tr("Mise à jour automatique"), statsCard);
    stHint->setObjectName("AccentText");
    stLayout->addWidget(stHint);
    stLayout->addStretch();

    midRow->addWidget(statsCard, 1);

    pageLayout->addLayout(midRow);

    // Bottom: lots CRUD + filters + table
    auto *lotCard = makeCard(page, "Card");
    auto *lotLayout = new QVBoxLayout(lotCard);
    lotLayout->setContentsMargins(14, 12, 14, 12);
    lotLayout->setSpacing(10);

    auto *lotTitle = new QLabel(tr("Lots stockés"), lotCard);
    lotTitle->setObjectName("SectionTitle");
    lotLayout->addWidget(lotTitle);

    auto *lotFormGrid = new QFormLayout();
    lotFormGrid->setLabelAlignment(Qt::AlignLeft);
    lotFormGrid->setFormAlignment(Qt::AlignTop);
    lotFormGrid->setHorizontalSpacing(14);
    lotFormGrid->setVerticalSpacing(4);

    m_idLot = new QLineEdit(lotCard);
    m_idLot->setObjectName("Input");
    m_idLot->setPlaceholderText("LOT-100");

    m_espece = new QLineEdit(lotCard);
    m_espece->setObjectName("Input");
    m_espece->setPlaceholderText(tr("Sardine"));

    m_quantite = new QSpinBox(lotCard);
    m_quantite->setObjectName("Input");
    m_quantite->setRange(1, 100000000);

    m_entree = new QDateTimeEdit(QDateTime::currentDateTime(), lotCard);
    m_entree->setObjectName("Input");
    m_entree->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_entree->setCalendarPopup(true);

    auto *sortieWrap = new QWidget(lotCard);
    auto *sortieLayout = new QHBoxLayout(sortieWrap);
    sortieLayout->setContentsMargins(0, 0, 0, 0);
    sortieLayout->setSpacing(10);

    m_sortieRenseignee = new QCheckBox(tr("Renseignée"), sortieWrap);
    m_sortieRenseignee->setObjectName("Check");

    m_sortie = new QDateTimeEdit(QDateTime::currentDateTime(), sortieWrap);
    m_sortie->setObjectName("Input");
    m_sortie->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_sortie->setCalendarPopup(true);
    m_sortie->setEnabled(false);

    sortieLayout->addWidget(m_sortieRenseignee);
    sortieLayout->addWidget(m_sortie, 1);

    m_chambreLot = new QComboBox(lotCard);
    m_chambreLot->setObjectName("Input");

    auto *labLot = new QLabel(tr("Lot"), lotCard);
    labLot->setObjectName("Muted");
    auto *labEsp = new QLabel(tr("Espèce"), lotCard);
    labEsp->setObjectName("Muted");
    auto *labQ = new QLabel(tr("Quantité"), lotCard);
    labQ->setObjectName("Muted");
    auto *labEnt = new QLabel(tr("Date entrée"), lotCard);
    labEnt->setObjectName("Muted");
    auto *labSor = new QLabel(tr("Date sortie"), lotCard);
    labSor->setObjectName("Muted");
    auto *labCh = new QLabel(tr("Chambre"), lotCard);
    labCh->setObjectName("Muted");

    lotFormGrid->addRow(labLot, m_idLot);
    lotFormGrid->addRow(labEsp, m_espece);
    lotFormGrid->addRow(labQ, m_quantite);
    lotFormGrid->addRow(labEnt, m_entree);
    lotFormGrid->addRow(labSor, sortieWrap);
    lotFormGrid->addRow(labCh, m_chambreLot);

    lotLayout->addLayout(lotFormGrid);

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

    auto *lotFilterCard = makeCard(lotCard, "CardSmall");
    lotFilterCard->setFixedHeight(78);
    auto *lotFilterLayout = new QHBoxLayout(lotFilterCard);
    lotFilterLayout->setContentsMargins(16, 14, 16, 14);
    lotFilterLayout->setSpacing(10);

    m_searchLot = new QLineEdit(lotFilterCard);
    m_searchLot->setObjectName("Input");
    m_searchLot->setPlaceholderText(tr("Recherche lot"));

    m_searchLotChambre = new QLineEdit(lotFilterCard);
    m_searchLotChambre->setObjectName("Input");
    m_searchLotChambre->setPlaceholderText(tr("Recherche chambre"));

    m_searchLotEspece = new QLineEdit(lotFilterCard);
    m_searchLotEspece->setObjectName("Input");
    m_searchLotEspece->setPlaceholderText(tr("Recherche espèce"));

    m_btnExportPdf = new QPushButton(tr("Exporter PDF"), lotFilterCard);
    m_btnExportPdf->setObjectName("AltPrimaryButton");
    m_btnExportPdf->setCursor(Qt::PointingHandCursor);

    lotFilterLayout->addWidget(m_searchLot, 1);
    lotFilterLayout->addWidget(m_searchLotChambre, 1);
    lotFilterLayout->addWidget(m_searchLotEspece, 1);
    lotFilterLayout->addWidget(m_btnExportPdf);

    lotLayout->addWidget(lotFilterCard);

    m_tableLots = new QTableView(lotCard);
    m_tableLots->setObjectName("Table");
    m_tableLots->setModel(m_lotProxy);
    m_tableLots->setSortingEnabled(true);
    m_tableLots->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableLots->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableLots->setShowGrid(false);
    m_tableLots->verticalHeader()->setVisible(false);
    m_tableLots->horizontalHeader()->setStretchLastSection(true);
    // Ensure rows are visible and the view expands vertically.
    m_tableLots->verticalHeader()->setDefaultSectionSize(28);
    m_tableLots->setAlternatingRowColors(true);
    m_tableLots->resizeColumnsToContents();
    m_tableLots->setMinimumHeight(200);
    m_tableLots->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_tableLots->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableLots->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

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
        QLabel#Logo { background: #ffffff; color: #132437; border: 2px solid #ffffff; border-radius: 39px; font-weight: 800; }
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
    // Chambre
    connect(m_btnAddChambre, &QPushButton::clicked, this, &MainWindow::onAddChambre);
    connect(m_btnUpdateChambre, &QPushButton::clicked, this, &MainWindow::onUpdateChambre);
    connect(m_btnDeleteChambre, &QPushButton::clicked, this, &MainWindow::onDeleteChambre);
    connect(m_btnClearChambre, &QPushButton::clicked, this, &MainWindow::onClearChambreForm);

    connect(m_searchChambreId, &QLineEdit::textChanged, m_chambreProxy, &ChambreFroideSortFilterProxyModel::setSearchIdChambre);
    connect(m_searchChambreEmpl, &QLineEdit::textChanged, m_chambreProxy, &ChambreFroideSortFilterProxyModel::setSearchEmplacement);

    connect(m_tableChambres->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::loadSelectedChambreToForm);

    // Lots
    connect(m_btnAddLot, &QPushButton::clicked, this, &MainWindow::onAddLot);
    connect(m_btnUpdateLot, &QPushButton::clicked, this, &MainWindow::onUpdateLot);
    connect(m_btnDeleteLot, &QPushButton::clicked, this, &MainWindow::onDeleteLot);
    connect(m_btnClearLot, &QPushButton::clicked, this, &MainWindow::onClearLotForm);
    connect(m_btnExportPdf, &QPushButton::clicked, this, &MainWindow::onExportLotsPdf);

    connect(m_searchLot, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchLot);
    connect(m_searchLotChambre, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchChambre);
    connect(m_searchLotEspece, &QLineEdit::textChanged, m_lotProxy, &StockageLotSortFilterProxyModel::setSearchEspece);

    connect(m_tableLots->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::loadSelectedLotToForm);

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

void MainWindow::refreshStats()
{
    const QLocale loc;

    const int cap = totalCapacity();
    const int used = totalUsed();

    const double occ = (cap > 0) ? (double(used) * 100.0 / double(cap)) : 0.0;
    const double avgDays = averageDurationDays();

    if (m_statOccupancy)
        m_statOccupancy->setText(loc.toString(occ, 'f', 1) + "%");
    if (m_occupancyCircle)
        m_occupancyCircle->setValue(occ / 100.0);
    if (m_statTotalVolume)
        m_statTotalVolume->setText(loc.toString(used));
    if (m_statAvgDuration)
        m_statAvgDuration->setText(loc.toString(avgDays, 'f', 1));
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
