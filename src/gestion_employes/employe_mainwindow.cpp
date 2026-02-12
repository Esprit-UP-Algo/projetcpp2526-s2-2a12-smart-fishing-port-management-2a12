#include "employe_mainwindow.h"
#include "ui_employe_mainwindow.h"

#include "employe_userdialog.h"

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
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPainterPath>

namespace employes {

EmployeMainWindow::EmployeMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeMainWindow)
{
    ui->setupUi(this);

    setupLogo();

    // Set Employee Management as active by default
    ui->btnEmployees->setChecked(true);

    // ── Employee page connections ──
    ui->tableEmployees->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableSessions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatRole->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatStatut->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableStatService->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->btnCreerUser,        &QPushButton::clicked, this, &EmployeMainWindow::onCreerUser);
    connect(ui->btnModifierUser,     &QPushButton::clicked, this, &EmployeMainWindow::onModifierUser);
    connect(ui->btnSupprimerUser,    &QPushButton::clicked, this, &EmployeMainWindow::onSupprimerUser);
    connect(ui->btnActiverDesactiver,&QPushButton::clicked, this, &EmployeMainWindow::onActiverDesactiverUser);
    connect(ui->btnRechercherUser,   &QPushButton::clicked, this, &EmployeMainWindow::onRechercherUser);
    connect(ui->comboTriUser,   QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EmployeMainWindow::onTriUserChanged);
    connect(ui->comboOrdreUser, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &EmployeMainWindow::onTriUserChanged);
    connect(ui->btnPayslipPDF,   &QPushButton::clicked, this, &EmployeMainWindow::onPayslipPDF);
    connect(ui->btnPayslipExcel, &QPushButton::clicked, this, &EmployeMainWindow::onPayslipExcel);
    connect(ui->btnPayslipEmail, &QPushButton::clicked, this, &EmployeMainWindow::onPayslipEmail);
    connect(ui->btnTerminerSession, &QPushButton::clicked, this, &EmployeMainWindow::onTerminerSession);

    // Sidebar connections
    for (auto *btn : {ui->btnEmployees, ui->btnNavires, ui->btnQuais,
                      ui->btnCaptures, ui->btnStockage, ui->btnVentes}) {
        connect(btn, &QPushButton::clicked, this, [this, btn]() {
            setActiveSidebarButton(btn);
        });
    }

#ifdef INTEGRATION_APP
    connect(ui->btnQuais,    &QPushButton::clicked, this, &EmployeMainWindow::onSidebarQuais);
    connect(ui->btnNavires,  &QPushButton::clicked, this, &EmployeMainWindow::onSidebarNavires);
    connect(ui->btnCaptures, &QPushButton::clicked, this, &EmployeMainWindow::onSidebarCaptures);
    connect(ui->btnStockage, &QPushButton::clicked, this, &EmployeMainWindow::onSidebarStockage);
    connect(ui->btnVentes,   &QPushButton::clicked, this, &EmployeMainWindow::onSidebarVentes);
#endif

    // Record initial session
    EmployeSession s;
    s.utilisateur = "admin";
    s.dateConnexion = QDate::currentDate().toString("yyyy-MM-dd");
    s.heure = QTime::currentTime().toString("HH:mm:ss");
    s.ip = "127.0.0.1";
    s.action = "Connexion";
    m_sessions.append(s);

    refreshUserTable();
}

EmployeMainWindow::~EmployeMainWindow()
{
    delete ui;
}

void EmployeMainWindow::setSidebarActiveEmployees()
{
    setActiveSidebarButton(ui->btnEmployees);
}

// ─── Sidebar active button ──────────────────────────

void EmployeMainWindow::pulseSidebarButton(QWidget *widget)
{
    if (!widget) return;
    auto *effect = qobject_cast<QGraphicsOpacityEffect *>(widget->graphicsEffect());
    if (!effect) {
        effect = new QGraphicsOpacityEffect(widget);
        effect->setOpacity(1.0);
        widget->setGraphicsEffect(effect);
    }
    auto *anim = new QPropertyAnimation(effect, "opacity", widget);
    anim->setDuration(160);
    anim->setKeyValueAt(0.0, 1.0);
    anim->setKeyValueAt(0.45, 0.65);
    anim->setKeyValueAt(1.0, 1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void EmployeMainWindow::setActiveSidebarButton(QPushButton *activeBtn)
{
    if (!activeBtn) return;
    pulseSidebarButton(activeBtn);
    activeBtn->setChecked(true);
}

static QPixmap makeCircularLogo(const QPixmap &src, int diameterPx, int insetPx)
{
    QPixmap out(diameterPx, diameterPx);
    out.fill(Qt::transparent);
    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPainterPath path;
    path.addEllipse(0, 0, diameterPx, diameterPx);
    p.setClipPath(path);
    const int s = diameterPx - (insetPx * 2);
    p.drawPixmap(insetPx, insetPx, s, s, src);
    return out;
}

static QPixmap cropAndMakeCircularLogo(const QPixmap &src, int diameterPx, int insetPx)
{
    if (src.isNull()) return {};
    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    if (img.isNull() || img.width() < 2) return makeCircularLogo(src, diameterPx, insetPx);
    const QColor bg = QColor::fromRgba(img.pixel(0, 0));
    const int tolerance = 18;
    int minX = img.width(), minY = img.height(), maxX = -1, maxY = -1;
    for (int y = 0; y < img.height(); ++y) {
        const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            const QColor c = QColor::fromRgba(line[x]);
            if ((qAbs(c.red()-bg.red())+qAbs(c.green()-bg.green())+qAbs(c.blue()-bg.blue())) > tolerance) {
                minX = qMin(minX, x); minY = qMin(minY, y);
                maxX = qMax(maxX, x); maxY = qMax(maxY, y);
            }
        }
    }
    if (maxX <= minX) return makeCircularLogo(src, diameterPx, insetPx);
    const int pad = 6;
    minX = qMax(0, minX - pad); minY = qMax(0, minY - pad);
    maxX = qMin(img.width() - 1, maxX + pad); maxY = qMin(img.height() - 1, maxY + pad);
    return makeCircularLogo(QPixmap::fromImage(img.copy(minX, minY, maxX-minX+1, maxY-minY+1)), diameterPx, insetPx);
}

void EmployeMainWindow::setupLogo()
{
    QPixmap pm(":/vision_sight.jpeg");
    if (!pm.isNull())
        ui->logoLabel->setPixmap(cropAndMakeCircularLogo(pm, 96, 8));
}

#ifdef INTEGRATION_APP
void EmployeMainWindow::onSidebarQuais() { emit requestShowQuais(); }
void EmployeMainWindow::onSidebarNavires() { emit requestShowNavires(); }
void EmployeMainWindow::onSidebarCaptures() { emit requestShowCaptures(); }
void EmployeMainWindow::onSidebarStockage() { emit requestShowStockage(); }
void EmployeMainWindow::onSidebarVentes() { emit requestShowTransactions(); }
#endif

// ═══════════════════════════════════════════════════════
// ═══════  EMPLOYEE MANAGEMENT  ═══════════════════════
// ═══════════════════════════════════════════════════════

void EmployeMainWindow::populateUserRow(int row, const EmployeUser &u)
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

bool EmployeMainWindow::matchesUserSearch(const EmployeUser &u) const
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

QVector<EmployeUser> EmployeMainWindow::filteredAndSortedUsers() const
{
    QVector<EmployeUser> result;
    for (const auto &u : m_users)
        if (matchesUserSearch(u))
            result.append(u);

    int sortCol  = ui->comboTriUser->currentIndex();   // 0=CIN, 1=Rôle, 2=Date
    bool ascending = ui->comboOrdreUser->currentIndex() == 0;

    std::sort(result.begin(), result.end(),
              [sortCol, ascending](const EmployeUser &a, const EmployeUser &b) {
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

void EmployeMainWindow::refreshUserTable()
{
    auto data = filteredAndSortedUsers();
    ui->tableEmployees->setRowCount(data.size());
    for (int i = 0; i < data.size(); ++i)
        populateUserRow(i, data[i]);

    updatePayrollCombo();
    refreshUserStats();
    refreshSessions();
}

void EmployeMainWindow::updatePayrollCombo()
{
    ui->comboPayrollUser->clear();
    ui->comboPayrollUser->addItem(tr("Sélectionner employé..."));
    for (const auto &u : m_users)
        ui->comboPayrollUser->addItem(u.nom + " " + u.prenom + " (" + u.cin + ")");
}

// ─── Employee CRUD ──────────────────────────

void EmployeMainWindow::onCreerUser()
{
    EmployeUserDialog dlg(this, tr("Nouvel Employé"));
    if (dlg.exec() != QDialog::Accepted) return;
    EmployeUser u = dlg.user();
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

void EmployeMainWindow::onModifierUser()
{
    int row = ui->tableEmployees->currentRow();
    if (row < 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }

    // Find by CIN
    QString cin = ui->tableEmployees->item(row, 0)->text();
    int idx = -1;
    for (int i = 0; i < m_users.size(); ++i)
        if (m_users[i].cin == cin) { idx = i; break; }
    if (idx < 0) return;

    EmployeUserDialog dlg(this, tr("Modifier Employé"));
    dlg.setUser(m_users[idx]);
    if (dlg.exec() != QDialog::Accepted) return;
    m_users[idx] = dlg.user();
    refreshUserTable();
}

void EmployeMainWindow::onSupprimerUser()
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

void EmployeMainWindow::onActiverDesactiverUser()
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

void EmployeMainWindow::onRechercherUser()
{
    refreshUserTable();
}

void EmployeMainWindow::onTriUserChanged()
{
    refreshUserTable();
}

// ─── Employee Statistics ─────────────────────

void EmployeMainWindow::refreshUserStats()
{
    int total = m_users.size();
    QLocale loc;

    // Helper lambda to fill a stat table
    auto fillStatTable = [&](QTableWidget *table, std::function<QString(const EmployeUser&)> keyFn) {
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

    fillStatTable(ui->tableStatRole,   [](const EmployeUser &u){ return u.role; });
    fillStatTable(ui->tableStatStatut, [](const EmployeUser &u){ return u.statut; });
    // Service = role for now (can be extended)
    fillStatTable(ui->tableStatService,[](const EmployeUser &u){ return u.role; });
}

// ─── Sessions ────────────────────────────────

void EmployeMainWindow::refreshSessions()
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

void EmployeMainWindow::onTerminerSession()
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

void EmployeMainWindow::onPayslipPDF()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const EmployeUser &u = m_users[idx - 1];
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

void EmployeMainWindow::onPayslipExcel()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const EmployeUser &u = m_users[idx - 1];
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

void EmployeMainWindow::onPayslipEmail()
{
    int idx = ui->comboPayrollUser->currentIndex();
    if (idx <= 0) { QMessageBox::information(this, tr("Info"), tr("Sélectionnez un employé.")); return; }
    const EmployeUser &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    QString subject = QUrl::toPercentEncoding("Fiche de paie - " + month);
    QString body = QUrl::toPercentEncoding(
        "Bonjour " + u.prenom + ",\n\nVeuillez trouver ci-joint votre fiche de paie pour le mois de "
        + month + ".\n\nCordialement,\nVISION SIGHT RH");
    QString mailto = "mailto:" + u.email + "?subject=" + subject + "&body=" + body;
    QDesktopServices::openUrl(QUrl(mailto));
}

} // namespace employes
