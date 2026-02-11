#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "userdialog.h"

#include <QDate>
#include <QFileDialog>
#include <QLocale>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QHeaderView>
#include <QStyle>
#include <QDesktopServices>
#include <QUrl>
#include <algorithm>
#include <cmath>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set Employee Management as active by default
    setActiveSidebarButton(ui->btnEmployees);

    // ── Employee page connections ──
    ui->tableEmployees->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSessions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatRole->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatStatut->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatService->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btnCreerUser,        &QPushButton::clicked, this, &MainWindow::onCreerUser);
    connect(ui->btnModifierUser,     &QPushButton::clicked, this, &MainWindow::onModifierUser);
    connect(ui->btnSupprimerUser,    &QPushButton::clicked, this, &MainWindow::onSupprimerUser);
    connect(ui->btnActiverDesactiver,&QPushButton::clicked, this, &MainWindow::onActiverDesactiverUser);
    connect(ui->btnRechercherUser,   &QPushButton::clicked, this, &MainWindow::onRechercherUser);
    connect(ui->comboTriUser,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTriUserChanged);
    connect(ui->comboOrdreUser, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onTriUserChanged);
    connect(ui->btnPayslipPDF,   &QPushButton::clicked, this, &MainWindow::onPayslipPDF);
    connect(ui->btnPayslipExcel, &QPushButton::clicked, this, &MainWindow::onPayslipExcel);
    connect(ui->btnPayslipEmail, &QPushButton::clicked, this, &MainWindow::onPayslipEmail);
    connect(ui->btnTerminerSession, &QPushButton::clicked, this, &MainWindow::onTerminerSession);

    // Record initial session
    Session s;
    s.utilisateur = "admin";
    s.dateConnexion = QDate::currentDate().toString("yyyy-MM-dd");
    s.heure = QTime::currentTime().toString("HH:mm:ss");
    s.ip = "127.0.0.1";
    s.action = "Connexion";
    m_sessions.append(s);


    refreshUserTable();
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
                      ui->btnCaptures, ui->btnStockage}) {
        btn->setProperty("active", false);
        btn->style()->unpolish(btn);
        btn->style()->polish(btn);
    }
    // Highlight the clicked one
    activeBtn->setProperty("active", true);
    activeBtn->style()->unpolish(activeBtn);
    activeBtn->style()->polish(activeBtn);

    // Switch pages

        ui->stackedWidget->setCurrentIndex(1);
}



// ═══════════════════════════════════════════════════════
// ═══════  EMPLOYEE MANAGEMENT  ═══════════════════════
// ═══════════════════════════════════════════════════════

void MainWindow::populateUserRow(int row, const User &u)
{
    auto makeItem = [](const QString &txt) {
        auto *it = new QTableWidgetItem(txt);
        it->setFlags(it->flags() & ~Qt::ItemIsEditable);
        return it;
    };
    ui->tableEmployees->setItem(row, 0, makeItem(u.cin));
    ui->tableEmployees->setItem(row, 1, makeItem(u.nom));
    ui->tableEmployees->setItem(row, 2, makeItem(u.prenom));
    ui->tableEmployees->setItem(row, 3, makeItem(u.email));
    ui->tableEmployees->setItem(row, 4, makeItem(u.login));
    ui->tableEmployees->setItem(row, 5, makeItem(u.role));
    ui->tableEmployees->setItem(row, 6, makeItem(u.dateCreation.toString("dd/MM/yyyy")));
    ui->tableEmployees->setItem(row, 7, makeItem(QString::number(u.heures, 'f', 1)));
    ui->tableEmployees->setItem(row, 8, makeItem(u.statut));
}

bool MainWindow::matchesUserSearch(const User &u) const
{
    QString nom   = ui->lineRechercheNomUser->text().trimmed();
    QString email = ui->lineRechercheEmailUser->text().trimmed();
    QString role  = ui->comboFilterRole->currentText();
    QString stat  = ui->comboFilterStatut->currentText();

    if (!nom.isEmpty() && !u.nom.contains(nom, Qt::CaseInsensitive)
        && !u.prenom.contains(nom, Qt::CaseInsensitive))
        return false;
    if (!email.isEmpty() && !u.email.contains(email, Qt::CaseInsensitive))
        return false;
    if (role != "Tous les rôles" && u.role != role)
        return false;
    if (stat != "Tous les statuts" && u.statut != stat)
        return false;
    return true;
}

QVector<User> MainWindow::filteredAndSortedUsers() const
{
    QVector<User> result;
    for (const auto &u : m_users)
        if (matchesUserSearch(u))
            result.append(u);

    int sortCol  = ui->comboTriUser->currentIndex();   // 0=CIN, 1=Rôle, 2=Date
    bool ascending = ui->comboOrdreUser->currentIndex() == 0;

    std::sort(result.begin(), result.end(),
              [sortCol, ascending](const User &a, const User &b) {
        int cmp = 0;
        switch(sortCol) {
        case 0: cmp = a.cin.compare(b.cin, Qt::CaseInsensitive); break;
        case 1: cmp = a.role.compare(b.role, Qt::CaseInsensitive); break;
        case 2: cmp = (a.dateCreation < b.dateCreation) ? -1 : (a.dateCreation > b.dateCreation ? 1 : 0); break;
        }
        return ascending ? cmp < 0 : cmp > 0;
    });
    return result;
}

void MainWindow::refreshUserTable()
{
    auto data = filteredAndSortedUsers();
    ui->tableEmployees->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i)
        populateUserRow(i, data[i]);

    updatePayrollCombo();
    refreshUserStats();
    refreshSessions();
}

void MainWindow::updatePayrollCombo()
{
    ui->comboPayrollUser->clear();
    ui->comboPayrollUser->addItem(tr("Sélectionner employé..."));
    for (const auto &u : m_users)
        ui->comboPayrollUser->addItem(u.nom + " " + u.prenom + " (" + u.cin + ")");
}

// ─── Employee CRUD ──────────────────────────

void MainWindow::onCreerUser()
{
    UserDialog dlg(this, tr("Nouvel Employé"));
    if (dlg.exec() != QDialog::Accepted) return;
    User u = dlg.user();
    if (u.cin.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Le CIN est obligatoire."));
        return;
    }
    // Check unique CIN
    for (const auto &existing : m_users) {
        if (existing.cin == u.cin) {
            QMessageBox::warning(this, tr("Doublon"), tr("Un employé avec ce CIN existe déjà."));
            return;
        }
    }
    m_users.append(u);
    refreshUserTable();
}

void MainWindow::onModifierUser()
{
    int row = ui->tableEmployees->currentRow();
    if (row < 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }

    // Find by CIN
    QString cin = ui->tableEmployees->item(row, 0)->text();
    int idx = -1;
    for (int i = 0; i < m_users.size(); ++i)
        if (m_users[i].cin == cin) { idx = i; break; }
    if (idx < 0) return;

    UserDialog dlg(this, tr("Modifier Employé"));
    dlg.setUser(m_users[idx]);
    if (dlg.exec() != QDialog::Accepted) return;
    m_users[idx] = dlg.user();
    refreshUserTable();
}

void MainWindow::onSupprimerUser()
{
    int row = ui->tableEmployees->currentRow();
    if (row < 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }

    QString cin = ui->tableEmployees->item(row, 0)->text();
    if (QMessageBox::question(this, tr("Confirmer"),
            tr("Supprimer l'employé %1 ?").arg(cin)) != QMessageBox::Yes)
        return;

    for (int i = 0; i < m_users.size(); ++i) {
        if (m_users[i].cin == cin) { m_users.removeAt(i); break; }
    }
    refreshUserTable();
}

void MainWindow::onActiverDesactiverUser()
{
    int row = ui->tableEmployees->currentRow();
    if (row < 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }

    QString cin = ui->tableEmployees->item(row, 0)->text();
    for (auto &u : m_users) {
        if (u.cin == cin) {
            u.statut = (u.statut == "Actif") ? "Inactif" : "Actif";
            break;
        }
    }
    refreshUserTable();
}

// ─── Employee Search / Sort ──────────────────

void MainWindow::onRechercherUser()
{
    refreshUserTable();
}

void MainWindow::onTriUserChanged()
{
    refreshUserTable();
}

// ─── Employee Statistics ─────────────────────

void MainWindow::refreshUserStats()
{
    int total = m_users.size();
    QLocale loc;

    // Helper lambda to fill a stat table
    auto fillStatTable = [&](QTableWidget *table, std::function<QString(const User&)> keyFn) {
        QMap<QString, int> counts;
        for (const auto &u : m_users)
            counts[keyFn(u)]++;

        table->setRowCount(counts.size());
        int r = 0;
        for (auto it = counts.constBegin(); it != counts.constEnd(); ++it, ++r) {
            auto *nameItem = new QTableWidgetItem(it.key());
            nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
            auto *countItem = new QTableWidgetItem(QString::number(it.value()));
            countItem->setFlags(countItem->flags() & ~Qt::ItemIsEditable);
            double pct = total > 0 ? (it.value() * 100.0 / total) : 0;
            auto *pctItem = new QTableWidgetItem(loc.toString(pct, 'f', 1) + "%");
            pctItem->setFlags(pctItem->flags() & ~Qt::ItemIsEditable);
            table->setItem(r, 0, nameItem);
            table->setItem(r, 1, countItem);
            table->setItem(r, 2, pctItem);
        }
    };

    fillStatTable(ui->tableStatRole,   [](const User &u){ return u.role; });
    fillStatTable(ui->tableStatStatut, [](const User &u){ return u.statut; });
    // Service = role for now (can be extended)
    fillStatTable(ui->tableStatService,[](const User &u){ return u.role; });
}

// ─── Sessions ────────────────────────────────

void MainWindow::refreshSessions()
{
    ui->tableSessions->setRowCount(m_sessions.size());
    for (int i = 0; i < m_sessions.size(); ++i) {
        auto makeItem = [](const QString &txt) {
            auto *it = new QTableWidgetItem(txt);
            it->setFlags(it->flags() & ~Qt::ItemIsEditable);
            return it;
        };
        ui->tableSessions->setItem(i, 0, makeItem(m_sessions[i].utilisateur));
        ui->tableSessions->setItem(i, 1, makeItem(m_sessions[i].dateConnexion));
        ui->tableSessions->setItem(i, 2, makeItem(m_sessions[i].heure));
        ui->tableSessions->setItem(i, 3, makeItem(m_sessions[i].ip));
        ui->tableSessions->setItem(i, 4, makeItem(m_sessions[i].action));
    }
}

void MainWindow::onTerminerSession()
{
    int row = ui->tableSessions->currentRow();
    if (row < 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez une session.")); return; }
    if (QMessageBox::question(this, tr("Confirmer"),
            tr("Terminer cette session ?")) != QMessageBox::Yes)
        return;
    m_sessions.removeAt(row);
    refreshSessions();
}

// ─── Payslip Export ──────────────────────────

void MainWindow::onPayslipPDF()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const User &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    QString path = QFileDialog::getSaveFileName(this, tr("Exporter PDF"),
        u.nom + "_" + month + ".pdf", "PDF (*.pdf)");
    if (path.isEmpty()) return;

    QPdfWriter writer(path);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    QPainter p(&writer);

    QFont titleFont("Arial", 24, QFont::Bold);
    QFont headerFont("Arial", 14, QFont::Bold);
    QFont bodyFont("Arial", 12);
    QLocale loc;

    int y = 200;
    p.setFont(titleFont);
    p.drawText(300, y, "FICHE DE PAIE"); y += 400;

    p.setFont(headerFont);
    p.drawText(200, y, "Employé:"); p.setFont(bodyFont);
    p.drawText(1400, y, u.nom + " " + u.prenom); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "CIN:"); p.setFont(bodyFont);
    p.drawText(1400, y, u.cin); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "Email:"); p.setFont(bodyFont);
    p.drawText(1400, y, u.email); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "Rôle:"); p.setFont(bodyFont);
    p.drawText(1400, y, u.role); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "Mois:"); p.setFont(bodyFont);
    p.drawText(1400, y, month); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "Heures travaillées:"); p.setFont(bodyFont);
    p.drawText(1400, y, QString::number(u.heures, 'f', 1)); y += 250;
    p.setFont(headerFont);
    p.drawText(200, y, "Statut:"); p.setFont(bodyFont);
    p.drawText(1400, y, u.statut); y += 400;

    p.setPen(QPen(Qt::gray, 3));
    p.drawLine(200, y, 2200, y); y += 200;
    p.setPen(Qt::black);
    p.setFont(bodyFont);
    p.drawText(200, y, "Généré le " + QDate::currentDate().toString("dd/MM/yyyy"));

    p.end();
    QMessageBox::information(this, tr("Succès"), tr("Fiche de paie exportée en PDF."));
}

void MainWindow::onPayslipExcel()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const User &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    QString path = QFileDialog::getSaveFileName(this, tr("Exporter CSV"),
        u.nom + "_" + month + ".csv", "CSV (*.csv)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de créer le fichier."));
        return;
    }
    QTextStream out(&file);
    out << "Champ;Valeur\n";
    out << "Nom;" << u.nom << "\n";
    out << "Prénom;" << u.prenom << "\n";
    out << "CIN;" << u.cin << "\n";
    out << "Email;" << u.email << "\n";
    out << "Rôle;" << u.role << "\n";
    out << "Mois;" << month << "\n";
    out << "Heures;" << QString::number(u.heures, 'f', 1) << "\n";
    out << "Statut;" << u.statut << "\n";
    file.close();
    QMessageBox::information(this, tr("Succès"), tr("Fiche de paie exportée en CSV."));
}

void MainWindow::onPayslipEmail()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const User &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    QString subject = QUrl::toPercentEncoding("Fiche de paie - " + month);
    QString body = QUrl::toPercentEncoding(
        "Bonjour " + u.prenom + ",\n\nVeuillez trouver ci-joint votre fiche de paie pour le mois de "
        + month + ".\n\nCordialement,\nVISION SIGHT RH");
    QString mailto = "mailto:" + u.email + "?subject=" + subject + "&body=" + body;
    QDesktopServices::openUrl(QUrl(mailto));
}
