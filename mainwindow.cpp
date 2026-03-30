#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "transactiondialog.h"
#include "connection.h"

#include <QDate>
#include <QFileDialog>
#include <QLocale>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QHeaderView>
#include <QInputDialog>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QScrollBar>
#include <QStyle>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QRegularExpression>
#include <QStatusBar>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QEventLoop>
#include <QTimer>
#include <algorithm>
#include <cmath>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Ventes dropdown starts collapsed
    ui->ventesMenu->setVisible(false);
    ui->ventesMenu->setMaximumHeight(0);

    // Mark submenu buttons as 'subitem' via dynamic property (for stylesheet selectors)
    for (auto *btn : {ui->btnVentesAffichage, ui->btnVentesStatistique, ui->btnVentesAvancees}) {
        btn->setProperty("subitem", true);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
    // Final fallback: if mapping returned nothing but table has rows, try SELECT * and populate m_transactions
    if (m_transactions.isEmpty()) {
        QSqlDatabase db2 = Connection::createInstance().database();
        if (db2.isOpen()) {
            // try to find owner to qualify table
            QString qualified = m_tableName;
            QSqlQuery who(db2);
            if (who.exec(QString("SELECT OWNER FROM ALL_TABLES WHERE TABLE_NAME='%1' AND ROWNUM=1").arg(m_tableName.toUpper())) && who.next()) {
                QString owner = who.value(0).toString();
                if (!owner.isEmpty()) qualified = owner + "." + m_tableName;
            }
            QSqlQuery cnt(db2);
            if (cnt.exec(QString("SELECT COUNT(*) FROM %1").arg(qualified)) && cnt.next()) {
                int rows = cnt.value(0).toInt();
                if (rows > 0) {
                    QSqlQuery q2(db2);
                    if (q2.exec(QString("SELECT * FROM %1").arg(qualified))) {
                        QSqlRecord rec = q2.record();
                        auto findIndex = [&](const QString &logical)->int {
                            QString nk = logical.toUpper(); nk.remove('_');
                            for (int i = 0; i < rec.count(); ++i) {
                                QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                if (rnn == nk) return i;
                            }
                            for (int i = 0; i < rec.count(); ++i) {
                                QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                if (rnn.contains(nk) || nk.contains(rnn)) return i;
                            }
                            return -1;
                        };
                        int idxNum = findIndex("NUMFACTURE");
                        int idxPecheur = findIndex("PECHEUR"); if (idxPecheur < 0) idxPecheur = findIndex("PECHER");
                        int idxLot = findIndex("LOT");
                        int idxPrix = findIndex("PRIX_KG"); if (idxPrix < 0) idxPrix = findIndex("PRIXKG");
                        int idxQte = findIndex("QUANTITE");
                        int idxTotal = findIndex("TOTAL");
                        int idxDate = findIndex("DATETRANSACTION"); if (idxDate < 0) idxDate = findIndex("DATE_TRANSACTION");
                        int idxMode = findIndex("MODEPAIEMENT");
                        int idxStatut = findIndex("STATUTPAIEMENT");
                        while (q2.next()) {
                            Transaction t2;
                            if (idxNum >= 0) t2.numFacture = q2.value(idxNum).toString();
                            if (idxPecheur >= 0) t2.pecheur = q2.value(idxPecheur).toString();
                            if (idxLot >= 0) t2.lot = q2.value(idxLot).toString();
                            if (idxPrix >= 0) t2.prixKg = q2.value(idxPrix).toDouble();
                            if (idxQte >= 0) t2.quantite = q2.value(idxQte).toDouble();
                            if (idxTotal >= 0) t2.total = q2.value(idxTotal).toDouble();
                            if (idxDate >= 0) t2.dateTransaction = q2.value(idxDate).toDateTime();
                            if (idxMode >= 0) t2.modePaiement = q2.value(idxMode).toString();
                            if (idxStatut >= 0) t2.statutPaiement = q2.value(idxStatut).toString();
                            // resolve numeric pecheur id to name if possible
                            if (m_pecheurIsNumeric) {
                                bool okid = false; int pid = t2.pecheur.toInt(&okid);
                                if (okid) {
                                    QString uname = userNameForId(pid);
                                    if (!uname.isEmpty()) t2.pecheur = uname;
                                }
                            }
                            m_transactions.append(t2);
                        }
                        qDebug() << "Fallback SELECT * populated" << m_transactions.size() << "rows from" << qualified;
                    }
                }
            }
        }
    }

    // Table headers resize
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Initialize Fish Pricing
    m_fishPricing = new FishPricingDialog(this);
    
    // Fish pricing connections
    connect(ui->fishComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &MainWindow::onFishSelected);
    connect(ui->btnAfficherPrix, &QPushButton::clicked, this, &MainWindow::onAfficherPrix);
    
    // Create default price file if needed and load it automatically
    m_fishPricing->createDefaultPriceFileIfNeeded();
    QString priceFile = QCoreApplication::applicationDirPath() + "/prix_poissons.txt";
    qDebug() << "Loading price file from:" << priceFile;
    m_fishPricing->loadFishPricesFromFile(priceFile);
    connect(m_fishPricing, &FishPricingDialog::fishListLoaded, this, [this](const QStringList &fishes) {
        // Clear all items
        ui->fishComboBox->clear();
        // Add placeholder
        ui->fishComboBox->addItem("-- Sélectionner un poisson --");
        // Add fish items from file
        if (!fishes.isEmpty()) {
            ui->fishComboBox->addItems(fishes);
            qDebug() << "ComboBox populated with" << fishes.size() << "fishes from file";
        } else {
            qDebug() << "WARNING: No fishes loaded from file!";
        }
        // Mark as loaded
        m_fishPricesLoaded = true;
        qDebug() << "Fish prices loaded:" << fishes.size() << "types loaded";
    });

    // CRUD buttons
    connect(ui->btnCreer,    &QPushButton::clicked, this, &MainWindow::onCreer);
    connect(ui->btnModifier, &QPushButton::clicked, this, &MainWindow::onModifier);
    connect(ui->btnSupprimer,&QPushButton::clicked, this, &MainWindow::onSupprimer);

    // Search
    connect(ui->lineRecherchePecheur, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->lineRechercheFacture, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->lineRechercheLot,     &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->dateDebut, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onSearchChanged);
    connect(ui->dateFin,   &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onSearchChanged);

    // Sort
    connect(ui->comboTri,   QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);
    connect(ui->comboOrdre, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);

    // PDF
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &MainWindow::onExportPDF);

    // Ventes dropdown
    connect(ui->btnVentes, &QPushButton::clicked, this, &MainWindow::onToggleVentesMenu);
    connect(ui->btnVentesAffichage, &QPushButton::clicked, this, &MainWindow::onVentesAffichage);
    connect(ui->btnVentesStatistique, &QPushButton::clicked, this, &MainWindow::onVentesStatistique);
    connect(ui->btnVentesAvancees, &QPushButton::clicked, this, &MainWindow::onVentesAvancees);

    // Sidebar navigation buttons — highlight active
    auto sidebarButtons = {ui->btnEmployees, ui->btnNavires, ui->btnQuais,
                           ui->btnCaptures, ui->btnStockage, ui->btnVentes};
    for (auto *btn : sidebarButtons) {
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            setActiveSidebarButton(btn);
        });
    }
    // Default view: Gestion Des Ventes -> Affichage
    setActiveSidebarButton(ui->btnVentes);
    setActiveVentesOption(ui->btnVentesAffichage);
    showVentesAffichageBlock();
    // Initial load from DB
    setVentesMenuExpanded(false, false);
    scrollToSectionTop(ui->rechercheFrame);

    loadDataFromDb();
    refreshTable();
}

MainWindow::~MainWindow()
{
    if (m_ventesMenuAnim) {
        m_ventesMenuAnim->stop();
        delete m_ventesMenuAnim;
        m_ventesMenuAnim = nullptr;
    }
    delete ui;
}

// ─── Sidebar active button ──────────────────────────

void MainWindow::setActiveSidebarButton(QPushButton *activeBtn)
{
    // Reset all sidebar buttons
    for (auto *btn : {ui->btnEmployees, ui->btnNavires, ui->btnQuais,
                      ui->btnCaptures, ui->btnStockage, ui->btnVentes}) {
        btn->setProperty("active", false);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
    // Highlight the clicked one
    activeBtn->setProperty("active", true);
    activeBtn->style()->unpolish(activeBtn);
    activeBtn->style()->polish(activeBtn);

    // Collapse Ventes dropdown when leaving it
    if (activeBtn != ui->btnVentes) {
        setVentesMenuExpanded(false, true);
        setActiveVentesOption(nullptr);
    }
}

// ─── Ventes dropdown ─────────────────────────────────

void MainWindow::setVentesMenuExpanded(bool expanded, bool animated)
{
    if (m_ventesMenuExpanded == expanded && ui->ventesMenu->isVisible() == expanded)
        return;

    m_ventesMenuExpanded = expanded;

    if (m_ventesMenuAnim) {
        m_ventesMenuAnim->stop();
        delete m_ventesMenuAnim;
        m_ventesMenuAnim = nullptr;
    }

    const int startH = ui->ventesMenu->maximumHeight();
    int endH = 0;
    if (expanded) {
        ui->ventesMenu->setVisible(true);
        ui->ventesMenu->setMaximumHeight(0);
        endH = ui->ventesMenu->layout() ? ui->ventesMenu->layout()->sizeHint().height()
                                        : ui->ventesMenu->sizeHint().height();
        if (endH < 0) endH = 0;
    } else {
        endH = 0;
    }

    if (!animated) {
        ui->ventesMenu->setMaximumHeight(endH);
        if (!expanded)
            ui->ventesMenu->setVisible(false);
        return;
    }

    m_ventesMenuAnim = new QPropertyAnimation(ui->ventesMenu, "maximumHeight", this);
    m_ventesMenuAnim->setDuration(160);
    m_ventesMenuAnim->setEasingCurve(QEasingCurve::InOutCubic);
    m_ventesMenuAnim->setStartValue(startH);
    m_ventesMenuAnim->setEndValue(endH);
    connect(m_ventesMenuAnim, &QPropertyAnimation::finished, this, [this, expanded]() {
        if (!expanded) {
            ui->ventesMenu->setVisible(false);
            ui->ventesMenu->setMaximumHeight(0);
        }
    });
    m_ventesMenuAnim->start();
}

void MainWindow::onToggleVentesMenu()
{
    setVentesMenuExpanded(!m_ventesMenuExpanded, true);
}

void MainWindow::setActiveVentesOption(QPushButton *active)
{
    for (auto *btn : {ui->btnVentesAffichage, ui->btnVentesStatistique, ui->btnVentesAvancees}) {
        btn->setProperty("active", btn == active);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
}

void MainWindow::scrollToSectionTop(QWidget *section)
{
    if (!section) return;

    QWidget *contents = ui->mainScrollArea->widget();
    if (!contents) return;

    // Map the section's top-left to the scroll contents coordinate system
    const int targetY = section->mapTo(contents, QPoint(0, 0)).y();
    constexpr int topPadding = 8;

    if (auto *bar = ui->mainScrollArea->verticalScrollBar()) {
        bar->setValue(std::max(0, targetY - topPadding));
    }
}

void MainWindow::showOnlyVentesSection(QWidget *section)
{
    // When a Ventes submenu is selected, show only the requested section
    // (matches the expected "sub-page" behavior).
    const bool showAll = (section == nullptr);
    for (auto *frame : {ui->rechercheFrame, ui->triFrame, ui->affichageFrame,
                        ui->statsFrame, ui->optionsFrame}) {
        frame->setVisible(showAll || frame == section);
    }
}

void MainWindow::showVentesAffichageBlock()
{
    // Affichage view should also include Recherche + Tri.
    ui->rechercheFrame->setVisible(true);
    ui->triFrame->setVisible(true);
    ui->affichageFrame->setVisible(true);
    ui->statsFrame->setVisible(false);
    ui->optionsFrame->setVisible(false);
}

void MainWindow::onVentesAffichage()
{
    setActiveSidebarButton(ui->btnVentes);
    setActiveVentesOption(ui->btnVentesAffichage);
    showVentesAffichageBlock();
    scrollToSectionTop(ui->rechercheFrame);
    setVentesMenuExpanded(false, true);
}

void MainWindow::onVentesStatistique()
{
    setActiveSidebarButton(ui->btnVentes);
    setActiveVentesOption(ui->btnVentesStatistique);
    showOnlyVentesSection(ui->statsFrame);
    scrollToSectionTop(ui->statsFrame);
    setVentesMenuExpanded(false, true);
}

void MainWindow::onVentesAvancees()
{
    setActiveSidebarButton(ui->btnVentes);
    setActiveVentesOption(ui->btnVentesAvancees);
    showOnlyVentesSection(ui->optionsFrame);
    scrollToSectionTop(ui->optionsFrame);
    setVentesMenuExpanded(false, true);
}


void MainWindow::loadDataFromDb()
{
    m_transactions.clear();
    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen()) return;

    // Build column name map for TRANSACTION table: normalized key -> actual column name
    m_colMap.clear();
    QSqlQuery cmap(db);
    if (cmap.exec(QString("SELECT column_name FROM user_tab_columns WHERE table_name = '%1'" ).arg(m_tableName))) {
        while (cmap.next()) {
            QString cname = cmap.value(0).toString().toUpper();
            QString key = cname;
            key.remove('_');
            m_colMap.insert(key, cname);
        }
    }

    QSqlQuery q(db);
    // Fetch everything from the table. Some DBs may not have the LOT column —
    // try the full select first, then fallback to a select without LOT.
    // Decide whether DB exposes the LOT column using the normalized column map
    m_hasLotColumn = false;
    // normalized key used in m_colMap removes underscores and is uppercase
    const QString lotKey = QStringLiteral("LOT");
    if (m_colMap.contains(lotKey)) {
        m_hasLotColumn = true;
    } else {
        // accept variants containing 'LOT' (e.g. NUMLOT, LOT_NUM)
        for (auto it = m_colMap.constBegin(); it != m_colMap.constEnd(); ++it) {
            if (it.key().contains(lotKey, Qt::CaseInsensitive)) { m_hasLotColumn = true; break; }
        }
    }
    // Determine PECHEUR column type (numeric vs string) using resolved column name
    m_pecheurIsNumeric = false;
    QString actualPecheur = colName("PECHEUR");
    if (!actualPecheur.isEmpty()) {
        QSqlQuery peq(db);
        if (peq.exec(QString("SELECT data_type FROM user_tab_columns WHERE table_name = '%1' AND column_name = '%2'")
                     .arg(m_tableName, actualPecheur))) {
            if (peq.next()) {
                const QString dtype = peq.value(0).toString().toUpper();
                if (dtype.contains("NUMBER") || dtype.contains("INT")) m_pecheurIsNumeric = true;
            }
        }
    }
    // fallback: if map didn't detect LOT, leave m_hasLotColumn false and allow
    // the SELECT * fallbacks below to populate `t.lot` when present under another name.

    // Resolve actual column names (fall back to literals if not found)
    // Use `colName()` which performs fuzzy matching / levenshtein fallback
    auto col = [this](const QString &logical) -> QString {
        return this->colName(logical);
    };

    if (m_hasLotColumn) {
        QString sel = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9 FROM %10")
            .arg(col("NUMFACTURE"), col("PECHEUR"), col("LOT"), col("PRIX_KG"), col("QUANTITE"), col("TOTAL"), col("DATETRANSACTION"), col("MODEPAIEMENT"), col("STATUTPAIEMENT"), m_tableName);
        if (!q.exec(sel)) {
            qDebug() << "Select failed:" << q.lastError().text() << " SQL:" << sel;
            // Try fallback: SELECT * from table (unqualified), then try qualified owner.table
            QSqlQuery q2(db);
            bool fallbackDone = false;
            if (q2.exec(QString("SELECT * FROM %1").arg(m_tableName))) {
                QSqlRecord rec = q2.record();
                auto findIndexLocal = [&](const QString &logical)->int {
                    QString nk = logical.toUpper(); nk.remove('_');
                    for (int i = 0; i < rec.count(); ++i) {
                        QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                        if (rnn == nk) return i;
                    }
                    for (int i = 0; i < rec.count(); ++i) {
                        QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                        if (rnn.contains(nk) || nk.contains(rnn)) return i;
                    }
                    return -1;
                };
                int idxNum = findIndexLocal("NUMFACTURE");
                int idxPecheur = findIndexLocal("PECHEUR"); if (idxPecheur < 0) idxPecheur = findIndexLocal("PECHER");
                int idxLot = findIndexLocal("LOT");
                int idxPrix = findIndexLocal("PRIX_KG"); if (idxPrix < 0) idxPrix = findIndexLocal("PRIXKG");
                int idxQte = findIndexLocal("QUANTITE");
                int idxTotal = findIndexLocal("TOTAL");
                int idxDate = findIndexLocal("DATETRANSACTION"); if (idxDate < 0) idxDate = findIndexLocal("DATE_TRANSACTION");
                int idxMode = findIndexLocal("MODEPAIEMENT");
                int idxStatut = findIndexLocal("STATUTPAIEMENT");
                while (q2.next()) {
                    Transaction t;
                    if (idxNum >= 0) t.numFacture = q2.value(idxNum).toString();
                    if (idxPecheur >= 0) t.pecheur = q2.value(idxPecheur).toString();
                    if (idxLot >= 0) t.lot = q2.value(idxLot).toString();
                    if (idxPrix >= 0) t.prixKg = q2.value(idxPrix).toDouble();
                    if (idxQte >= 0) t.quantite = q2.value(idxQte).toDouble();
                    if (idxTotal >= 0) t.total = q2.value(idxTotal).toDouble();
                    if (idxDate >= 0) t.dateTransaction = q2.value(idxDate).toDateTime();
                    if (idxMode >= 0) t.modePaiement = q2.value(idxMode).toString();
                    if (idxStatut >= 0) t.statutPaiement = q2.value(idxStatut).toString();
                    if (m_pecheurIsNumeric) {
                        bool okid = false; int pid = t.pecheur.toInt(&okid);
                        if (okid) {
                            QString uname = userNameForId(pid);
                            if (!uname.isEmpty()) t.pecheur = uname;
                        }
                    }
                    m_transactions.append(t);
                }
                fallbackDone = true;
                qDebug() << "Fallback SELECT * populated" << m_transactions.size() << "rows (unqualified).";
            } else {
                // try owner qualified
                QSqlQuery ownerQ(db);
                if (ownerQ.exec(QString("SELECT OWNER FROM ALL_TABLES WHERE TABLE_NAME = '%1' AND ROWNUM=1").arg(m_tableName.toUpper())) && ownerQ.next()) {
                    QString owner = ownerQ.value(0).toString();
                    if (!owner.isEmpty()) {
                        QString qual = owner + "." + m_tableName;
                        if (q2.exec(QString("SELECT * FROM %1").arg(qual))) {
                            QSqlRecord rec = q2.record();
                            auto findIndexLocal = [&](const QString &logical)->int {
                                QString nk = logical.toUpper(); nk.remove('_');
                                for (int i = 0; i < rec.count(); ++i) {
                                    QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                    if (rnn == nk) return i;
                                }
                                for (int i = 0; i < rec.count(); ++i) {
                                    QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                    if (rnn.contains(nk) || nk.contains(rnn)) return i;
                                }
                                return -1;
                            };
                            int idxNum = findIndexLocal("NUMFACTURE");
                            int idxPecheur = findIndexLocal("PECHEUR"); if (idxPecheur < 0) idxPecheur = findIndexLocal("PECHER");
                            int idxLot = findIndexLocal("LOT");
                            int idxPrix = findIndexLocal("PRIX_KG"); if (idxPrix < 0) idxPrix = findIndexLocal("PRIXKG");
                            int idxQte = findIndexLocal("QUANTITE");
                            int idxTotal = findIndexLocal("TOTAL");
                            int idxDate = findIndexLocal("DATETRANSACTION"); if (idxDate < 0) idxDate = findIndexLocal("DATE_TRANSACTION");
                            int idxMode = findIndexLocal("MODEPAIEMENT");
                            int idxStatut = findIndexLocal("STATUTPAIEMENT");
                            while (q2.next()) {
                                Transaction t;
                                if (idxNum >= 0) t.numFacture = q2.value(idxNum).toString();
                                if (idxPecheur >= 0) t.pecheur = q2.value(idxPecheur).toString();
                                if (idxLot >= 0) t.lot = q2.value(idxLot).toString();
                                if (idxPrix >= 0) t.prixKg = q2.value(idxPrix).toDouble();
                                if (idxQte >= 0) t.quantite = q2.value(idxQte).toDouble();
                                if (idxTotal >= 0) t.total = q2.value(idxTotal).toDouble();
                                if (idxDate >= 0) t.dateTransaction = q2.value(idxDate).toDateTime();
                                if (idxMode >= 0) t.modePaiement = q2.value(idxMode).toString();
                                if (idxStatut >= 0) t.statutPaiement = q2.value(idxStatut).toString();
                                if (m_pecheurIsNumeric) {
                                    bool okid = false; int pid = t.pecheur.toInt(&okid);
                                    if (okid) {
                                        QString uname = userNameForId(pid);
                                        if (!uname.isEmpty()) t.pecheur = uname;
                                    }
                                }
                                m_transactions.append(t);
                            }
                            fallbackDone = true;
                            qDebug() << "Fallback SELECT * populated" << m_transactions.size() << "rows (qualified).";
                        }
                    }
                }
            }
            if (!fallbackDone) {
                statusBar()->showMessage(tr("Échec lecture transactions: %1").arg(q.lastError().text()), 8000);
                return;
            }
        }
        while (q.next()) {
            Transaction t;
            t.numFacture      = q.value(0).toString();
            t.pecheur         = q.value(1).toString();
            // If PECHEUR stored as numeric id, try to resolve to a name for display
            if (m_pecheurIsNumeric) {
                bool okId = false;
                int pid = t.pecheur.toInt(&okId);
                if (okId) {
                    QString uname = userNameForId(pid);
                    if (!uname.isEmpty()) t.pecheur = uname;
                }
            }
            t.lot             = q.value(2).toString();
            t.prixKg          = q.value(3).toDouble();
            t.quantite        = q.value(4).toDouble();
            t.total           = q.value(5).toDouble();
            t.dateTransaction = q.value(6).toDateTime();
            t.modePaiement    = q.value(7).toString();
            t.statutPaiement  = q.value(8).toString();
            m_transactions.append(t);
        }
    } else {
        QString sel2 = QString("SELECT %1, %2, %3, %4, %5, %6, %7, %8 FROM %9")
            .arg(col("NUMFACTURE"), col("PECHEUR"), col("PRIX_KG"), col("QUANTITE"), col("TOTAL"), col("DATETRANSACTION"), col("MODEPAIEMENT"), col("STATUTPAIEMENT"), m_tableName);
        if (!q.exec(sel2)) {
            qDebug() << "Select failed (no-lot):" << q.lastError().text() << " SQL:" << sel2;
            // fallback to SELECT * similar to the other branch
            QSqlQuery q2(db);
            bool done = false;
            if (q2.exec(QString("SELECT * FROM %1").arg(m_tableName))) {
                QSqlRecord rec = q2.record();
                auto findIndexLocal = [&](const QString &logical)->int {
                    QString nk = logical.toUpper(); nk.remove('_');
                    for (int i = 0; i < rec.count(); ++i) {
                        QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                        if (rnn == nk) return i;
                    }
                    for (int i = 0; i < rec.count(); ++i) {
                        QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                        if (rnn.contains(nk) || nk.contains(rnn)) return i;
                    }
                    return -1;
                };
                int idxNum = findIndexLocal("NUMFACTURE");
                int idxPecheur = findIndexLocal("PECHEUR"); if (idxPecheur < 0) idxPecheur = findIndexLocal("PECHER");
                int idxPrix = findIndexLocal("PRIX_KG"); if (idxPrix < 0) idxPrix = findIndexLocal("PRIXKG");
                int idxQte = findIndexLocal("QUANTITE");
                int idxTotal = findIndexLocal("TOTAL");
                int idxDate = findIndexLocal("DATETRANSACTION"); if (idxDate < 0) idxDate = findIndexLocal("DATE_TRANSACTION");
                int idxMode = findIndexLocal("MODEPAIEMENT");
                int idxStatut = findIndexLocal("STATUTPAIEMENT");
                while (q2.next()) {
                    Transaction t;
                    if (idxNum >= 0) t.numFacture = q2.value(idxNum).toString();
                    if (idxPecheur >= 0) t.pecheur = q2.value(idxPecheur).toString();
                    if (idxPrix >= 0) t.prixKg = q2.value(idxPrix).toDouble();
                    if (idxQte >= 0) t.quantite = q2.value(idxQte).toDouble();
                    if (idxTotal >= 0) t.total = q2.value(idxTotal).toDouble();
                    if (idxDate >= 0) t.dateTransaction = q2.value(idxDate).toDateTime();
                    if (idxMode >= 0) t.modePaiement = q2.value(idxMode).toString();
                    if (idxStatut >= 0) t.statutPaiement = q2.value(idxStatut).toString();
                    if (m_pecheurIsNumeric) {
                        bool okid = false; int pid = t.pecheur.toInt(&okid);
                        if (okid) {
                            QString uname = userNameForId(pid);
                            if (!uname.isEmpty()) t.pecheur = uname;
                        }
                    }
                    m_transactions.append(t);
                }
                done = true;
                qDebug() << "Fallback SELECT * populated" << m_transactions.size() << "rows (no-lot branch).";
            } else {
                // try qualified
                QSqlQuery ownerQ(db);
                if (ownerQ.exec(QString("SELECT OWNER FROM ALL_TABLES WHERE TABLE_NAME = '%1' AND ROWNUM=1").arg(m_tableName.toUpper())) && ownerQ.next()) {
                    QString owner = ownerQ.value(0).toString();
                    if (!owner.isEmpty()) {
                        QString qual = owner + "." + m_tableName;
                        if (q2.exec(QString("SELECT * FROM %1").arg(qual))) {
                            QSqlRecord rec = q2.record();
                            auto findIndexLocal = [&](const QString &logical)->int {
                                QString nk = logical.toUpper(); nk.remove('_');
                                for (int i = 0; i < rec.count(); ++i) {
                                    QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                    if (rnn == nk) return i;
                                }
                                for (int i = 0; i < rec.count(); ++i) {
                                    QString rn = rec.fieldName(i).toUpper(); QString rnn = rn; rnn.remove('_');
                                    if (rnn.contains(nk) || nk.contains(rnn)) return i;
                                }
                                return -1;
                            };
                            int idxNum = findIndexLocal("NUMFACTURE");
                            int idxPecheur = findIndexLocal("PECHEUR"); if (idxPecheur < 0) idxPecheur = findIndexLocal("PECHER");
                            int idxPrix = findIndexLocal("PRIX_KG"); if (idxPrix < 0) idxPrix = findIndexLocal("PRIXKG");
                            int idxQte = findIndexLocal("QUANTITE");
                            int idxTotal = findIndexLocal("TOTAL");
                            int idxDate = findIndexLocal("DATETRANSACTION"); if (idxDate < 0) idxDate = findIndexLocal("DATE_TRANSACTION");
                            int idxMode = findIndexLocal("MODEPAIEMENT");
                            int idxStatut = findIndexLocal("STATUTPAIEMENT");
                            while (q2.next()) {
                                Transaction t;
                                if (idxNum >= 0) t.numFacture = q2.value(idxNum).toString();
                                if (idxPecheur >= 0) t.pecheur = q2.value(idxPecheur).toString();
                                if (idxPrix >= 0) t.prixKg = q2.value(idxPrix).toDouble();
                                if (idxQte >= 0) t.quantite = q2.value(idxQte).toDouble();
                                if (idxTotal >= 0) t.total = q2.value(idxTotal).toDouble();
                                if (idxDate >= 0) t.dateTransaction = q2.value(idxDate).toDateTime();
                                if (idxMode >= 0) t.modePaiement = q2.value(idxMode).toString();
                                if (idxStatut >= 0) t.statutPaiement = q2.value(idxStatut).toString();
                                if (m_pecheurIsNumeric) {
                                    bool okid = false; int pid = t.pecheur.toInt(&okid);
                                    if (okid) {
                                        QString uname = userNameForId(pid);
                                        if (!uname.isEmpty()) t.pecheur = uname;
                                    }
                                }
                                m_transactions.append(t);
                            }
                            done = true;
                            qDebug() << "Fallback SELECT * populated" << m_transactions.size() << "rows (qualified no-lot).";
                        }
                    }
                }
            }
            if (!done) return;
        }
        while (q.next()) {
            Transaction t;
            t.numFacture      = q.value(0).toString();
            t.pecheur         = q.value(1).toString();
            if (m_pecheurIsNumeric) {
                bool okId = false;
                int pid = t.pecheur.toInt(&okId);
                if (okId) {
                    QString uname = userNameForId(pid);
                    if (!uname.isEmpty()) t.pecheur = uname;
                }
            }
            t.lot             = QString();
            t.prixKg          = q.value(2).toDouble();
            t.quantite        = q.value(3).toDouble();
            t.total           = q.value(4).toDouble();
            t.dateTransaction = q.value(5).toDateTime();
            t.modePaiement    = q.value(6).toString();
            t.statutPaiement  = q.value(7).toString();
            m_transactions.append(t);
        }
    // Debug/status: show how many transactions loaded and whether LOT detected
    QString lotState = m_hasLotColumn ? tr("oui") : tr("non");
    statusBar()->showMessage(tr("Transactions chargées: %1 — LOT présent: %2").arg(m_transactions.size()).arg(lotState), 8000);
    qDebug() << "Loaded" << m_transactions.size() << "transactions; LOT present:" << m_hasLotColumn;
    // show up to 5 lot values for inspection
    for (int i = 0; i < m_transactions.size() && i < 5; ++i) {
        qDebug() << "Tx" << i << "num:" << m_transactions[i].numFacture << "lot:" << m_transactions[i].lot;
    }
    }
}

// ─── CRUD ───────────────────────────────────────────────

void MainWindow::onCreer()
{
    TransactionDialog dlg(this, tr("Créer transaction"));
    if (dlg.exec() != QDialog::Accepted) return;

    Transaction t = dlg.transaction();
    t.calcTotal(); // ensure total is correct
    
    // Numéro de facture est NUMBER(10,0) -> conversion entière
    if (t.numFacture.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le numéro de facture est obligatoire."));
        return;
    }
    bool ok;
    int numFactureInt = t.numFacture.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le numéro de facture doit être un nombre valide."));
        return;
    }

    // Check duplicate in DB? Or let DB constraint handle it?
    // Let's check locally first via pre-loaded list or just fetch
    // Actually, SQL constraint is better but handling error is good too.
    // For now we trust m_transactions is sync.
    for (const auto &existing : m_transactions) {
        if (existing.numFacture == t.numFacture) {
            QMessageBox::warning(this, tr("Erreur"), tr("Une transaction avec ce numéro existe déjà."));
            return;
        }
    }

    QSqlDatabase db = Connection::createInstance().database();
    if (db.isOpen()) {
            // If the DB doesn't have a LOT column, try to add it automatically before update.
            if (!m_hasLotColumn) {
                QSqlQuery alt(db);
                const QString lotCol = QStringLiteral("LOT");
                const QString alterSql = QString("ALTER TABLE %1 ADD (%2 VARCHAR2(200))").arg(m_tableName, lotCol);
                if (alt.exec(alterSql)) {
                    m_hasLotColumn = true;
                    QSqlQuery cmap2(db);
                    m_colMap.clear();
                    if (cmap2.exec(QString("SELECT column_name FROM user_tab_columns WHERE table_name = '%1'" ).arg(m_tableName))) {
                        while (cmap2.next()) {
                            QString cname = cmap2.value(0).toString().toUpper();
                            QString key = cname; key.remove('_');
                            m_colMap.insert(key, cname);
                        }
                    }
                    statusBar()->showMessage(tr("Colonne LOT créée automatiquement."), 5000);
                } else {
                    qDebug() << "ALTER TABLE failed:" << alt.lastError().text();
                    QMessageBox::warning(this, tr("Droits insuffisants"),
                        tr("Impossible de créer la colonne LOT automatiquement:\n%1\n\nExécutez manuellement en tant que propriétaire:\nALTER TABLE %2 ADD (LOT VARCHAR2(200));")
                        .arg(alt.lastError().text(), m_tableName));
                }
            }
            // If the DB doesn't have a LOT column, try to add it automatically.
            if (!m_hasLotColumn) {
                QSqlQuery alt(db);
                const QString lotCol = QStringLiteral("LOT");
                // Attempt to add a VARCHAR2 column for Lot (safe default)
                const QString alterSql = QString("ALTER TABLE %1 ADD (%2 VARCHAR2(200))").arg(m_tableName, lotCol);
                if (alt.exec(alterSql)) {
                    m_hasLotColumn = true;
                    // refresh column map so future colName() resolves correctly
                    QSqlQuery cmap2(db);
                    m_colMap.clear();
                    if (cmap2.exec(QString("SELECT column_name FROM user_tab_columns WHERE table_name = '%1'" ).arg(m_tableName))) {
                        while (cmap2.next()) {
                            QString cname = cmap2.value(0).toString().toUpper();
                            QString key = cname; key.remove('_');
                            m_colMap.insert(key, cname);
                        }
                    }
                    statusBar()->showMessage(tr("Colonne LOT créée automatiquement."), 5000);
                } else {
                    qDebug() << "ALTER TABLE failed:" << alt.lastError().text();
                    QMessageBox::warning(this, tr("Droits insuffisants"),
                        tr("Impossible de créer la colonne LOT automatiquement:\n%1\n\nExécutez manuellement en tant que propriétaire:\nALTER TABLE %2 ADD (LOT VARCHAR2(200));")
                        .arg(alt.lastError().text(), m_tableName));
                }
            }
            // Double-check in DB that the NUMFACTURE doesn't already exist (avoid PK violation)
            QSqlQuery chk(db);
            QString chkSql = QString("SELECT COUNT(*) FROM %1 WHERE %2 = :id").arg(m_tableName, colName("NUMFACTURE"));
            if (chk.prepare(chkSql)) {
                chk.bindValue(":id", numFactureInt);
                if (chk.exec() && chk.next()) {
                    if (chk.value(0).toInt() > 0) {
                        QMessageBox::warning(this, tr("Erreur"), tr("Une transaction avec ce numéro existe déjà dans la base de données."));
                        return;
                    }
                }
            }
            QVariant pecheurBind;
            if (m_pecheurIsNumeric) {
                bool okPecheur;
                int pecheurId = resolvePecheurId(t.pecheur, okPecheur);
                if (!okPecheur) return;
                pecheurBind = QVariant::fromValue(pecheurId);
            } else {
                // store name directly
                pecheurBind = QVariant::fromValue(t.pecheur);
            }

            QSqlQuery q(db);
        if (m_hasLotColumn) {
            // Resolve actual LOT column name and detect its data type so we bind correct QVariant
            const QString actualLotCol = colName("LOT");
            const QString numFactCol = colName("NUMFACTURE");
            const QString pecheurCol = colName("PECHEUR");
            const QString prixCol = colName("PRIX_KG");
            const QString qteCol = colName("QUANTITE");
            const QString totalCol = colName("TOTAL");
            const QString dateCol = colName("DATETRANSACTION");
            const QString modeCol = colName("MODEPAIEMENT");
            const QString statutCol = colName("STATUTPAIEMENT");

            QString ins = QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9, %10) VALUES (:id, :pecheur, :lot, :prix, :qte, :total, :date, :mode, :statut)")
                    .arg(m_tableName, numFactCol, pecheurCol, actualLotCol, prixCol, qteCol, totalCol, dateCol, modeCol, statutCol);
            q.prepare(ins);
            q.bindValue(":id", numFactureInt);
            q.bindValue(":pecheur", pecheurBind);

            // Determine LOT column type
            QVariant lotBind;
            bool lotIsNumeric = false;
            QSqlQuery dtypeQ(db);
            if (dtypeQ.exec(QString("SELECT data_type FROM user_tab_columns WHERE table_name = '%1' AND column_name = '%2'").arg(m_tableName, actualLotCol)) && dtypeQ.next()) {
                QString dtype = dtypeQ.value(0).toString().toUpper();
                if (dtype.contains("NUMBER") || dtype.contains("INT") || dtype.contains("DECIMAL") || dtype.contains("FLOAT")) lotIsNumeric = true;
            }
            if (lotIsNumeric) {
                bool okLot = false;
                double lotNum = t.lot.toDouble(&okLot);
                if (!okLot) {
                    QMessageBox::warning(this, tr("Erreur"), tr("Le champ Lot doit être numérique pour la colonne %1.").arg(actualLotCol));
                    return;
                }
                lotBind = QVariant::fromValue(lotNum);
            } else {
                lotBind = QVariant::fromValue(t.lot);
            }

            q.bindValue(":lot", lotBind);
            q.bindValue(":prix", t.prixKg);
            q.bindValue(":qte", t.quantite);
            q.bindValue(":total", t.total);
            q.bindValue(":date", t.dateTransaction);
            q.bindValue(":mode", t.modePaiement);
            // Validate statut against DB check constraint if present
            QStringList allowed = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
            QString statutVal = t.statutPaiement;
            if (!allowed.isEmpty()) {
                bool okAllowed = false;
                for (const QString &a : allowed) if (a.compare(statutVal, Qt::CaseInsensitive) == 0) { okAllowed = true; break; }
                if (!okAllowed) {
                    statutVal = allowed.first();
                }
            }
            q.bindValue(":statut", statutVal);
        } else {
                QString ins2 = QString("INSERT INTO %1 (%2, %3, %4, %5, %6, %7, %8, %9) VALUES (:id, :pecheur, :prix, :qte, :total, :date, :mode, :statut)")
                    .arg(m_tableName, colName("NUMFACTURE"), colName("PECHEUR"), colName("PRIX_KG"), colName("QUANTITE"), colName("TOTAL"), colName("DATETRANSACTION"), colName("MODEPAIEMENT"), colName("STATUTPAIEMENT"));
                q.prepare(ins2);
            q.bindValue(":id", numFactureInt);
            q.bindValue(":pecheur", pecheurBind);
            q.bindValue(":prix", t.prixKg);
            q.bindValue(":qte", t.quantite);
            q.bindValue(":total", t.total);
            q.bindValue(":date", t.dateTransaction);
            q.bindValue(":mode", t.modePaiement);
            QStringList allowed2 = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
            QString statutVal2 = t.statutPaiement;
            if (!allowed2.isEmpty()) {
                bool okAllowed = false;
                for (const QString &a : allowed2) if (a.compare(statutVal2, Qt::CaseInsensitive) == 0) { okAllowed = true; break; }
                if (!okAllowed) statutVal2 = allowed2.first();
            }
            q.bindValue(":statut", statutVal2);
        }

        if (q.exec()) {
            QMessageBox::information(this, tr("Succès"), tr("Transaction ajoutée."));
            loadDataFromDb();
            refreshTable();
        } else {
            const QString err = q.lastError().text();
            if (err.contains("ORA-02290") || err.contains("check constraint", Qt::CaseInsensitive)) {
                QStringList allowedRetry = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
                if (!allowedRetry.isEmpty()) {
                    QString newStat = allowedRetry.first();
                    q.bindValue(":statut", newStat);
                    if (q.exec()) {
                        QMessageBox::information(this, tr("Succès"), tr("Transaction ajoutée (statut ajusté)."));
                        loadDataFromDb();
                        refreshTable();
                        return;
                    }
                }
            }
            QMessageBox::critical(this, tr("Erreur BDD"), tr("L'ajout a échoué.\n%1").arg(err));
        }
    } else {
         QMessageBox::warning(this, tr("Erreur"), tr("Base de données non connectée."));
    }
}

void MainWindow::onModifier()
{
    int row = ui->tableTransactions->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Modifier"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }
    // Find matching transaction in m_transactions
    // Note: row index in filteredAndSorted() != index in m_transactions
    // Use numFacture from table item
    QString numFacture = ui->tableTransactions->item(row, 0)->text();
    
    // Find effectively
    int idx = -1;
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].numFacture == numFacture) { idx = i; break; }
    }
    if (idx < 0) return;

    TransactionDialog dlg(this, tr("Modifier transaction"));
    dlg.setTransaction(m_transactions[idx]);
    if (dlg.exec() != QDialog::Accepted) return;

    Transaction t = dlg.transaction();
    t.calcTotal();

    // Check if ID changed and conflicts? (Usually PK shouldn't change, but UI allows it?)
    // If numFacture is PK, modifying it is tricky. Let's assume passed numFacture is OLD PK to WHERE clause,
    // and new t.numFacture is updated value.
    // For simplicity, TransactionDialog usually edits everything.
    // But if we change numFacture, we need to know the OLD one to find the record to update!
    // The current dialog returns a Transaction struct. It doesn't tell us the "before" state.
    // `t` is the new state. `m_transactions[idx]` is the old state.
    // So we use m_transactions[idx].numFacture for WHERE clause.

    QSqlDatabase db = Connection::createInstance().database();
    if (db.isOpen()) {
            QVariant pecheurBind;
            if (m_pecheurIsNumeric) {
                bool okPecheurUpd;
                int pecheurIdUpd = resolvePecheurId(t.pecheur, okPecheurUpd);
                if (!okPecheurUpd) return;
                pecheurBind = QVariant::fromValue(pecheurIdUpd);
            } else {
                pecheurBind = QVariant::fromValue(t.pecheur);
            }

            QSqlQuery q(db);
        if (m_hasLotColumn) {
            const QString actualLotCol = colName("LOT");
            const QString numFactCol = colName("NUMFACTURE");
            const QString pecheurCol = colName("PECHEUR");
            const QString prixCol = colName("PRIX_KG");
            const QString qteCol = colName("QUANTITE");
            const QString totalCol = colName("TOTAL");
            const QString dateCol = colName("DATETRANSACTION");
            const QString modeCol = colName("MODEPAIEMENT");
            const QString statutCol = colName("STATUTPAIEMENT");

            QString upd = QString("UPDATE %1 SET %2=:newId, %3=:pecheur, %4=:lot, %5=:prix, %6=:qte, %7=:total, %8=:date, %9=:mode, %10=:statut WHERE %11=:oldId")
                        .arg(m_tableName, numFactCol, pecheurCol, actualLotCol, prixCol, qteCol, totalCol, dateCol, modeCol, statutCol, numFactCol);
            q.prepare(upd);

            q.bindValue(":newId", t.numFacture.toInt());
            q.bindValue(":pecheur", pecheurBind);

            // Determine LOT column type and bind appropriately
            QVariant lotBind;
            bool lotIsNumeric = false;
            QSqlQuery dtypeQ(db);
            if (dtypeQ.exec(QString("SELECT data_type FROM user_tab_columns WHERE table_name = '%1' AND column_name = '%2'").arg(m_tableName, actualLotCol)) && dtypeQ.next()) {
                QString dtype = dtypeQ.value(0).toString().toUpper();
                if (dtype.contains("NUMBER") || dtype.contains("INT") || dtype.contains("DECIMAL") || dtype.contains("FLOAT")) lotIsNumeric = true;
            }
            if (lotIsNumeric) {
                bool okLot = false;
                double lotNum = t.lot.toDouble(&okLot);
                if (!okLot) {
                    QMessageBox::warning(this, tr("Erreur"), tr("Le champ Lot doit être numérique pour la colonne %1.").arg(actualLotCol));
                    return;
                }
                lotBind = QVariant::fromValue(lotNum);
            } else {
                lotBind = QVariant::fromValue(t.lot);
            }
            q.bindValue(":lot", lotBind);

            q.bindValue(":prix", QVariant::fromValue(t.prixKg));
            q.bindValue(":qte", QVariant::fromValue(t.quantite));
            q.bindValue(":total", QVariant::fromValue(t.total));
            q.bindValue(":date", t.dateTransaction);
            q.bindValue(":mode", t.modePaiement);
            QStringList allowed3 = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
            QString statutVal3 = t.statutPaiement;
            if (!allowed3.isEmpty()) {
                bool okAllowed = false;
                for (const QString &a : allowed3) if (a.compare(statutVal3, Qt::CaseInsensitive) == 0) { okAllowed = true; break; }
                if (!okAllowed) statutVal3 = allowed3.first();
            }
            q.bindValue(":statut", statutVal3);
            q.bindValue(":oldId", m_transactions[idx].numFacture.toInt());
        } else {
            {
                QString upd2 = QString("UPDATE %1 SET %2=:newId, %3=:pecheur, %4=:prix, %5=:qte, %6=:total, %7=:date, %8=:mode, %9=:statut WHERE %10=:oldId")
                            .arg(m_tableName, colName("NUMFACTURE"), colName("PECHEUR"), colName("PRIX_KG"), colName("QUANTITE"), colName("TOTAL"), colName("DATETRANSACTION"), colName("MODEPAIEMENT"), colName("STATUTPAIEMENT"), colName("NUMFACTURE"));
                q.prepare(upd2);
            }
            q.bindValue(":newId", t.numFacture.toInt());
            q.bindValue(":pecheur", pecheurBind);
            q.bindValue(":prix", QVariant::fromValue(t.prixKg));
            q.bindValue(":qte", QVariant::fromValue(t.quantite));
            q.bindValue(":total", QVariant::fromValue(t.total));
            q.bindValue(":date", t.dateTransaction);
            q.bindValue(":mode", t.modePaiement);
            q.bindValue(":statut", t.statutPaiement);
            q.bindValue(":oldId", m_transactions[idx].numFacture.toInt());
        }

        if (q.exec()) {
            QMessageBox::information(this, tr("Succès"), tr("Transaction modifiée."));
            loadDataFromDb();
            refreshTable();
        } else {
            const QString err = q.lastError().text();
            // If check-constraint violated, try to adjust statut and retry once
            if (err.contains("ORA-02290") || err.contains("check constraint", Qt::CaseInsensitive)) {
                QStringList allowedRetry = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
                if (!allowedRetry.isEmpty()) {
                    q.bindValue(":statut", allowedRetry.first());
                    if (q.exec()) {
                        QMessageBox::information(this, tr("Succès"), tr("Transaction modifiée (statut ajusté)."));
                        loadDataFromDb();
                        refreshTable();
                        return;
                    }
                }
            }
            if (err.contains("LOT", Qt::CaseInsensitive) || err.contains("ORA-00904") || err.contains("invalid identifier", Qt::CaseInsensitive)) {
                QSqlQuery q2(db);
                {
                    QString upd3 = QString("UPDATE %1 SET %2=:newId, %3=:pecheur, %4=:prix, %5=:qte, %6=:total, %7=:date, %8=:mode, %9=:statut WHERE %10=:oldId")
                                .arg(m_tableName, colName("NUMFACTURE"), colName("PECHEUR"), colName("PRIX_KG"), colName("QUANTITE"), colName("TOTAL"), colName("DATETRANSACTION"), colName("MODEPAIEMENT"), colName("STATUTPAIEMENT"), colName("NUMFACTURE"));
                    q2.prepare(upd3);
                }
                q2.bindValue(":newId", t.numFacture.toInt());
                q2.bindValue(":pecheur", pecheurBind);
                q2.bindValue(":prix", QVariant::fromValue(t.prixKg));
                q2.bindValue(":qte", QVariant::fromValue(t.quantite));
                q2.bindValue(":total", QVariant::fromValue(t.total));
                q2.bindValue(":date", t.dateTransaction);
                q2.bindValue(":mode", t.modePaiement);
                QStringList allowed4 = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
                QString statutVal4 = t.statutPaiement;
                if (!allowed4.isEmpty()) {
                    bool okAllowed = false;
                    for (const QString &a : allowed4) if (a.compare(statutVal4, Qt::CaseInsensitive) == 0) { okAllowed = true; break; }
                    if (!okAllowed) statutVal4 = allowed4.first();
                }
                q2.bindValue(":statut", statutVal4);
                q2.bindValue(":oldId", m_transactions[idx].numFacture.toInt());
                if (q2.exec()) {
                    QMessageBox::information(this, tr("Succès"), tr("Transaction modifiée (sans LOT)."));
                    loadDataFromDb();
                    refreshTable();
                } else {
                    // try adjust statut on q2 failure due to check constraint
                    const QString err2 = q2.lastError().text();
                    if ((err2.contains("ORA-02290") || err2.contains("check constraint", Qt::CaseInsensitive))) {
                        QStringList allowedRetry2 = allowedValuesForCheckConstraint(m_tableName, colName("STATUTPAIEMENT"));
                        if (!allowedRetry2.isEmpty()) {
                            q2.bindValue(":statut", allowedRetry2.first());
                            if (q2.exec()) {
                                QMessageBox::information(this, tr("Succès"), tr("Transaction modifiée (sans LOT, statut ajusté)."));
                                loadDataFromDb();
                                refreshTable();
                                return;
                            }
                        }
                    }
                    QMessageBox::critical(this, tr("Erreur BDD"), tr("La modification a échoué.\n%1").arg(q2.lastError().text()));
                }
            } else {
                QMessageBox::critical(this, tr("Erreur BDD"), tr("La modification a échoué.\n%1").arg(err));
            }
        }
    }
}

void MainWindow::onSupprimer()
{
    int row = ui->tableTransactions->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }
    QString numFacture = ui->tableTransactions->item(row, 0)->text();
    if (QMessageBox::question(this, tr("Supprimer"),
            tr("Supprimer la transaction %1 ?").arg(numFacture))
        != QMessageBox::Yes)
        return;

    QSqlDatabase db = Connection::createInstance().database();
    if (db.isOpen()) {
        QSqlQuery q(db);
        QString del = QString("DELETE FROM %1 WHERE %2 = :id").arg(m_tableName, colName("NUMFACTURE"));
        q.prepare(del);
        // Convert to int to ensure Oracle matches against NUMBER column correctly when appropriate
        if (m_pecheurIsNumeric) q.bindValue(":id", numFacture.toInt()); else q.bindValue(":id", numFacture);
        if (q.exec()) {
            QMessageBox::information(this, tr("Succès"), tr("Transaction supprimée."));
            loadDataFromDb();
            refreshTable();
        } else {
            QMessageBox::critical(this, tr("Erreur BDD"), tr("La suppression a échoué.\n%1").arg(q.lastError().text()));
        }
    }
}

// ─── Search ────────────────────────────────────────────

bool MainWindow::matchesSearch(const Transaction &t) const
{
    QString sp = ui->lineRecherchePecheur->text().trimmed();
    QString sf = ui->lineRechercheFacture->text().trimmed();
    QString sl = ui->lineRechercheLot->text().trimmed();

    if (!sp.isEmpty() && !t.pecheur.contains(sp, Qt::CaseInsensitive))
        return false;
    if (!sf.isEmpty() && !t.numFacture.contains(sf, Qt::CaseInsensitive))
        return false;
    if (!sl.isEmpty() && !t.lot.contains(sl, Qt::CaseInsensitive))
        return false;

    QDateTime deb = ui->dateDebut->dateTime();
    QDateTime fin = ui->dateFin->dateTime();
    if (deb.isValid() && fin.isValid() && deb < fin) {
        if (t.dateTransaction < deb || t.dateTransaction > fin)
            return false;
    }
    return true;
}

QVector<Transaction> MainWindow::filteredAndSorted() const
{
    QVector<Transaction> result;
    for (const auto &t : m_transactions)
        if (matchesSearch(t))
            result.append(t);

    int sortCol = ui->comboTri->currentIndex();   // 0=Date, 1=Prix, 2=Quantité, 3=Total
    bool asc = (ui->comboOrdre->currentIndex() == 1); // 0=Décroissant, 1=Croissant

    std::sort(result.begin(), result.end(),
              [sortCol, asc](const Transaction &a, const Transaction &b) {
        int cmp = 0;
        switch (sortCol) {
        case 0:  // Date
            if (a.dateTransaction < b.dateTransaction) cmp = -1;
            else if (a.dateTransaction > b.dateTransaction) cmp = 1;
            break;
        case 1:  // Prix/kg
            cmp = (a.prixKg < b.prixKg) ? -1 : (a.prixKg > b.prixKg ? 1 : 0);
            break;
        case 2:  // Quantité
            cmp = (a.quantite < b.quantite) ? -1 : (a.quantite > b.quantite ? 1 : 0);
            break;
        case 3:  // Total
            cmp = (a.total < b.total) ? -1 : (a.total > b.total ? 1 : 0);
            break;
        }
        return asc ? (cmp < 0) : (cmp > 0);
    });
    return result;
}

void MainWindow::onSearchChanged()
{
    refreshTable();
}

void MainWindow::onSortChanged()
{
    refreshTable();
}

// ─── Table ─────────────────────────────────────────────

void MainWindow::populateRow(int row, const Transaction &t)
{
    QLocale loc;
    auto setItem = [&](int col, const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        ui->tableTransactions->setItem(row, col, item);
    };
    setItem(0, t.numFacture);
    setItem(1, t.pecheur);
    setItem(2, t.lot);
    setItem(3, loc.toString(t.prixKg, 'f', 2));
    setItem(4, loc.toString(t.quantite, 'f', 3));
    setItem(5, loc.toString(t.total, 'f', 2));
    setItem(6, t.dateTransaction.toString("yyyy-MM-dd HH:mm"));
    setItem(7, t.modePaiement);
    setItem(8, t.statutPaiement);
}

int MainWindow::resolvePecheurId(const QString &input, bool &ok) const
{
    ok = false;
    const QString trimmed = input.trimmed();
    if (trimmed.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Le champ 'Pêcheur' est vide."));
        return -1;
    }

    // If already numeric, accept directly
    bool isNum = false;
    int id = trimmed.toInt(&isNum);
    if (isNum) {
        ok = true;
        return id;
    }

    // Try to lookup by name in UTILISATEUR table using a few common name columns
    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen()) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Base de données non connectée."));
        return -1;
    }

    const QString nameUpper = trimmed.toUpper();
    const QStringList nameCols = {"NOM", "NOM_UTILISATEUR", "USERNAME", "NAME", "LOGIN"};
    const QStringList idCols = {"ID", "ID_UTILISATEUR", "NUMUTIL", "IDUSER", "ID_UTIL"};

    QSqlQuery q(db);
    for (const QString &idCol : idCols) {
        for (const QString &nameCol : nameCols) {
            const QString sql = QString("SELECT %1 FROM UTILISATEUR WHERE UPPER(%2)=:name").arg(idCol, nameCol);
            if (!q.prepare(sql)) continue;
            q.bindValue(":name", nameUpper);
            if (!q.exec()) continue;
            if (q.next()) {
                bool okConv = false;
                int found = q.value(0).toInt(&okConv);
                if (okConv) { ok = true; return found; }
            }
        }
    }

    // Try LIKE search on common name columns and pick first match
    for (const QString &idCol : idCols) {
        for (const QString &nameCol : nameCols) {
            const QString sql = QString("SELECT %1 FROM UTILISATEUR WHERE UPPER(%2) LIKE :name").arg(idCol, nameCol);
            if (!q.prepare(sql)) continue;
            q.bindValue(":name", QStringLiteral("%") + nameUpper + QStringLiteral("%"));
            if (!q.exec()) continue;
            if (q.next()) {
                bool okConv = false;
                int found = q.value(0).toInt(&okConv);
                if (okConv) { ok = true; return found; }
            }
        }
    }

    // Not found: attempt to create a new UTILISATEUR entry with this name.
    // Detect available id/name columns from the data dictionary.
    QSqlQuery colq(db);
    if (!colq.exec("SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'UTILISATEUR'")) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Impossible d'interroger la structure de UTILISATEUR: %1").arg(colq.lastError().text()));
        return -1;
    }
    QStringList existingCols;
    while (colq.next()) {
        existingCols.append(colq.value(0).toString().toUpper());
    }

    // choose id column
    QString detectedIdCol;
    for (const QString &c : idCols) if (existingCols.contains(c)) { detectedIdCol = c; break; }
    if (detectedIdCol.isEmpty()) {
        // fallback: pick first numeric column
        QSqlQuery colq2(db);
        colq2.exec("SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'UTILISATEUR'");
        while (colq2.next()) {
            const QString cname = colq2.value(0).toString().toUpper();
            const QString dtype = colq2.value(1).toString().toUpper();
            if (dtype.contains("NUMBER") || dtype.contains("INT")) { detectedIdCol = cname; break; }
        }
    }

    // try multiple candidate name columns (prefer exact NOM)
    QString detectedNameCol;
    const QStringList tryNameCols = {"NOM", "NOM_UTILISATEUR", "USERNAME", "NAME", "LOGIN"};
    for (const QString &c : tryNameCols) if (existingCols.contains(c)) { detectedNameCol = c; break; }

    if (detectedIdCol.isEmpty() || detectedNameCol.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Impossible de trouver colonnes id/nom dans UTILISATEUR (id=%1, name=%2)").arg(detectedIdCol, detectedNameCol));
        return -1;
    }

    // Compute a new ID (MAX+1) — simple strategy when no sequence is available.
    QSqlQuery qmax(db);
    const QString maxSql = QString("SELECT NVL(MAX(%1),0)+1 FROM UTILISATEUR").arg(detectedIdCol);
    if (!qmax.exec(maxSql) || !qmax.next()) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Impossible de calculer un nouvel ID pour UTILISATEUR: %1").arg(qmax.lastError().text()));
        return -1;
    }
    int newId = qmax.value(0).toInt();

    // Discover NOT NULL columns and provide defaults for them so INSERT won't fail.
    QSqlQuery meta(db);
    if (!meta.exec("SELECT column_name, data_type, nullable FROM user_tab_columns WHERE table_name = 'UTILISATEUR'")) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Impossible d'interroger la structure de UTILISATEUR: %1").arg(meta.lastError().text()));
        return -1;
    }

    struct ColSpec { QString name; QString type; QString nullable; };
    QList<ColSpec> cols;
    while (meta.next()) {
        cols.append({ meta.value(0).toString().toUpper(), meta.value(1).toString().toUpper(), meta.value(2).toString().toUpper() });
    }

    // Build list of columns we must supply: detectedIdCol plus any NOT NULL columns (nullable == 'N')
    QStringList insertCols;
    insertCols.append(detectedIdCol);
    for (const ColSpec &c : cols) {
        if (c.name == detectedIdCol) continue;
        if (c.nullable == "N") insertCols.append(c.name);
    }

    // Ensure detectedNameCol is included so name is set
    if (!insertCols.contains(detectedNameCol)) insertCols.append(detectedNameCol);

    // Prepare placeholders and values
    QStringList placeholders;
    QVariantList bindVals;
    for (const QString &cname : insertCols) {
        placeholders.append(":" + cname);
        // Determine default value based on column semantics
        QString valUpper = cname.toUpper();
        QVariant v;
        // Find type for this column
        QString ctype;
        for (const ColSpec &cs : cols) if (cs.name == cname) { ctype = cs.type; break; }

        if (cname == detectedIdCol) {
            v = newId;
        } else if (cname == detectedNameCol || valUpper.contains("NOM") || valUpper.contains("NAME") || valUpper.contains("LOGIN")) {
            v = trimmed.isEmpty() ? QString("UNKNOWN") : trimmed;
        } else if (valUpper.contains("PRENOM")) {
            // try to extract first token as prenom, else reuse trimmed or 'N/A'
            QStringList parts = trimmed.split(' ', Qt::SkipEmptyParts);
            v = (!parts.isEmpty() ? parts.first() : (trimmed.isEmpty() ? QString("UNKNOWN") : trimmed));
        } else if (ctype.contains("CHAR") || ctype.contains("VARCHAR") || ctype.contains("CLOB")) {
            v = QString("N/A");
        } else if (ctype.contains("DATE") || ctype.contains("TIMESTAMP")) {
            v = QDateTime::currentDateTime();
        } else if (ctype.contains("NUMBER") || ctype.contains("INT")) {
            v = 0;
        } else {
            v = QString("N/A");
        }
        bindVals.append(v);
    }

    // Build and execute INSERT
    const QString insColsStr = insertCols.join(", ");
    const QString phStr = placeholders.join(", ");
    QString insStmt = QString("INSERT INTO UTILISATEUR (%1) VALUES (%2)").arg(insColsStr, phStr);
    QSqlQuery qins(db);
    if (!qins.prepare(insStmt)) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Préparation INSERT UTILISATEUR a échoué: %1\nSQL=%2").arg(qins.lastError().text(), insStmt));
        return -1;
    }
    for (int i = 0; i < insertCols.size(); ++i) {
        qins.bindValue(":" + insertCols[i], bindVals[i]);
    }

    if (!qins.exec()) {
        QMessageBox::warning(nullptr, tr("Erreur"), tr("Impossible de créer UTILISATEUR: %1\nSQL=%2").arg(qins.lastError().text(), insStmt));
        return -1;
    }
    // Commit so the new id is visible
    QSqlQuery qcom(db);
    qcom.exec("COMMIT");
    ok = true;
    return newId;
}

QString MainWindow::userNameForId(int id) const
{
    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen()) return QString();

    // Discover candidate id columns (numeric) and name columns, then try combinations
    QSqlQuery meta(db);
    if (!meta.exec("SELECT column_name, data_type FROM user_tab_columns WHERE table_name = 'UTILISATEUR'"))
        return QString();

    QStringList idCandidates;
    QStringList nameCandidates = {"NOM", "NOM_UTILISATEUR", "USERNAME", "NAME", "LOGIN"};
    while (meta.next()) {
        QString cname = meta.value(0).toString().toUpper();
        QString dtype = meta.value(1).toString().toUpper();
        if (dtype.contains("NUMBER") || dtype.contains("INT")) idCandidates.append(cname);
    }

    QSqlQuery q(db);
    for (const QString &idCol : idCandidates) {
        for (const QString &nameCol : nameCandidates) {
            QString sql = QString("SELECT %1 FROM UTILISATEUR WHERE %2 = :id").arg(nameCol, idCol);
            if (!q.prepare(sql)) continue;
            q.bindValue(":id", id);
            if (!q.exec()) continue;
            if (q.next()) {
                QString found = q.value(0).toString();
                if (!found.trimmed().isEmpty()) return found;
            }
        }
    }
    return QString();
}

QString MainWindow::colName(const QString &logical) const
{
    QString k = logical.toUpper();
    k.remove('_');
    if (m_colMap.contains(k)) return m_colMap.value(k);

    // Levenshtein distance helper (small, acceptable cost for small maps)
    auto levenshtein = [](const QString &s1, const QString &s2) {
        const int n = s1.size();
        const int m = s2.size();
        if (n == 0) return m;
        if (m == 0) return n;
        QVector<int> prev(m+1), cur(m+1);
        for (int j = 0; j <= m; ++j) prev[j] = j;
        for (int i = 1; i <= n; ++i) {
            cur[0] = i;
            for (int j = 1; j <= m; ++j) {
                int cost = (s1[i-1] == s2[j-1]) ? 0 : 1;
                cur[j] = std::min(std::min(prev[j] + 1, cur[j-1] + 1), prev[j-1] + cost);
            }
            prev = cur;
        }
        return cur[m];
    };

    // Find the best candidate by comparing normalized keys and column names
    int bestDist = INT_MAX;
    QString bestVal;
    for (auto it = m_colMap.constBegin(); it != m_colMap.constEnd(); ++it) {
        const QString mappedKey = it.key();
        int d = levenshtein(k, mappedKey);
        if (d < bestDist) { bestDist = d; bestVal = it.value(); }
        // also compare against the actual column name without underscores
        QString mappedVal = it.value().toUpper(); mappedVal.remove('_');
        int d2 = levenshtein(k, mappedVal);
        if (d2 < bestDist) { bestDist = d2; bestVal = it.value(); }
    }
    // Accept small differences (typos like PECHEUR vs PECHER)
    if (bestDist <= 2 && !bestVal.isEmpty()) return bestVal;
    return logical;
}

QStringList MainWindow::allowedValuesForCheckConstraint(const QString &table, const QString &column) const
{
    QStringList res;
    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen()) return res;

    // Collect all IN(...) lists from CHECK constraints on the table (USER then ALL)
    QSqlQuery q(db);
    const QString tblUp = table.toUpper();
    const QStringList queries = {
        QString("SELECT search_condition FROM user_constraints WHERE table_name = '%1' AND constraint_type = 'C'").arg(tblUp),
        QString("SELECT search_condition FROM all_constraints WHERE table_name = '%1' AND constraint_type = 'C'").arg(tblUp)
    };

    QRegularExpression rx("IN\\s*\\(([^)]+)\\)", QRegularExpression::CaseInsensitiveOption);
    QSet<QString> seen;
    for (const QString &sql : queries) {
        if (!q.exec(sql)) continue;
        while (q.next()) {
            QString cond = q.value(0).toString();
            if (cond.isEmpty()) continue;
            // find all IN(...) occurrences
            QRegularExpressionMatchIterator it = rx.globalMatch(cond);
            while (it.hasNext()) {
                QRegularExpressionMatch m = it.next();
                QString inside = m.captured(1);
                const QStringList parts = inside.split(',', Qt::SkipEmptyParts);
                for (QString p : parts) {
                    p = p.trimmed();
                    if (p.startsWith("'") && p.endsWith("'") && p.size() >= 2) p = p.mid(1, p.size()-2);
                    if (!p.isEmpty() && !seen.contains(p)) { seen.insert(p); res.append(p); }
                }
            }
        }
        if (!res.isEmpty()) return res; // return early if we found values
    }
    return res;
}

void MainWindow::refreshTable()
{
    QVector<Transaction> data = filteredAndSorted();
    ui->tableTransactions->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i)
        populateRow(i, data[i]);

    refreshStats();
    // Anomalies now handled by chat assistant
    refreshPrevisions();
}

// ─── PDF ───────────────────────────────────────────────

void MainWindow::onExportPDF()
{
    if (m_transactions.isEmpty()) {
        QMessageBox::information(this, tr("PDF"), tr("Aucune transaction à exporter."));
        return;
    }

    // Ask user for invoice number
    bool ok;
    QString invoiceNum = QInputDialog::getText(this, tr("Exporter facture"), 
                                               tr("Numéro de facture:"), 
                                               QLineEdit::Normal, QString(), &ok);
    if (!ok || invoiceNum.trimmed().isEmpty()) return;
    invoiceNum = invoiceNum.trimmed();

    // Find the transaction with this invoice number
    auto data = filteredAndSorted();
    const Transaction *found = nullptr;
    for (const auto &t : data) {
        if (t.numFacture == invoiceNum) {
            found = &t;
            break;
        }
    }

    if (!found) {
        QMessageBox::information(this, tr("PDF"), 
                                tr("Facture %1 non trouvée dans la sélection.").arg(invoiceNum));
        return;
    }

    QString path = QFileDialog::getSaveFileName(
        this, tr("Exporter facture"), QString("facture_%1.pdf").arg(invoiceNum), tr("PDF (*.pdf)"));
    if (path.isEmpty()) return;

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::warning(this, tr("PDF"), tr("Impossible de créer le fichier PDF."));
        return;
    }

    QLocale loc;
    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont bodyFont("Arial", 9);
    int pageW = writer.width();
    int y = 100;

    // Export the found transaction
    const Transaction &t = *found;

    // Title
    painter.setFont(titleFont);
    painter.drawText(QRect(0, y, pageW, 200), Qt::AlignHCenter, "VISION SIGHT - Facture");
    y += 250;

    // Transaction details
    painter.setFont(headerFont);
    QStringList labels = {
        tr("Num Facture:"), tr("Pêcheur:"), tr("Lot:"),
        tr("Prix/kg:"), tr("Quantité:"), tr("Total:"),
        tr("Date:"), tr("Mode paiement:"), tr("Statut:")
    };
    QStringList values = {
        t.numFacture, t.pecheur, t.lot,
        loc.toString(t.prixKg, 'f', 2),
        loc.toString(t.quantite, 'f', 3),
        loc.toString(t.total, 'f', 2),
        t.dateTransaction.toString("yyyy-MM-dd HH:mm"),
        t.modePaiement, t.statutPaiement
    };

    for (int i = 0; i < labels.size(); ++i) {
        painter.setFont(headerFont);
        painter.drawText(200, y, labels[i]);
        painter.setFont(bodyFont);
        painter.drawText(900, y, values[i]);
        y += 120;
    }

    y += 200;

    // Add QR Code with invoice data
    painter.setFont(headerFont);
    painter.drawText(200, y, "Code QR - Scannez pour voir la facture:");
    y += 150;

    // Create QR code data using VCARD format - displays as popup on phone scan
    QString qrData = QString(
        "BEGIN:VCARD\n"
        "VERSION:3.0\n"
        "FN:FACTURE %1\n"
        "ORG:VISION SIGHT\n"
        "NOTE:Facture %1\\n"
        "Pêcheur: %2\\n"
        "Poisson: %3\\n"
        "Total: %4 TND\\n"
        "Date: %5\n"
        "END:VCARD"
    ).arg(t.numFacture)
     .arg(t.numFacture)
     .arg(t.pecheur)
     .arg(t.lot)
     .arg(QString::number(t.total, 'f', 2))
     .arg(t.dateTransaction.toString("yyyy-MM-dd"));

    // Generate QR Code image from text using simple encoding
    QPixmap qrPixmap = generateQRCode(qrData, 200);
    
    // Draw QR Code on PDF
    if (!qrPixmap.isNull()) {
        painter.drawPixmap(200, y, 400, 400, qrPixmap);
    } else {
        painter.setFont(bodyFont);
        painter.drawText(200, y, "QR Code non généré");
    }

    painter.end();
    QMessageBox::information(this, tr("PDF"),
                             tr("Facture %1 exportée : %2").arg(invoiceNum, path));
}

// ─── QR Code Generation ────────────────────────────────

QPixmap MainWindow::generateQRCode(const QString &text, int size)
{
    // Option: Use online QR API for simple QR code generation
    // This avoids adding a new qrencode/zint library dependency
    
    if (text.isEmpty()) {
        return QPixmap();
    }
    
    // Use QR API service (chart.googleapis.com or qr-server)
    // Format: https://api.qrserver.com/v1/create-qr-code/?size=200x200&data=...
    
    QNetworkAccessManager manager;
    QUrl url(QString("https://api.qrserver.com/v1/create-qr-code/?size=%1x%1&data=%2")
             .arg(size)
             .arg(QString(QUrl::toPercentEncoding(text))));
    
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt/Vision-Sight");
    
    QEventLoop loop;
    QNetworkReply *reply = manager.get(request);
    
    // Connect signals to break the event loop
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    
    // Timeout of 5 seconds
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, reply, &QNetworkReply::abort);
    timer.start(5000);
    
    loop.exec();
    timer.stop();
    
    QPixmap qrPixmap;
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        qrPixmap.loadFromData(data);
        qDebug() << "QR Code generated successfully, size:" << qrPixmap.size();
    } else {
        qDebug() << "QR Code generation failed:" << reply->errorString();
        // Fallback: Create a simple placeholder QR-like image (gray pattern)
        qrPixmap = QPixmap(size, size);
        qrPixmap.fill(Qt::white);
        QPainter p(&qrPixmap);
        p.setPen(Qt::gray);
        for (int i = 0; i < size; i += 20) {
            p.drawLine(i, 0, i, size);
            p.drawLine(0, i, size, i);
        }
        p.end();
    }
    
    reply->deleteLater();
    return qrPixmap;
}

// ─── Statistics ────────────────────────────────────────

void MainWindow::refreshStats()
{
    QLocale loc;
    QDate today = QDate::currentDate();

    double caJour = 0, caMois = 0, caAnnee = 0;
    QMap<QString, double> qteParEspece;
    QMap<QString, double> revParEspece;

    for (const auto &t : m_transactions) {
        QDate d = t.dateTransaction.date();
        if (d == today)         caJour  += t.total;
        if (d.year() == today.year() && d.month() == today.month())
            caMois += t.total;
        if (d.year() == today.year())
            caAnnee += t.total;

        qteParEspece[t.lot] += t.quantite;
        revParEspece[t.lot] += t.total;
    }

    double caMoyen = m_transactions.isEmpty() ? 0.0 : caAnnee / m_transactions.size();

    ui->lblChartValue_jour->setText(loc.toString(caJour, 'f', 2));
    ui->lblChartValue_mois->setText(loc.toString(caMois, 'f', 2));
    ui->lblChartValue_annee->setText(loc.toString(caAnnee, 'f', 2));
    ui->lblChartValue_moyen->setText(loc.toString(caMoyen, 'f', 2));

    // Best species by quantity (+ vendu)
    QString bestQte;
    double maxQ = 0;
    for (auto it = qteParEspece.cbegin(); it != qteParEspece.cend(); ++it) {
        if (it.value() > maxQ) { maxQ = it.value(); bestQte = it.key(); }
    }
    ui->lblChartQTE->setText(bestQte.isEmpty() ? "--" : bestQte);
    ui->lblChartUnitQTE->setText(bestQte.isEmpty() ? "TND" : loc.toString(maxQ, 'f', 2) + " TND");

    // Worst species by revenue (- vendu)
    QString worstRev;
    double minR = std::numeric_limits<double>::max();
    for (auto it = revParEspece.cbegin(); it != revParEspece.cend(); ++it) {
        if (it.value() < minR) { minR = it.value(); worstRev = it.key(); }
    }
    ui->lblChartREV->setText(worstRev.isEmpty() ? "--" : worstRev);
    ui->lblChartUnitREV->setText(worstRev.isEmpty() ? "TND" : loc.toString(minR, 'f', 2) + " TND");
}

// ─── Anomaly detection ────────────────────────────────

void MainWindow::refreshAnomalies()
{
    // Anomalies are now displayed via the chat assistant interface in Options Avancées
    // This function is kept for compatibility but no longer updates the UI table
}

// ─── Forecast ──────────────────────────────────────────

void MainWindow::refreshPrevisions()
{
    // Fish pricing display is now handled in onFishSelected()
}

// ─── Fish Pricing ──────────────────────────────────────

void MainWindow::onFishSelected(const QString &fishName)
{
    // Ignore placeholder and wait until prices are loaded
    if (fishName.isEmpty() || fishName.startsWith("--") || !m_fishPricesLoaded) {
        if (!m_fishPricesLoaded) {
            qDebug() << "Waiting for fish prices to load...";
        }
        ui->priceResultLabel->setText("Sélectionnez un poisson pour voir le prix estimé");
        return;
    }

    FishPrice fishInfo = m_fishPricing->getFishInfo(fishName);
    
    if (fishInfo.averagePrice < 0) {
        qDebug() << "Fish not found:" << fishName;
        qDebug() << "Available fishes:" << m_fishPricing->getAvailableFishes();
        ui->priceResultLabel->setText("<div style='color: #ff6b6b;'><b>⚠ Erreur:</b> Poisson non trouvé dans la base de données</div>");
    } else {
        QString result = QString(
            "<div style='color: #0033cc; font-weight: bold; text-align: center; font-size: 14px;'>%1</div>"
            "<div style='color: #1e90ff; text-align: center; font-size: 22px; margin: 12px 0; font-weight: bold;'>"
            "%2 TND/kg</div>"
            "<div style='color: #0033cc; text-align: center; font-size: 12px;'>"
            "Min: %3 | Max: %4<br>"
            "<span style='font-size: 11px;'>Basé sur %5 transactions</span></div>"
        ).arg(fishName)
         .arg(QString::number(fishInfo.averagePrice, 'f', 2))
         .arg(QString::number(fishInfo.minPrice, 'f', 2))
         .arg(QString::number(fishInfo.maxPrice, 'f', 2))
         .arg(QString::number(fishInfo.count));
        
        ui->priceResultLabel->setText(result);
        qDebug() << "Price displayed for " << fishName << " : " << fishInfo.averagePrice << "TND/kg";
    }
}

void MainWindow::onAfficherPrix()
{
    QString selectedFish = ui->fishComboBox->currentText();
    
    if (selectedFish.isEmpty() || selectedFish.startsWith("--")) {
        ui->priceResultLabel->setText("<div style='color: #ff6b6b;'><b>⚠</b> Veuillez sélectionner un poisson</div>");
        qDebug() << "No fish selected";
        return;
    }

    qDebug() << "Button clicked: Getting price for:" << selectedFish;
    qDebug() << "Fish prices loaded:" << m_fishPricesLoaded;
    qDebug() << "Available fishes:" << m_fishPricing->getAvailableFishes();

    FishPrice fishInfo = m_fishPricing->getFishInfo(selectedFish);
    
    if (fishInfo.averagePrice < 0) {
        qDebug() << "Fish not found in database:" << selectedFish;
        qDebug() << "Available:" << m_fishPricing->getAvailableFishes();
        ui->priceResultLabel->setText(QString("<div style='color: #ff6b6b;'><b>⚠ Erreur:</b> '%1' non trouvé<br><small>Poissons disponibles: %2</small></div>")
            .arg(selectedFish, m_fishPricing->getAvailableFishes().join(", ")));
    } else {
        QString result = QString(
            "<div style='color: #0033cc; font-weight: bold; text-align: center; font-size: 14px;'>%1</div>"
            "<div style='color: #1e90ff; text-align: center; font-size: 22px; margin: 12px 0; font-weight: bold;'>"
            "%2 TND/kg</div>"
            "<div style='color: #0033cc; text-align: center; font-size: 12px;'>"
            "Min: %3 TND | Max: %4 TND<br>"
            "<span style='font-size: 11px;'>Basé sur %5 transactions</span></div>"
        ).arg(selectedFish)
         .arg(QString::number(fishInfo.averagePrice, 'f', 2))
         .arg(QString::number(fishInfo.minPrice, 'f', 2))
         .arg(QString::number(fishInfo.maxPrice, 'f', 2))
         .arg(QString::number(fishInfo.count));
        
        ui->priceResultLabel->setText(result);
        qDebug() << "Price displayed for" << selectedFish << ":" << fishInfo.averagePrice << "TND/kg";
    }
}

