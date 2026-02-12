#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QUrl>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMap>
#include <QPageSize>
#include <QCalendarWidget>
#include <QTextCharFormat>
#include <QTimer>
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QFrame>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <algorithm>

static QTableWidgetItem* centeredItem(const QString &text) {
    auto *item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

namespace navires {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nextId(4)
{
    ui->setupUi(this);

    setupSidebarIndicator();

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::onWeatherReply);

    connect(ui->btnAjouter,    &QPushButton::clicked, this, &MainWindow::onAjouterClicked);
    connect(ui->btnModifier,   &QPushButton::clicked, this, &MainWindow::onModifierClicked);
    connect(ui->btnSupprimer,  &QPushButton::clicked, this, &MainWindow::onSupprimerClicked);

    connect(ui->searchType,      &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchNom,       &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchMatricule, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchPosition,  &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);

    connect(ui->comboSort, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);

    connect(ui->btnExportPDF, &QPushButton::clicked, this, &MainWindow::onExportPDF);
    connect(ui->btnFetchWeather, &QPushButton::clicked, this, &MainWindow::onFetchWeather);
    connect(ui->btnAddMouvement, &QPushButton::clicked, this, &MainWindow::onAddMouvement);

    // Installer un event filter sur le viewport interne du calendrier
    // pour capturer tous les clics de manière fiable
    QAbstractItemView *calendarView = ui->calendarMouvements->findChild<QAbstractItemView*>();
    if (calendarView) {
        calendarView->viewport()->installEventFilter(this);
    }

    connect(ui->tableNavires, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);
    connect(ui->checkSelectAll, &QCheckBox::stateChanged, this, &MainWindow::onSelectAllChanged);

    // Sidebar buttons - mutually exclusive
    connect(ui->btnGestionEmployees, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btnGestionNavires, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btnGestionQuais, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btnGestionCaptures, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btnStockageFrigorifique, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);
    connect(ui->btnGestionVentes, &QPushButton::clicked, this, &MainWindow::onSidebarButtonClicked);

#ifdef INTEGRATION_APP
    connect(ui->btnGestionQuais, &QPushButton::clicked, this, &MainWindow::onSidebarQuais);
    connect(ui->btnGestionVentes, &QPushButton::clicked, this, &MainWindow::onSidebarVentes);
    connect(ui->btnGestionCaptures, &QPushButton::clicked, this, &MainWindow::onSidebarCaptures);
#endif

    loadSampleData();
    refreshTable();
    refreshMouvementsTable();
    updateStats();

    // Style table row selection
    ui->tableNavires->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableNavires->setSelectionMode(QAbstractItemView::MultiSelection);

    // Center all table headers
    ui->tableNavires->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableMaintenance->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableRevenus->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableMouvements->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // Intercepter les clics sur le viewport du calendrier
    if (event->type() == QEvent::MouseButtonRelease) {
        QAbstractItemView *calendarView = ui->calendarMouvements->findChild<QAbstractItemView*>();
        if (calendarView && obj == calendarView->viewport()) {
            // Attendre que le calendrier mette à jour sa sélection
            QTimer::singleShot(0, this, [this]() {
                QDate date = ui->calendarMouvements->selectedDate();
                if (date.isValid()) {
                    onCalendarDateClicked(date);
                }
            });
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSidebarIndicator()
{
    if (!ui || !ui->sidebar)
        return;

    m_sidebarActiveIndicator = new QFrame(ui->sidebar);
    m_sidebarActiveIndicator->setObjectName("activeIndicator");
    m_sidebarActiveIndicator->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    m_sidebarActiveIndicator->setStyleSheet(
        "QFrame#activeIndicator {"
        "  background: rgba(57,192,250,22);"
        "  border: 1px solid rgba(57,192,250,90);"
        "  border-radius: 10px;"
        "}"
    );
    m_sidebarActiveIndicator->hide();
    m_sidebarActiveIndicator->lower();

    m_sidebarIndicatorAnim = new QPropertyAnimation(m_sidebarActiveIndicator, "geometry", this);
    m_sidebarIndicatorAnim->setDuration(180);
    m_sidebarIndicatorAnim->setEasingCurve(QEasingCurve::OutCubic);

    QTimer::singleShot(0, this, [this]() {
        moveSidebarIndicatorTo(ui->btnGestionNavires, /*animate*/ false);
    });
}

QRect MainWindow::sidebarIndicatorTargetRect(QPushButton *btn) const
{
    if (!btn)
        return {};

    return btn->geometry();
}

void MainWindow::moveSidebarIndicatorTo(QPushButton *btn, bool animate)
{
    if (!m_sidebarActiveIndicator || !m_sidebarIndicatorAnim || !btn)
        return;

    const QRect target = sidebarIndicatorTargetRect(btn);
    if (!target.isValid())
        return;

    m_sidebarActiveIndicator->show();
    m_sidebarActiveIndicator->lower();

    m_sidebarIndicatorAnim->stop();
    if (!animate) {
        m_sidebarActiveIndicator->setGeometry(target);
        return;
    }

    const QRect start = m_sidebarActiveIndicator->geometry().isValid()
                            ? m_sidebarActiveIndicator->geometry()
                            : target;
    m_sidebarIndicatorAnim->setStartValue(start);
    m_sidebarIndicatorAnim->setEndValue(target);
    m_sidebarIndicatorAnim->start();
}

void MainWindow::setSidebarActiveNavires()
{
    ui->btnGestionEmployees->setChecked(false);
    ui->btnGestionNavires->setChecked(true);
    ui->btnGestionQuais->setChecked(false);
    ui->btnGestionCaptures->setChecked(false);
    ui->btnStockageFrigorifique->setChecked(false);
    ui->btnGestionVentes->setChecked(false);

    ui->btnGestionNavires->style()->unpolish(ui->btnGestionNavires);
    ui->btnGestionNavires->style()->polish(ui->btnGestionNavires);

    moveSidebarIndicatorTo(ui->btnGestionNavires, /*animate*/ false);
}

void MainWindow::loadSampleData()
{
    navires.append(Navire(1, QStringLiteral("Oc\u00e9an Bleu"), "M-1234", QStringLiteral("P\u00eache"), 800, "Actif", "Port A", 15));
    navires.append(Navire(2, "Mistral", "M-1235", "Transport", 1200, "En maintenance", "Port B", 8));
    navires.append(Navire(3, "Albatros", "M-1236", QStringLiteral("P\u00eache"), 500, "Actif", "Port A", 12));

    Mouvement m1;
    m1.navireNom = QStringLiteral("Oc\u00e9an Bleu");
    m1.heureDepart = QDateTime(QDate(2026, 2, 8), QTime(5, 30));
    m1.heureRetour = QDateTime(QDate(2026, 2, 8), QTime(18, 45));
    m1.cargaisonRetour = "350 kg sardines, 120 kg thon";
    m1.distanceKm = 85.0;
    mouvements.append(m1);

    Mouvement m2;
    m2.navireNom = "Albatros";
    m2.heureDepart = QDateTime(QDate(2026, 2, 9), QTime(4, 0));
    m2.heureRetour = QDateTime(QDate(2026, 2, 9), QTime(16, 30));
    m2.cargaisonRetour = "200 kg crevettes";
    m2.distanceKm = 62.5;
    mouvements.append(m2);
}

void MainWindow::refreshTable()
{
    applyFilters();
}

void MainWindow::refreshMouvementsTable()
{
    ui->tableMouvements->setRowCount(0);
    for (const Mouvement &m : std::as_const(mouvements)) {
        int row = ui->tableMouvements->rowCount();
        ui->tableMouvements->insertRow(row);
        ui->tableMouvements->setItem(row, 0, centeredItem(m.navireNom));
        ui->tableMouvements->setItem(row, 1, centeredItem(m.heureDepart.toString("dd/MM/yyyy HH:mm")));
        ui->tableMouvements->setItem(row, 2, centeredItem(m.heureRetour.toString("dd/MM/yyyy HH:mm")));
        ui->tableMouvements->setItem(row, 3, centeredItem(m.cargaisonRetour));
        ui->tableMouvements->setItem(row, 4, centeredItem(QString::number(m.distanceKm, 'f', 1)));
    }
    ui->tableMouvements->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    highlightCalendarDates();
}

void MainWindow::refreshMouvementsForDate(const QDate &date)
{
    ui->tableMouvements->setRowCount(0);
    for (const Mouvement &m : std::as_const(mouvements)) {
        if (m.heureDepart.date() == date || m.heureRetour.date() == date) {
            int row = ui->tableMouvements->rowCount();
            ui->tableMouvements->insertRow(row);
            ui->tableMouvements->setItem(row, 0, centeredItem(m.navireNom));
            ui->tableMouvements->setItem(row, 1, centeredItem(m.heureDepart.toString("HH:mm")));
            ui->tableMouvements->setItem(row, 2, centeredItem(m.heureRetour.toString("HH:mm")));
            ui->tableMouvements->setItem(row, 3, centeredItem(m.cargaisonRetour));
            ui->tableMouvements->setItem(row, 4, centeredItem(QString::number(m.distanceKm, 'f', 1)));
        }
    }
    ui->tableMouvements->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::highlightCalendarDates()
{
    QTextCharFormat normalFmt;
    normalFmt.setBackground(Qt::transparent);
    QTextCharFormat mvtFmt;
    mvtFmt.setBackground(QColor(0x1e, 0x8d, 0xe0));
    mvtFmt.setForeground(Qt::white);

    // Reset all dates
    QDate start(2020, 1, 1);
    QDate end(2035, 12, 31);
    for (QDate d = start; d <= end; d = d.addDays(1))
        ui->calendarMouvements->setDateTextFormat(d, normalFmt);

    // Highlight dates with movements
    for (const Mouvement &m : std::as_const(mouvements)) {
        ui->calendarMouvements->setDateTextFormat(m.heureDepart.date(), mvtFmt);
        if (m.heureRetour.date() != m.heureDepart.date())
            ui->calendarMouvements->setDateTextFormat(m.heureRetour.date(), mvtFmt);
    }
}

void MainWindow::onCalendarDateClicked(const QDate &date)
{
    refreshMouvementsForDate(date);
}

void MainWindow::onAjouterClicked()
{
    DialogNavire dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Navire n = dlg.getNavire();
        if (n.nom.isEmpty() || n.matricule.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom et le matricule sont obligatoires.");
            return;
        }
        n.id        = nextId++;
        n.nbSorties = 0;
        navires.append(n);
        applyFilters();
        updateStats();
    }
}

void MainWindow::onModifierClicked()
{
    int row = ui->tableNavires->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Modifier", QStringLiteral("Veuillez s\u00e9lectionner un navire dans le tableau."));
        return;
    }

    QString matricule = ui->tableNavires->item(row, 2)->text();
    int idx = -1;
    for (int i = 0; i < navires.size(); ++i) {
        if (navires[i].matricule == matricule) { idx = i; break; }
    }
    if (idx < 0) return;

    Navire &n = navires[idx];
    DialogNavire dlg(n, this);
    if (dlg.exec() == QDialog::Accepted) {
        Navire updated = dlg.getNavire();
        if (updated.nom.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Le nom est obligatoire.");
            return;
        }
        n.nom       = updated.nom;
        n.matricule = updated.matricule;
        n.type      = updated.type;
        n.capacite  = updated.capacite;
        n.statut    = updated.statut;
        n.position  = updated.position;
        applyFilters();
        updateStats();
    }
}

void MainWindow::onSupprimerClicked()
{
    QList<int> selectedRows;
    auto ranges = ui->tableNavires->selectedRanges();
    for (const auto &r : ranges) {
        for (int row = r.topRow(); row <= r.bottomRow(); ++row) {
            if (!selectedRows.contains(row)) selectedRows.append(row);
        }
    }
    if (selectedRows.isEmpty()) {
        QMessageBox::information(this, "Supprimer", QStringLiteral("Veuillez s\u00e9lectionner au moins un navire."));
        return;
    }

    QStringList noms;
    QStringList matricules;
    for (int row : std::as_const(selectedRows)) {
        noms.append(ui->tableNavires->item(row, 0)->text());
        matricules.append(ui->tableNavires->item(row, 2)->text());
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmer la suppression");
    msgBox.setText(QString("Supprimer %1 navire(s) ?\n\n%2")
                       .arg(noms.size())
                       .arg(noms.join(", ")));
    msgBox.setStyleSheet(R"(
        QMessageBox { background: #132437; }
        QLabel { color: #e6eef6; font-size: 14px; }
        QPushButton { background: #1e8de0; color: white; border: 0; border-radius: 8px; padding: 8px 20px; font-weight: 700; min-width: 80px; }
        QPushButton:hover { background: #2a99ec; }
    )");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() == QMessageBox::Yes) {
        for (const QString &mat : std::as_const(matricules)) {
            for (int i = navires.size() - 1; i >= 0; --i) {
                if (navires[i].matricule == mat) {
                    navires.removeAt(i);
                    break;
                }
            }
        }
        applyFilters();
        updateStats();
    }
}

void MainWindow::onSearchChanged()
{
    applyFilters();
}

void MainWindow::applyFilters()
{
    QString filterType = ui->searchType->text().trimmed().toLower();
    QString filterNom  = ui->searchNom->text().trimmed().toLower();
    QString filterMat  = ui->searchMatricule->text().trimmed().toLower();
    QString filterPos  = ui->searchPosition->text().trimmed().toLower();

    QList<Navire> filtered;
    for (const Navire &n : std::as_const(navires)) {
        bool match = true;
        if (!filterType.isEmpty() && !n.type.toLower().contains(filterType))      match = false;
        if (!filterNom.isEmpty()  && !n.nom.toLower().contains(filterNom))        match = false;
        if (!filterMat.isEmpty()  && !n.matricule.toLower().contains(filterMat))  match = false;
        if (!filterPos.isEmpty()  && !n.position.toLower().contains(filterPos))   match = false;
        if (match) filtered.append(n);
    }

    int sortIdx = ui->comboSort->currentIndex();
    switch (sortIdx) {
    case 1: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.nom.toLower() < b.nom.toLower(); }); break;
    case 2: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.nom.toLower() > b.nom.toLower(); }); break;
    case 3: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.capacite < b.capacite; }); break;
    case 4: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.capacite > b.capacite; }); break;
    case 5: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.statut.toLower() < b.statut.toLower(); }); break;
    case 6: std::sort(filtered.begin(), filtered.end(),
                  [](const Navire &a, const Navire &b){ return a.statut.toLower() > b.statut.toLower(); }); break;
    default: break;
    }

    ui->tableNavires->setRowCount(0);
    for (const Navire &n : std::as_const(filtered)) {
        int row = ui->tableNavires->rowCount();
        ui->tableNavires->insertRow(row);
        ui->tableNavires->setItem(row, 0, centeredItem(n.nom));
        ui->tableNavires->setItem(row, 1, centeredItem(QString("NAV-%1").arg(n.id, 3, 10, QChar('0'))));
        ui->tableNavires->setItem(row, 2, centeredItem(n.matricule));
        ui->tableNavires->setItem(row, 3, centeredItem(n.type));
        ui->tableNavires->setItem(row, 4, centeredItem(QString::number(n.capacite)));
        ui->tableNavires->setItem(row, 5, centeredItem(n.statut));
        ui->tableNavires->setItem(row, 6, centeredItem(n.position));
    }
    ui->tableNavires->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::onSortChanged(int)
{
    applyFilters();
}

void MainWindow::paintDonutRing(QFrame *frame, double ratio, const QColor &arcColor, const QColor &bgArc)
{
    int sz = qMin(frame->width(), frame->height());
    if (sz < 20) sz = 180;
    QPixmap pix(sz, sz);
    pix.fill(Qt::transparent);

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing, true);

    int penW = 14;
    QRectF r(penW / 2.0, penW / 2.0, sz - penW, sz - penW);

    // Background arc (full circle)
    QPen bgPen(bgArc, penW);
    bgPen.setCapStyle(Qt::RoundCap);
    p.setPen(bgPen);
    p.drawArc(r, 0, 360 * 16);

    // Foreground arc
    if (ratio > 0) {
        QPen fgPen(arcColor, penW);
        fgPen.setCapStyle(Qt::RoundCap);
        p.setPen(fgPen);
        int span = qRound(ratio * 360.0 * 16.0);
        if (span > 360 * 16) span = 360 * 16;
        p.drawArc(r, 90 * 16, -span);  // start at top, clockwise
    }
    p.end();

    frame->setStyleSheet(QString("border-image: none; background-image: url(); background: transparent;"));

    // Remove old pixmap label if exists
    QLabel *lbl = frame->findChild<QLabel*>("donutPixmap");
    if (!lbl) {
        lbl = new QLabel(frame);
        lbl->setObjectName("donutPixmap");
        lbl->setAttribute(Qt::WA_TransparentForMouseEvents);
        lbl->lower();
    }
    lbl->setPixmap(pix);
    lbl->setFixedSize(sz, sz);
    lbl->move(0, 0);
    lbl->setStyleSheet("background: transparent; border: none;");
    lbl->show();
}

void MainWindow::updateStats()
{
    // ---- Circle 1: Nombre de navires par type ----
    QMap<QString, int> types;
    for (const Navire &n : std::as_const(navires)) types[n.type]++;
    int nbTypes = types.size();
    ui->statCircleTitle1->setText("Nb par type");
    QStringList typeDetails;
    for (auto it = types.begin(); it != types.end(); ++it)
        typeDetails.append(QString("%1 %2").arg(it.key()).arg(it.value()));
    ui->statCircleValue1->setText(typeDetails.join("\n"));
    ui->statCircleSub1->setText(QString("%1 types").arg(nbTypes));
    double r1 = navires.isEmpty() ? 0 : qMin(1.0, nbTypes / 5.0);
    paintDonutRing(ui->statCircle1, r1);

    // ---- Circle 2: Navire le plus actif ----
    int maxSorties = 0;
    QString plusActif = QStringLiteral("\u2014");
    for (const Navire &n : std::as_const(navires)) {
        if (n.nbSorties > maxSorties) {
            maxSorties = n.nbSorties;
            plusActif = n.nom;
        }
    }
    ui->statCircleTitle2->setText("+ actif");
    ui->statCircleValue2->setText(plusActif);
    ui->statCircleSub2->setText(QString("%1 sorties").arg(maxSorties));
    int totalSorties = 0;
    for (const Navire &n : std::as_const(navires)) totalSorties += n.nbSorties;
    double r2 = totalSorties > 0 ? qMin(1.0, (double)maxSorties / totalSorties * 2) : 0;
    paintDonutRing(ui->statCircle2, r2);

    // ---- Circle 3: Distance moyenne parcourue ----
    double totalDist = 0;
    int nbMouvements = mouvements.size();
    for (const Mouvement &m : std::as_const(mouvements)) totalDist += m.distanceKm;
    double moyenne = nbMouvements > 0 ? totalDist / nbMouvements : 0;
    ui->statCircleTitle3->setText("Distance moyenne");
    ui->statCircleValue3->setText(QString("%1").arg(moyenne, 0, 'f', 1));
    ui->statCircleSub3->setText("km");
    double r3 = qMin(1.0, moyenne / 200.0);
    paintDonutRing(ui->statCircle3, r3);

    // ---- Maintenance table ----
    ui->tableMaintenance->setRowCount(0);
    for (const Navire &n : std::as_const(navires)) {
        if (n.statut == "En maintenance") {
            int row = ui->tableMaintenance->rowCount();
            ui->tableMaintenance->insertRow(row);
            ui->tableMaintenance->setItem(row, 0, centeredItem(QStringLiteral("\u2014")));
            ui->tableMaintenance->setItem(row, 1, centeredItem(n.nom));
            ui->tableMaintenance->setItem(row, 2, centeredItem("0,00"));
            ui->tableMaintenance->setItem(row, 3, centeredItem(QStringLiteral("\u2014")));
        }
    }
    if (ui->tableMaintenance->rowCount() == 0) {
        ui->tableMaintenance->insertRow(0);
        ui->tableMaintenance->setItem(0, 0, centeredItem(QStringLiteral("\u2014")));
        ui->tableMaintenance->setItem(0, 1, centeredItem("Aucune maintenance"));
        ui->tableMaintenance->setItem(0, 2, centeredItem("0,00"));
        ui->tableMaintenance->setItem(0, 3, centeredItem(QStringLiteral("\u2014")));
    }

    // ---- Revenus table ----
    ui->tableRevenus->setRowCount(0);
    for (const Navire &n : std::as_const(navires)) {
        int row = ui->tableRevenus->rowCount();
        ui->tableRevenus->insertRow(row);
        ui->tableRevenus->setItem(row, 0, centeredItem(n.nom.toUpper()));
        ui->tableRevenus->setItem(row, 1, centeredItem("0,00"));
        ui->tableRevenus->setItem(row, 2, centeredItem("0,00"));
        ui->tableRevenus->setItem(row, 3, centeredItem("0,00"));
    }
}

void MainWindow::onExportPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    "navires_rapport.pdf", "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&printer);
    int pageW = printer.pageRect(QPrinter::DevicePixel).width();
    int y = 100;

    QFont titleFont("Segoe UI", 24, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor(0x13, 0x24, 0x37));
    painter.drawText(100, y, "VISION SIGHT — Rapport Navires");
    y += 120;

    QFont subFont("Segoe UI", 12);
    painter.setFont(subFont);
    painter.drawText(100, y, QStringLiteral("G\u00e9n\u00e9r\u00e9 le %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")));
    y += 80;

    QString qrText = ui->lineQRCode->text().trimmed();
    if (!qrText.isEmpty()) {
        painter.drawText(100, y, QString("QR Code: %1").arg(qrText));
        y += 60;
    }

    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), 3));
    painter.drawLine(100, y, pageW - 100, y);
    y += 60;

    QFont headerFont("Segoe UI", 11, QFont::Bold);
    painter.setFont(headerFont);
    painter.setPen(QColor(0x13, 0x24, 0x37));
    QStringList headers = {"Nom", "ID", "Matricule", "Type", QStringLiteral("Capacit\u00e9"), "Statut", "Position"};
    int colW = (pageW - 200) / headers.size();
    for (int i = 0; i < headers.size(); ++i)
        painter.drawText(100 + i * colW, y, colW, 50, Qt::AlignLeft, headers[i]);
    y += 60;

    QFont dataFont("Segoe UI", 10);
    painter.setFont(dataFont);
    for (const Navire &n : std::as_const(navires)) {
        if (y > printer.pageRect(QPrinter::DevicePixel).height() - 200) {
            printer.newPage();
            y = 100;
        }
        QStringList row = {
            n.nom,
            QString("NAV-%1").arg(n.id, 3, 10, QChar('0')),
            n.matricule, n.type,
            QString::number(n.capacite),
            n.statut, n.position
        };
        for (int i = 0; i < row.size(); ++i)
            painter.drawText(100 + i * colW, y, colW, 40, Qt::AlignLeft, row[i]);
        y += 45;
    }

    y += 40;
    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), 3));
    painter.drawLine(100, y, pageW - 100, y);
    y += 60;

    painter.setFont(headerFont);
    painter.setPen(QColor(0x13, 0x24, 0x37));
    painter.drawText(100, y, "Statistiques");
    y += 50;

    painter.setFont(dataFont);
    painter.drawText(100, y, QString("Navires par type : %1").arg(ui->statCircleSub1->text()));
    y += 40;
    painter.drawText(100, y, QString("Navire le plus actif : %1 (%2)").arg(ui->statCircleValue2->text(), ui->statCircleSub2->text()));
    y += 40;
    painter.drawText(100, y, QString("Distance moyenne parcourue : %1 km").arg(ui->statCircleValue3->text()));

    painter.end();
    QMessageBox::information(this, "Export", QStringLiteral("PDF export\u00e9 avec succ\u00e8s :\n%1").arg(fileName));
}

void MainWindow::onFetchWeather()
{
    QString city = ui->lineWeatherCity->text().trimmed();
    if (city.isEmpty()) {
        ui->lblWeatherInfo->setText("Veuillez entrer un nom de ville.");
        return;
    }
    ui->lblWeatherInfo->setText("Chargement...");
    ui->lblWeatherDetails->setText("");

    QString apiKey = "YOUR_API_KEY";
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2&units=metric&lang=fr")
                      .arg(city, apiKey);
    networkManager->get(QNetworkRequest(QUrl(url)));
}

void MainWindow::onWeatherReply(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        ui->lblWeatherInfo->setText("Erreur de connexion.");
        ui->lblWeatherDetails->setText("");
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (obj.contains("cod") && obj["cod"].toInt() != 200) {
        ui->lblWeatherInfo->setText(QStringLiteral("Ville non trouv\u00e9e."));
        ui->lblWeatherDetails->setText("");
        reply->deleteLater();
        return;
    }

    QString cityName = obj["name"].toString();
    QJsonObject main = obj["main"].toObject();
    double temp = main["temp"].toDouble();
    double humidity = main["humidity"].toDouble();
    QJsonObject wind = obj["wind"].toObject();
    double windSpeed = wind["speed"].toDouble();
    QString desc = obj["weather"].toArray().first().toObject()["description"].toString();

    ui->lblWeatherInfo->setText(QStringLiteral("%1 \u2014 %2").arg(cityName, desc));
    ui->lblWeatherDetails->setText(
        QStringLiteral("%1\u00b0C  |  %2%%  |  %3 km/h")
            .arg(temp, 0, 'f', 1)
            .arg(humidity, 0, 'f', 0)
            .arg(windSpeed * 3.6, 0, 'f', 1)
    );

    reply->deleteLater();
}

void MainWindow::onAddMouvement()
{
    QStringList navireNames;
    for (const Navire &n : std::as_const(navires)) navireNames.append(n.nom);

    DialogMouvement dlg(navireNames, this);
    if (dlg.exec() == QDialog::Accepted) {
        Mouvement m = dlg.getMouvement();
        mouvements.append(m);

        for (Navire &n : navires) {
            if (n.nom == m.navireNom) {
                n.nbSorties++;
                break;
            }
        }

        refreshMouvementsTable();
        updateStats();
    }
}

void MainWindow::onTableSelectionChanged()
{
    int selectedCount = ui->tableNavires->selectionModel()->selectedRows().size();
    int totalRows = ui->tableNavires->rowCount();
    // Sync the "Select All" checkbox without re-triggering onSelectAllChanged
    ui->checkSelectAll->blockSignals(true);
    if (selectedCount == 0)
        ui->checkSelectAll->setCheckState(Qt::Unchecked);
    else if (selectedCount == totalRows && totalRows > 0)
        ui->checkSelectAll->setCheckState(Qt::Checked);
    else
        ui->checkSelectAll->setCheckState(Qt::PartiallyChecked);
    ui->checkSelectAll->blockSignals(false);
}

void MainWindow::onSidebarButtonClicked()
{
    // Get the sender button
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    // Uncheck all sidebar buttons
    ui->btnGestionEmployees->setChecked(false);
    ui->btnGestionNavires->setChecked(false);
    ui->btnGestionQuais->setChecked(false);
    ui->btnGestionCaptures->setChecked(false);
    ui->btnStockageFrigorifique->setChecked(false);
    ui->btnGestionVentes->setChecked(false);

    // Check only the clicked button
    clickedButton->setChecked(true);

    moveSidebarIndicatorTo(clickedButton, /*animate*/ true);
}

#ifdef INTEGRATION_APP
void MainWindow::onSidebarQuais()
{
    emit requestShowQuais();
}

void MainWindow::onSidebarVentes()
{
    emit requestShowTransactions();
}

void MainWindow::onSidebarCaptures()
{
    emit requestShowCaptures();
}
#endif


void MainWindow::onSelectAllChanged(int state)
{
    if (state == Qt::Checked)
        ui->tableNavires->selectAll();
    else
        ui->tableNavires->clearSelection();
}

} // namespace navires
