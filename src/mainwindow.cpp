#include "mainwindow.h"

#include <QAbstractItemView>
#include <QApplication>
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
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QTableView>
#include <QToolButton>
#include <QVBoxLayout>

#include "pdfexporter.h"
#include "quaimodel.h"
#include "quaisortfilterproxymodel.h"

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

static QIcon makeHamburgerIcon(const QColor &color, int sizePx)
{
    const qreal dpr = qApp ? qApp->devicePixelRatio() : 1.0;
    QPixmap pm(qRound(sizePx * dpr), qRound(sizePx * dpr));
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing, true);

    QPen pen(color, 2.2, Qt::SolidLine, Qt::RoundCap);
    p.setPen(pen);

    const int pad = 3;
    const int w = sizePx - pad * 2;
    const int x1 = pad;
    const int x2 = pad + w;
    const int y1 = qRound(sizePx * 0.30);
    const int y2 = qRound(sizePx * 0.50);
    const int y3 = qRound(sizePx * 0.70);

    p.drawLine(QPointF(x1, y1), QPointF(x2, y1));
    p.drawLine(QPointF(x1, y2), QPointF(x2, y2));
    p.drawLine(QPointF(x1, y3), QPointF(x2, y3));

    return QIcon(pm);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("VISION SIGHT"));
    resize(1366, 768);

    m_model = new QuaiModel(this);
    m_proxy = new QuaiSortFilterProxyModel(this);
    m_proxy->setSourceModel(m_model);

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

    // Logo from Qt resources. Add the file at: resources/vision_sight.png
    // and ensure it's listed in resources/resources.qrc.
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

    const QStringList items = {tr("Tableau de bord"), tr("Quais"), tr("Navires"), tr("Statistiques"), tr("Paramètres")};
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
    auto *h2 = new QLabel(tr("Module: VISION SIGHT  Entité: Quai"), page);
    h2->setObjectName("HeaderMuted");

    titleCol->addWidget(h1);
    titleCol->addWidget(h2);

    headerRow->addLayout(titleCol);
    headerRow->addStretch();

    auto *menuBtn = new QToolButton(page);
    menuBtn->setText(QString());
    menuBtn->setIcon(makeHamburgerIcon(QColor("#e6eef6"), 18));
    menuBtn->setIconSize(QSize(18, 18));
    menuBtn->setObjectName("MenuButton");
    menuBtn->setFixedSize(36, 36);
    headerRow->addWidget(menuBtn);

    pageLayout->addLayout(headerRow);

    // Card: form + map
    auto *formCard = makeCard(page, "Card");
    formCard->setMinimumHeight(260);
    auto *formLayout = new QHBoxLayout(formCard);
    formLayout->setContentsMargins(18, 18, 18, 18);
    formLayout->setSpacing(18);

    // Left: form
    auto *leftCol = new QVBoxLayout();
    leftCol->setSpacing(10);

    auto *section = new QLabel(tr("Quai"), formCard);
    section->setObjectName("SectionTitle");
    leftCol->addWidget(section);

    auto *grid = new QFormLayout();
    grid->setLabelAlignment(Qt::AlignLeft);
    grid->setFormAlignment(Qt::AlignTop);
    grid->setHorizontalSpacing(14);
    grid->setVerticalSpacing(4);

    m_idQuai = new QLineEdit(formCard);
    m_idQuai->setPlaceholderText("Q-104");
    m_idQuai->setObjectName("Input");

    m_matricule = new QLineEdit(formCard);
    m_matricule->setPlaceholderText("NAV-900");
    m_matricule->setObjectName("Input");

    m_arrivee = new QDateTimeEdit(QDateTime::currentDateTime(), formCard);
    m_arrivee->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_arrivee->setCalendarPopup(true);
    m_arrivee->setObjectName("Input");

    m_depart = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(2 * 3600), formCard);
    m_depart->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_depart->setCalendarPopup(true);
    m_depart->setObjectName("Input");

    m_etat = new QComboBox(formCard);
    m_etat->addItems({tr("Libre"), tr("Occupé"), tr("En maintenance")});
    m_etat->setObjectName("Input");

    m_prix = new QDoubleSpinBox(formCard);
    m_prix->setRange(0.0, 1e9);
    m_prix->setDecimals(2);
    m_prix->setSingleStep(50.0);
    m_prix->setObjectName("Input");

    auto *labId = new QLabel(tr("IDQuai"), formCard);
    labId->setObjectName("Muted");
    auto *labMat = new QLabel(tr("Matricule"), formCard);
    labMat->setObjectName("Muted");
    auto *labArr = new QLabel(tr("Date/Heure arrivée"), formCard);
    labArr->setObjectName("Muted");
    auto *labDep = new QLabel(tr("Date/Heure départ"), formCard);
    labDep->setObjectName("Muted");
    auto *labEtat = new QLabel(tr("État quai"), formCard);
    labEtat->setObjectName("Muted");
    auto *labPrix = new QLabel(tr("Prix location (DT)"), formCard);
    labPrix->setObjectName("Muted");

    grid->addRow(labId, m_idQuai);
    grid->addRow(labMat, m_matricule);
    grid->addRow(labArr, m_arrivee);
    grid->addRow(labDep, m_depart);
    grid->addRow(labEtat, m_etat);
    grid->addRow(labPrix, m_prix);

    leftCol->addLayout(grid);

    auto *btnRow = new QHBoxLayout();
    btnRow->setSpacing(10);

    m_btnAdd = new QPushButton(tr("Ajouter"), formCard);
    m_btnAdd->setObjectName("PrimaryButton");
    m_btnAdd->setCursor(Qt::PointingHandCursor);

    m_btnUpdate = new QPushButton(tr("Modifier"), formCard);
    m_btnUpdate->setObjectName("SecondaryButton");
    m_btnUpdate->setCursor(Qt::PointingHandCursor);

    m_btnDelete = new QPushButton(tr("Supprimer"), formCard);
    m_btnDelete->setObjectName("DangerButton");
    m_btnDelete->setCursor(Qt::PointingHandCursor);

    m_btnClear = new QPushButton(tr("Réinitialiser"), formCard);
    m_btnClear->setObjectName("GhostButton");
    m_btnClear->setCursor(Qt::PointingHandCursor);

    btnRow->addWidget(m_btnAdd);
    btnRow->addWidget(m_btnUpdate);
    btnRow->addWidget(m_btnDelete);
    btnRow->addWidget(m_btnClear);
    btnRow->addStretch();

    leftCol->addLayout(btnRow);

    formLayout->addLayout(leftCol, 1);

    // Right: map placeholder
    auto *mapBox = new QFrame(formCard);
    mapBox->setObjectName("MapBox");
    mapBox->setMinimumWidth(280);

    auto *mapLayout = new QVBoxLayout(mapBox);
    mapLayout->setContentsMargins(12, 12, 12, 12);

    auto *mapPreview = new QLabel(mapBox);
    mapPreview->setObjectName("MapPreview");
    mapPreview->setMinimumHeight(170);
    mapPreview->setAlignment(Qt::AlignCenter);
    mapPreview->setText(tr("[Carte / Zone d'accostage]"));

    auto *mapCaption = new QLabel(tr("Zone d'accostage"), mapBox);
    mapCaption->setObjectName("Muted");
    mapCaption->setAlignment(Qt::AlignHCenter);

    mapLayout->addWidget(mapPreview, 1);
    mapLayout->addWidget(mapCaption);

    formLayout->addWidget(mapBox);

    pageLayout->addWidget(formCard);

    // Filters bar
    auto *filterCard = makeCard(page, "CardSmall");
    filterCard->setFixedHeight(78);
    auto *filterLayout = new QHBoxLayout(filterCard);
    filterLayout->setContentsMargins(16, 14, 16, 14);
    filterLayout->setSpacing(10);

    m_searchMatricule = new QLineEdit(filterCard);
    m_searchMatricule->setObjectName("Input");
    m_searchMatricule->setPlaceholderText(tr("Recherche navire (matricule)"));

    m_searchDate = new QLineEdit(filterCard);
    m_searchDate->setObjectName("Input");
    m_searchDate->setPlaceholderText(tr("Recherche date (AAAA-MM-JJ)"));

    m_searchQuai = new QLineEdit(filterCard);
    m_searchQuai->setObjectName("Input");
    m_searchQuai->setPlaceholderText(tr("Recherche quai (IDQuai)"));

    m_btnPdf = new QPushButton(tr("Exporter PDF"), filterCard);
    m_btnPdf->setObjectName("AltPrimaryButton");
    m_btnPdf->setCursor(Qt::PointingHandCursor);

    filterLayout->addWidget(m_searchMatricule, 1);
    filterLayout->addWidget(m_searchDate, 1);
    filterLayout->addWidget(m_searchQuai, 1);
    filterLayout->addWidget(m_btnPdf);

    pageLayout->addWidget(filterCard);

    // Table
    auto *tableCard = makeCard(page, "Card");
    auto *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(12, 12, 12, 12);

    m_table = new QTableView(tableCard);
    m_table->setModel(m_proxy);
    m_table->setSortingEnabled(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(false);
    m_table->setShowGrid(false);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    m_table->setObjectName("Table");

    tableLayout->addWidget(m_table);
    pageLayout->addWidget(tableCard);

    // Stats row
    auto *statsRow = new QHBoxLayout();
    statsRow->setSpacing(12);

    auto makeStat = [&](const QString &labelText, QLabel **valueOut, const QString &objName) {
        auto *card = makeCard(page, "StatCard");
        card->setMinimumHeight(110);
        auto *vl = new QVBoxLayout(card);
        vl->setContentsMargins(14, 12, 14, 12);
        vl->setSpacing(6);

        auto *lab = new QLabel(labelText, card);
        lab->setObjectName("Muted");

        auto *val = new QLabel("", card);
        val->setObjectName(objName);
        QFont f = val->font();
        f.setPixelSize(26);
        f.setBold(true);
        val->setFont(f);

        auto *sub = new QLabel(tr("Mise à jour automatique"), card);
        sub->setObjectName("AccentText");

        vl->addWidget(lab);
        vl->addWidget(val);
        vl->addWidget(sub);

        statsRow->addWidget(card, 1);
        *valueOut = val;
    };

    makeStat(tr("Quais occupés"), &m_statQuaisOcc, "StatValue");
    makeStat(tr("Retards"), &m_statRetards, "StatValue");
    makeStat(tr("Arrivées (48h)"), &m_statArrivees, "StatValue");
    makeStat(tr("Top quai"), &m_statTopQuai, "StatValue");

    pageLayout->addLayout(statsRow);
    pageLayout->addStretch();

    rootLayout->addWidget(scroll);
}

void MainWindow::applyTheme()
{
    // Palette based on the provided screenshot.
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
        QLabel#Logo { background: #ffffff; color: #132437; border: 2px solid #ffffff; border-radius: 39px; }
        QLabel#BrandTitle { color: #e6eef6; font-size: 16px; font-weight: 800; letter-spacing: 1px; }
        QLabel#BrandSub { color: #9bb0c3; font-size: 13px; }

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

        QToolButton#MenuButton {
            color: #e6eef6;
            background: #132437;
            border: 1px solid #23374e;
            border-radius: 10px;
            font-size: 18px;
        }
        QToolButton#MenuButton:hover { background: #18304a; }

        QFrame#Card, QFrame#CardSmall, QFrame#StatCard {
            background: #132437;
            border: 1px solid #23374e;
            border-radius: 16px;
        }
        QFrame#CardSmall { border-radius: 12px; }
        QFrame#StatCard { border-radius: 14px; }

        QFrame#MapBox {
            background: #101c2c;
            border: 1px solid #23374e;
            border-radius: 12px;
        }
        QLabel#MapPreview { color: #9bb0c3; }

        QLineEdit#Input, QComboBox#Input, QDateTimeEdit#Input, QDoubleSpinBox#Input {
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

        /* Swap colors: Ajouter gets the Modifier look */
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

        /* Swap colors: Supprimer gets the old Ajouter blue */
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
    connect(m_btnAdd, &QPushButton::clicked, this, &MainWindow::onAdd);
    connect(m_btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdate);
    connect(m_btnDelete, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(m_btnClear, &QPushButton::clicked, this, &MainWindow::onClearForm);
    connect(m_btnPdf, &QPushButton::clicked, this, &MainWindow::onExportPdf);

    connect(m_searchMatricule, &QLineEdit::textChanged, m_proxy, &QuaiSortFilterProxyModel::setSearchMatricule);
    connect(m_searchQuai, &QLineEdit::textChanged, m_proxy, &QuaiSortFilterProxyModel::setSearchIdQuai);
    connect(m_searchDate, &QLineEdit::textChanged, m_proxy, &QuaiSortFilterProxyModel::setSearchDate);

    connect(m_model, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::modelReset, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &MainWindow::refreshStats);

    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::loadSelectedRowToForm);
}

void MainWindow::refreshStats()
{
    const auto &items = m_model->items();

    int occ = 0;
    int retards = 0;
    int arrivals48 = 0;

    QHash<QString, int> arrivalsByQuai;

    const QDateTime now = QDateTime::currentDateTime();
    const QDateTime from48 = now.addSecs(-48 * 3600);

    for (const auto &q : items)
    {
        if (q.etatQuai.compare("Occupé", Qt::CaseInsensitive) == 0)
            ++occ;
        if (q.retardMinutes() > 0)
            ++retards;
        if (q.dateHeureArrivee >= from48 && q.dateHeureArrivee <= now)
            ++arrivals48;

        arrivalsByQuai[q.idQuai] += 1;
    }

    QString topQuai = "";
    int topCount = 0;
    for (auto it = arrivalsByQuai.constBegin(); it != arrivalsByQuai.constEnd(); ++it)
    {
        if (it.value() > topCount)
        {
            topCount = it.value();
            topQuai = it.key();
        }
    }

    if (m_statQuaisOcc)
        m_statQuaisOcc->setText(QString::number(occ));
    if (m_statRetards)
        m_statRetards->setText(QString::number(retards));
    if (m_statArrivees)
        m_statArrivees->setText(QString::number(arrivals48));
    if (m_statTopQuai)
        m_statTopQuai->setText(topQuai);
}

void MainWindow::loadSelectedRowToForm()
{
    const QModelIndex proxyIndex = m_table->currentIndex();
    if (!proxyIndex.isValid())
        return;

    const QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);
    const int row = sourceIndex.row();
    if (row < 0 || row >= m_model->rowCount())
        return;

    const auto &q = m_model->items().at(row);

    m_idQuai->setText(q.idQuai);
    m_matricule->setText(q.matricule);
    m_arrivee->setDateTime(q.dateHeureArrivee);
    m_depart->setDateTime(q.dateHeureDepart);

    const int idx = m_etat->findText(q.etatQuai);
    m_etat->setCurrentIndex(idx >= 0 ? idx : 0);

    m_prix->setValue(q.prixLocation);
}

void MainWindow::onAdd()
{
    Quai q;
    q.idQuai = m_idQuai->text();
    q.matricule = m_matricule->text();
    q.dateHeureArrivee = m_arrivee->dateTime();
    q.dateHeureDepart = m_depart->dateTime();
    q.etatQuai = m_etat->currentText();
    q.prixLocation = m_prix->value();

    QString error;
    if (!m_model->addQuai(q, &error))
    {
        QMessageBox::warning(this, tr("Ajouter"), error);
        return;
    }

    onClearForm();
    refreshStats();
}

void MainWindow::onUpdate()
{
    const QModelIndex proxyIndex = m_table->currentIndex();
    if (!proxyIndex.isValid())
    {
        QMessageBox::information(this, tr("Modifier"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    const int row = m_proxy->mapToSource(proxyIndex).row();

    Quai q;
    q.idQuai = m_idQuai->text();
    q.matricule = m_matricule->text();
    q.dateHeureArrivee = m_arrivee->dateTime();
    q.dateHeureDepart = m_depart->dateTime();
    q.etatQuai = m_etat->currentText();
    q.prixLocation = m_prix->value();

    QString error;
    if (!m_model->updateQuai(row, q, &error))
    {
        QMessageBox::warning(this, tr("Modifier"), error);
        return;
    }

    refreshStats();
}

void MainWindow::onDelete()
{
    const QModelIndex proxyIndex = m_table->currentIndex();
    if (!proxyIndex.isValid())
    {
        QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (QMessageBox::question(this, tr("Supprimer"), tr("Supprimer l'enregistrement sélectionné ?")) != QMessageBox::Yes)
        return;

    const int row = m_proxy->mapToSource(proxyIndex).row();
    m_model->removeQuai(row);
    onClearForm();
    refreshStats();
}

void MainWindow::onClearForm()
{
    m_idQuai->clear();
    m_matricule->clear();
    m_arrivee->setDateTime(QDateTime::currentDateTime());
    m_depart->setDateTime(QDateTime::currentDateTime().addSecs(2 * 3600));
    m_etat->setCurrentIndex(0);
    m_prix->setValue(0.0);
}

void MainWindow::onExportPdf()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), "quais.pdf", tr("PDF (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT  Quais"), m_proxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter PDF"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter PDF"), tr("PDF exporté: %1").arg(filePath));
}
