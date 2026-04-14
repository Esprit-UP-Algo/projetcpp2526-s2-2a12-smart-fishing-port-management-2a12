#include "employe_mainwindow.h"
#include "ui_employe_mainwindow.h"

#include "employe_userdialog.h"
#include "payslip_pdf_exporter.h"
#include "smtp_mailer.h"
#include "smtp_settingsdialog.h"
#include "../../modele/gestion_employes/employe_dao.h"
#include "../../modele/gestion_employes/input_validator.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QAction>
#include <QActionGroup>
#include <QLocale>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPdfWriter>
#include <QHeaderView>
#include <QStyle>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <algorithm>
#include <cmath>
#include <limits>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFutureWatcher>
#include <QPointer>
#include <QtConcurrent/QtConcurrent>

namespace employes {

EmployeMainWindow::EmployeMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeMainWindow)
{
    ui->setupUi(this);

    setupEmployeesDropdown();

    setupLogo();

    // Set Employee Management as active by default
    ui->btnEmployees->setChecked(true);

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

void EmployeMainWindow::setupEmployeesDropdown()
{
    // Build the 3-subsection dropdown for the Employees sidebar button.
    m_empDropdownMenu = new QMenu(this);
    m_empDropdownGroup = new QActionGroup(this);
    m_empDropdownGroup->setExclusive(true);

    m_empActionAffichage = m_empDropdownMenu->addAction(tr("Affichage"));
    m_empActionAffichage->setCheckable(true);
    m_empDropdownGroup->addAction(m_empActionAffichage);

    m_empActionStatistique = m_empDropdownMenu->addAction(tr("Statistique"));
    m_empActionStatistique->setCheckable(true);
    m_empDropdownGroup->addAction(m_empActionStatistique);

    m_empActionFichePaie = m_empDropdownMenu->addAction(tr("Fiche de paie"));
    m_empActionFichePaie->setCheckable(true);
    m_empDropdownGroup->addAction(m_empActionFichePaie);

    m_empDropdownMenu->setStyleSheet(
        "QMenu {"
        "  background-color: #101c2c;"
        "  border: 1px solid #23374e;"
        "  padding: 4px;"
        "}"
        "QMenu::item {"
        "  color: #e6eef6;"
        "  padding: 8px 14px;"
        "  border-radius: 6px;"
        "}"
        "QMenu::item:selected {"
        "  background-color: #18304a;"
        "}"
        "QMenu::item:checked {"
        "  color: #39c0fa;"
        "  background-color: rgba(57, 192, 250, 22);"
        "}"
    );

    connect(ui->btnEmployees, &QPushButton::clicked, this, [this]() {
        setEmployeesSubmenuExpanded(!m_empDropdownExpanded);
    });

    connect(m_empDropdownMenu, &QMenu::aboutToHide, this, [this]() {
        m_empDropdownExpanded = false;
    });

    connect(m_empActionAffichage, &QAction::triggered, this, [this]() {
        setEmployeesSection(EmployeesSection::Affichage);
    });
    connect(m_empActionStatistique, &QAction::triggered, this, [this]() {
        setEmployeesSection(EmployeesSection::Statistique);
    });
    connect(m_empActionFichePaie, &QAction::triggered, this, [this]() {
        setEmployeesSection(EmployeesSection::FicheDePaie);
    });

    // Add dedicated pages for statistics and payslip to the existing stacked widget.
    m_empStatsPage = new QWidget(ui->stackedWidget);
    {
        auto *pageLayout = new QVBoxLayout(m_empStatsPage);
        pageLayout->setContentsMargins(0, 0, 0, 0);
        pageLayout->setSpacing(0);

        auto *scroll = new QScrollArea(m_empStatsPage);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);

        auto *content = new QWidget(scroll);
        auto *contentLayout = new QVBoxLayout(content);
        contentLayout->setSpacing(14);
        contentLayout->setContentsMargins(16, 14, 16, 14);
        contentLayout->addWidget(ui->empStatsFrame);
        contentLayout->addStretch();

        scroll->setWidget(content);
        pageLayout->addWidget(scroll);
    }
    ui->stackedWidget->addWidget(m_empStatsPage);

    m_empPayslipPage = new QWidget(ui->stackedWidget);
    {
        auto *pageLayout = new QVBoxLayout(m_empPayslipPage);
        pageLayout->setContentsMargins(0, 0, 0, 0);
        pageLayout->setSpacing(0);

        auto *scroll = new QScrollArea(m_empPayslipPage);
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);

        auto *content = new QWidget(scroll);
        auto *contentLayout = new QVBoxLayout(content);
        contentLayout->setSpacing(14);
        contentLayout->setContentsMargins(16, 14, 16, 14);
        contentLayout->addWidget(ui->empPayslipFrame);
        contentLayout->addStretch();

        scroll->setWidget(content);
        pageLayout->addWidget(scroll);
    }
    ui->stackedWidget->addWidget(m_empPayslipPage);

    setEmployeesSection(EmployeesSection::Affichage);
}

void EmployeMainWindow::setEmployeesSubmenuExpanded(bool expanded)
{
    if (!m_empDropdownMenu || !ui->btnEmployees) {
        m_empDropdownExpanded = false;
        return;
    }

    if (expanded) {
        m_empDropdownMenu->popup(ui->btnEmployees->mapToGlobal(QPoint(0, ui->btnEmployees->height())));
        m_empDropdownExpanded = true;
    } else {
        m_empDropdownMenu->hide();
        m_empDropdownExpanded = false;
    }
}

void EmployeMainWindow::setEmployeesSection(EmployeesSection section)
{
    switch (section) {
    case EmployeesSection::Affichage:
        ui->stackedWidget->setCurrentIndex(0);
        setActiveEmployeesSubButton(m_empActionAffichage);
        break;
    case EmployeesSection::Statistique:
        if (m_empStatsPage) {
            ui->stackedWidget->setCurrentWidget(m_empStatsPage);
            setActiveEmployeesSubButton(m_empActionStatistique);
        }
        break;
    case EmployeesSection::FicheDePaie:
        if (m_empPayslipPage) {
            ui->stackedWidget->setCurrentWidget(m_empPayslipPage);
            setActiveEmployeesSubButton(m_empActionFichePaie);
        }
        break;
    }
}

void EmployeMainWindow::setActiveEmployeesSubButton(QAction *action)
{
    if (!action) {
        return;
    }
    action->setChecked(true);
}

void EmployeMainWindow::sendPayslipEmailAsync(const EmployeUser &user,
                                              const QString &month,
                                              const QString &pdfPath,
                                              const QString &subject,
                                              const QString &body)
{
    auto *watcher = new QFutureWatcher<QPair<bool, QString>>(this);
    const QString recipient = user.email;

    connect(watcher, &QFutureWatcher<QPair<bool, QString>>::finished, this,
            [this, watcher, pdfPath, recipient, user]() {
        const QPair<bool, QString> result = watcher->result();
        watcher->deleteLater();
        QFile::remove(pdfPath);

        if (!result.first) {
            const QString message = result.second;
            if (message.contains("Configuration SMTP manquante", Qt::CaseInsensitive)
                || message.contains("Email invalide", Qt::CaseInsensitive)
                || message.contains("PDF introuvable", Qt::CaseInsensitive)) {
                QMessageBox::warning(this, tr("Erreur envoi"), message);
            } else {
                QMessageBox::critical(this, tr("Erreur envoi"), message);
            }
            return;
        }

        QMessageBox::information(this,
                                 tr("Succès"),
                                 tr("Email envoyé avec succès à %1 %2 (%3).")
                                 .arg(user.prenom, user.nom, recipient));
    });

    watcher->setFuture(QtConcurrent::run([recipient, subject, body, pdfPath]() {
        QString smtpError;
        const bool ok = SmtpMailer::envoyerEmailSMTP(recipient, subject, body, pdfPath, &smtpError);
        return qMakePair(ok, smtpError);
    }));
}


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
    QPixmap pm(":/ressources/vision_sight.jpeg");
    if (!pm.isNull()) {
        ui->logoLabel->setPixmap(cropAndMakeCircularLogo(pm, 96, 8));
    }
}

#ifdef INTEGRATION_APP
void EmployeMainWindow::onSidebarQuais() { emit requestShowQuais(); }
void EmployeMainWindow::onSidebarNavires() { emit requestShowNavires(); }
void EmployeMainWindow::onSidebarCaptures() { emit requestShowCaptures(); }
void EmployeMainWindow::onSidebarStockage() { emit requestShowStockage(); }
void EmployeMainWindow::onSidebarVentes() { emit requestShowTransactions(); }
#endif

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
    ui->tableEmployees->setItem(row, 6, makeItem(QString::number(u.heures, 'f', 1)));
    ui->tableEmployees->setItem(row, 7, makeItem(u.statut));
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

    int sortCol  = ui->comboTriUser->currentIndex();
    bool ascending = ui->comboOrdreUser->currentIndex() == 0;

    std::sort(result.begin(), result.end(),
              [sortCol, ascending](const EmployeUser &a, const EmployeUser &b) {
        int cmp = 0;
        switch(sortCol) {
        case 0: cmp = a.cin.compare(b.cin, Qt::CaseInsensitive); break;
        case 1: cmp = a.role.compare(b.role, Qt::CaseInsensitive); break;
        }
        return ascending ? cmp < 0 : cmp > 0;
    });
    return result;
}

void EmployeMainWindow::refreshUserTable()
{
    // Charger les données depuis la base de données via le DAO
    m_users = EmployeDAO::afficher();
    
    // Afficher les données filtrées et triées
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

void EmployeMainWindow::onCreerUser()
{
    EmployeUserDialog dlg(this, tr("Nouvel Employé"));
    
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    
    EmployeUser employe = dlg.user();
    employe.statut = "Actif";
    
    QString validationError = EmployeDAO::validateEmploye(employe);
    if (!validationError.isEmpty()) {
        QMessageBox::warning(this, 
            tr("Erreur Validation"), 
            validationError);
        return;
    }
    
    QString dbError = EmployeDAO::ajouter(employe);
    if (!dbError.isEmpty()) {
        QMessageBox::critical(this, 
            tr("Erreur Base de Données"), 
            dbError);
        return;
    }
    
    QMessageBox::information(this,
        tr("Succès"),
        tr("L'employé a été ajouté."));
    
    refreshUserTable();
}

void EmployeMainWindow::onModifierUser()
{
    // Vérifier qu'une ligne est sélectionnée
    int row = ui->tableEmployees->currentRow();
    if (row < 0) {
        QMessageBox::information(this, 
            tr("Info"), 
            tr("Veuillez sélectionner un employé à modifier."));
        return;
    }
    
    // Récupérer le CIN de la ligne sélectionnée
    QTableWidgetItem *cinItem = ui->tableEmployees->item(row, 0);
    if (!cinItem) {
        QMessageBox::warning(this, 
            tr("❌ Erreur"), 
            tr("Impossible de récupérer le CIN de l'employé."));
        return;
    }
    
    QString cin = cinItem->text();
    
    // Charger l'employé courant (pour afficher ses données dans le dialogue)
    QVector<EmployeUser> employeList = EmployeDAO::afficher();
    EmployeUser employeActuel;
    bool found = false;
    
    for (const auto &e : employeList) {
        if (e.cin == cin) {
            employeActuel = e;
            found = true;
            break;
        }
    }
    
    if (!found) {
        QMessageBox::warning(this, 
            tr("Erreur"), 
            tr("L'employé avec CIN '%1' n'a pas été trouvé en base de données.").arg(cin));
        return;
    }
    
    // Ouvrir le dialogue de modification
    EmployeUserDialog dlg(this, tr("Modifier Employé"));
    dlg.setUser(employeActuel);
    
    if (dlg.exec() != QDialog::Accepted) {
        return;  // L'utilisateur a annulé
    }
    
    // Récupérer les données modifiées
    EmployeUser employeModifie = dlg.user();
    
    // Valider tous les champs
    QString validationError = EmployeDAO::validateEmploye(employeModifie, false);
    if (!validationError.isEmpty()) {
        QMessageBox::warning(this, 
            tr("Erreur Validation"), 
            validationError);
        return;
    }
    
    // Modifier en base de données
    QString dbError = EmployeDAO::modifier(employeModifie);
    if (!dbError.isEmpty()) {
        QMessageBox::critical(this, 
            tr("Erreur Base de Données"), 
            dbError);
        return;
    }
    
    // Succès - afficher le message et rafraîchir
    QMessageBox::information(this, 
        tr("✓ Succès"), 
        tr("L'employé '%1 %2' a été modifié avec succès.")
        .arg(employeModifie.prenom, employeModifie.nom));
    
    // Rafraîchir l'affichage
    refreshUserTable();
}

void EmployeMainWindow::onSupprimerUser()
{
    int row = ui->tableEmployees->currentRow();
    if (row < 0) {
        QMessageBox::information(this, 
            tr("Info"), 
            tr("Veuillez selectionner un employe a supprimer."));
        return;
    }
    
    QTableWidgetItem *cinItem = ui->tableEmployees->item(row, 0);
    if (!cinItem) {
        QMessageBox::warning(this, 
            tr("❌ Erreur"), 
            tr("Impossible de récupérer le CIN de l'employé."));
        return;
    }
    
    QString cin = cinItem->text();
    QString nom = (ui->tableEmployees->item(row, 1) ? 
                   ui->tableEmployees->item(row, 1)->text() : "");
    QString prenom = (ui->tableEmployees->item(row, 2) ? 
                   ui->tableEmployees->item(row, 2)->text() : "");
    
    int response = QMessageBox::question(this, 
        tr("Confirmer suppression"), 
        tr("Supprimer cet employé?\n"
           "%1 %2 (CIN: %3)?")
        .arg(prenom, nom, cin),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    
    if (response != QMessageBox::Yes) {
        return;
    }
    
    QString dbError = EmployeDAO::supprimer(cin);
    if (!dbError.isEmpty()) {
        QMessageBox::critical(this, 
            tr("Erreur Base de Données"), 
            dbError);
        return;
    }
    
    QMessageBox::information(this, 
        tr("Suppression OK"), 
        tr("L'employé a été supprimé."));
    
    refreshUserTable();
}

void EmployeMainWindow::onActiverDesactiverUser()
{
    // Vérifier qu'une ligne est sélectionnée
    int row = ui->tableEmployees->currentRow();
    if (row < 0) {
        QMessageBox::information(this, 
            tr("Info"), 
            tr("Veuillez sélectionner un employé."));
        return;
    }
    
    QTableWidgetItem *cinItem = ui->tableEmployees->item(row, 0);
    if (!cinItem) {
        return;
    }
    
    QString cin = cinItem->text();
    
    // Charger l'employé courant
    QVector<EmployeUser> employeList = EmployeDAO::afficher();
    EmployeUser employe;
    bool found = false;
    
    for (const auto &e : employeList) {
        if (e.cin == cin) {
            employe = e;
            found = true;
            break;
        }
    }
    
    if (!found) {
        return;
    }
    
    // Basculer le statut
    employe.statut = (employe.statut == "Actif") ? "Inactif" : "Actif";
    
    // Modifier en base
    QString dbError = EmployeDAO::modifier(employe);
    if (!dbError.isEmpty()) {
        QMessageBox::critical(this, 
            tr("Erreur"), 
            dbError);
        return;
    }
    
    QMessageBox::information(this, 
        tr("Succès"), 
        tr("L'employé est maintenant %1.")
        .arg(employe.statut));
    
    refreshUserTable();
}


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

    if (idx - 1 >= m_users.size()) {
        QMessageBox::warning(this, tr("Erreur"), tr("L'employé sélectionné est introuvable."));
        return;
    }

    const EmployeUser &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    QString safeNom = u.nom.trimmed().toLower().replace(' ', '_');
    QString safePrenom = u.prenom.trimmed().toLower().replace(' ', '_');
    QString safeMonth = month.trimmed().toLower().replace(' ', '_');
    QString defaultName = QString("fiche_paie_%1_%2_%3.pdf").arg(safeNom, safePrenom, safeMonth);

    QString path = QFileDialog::getSaveFileName(this, tr("Exporter PDF"),
        QDir::homePath() + "/" + defaultName, "PDF (*.pdf)");
    if (path.isEmpty()) return;

    const QString exportError = PayslipPdfExporter::createPDF(u, month, path);
    if (!exportError.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur export PDF"), exportError);
        return;
    }

    QMessageBox::information(this, tr("Succès"), tr("Fiche de paie exportée en PDF :\n%1").arg(path));
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

    if (idx - 1 >= m_users.size()) {
        QMessageBox::warning(this, tr("Erreur"), tr("L'employé sélectionné est introuvable."));
        return;
    }

    const EmployeUser &u = m_users[idx - 1];
    QString month = ui->comboPayrollMonth->currentText();

    SmtpConfig smtpConfig = SmtpMailer::loadConfig();
    if (!smtpConfig.isComplete()) {
        QMessageBox::warning(this,
                             tr("Configuration SMTP manquante"),
                             tr("La configuration SMTP est incomplète. Ouvrez les paramètres SMTP pour la renseigner."));

        SmtpSettingsDialog settingsDialog(this);
        if (settingsDialog.exec() != QDialog::Accepted) {
            return;
        }

        smtpConfig = SmtpMailer::loadConfig();
        if (!smtpConfig.isComplete()) {
            QMessageBox::warning(this,
                                 tr("Configuration SMTP manquante"),
                                 tr("La configuration SMTP est toujours incomplète."));
            return;
        }
    }

    const QString emailError = employes::InputValidator::validateEmail(u.email);
    if (!emailError.isEmpty()) {
        QMessageBox::warning(this, tr("Email invalide"), emailError);
        return;
    }

    const QString baseName = QString("fiche_paie_%1_%2_%3_%4.pdf")
        .arg(u.nom.trimmed().toLower().replace(' ', '_'),
             u.prenom.trimmed().toLower().replace(' ', '_'),
             month.trimmed().toLower().replace(' ', '_'),
             QString::number(QDateTime::currentMSecsSinceEpoch()));

    QDir tempDir(QDir::tempPath());
    if (!tempDir.mkpath("vision_sight_payslips")) {
        QMessageBox::warning(this, tr("Erreur"), tr("Impossible de préparer le dossier temporaire."));
        return;
    }

    const QString tempPdfPath = tempDir.filePath(QString("vision_sight_payslips/%1").arg(baseName));
    const QString exportError = PayslipPdfExporter::createPDF(u, month, tempPdfPath);
    if (!exportError.isEmpty()) {
        QMessageBox::warning(this, tr("Erreur export PDF"), exportError);
        return;
    }

    qDebug() << "[MAIL] PDF temporaire genere:" << tempPdfPath;

    const QString subject = tr("Fiche de paie - %1").arg(month);
    const QString body = tr(
        "Bonjour %1 %2,\n\n"
        "Veuillez trouver ci-joint votre fiche de paie pour le mois de %3.\n\n"
        "Cordialement,\n"
        "VISION SIGHT RH")
        .arg(u.prenom, u.nom, month);

    qDebug() << "[MAIL] Demarrage envoi asynchrone pour" << u.email;
    sendPayslipEmailAsync(u, month, tempPdfPath, subject, body);
}

} // namespace employes
