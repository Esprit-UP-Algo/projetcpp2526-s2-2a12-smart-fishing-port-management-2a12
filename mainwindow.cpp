#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "transactiondialog.h"

#include <QDate>
#include <QFileDialog>
#include <QLocale>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QHeaderView>
#include <QStyle>
#include <algorithm>
#include <cmath>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Table headers resize
    ui->tableTransactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableAnomalies->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablePrevisions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

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

    // Sidebar navigation buttons — highlight active
    auto sidebarButtons = {ui->btnEmployees, ui->btnNavires, ui->btnQuais,
                           ui->btnCaptures, ui->btnStockage, ui->btnVentes};
    for (auto *btn : sidebarButtons) {
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            setActiveSidebarButton(btn);
        });
    }
    // Set Gestion Des Ventes as active by default
    setActiveSidebarButton(ui->btnVentes);

    refreshTable();
}

MainWindow::~MainWindow()
{
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
}

// ─── CRUD ───────────────────────────────────────────────

void MainWindow::onCreer()
{
    TransactionDialog dlg(this, tr("Créer transaction"));
    if (dlg.exec() != QDialog::Accepted) return;

    Transaction t = dlg.transaction();
    if (t.numFacture.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le numéro de facture est obligatoire."));
        return;
    }
    // Check duplicate
    for (const auto &existing : m_transactions) {
        if (existing.numFacture.compare(t.numFacture, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, tr("Erreur"),
                                 tr("Une transaction avec ce numéro existe déjà."));
            return;
        }
    }
    m_transactions.append(t);
    refreshTable();
}

void MainWindow::onModifier()
{
    int row = ui->tableTransactions->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Modifier"),
                                 tr("Sélectionnez une ligne dans le tableau."));
        return;
    }
    // Find the matching transaction by numFacture shown in column 0
    QString numFacture = ui->tableTransactions->item(row, 0)->text();
    int idx = -1;
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].numFacture == numFacture) { idx = i; break; }
    }
    if (idx < 0) return;

    TransactionDialog dlg(this, tr("Modifier transaction"));
    dlg.setTransaction(m_transactions[idx]);
    if (dlg.exec() != QDialog::Accepted) return;

    Transaction t = dlg.transaction();
    if (t.numFacture.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le numéro de facture est obligatoire."));
        return;
    }
    // Check duplicate (excluding self)
    for (int i = 0; i < m_transactions.size(); ++i) {
        if (i == idx) continue;
        if (m_transactions[i].numFacture.compare(t.numFacture, Qt::CaseInsensitive) == 0) {
            QMessageBox::warning(this, tr("Erreur"),
                                 tr("Une transaction avec ce numéro existe déjà."));
            return;
        }
    }
    m_transactions[idx] = t;
    refreshTable();
}

void MainWindow::onSupprimer()
{
    int row = ui->tableTransactions->currentRow();
    if (row < 0) {
        QMessageBox::information(this, tr("Supprimer"),
                                 tr("Sélectionnez une ligne dans le tableau."));
        return;
    }
    QString numFacture = ui->tableTransactions->item(row, 0)->text();
    if (QMessageBox::question(this, tr("Supprimer"),
            tr("Supprimer la transaction %1 ?").arg(numFacture))
        != QMessageBox::Yes)
        return;

    for (int i = 0; i < m_transactions.size(); ++i) {
        if (m_transactions[i].numFacture == numFacture) {
            m_transactions.removeAt(i);
            break;
        }
    }
    refreshTable();
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

void MainWindow::refreshTable()
{
    QVector<Transaction> data = filteredAndSorted();
    ui->tableTransactions->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i)
        populateRow(i, data[i]);

    refreshStats();
    refreshAnomalies();
    refreshPrevisions();
}

// ─── PDF ───────────────────────────────────────────────

void MainWindow::onExportPDF()
{
    if (m_transactions.isEmpty()) {
        QMessageBox::information(this, tr("PDF"), tr("Aucune transaction à exporter."));
        return;
    }

    QString path = QFileDialog::getSaveFileName(
        this, tr("Exporter PDF"), "factures_transactions.pdf", tr("PDF (*.pdf)"));
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

    auto data = filteredAndSorted();

    for (int ti = 0; ti < data.size(); ++ti) {
        const Transaction &t = data[ti];

        if (ti > 0) {
            writer.newPage();
            y = 100;
        }

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
    }

    painter.end();
    QMessageBox::information(this, tr("PDF"),
                             tr("PDF exporté : %1").arg(path));
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
    ui->lblChartUnitQTE->setText(bestQte.isEmpty() ? "DT" : loc.toString(maxQ, 'f', 2) + " DT");

    // Worst species by revenue (- vendu)
    QString worstRev;
    double minR = std::numeric_limits<double>::max();
    for (auto it = revParEspece.cbegin(); it != revParEspece.cend(); ++it) {
        if (it.value() < minR) { minR = it.value(); worstRev = it.key(); }
    }
    ui->lblChartREV->setText(worstRev.isEmpty() ? "--" : worstRev);
    ui->lblChartUnitREV->setText(worstRev.isEmpty() ? "DT" : loc.toString(minR, 'f', 2) + " DT");
}

// ─── Anomaly detection ────────────────────────────────

void MainWindow::refreshAnomalies()
{
    // Detect anomalies: transactions with prix/kg or quantite > 2 std dev from mean
    ui->tableAnomalies->setRowCount(0);
    if (m_transactions.size() < 3) return;

    double sumP = 0, sumQ = 0;
    for (const auto &t : m_transactions) {
        sumP += t.prixKg;
        sumQ += t.quantite;
    }
    double meanP = sumP / m_transactions.size();
    double meanQ = sumQ / m_transactions.size();

    double varP = 0, varQ = 0;
    for (const auto &t : m_transactions) {
        varP += (t.prixKg  - meanP) * (t.prixKg  - meanP);
        varQ += (t.quantite - meanQ) * (t.quantite - meanQ);
    }
    double stdP = std::sqrt(varP / m_transactions.size());
    double stdQ = std::sqrt(varQ / m_transactions.size());

    QLocale loc;
    int row = 0;
    for (const auto &t : m_transactions) {
        QString motif;
        if (stdP > 0 && std::abs(t.prixKg - meanP) > 2.0 * stdP)
            motif += tr("Prix inhabituel");
        if (stdQ > 0 && std::abs(t.quantite - meanQ) > 2.0 * stdQ) {
            if (!motif.isEmpty()) motif += ", ";
            motif += tr("Quantité inhabituelle");
        }
        if (motif.isEmpty()) continue;

        ui->tableAnomalies->insertRow(row);
        auto setItem = [&](int col, const QString &text) {
            auto *item = new QTableWidgetItem(text);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableAnomalies->setItem(row, col, item);
        };
        setItem(0, t.numFacture);
        setItem(1, loc.toString(t.prixKg, 'f', 2));
        setItem(2, loc.toString(t.quantite, 'f', 3));
        setItem(3, t.lot);
        setItem(4, t.dateTransaction.toString("yyyy-MM-dd HH:mm"));
        setItem(5, motif);
        ++row;
    }
}

// ─── Forecast ──────────────────────────────────────────

void MainWindow::refreshPrevisions()
{
    ui->tablePrevisions->setRowCount(0);
    QLocale loc;
    QDate today = QDate::currentDate();

    // Compute average daily CA over the last 30 days
    QMap<QDate, double> dailyCA;
    for (const auto &t : m_transactions) {
        QDate d = t.dateTransaction.date();
        dailyCA[d] += t.total;
    }

    double sum = 0;
    int count = 0;
    for (auto it = dailyCA.cbegin(); it != dailyCA.cend(); ++it) {
        if (it.key() >= today.addDays(-30) && it.key() <= today) {
            sum += it.value();
            ++count;
        }
    }
    double avgDaily = (count > 0) ? (sum / count) : 0.0;

    // Forecast next 5 days
    for (int i = 0; i < 5; ++i) {
        QDate d = today.addDays(i);
        int row = ui->tablePrevisions->rowCount();
        ui->tablePrevisions->insertRow(row);

        auto *dateItem = new QTableWidgetItem(d.toString("yyyy-MM-dd"));
        dateItem->setFlags(dateItem->flags() & ~Qt::ItemIsEditable);
        ui->tablePrevisions->setItem(row, 0, dateItem);

        double val = (d <= today && dailyCA.contains(d)) ? dailyCA[d] : avgDaily;
        auto *valItem = new QTableWidgetItem(loc.toString(val, 'f', 2));
        valItem->setFlags(valItem->flags() & ~Qt::ItemIsEditable);
        ui->tablePrevisions->setItem(row, 1, valItem);
    }
}
