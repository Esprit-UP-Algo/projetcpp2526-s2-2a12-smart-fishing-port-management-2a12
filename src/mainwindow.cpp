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
#include "utilisateurmodel.h"
#include "utilisateursortfilterproxymodel.h"

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

    m_userModel = new UtilisateurModel(this);
    m_userProxy = new UtilisateurSortFilterProxyModel(this);
    m_userProxy->setSourceModel(m_userModel);

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

    const QStringList items = {tr("Utilisateurs")};
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
    auto *h2 = new QLabel(tr("Module: Gestion des employées · Entité: Utilisateur"), page);
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

    // Charts, chambres and stats removed — only users UI kept

    // Users management: CRUD + filters + table
    auto *userCard = makeCard(page, "Card");
    auto *userLayout = new QVBoxLayout(userCard);
    userLayout->setContentsMargins(14, 12, 14, 12);
    userLayout->setSpacing(10);

    auto *userTitle = new QLabel(tr("Gestion des employées"), userCard);
    userTitle->setObjectName("SectionTitle");
    userLayout->addWidget(userTitle);

    auto *userFormGrid = new QFormLayout();
    userFormGrid->setLabelAlignment(Qt::AlignLeft);
    userFormGrid->setFormAlignment(Qt::AlignTop);
    userFormGrid->setHorizontalSpacing(14);
    userFormGrid->setVerticalSpacing(4);

    m_userId = new QLineEdit(userCard);
    m_userId->setObjectName("Input");
    m_userId->setPlaceholderText("U-001");

    m_userNom = new QLineEdit(userCard);
    m_userNom->setObjectName("Input");
    m_userNom->setPlaceholderText(tr("Nom"));

    m_userPrenom = new QLineEdit(userCard);
    m_userPrenom->setObjectName("Input");
    m_userPrenom->setPlaceholderText(tr("Prénom"));

    m_userEmail = new QLineEdit(userCard);
    m_userEmail->setObjectName("Input");
    m_userEmail->setPlaceholderText(tr("email@example.com"));

    m_userLogin = new QLineEdit(userCard);
    m_userLogin->setObjectName("Input");
    m_userLogin->setPlaceholderText(tr("login"));

    m_userMotDePasse = new QLineEdit(userCard);
    m_userMotDePasse->setObjectName("Input");
    m_userMotDePasse->setEchoMode(QLineEdit::Password);

    m_userRole = new QComboBox(userCard);
    m_userRole->setObjectName("Input");
    m_userRole->addItems(QStringList{tr("Admin"), tr("User")});

    m_userStatut = new QComboBox(userCard);
    m_userStatut->setObjectName("Input");
    m_userStatut->addItems(QStringList{tr("Actif"), tr("Inactif")});

    m_userDateCreation = new QDateTimeEdit(QDateTime::currentDateTime(), userCard);
    m_userDateCreation->setObjectName("Input");
    m_userDateCreation->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_userDateCreation->setCalendarPopup(true);

    auto *labUid = new QLabel(tr("ID Utilisateur"), userCard);
    labUid->setObjectName("Muted");
    auto *labNom = new QLabel(tr("Nom"), userCard);
    labNom->setObjectName("Muted");
    auto *labPren = new QLabel(tr("Prénom"), userCard);
    labPren->setObjectName("Muted");
    auto *labEmail = new QLabel(tr("Email"), userCard);
    labEmail->setObjectName("Muted");
    auto *labLogin = new QLabel(tr("Login"), userCard);
    labLogin->setObjectName("Muted");
    auto *labPwd = new QLabel(tr("Mot de passe"), userCard);
    labPwd->setObjectName("Muted");
    auto *labRole = new QLabel(tr("Rôle"), userCard);
    labRole->setObjectName("Muted");
    auto *labStat = new QLabel(tr("Statut"), userCard);
    labStat->setObjectName("Muted");
    auto *labDateC = new QLabel(tr("Date création"), userCard);
    labDateC->setObjectName("Muted");

    userFormGrid->addRow(labUid, m_userId);
    userFormGrid->addRow(labNom, m_userNom);
    userFormGrid->addRow(labPren, m_userPrenom);
    userFormGrid->addRow(labEmail, m_userEmail);
    userFormGrid->addRow(labLogin, m_userLogin);
    userFormGrid->addRow(labPwd, m_userMotDePasse);
    userFormGrid->addRow(labRole, m_userRole);
    userFormGrid->addRow(labStat, m_userStatut);
    userFormGrid->addRow(labDateC, m_userDateCreation);

    userLayout->addLayout(userFormGrid);

    auto *userBtnRow = new QHBoxLayout();
    userBtnRow->setSpacing(10);

    m_btnAddUser = new QPushButton(tr("Ajouter"), userCard);
    m_btnAddUser->setObjectName("PrimaryButton");
    m_btnAddUser->setCursor(Qt::PointingHandCursor);

    m_btnUpdateUser = new QPushButton(tr("Modifier"), userCard);
    m_btnUpdateUser->setObjectName("SecondaryButton");
    m_btnUpdateUser->setCursor(Qt::PointingHandCursor);

    m_btnDeleteUser = new QPushButton(tr("Supprimer"), userCard);
    m_btnDeleteUser->setObjectName("DangerButton");
    m_btnDeleteUser->setCursor(Qt::PointingHandCursor);

    m_btnClearUser = new QPushButton(tr("Réinitialiser"), userCard);
    m_btnClearUser->setObjectName("GhostButton");
    m_btnClearUser->setCursor(Qt::PointingHandCursor);

    userBtnRow->addWidget(m_btnAddUser);
    userBtnRow->addWidget(m_btnUpdateUser);
    userBtnRow->addWidget(m_btnDeleteUser);
    userBtnRow->addWidget(m_btnClearUser);
    userBtnRow->addStretch();

    userLayout->addLayout(userBtnRow);

    auto *userFilterCard = makeCard(userCard, "CardSmall");
    userFilterCard->setFixedHeight(78);
    auto *userFilterLayout = new QHBoxLayout(userFilterCard);
    userFilterLayout->setContentsMargins(16, 14, 16, 14);
    userFilterLayout->setSpacing(10);

    m_searchUserNom = new QLineEdit(userFilterCard);
    m_searchUserNom->setObjectName("Input");
    m_searchUserNom->setPlaceholderText(tr("Recherche nom"));

    m_searchUserEmail = new QLineEdit(userFilterCard);
    m_searchUserEmail->setObjectName("Input");
    m_searchUserEmail->setPlaceholderText(tr("Recherche email"));

    m_searchUserRole = new QLineEdit(userFilterCard);
    m_searchUserRole->setObjectName("Input");
    m_searchUserRole->setPlaceholderText(tr("Recherche rôle"));

    m_searchUserStatut = new QLineEdit(userFilterCard);
    m_searchUserStatut->setObjectName("Input");
    m_searchUserStatut->setPlaceholderText(tr("Recherche statut"));

    m_btnExportUsersPdf = new QPushButton(tr("Exporter PDF"), userFilterCard);
    m_btnExportUsersPdf->setObjectName("AltPrimaryButton");
    m_btnExportUsersPdf->setCursor(Qt::PointingHandCursor);

    userFilterLayout->addWidget(m_searchUserNom, 1);
    userFilterLayout->addWidget(m_searchUserEmail, 1);
    userFilterLayout->addWidget(m_searchUserRole, 1);
    userFilterLayout->addWidget(m_searchUserStatut, 1);
    userFilterLayout->addWidget(m_btnExportUsersPdf);

    userLayout->addWidget(userFilterCard);

    m_tableUsers = new QTableView(userCard);
    m_tableUsers->setObjectName("Table");
    m_tableUsers->setModel(m_userProxy);
    m_tableUsers->setSortingEnabled(true);
    m_tableUsers->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableUsers->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableUsers->setShowGrid(false);
    m_tableUsers->verticalHeader()->setVisible(false);
    m_tableUsers->horizontalHeader()->setStretchLastSection(true);
    m_tableUsers->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    userLayout->addWidget(m_tableUsers, 1);

    pageLayout->addWidget(userCard);

    // Lots removed
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
    // Users only
    connect(m_btnAddUser, &QPushButton::clicked, this, &MainWindow::onAddUser);
    connect(m_btnUpdateUser, &QPushButton::clicked, this, &MainWindow::onUpdateUser);
    connect(m_btnDeleteUser, &QPushButton::clicked, this, &MainWindow::onDeleteUser);
    connect(m_btnClearUser, &QPushButton::clicked, this, &MainWindow::onClearUserForm);
    connect(m_btnExportUsersPdf, &QPushButton::clicked, this, &MainWindow::onExportUsersPdf);

    connect(m_searchUserNom, &QLineEdit::textChanged, m_userProxy, &UtilisateurSortFilterProxyModel::setSearchNom);
    connect(m_searchUserEmail, &QLineEdit::textChanged, m_userProxy, &UtilisateurSortFilterProxyModel::setSearchEmail);
    connect(m_searchUserRole, &QLineEdit::textChanged, m_userProxy, &UtilisateurSortFilterProxyModel::setSearchRole);
    connect(m_searchUserStatut, &QLineEdit::textChanged, m_userProxy, &UtilisateurSortFilterProxyModel::setSearchStatut);

    connect(m_tableUsers->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::loadSelectedUserToForm);

    // Refresh on user model changes
    auto refreshUsers = [this]() { refreshChambreCombo(); refreshStats(); };
    connect(m_userModel, &QAbstractItemModel::rowsInserted, this, refreshUsers);
    connect(m_userModel, &QAbstractItemModel::rowsRemoved, this, refreshUsers);
    connect(m_userModel, &QAbstractItemModel::modelReset, this, refreshUsers);
    connect(m_userModel, &QAbstractItemModel::dataChanged, this, refreshUsers);
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
    Q_UNUSED(idChambre);
    Q_UNUSED(excludeRow);
    return 0;
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
    return 0;
}

double MainWindow::averageDurationDays() const
{
    return 0.0;
}

void MainWindow::refreshChambreCombo()
{
    // No-op: chambres/lots combo removed
    Q_UNUSED(m_chambreModel);
}

void MainWindow::refreshStats()
{
    Q_UNUSED(this);
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

void MainWindow::loadSelectedUserToForm()
{
    const int row = selectedSourceRow(m_tableUsers, m_userProxy);
    if (row < 0 || row >= m_userModel->rowCount())
        return;

    const auto &u = m_userModel->items().at(row);
    m_userId->setText(u.idUtilisateur);
    m_userNom->setText(u.nom);
    m_userPrenom->setText(u.prenom);
    m_userEmail->setText(u.email);
    m_userLogin->setText(u.login);
    m_userMotDePasse->clear();
    const int idxRole = m_userRole->findText(u.role);
    if (idxRole >= 0)
        m_userRole->setCurrentIndex(idxRole);
    const int idxStat = m_userStatut->findText(u.statut);
    if (idxStat >= 0)
        m_userStatut->setCurrentIndex(idxStat);
    m_userDateCreation->setDateTime(u.dateCreation.isValid() ? u.dateCreation : QDateTime::currentDateTime());
}

void MainWindow::onAddUser()
{
    Utilisateur u;
    u.idUtilisateur = m_userId->text().trimmed();
    u.nom = m_userNom->text().trimmed();
    u.prenom = m_userPrenom->text().trimmed();
    u.email = m_userEmail->text().trimmed();
    u.login = m_userLogin->text().trimmed();
    u.motDePasse = m_userMotDePasse->text();
    u.role = m_userRole->currentText();
    u.statut = m_userStatut->currentText();
    u.dateCreation = m_userDateCreation->dateTime();

    QString error;
    if (!m_userModel->addUtilisateur(u, &error))
    {
        QMessageBox::warning(this, tr("Ajouter utilisateur"), error);
        return;
    }

    onClearUserForm();
}

void MainWindow::onUpdateUser()
{
    const int row = selectedSourceRow(m_tableUsers, m_userProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Modifier utilisateur"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    Utilisateur u;
    u.idUtilisateur = m_userId->text().trimmed();
    u.nom = m_userNom->text().trimmed();
    u.prenom = m_userPrenom->text().trimmed();
    u.email = m_userEmail->text().trimmed();
    u.login = m_userLogin->text().trimmed();
    if (!m_userMotDePasse->text().isEmpty())
        u.motDePasse = m_userMotDePasse->text();
    else
        u.motDePasse = m_userModel->items().at(row).motDePasse;
    u.role = m_userRole->currentText();
    u.statut = m_userStatut->currentText();
    u.dateCreation = m_userDateCreation->dateTime();

    QString error;
    if (!m_userModel->updateUtilisateur(row, u, &error))
        QMessageBox::warning(this, tr("Modifier utilisateur"), error);
}

void MainWindow::onDeleteUser()
{
    const int row = selectedSourceRow(m_tableUsers, m_userProxy);
    if (row < 0)
    {
        QMessageBox::information(this, tr("Supprimer utilisateur"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (QMessageBox::question(this, tr("Supprimer utilisateur"), tr("Supprimer l'utilisateur sélectionné ?")) != QMessageBox::Yes)
        return;

    QString error;
    if (!m_userModel->removeUtilisateur(row, &error))
        QMessageBox::warning(this, tr("Supprimer utilisateur"), error);

    onClearUserForm();
}

void MainWindow::onClearUserForm()
{
    m_userId->clear();
    m_userNom->clear();
    m_userPrenom->clear();
    m_userEmail->clear();
    m_userLogin->clear();
    m_userMotDePasse->clear();
    m_userRole->setCurrentIndex(0);
    m_userStatut->setCurrentIndex(0);
    m_userDateCreation->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::onExportUsersPdf()
{
    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), "utilisateurs.pdf", tr("PDF (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString error;
    if (!PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT — Utilisateurs"), m_userProxy, &error))
    {
        QMessageBox::warning(this, tr("Exporter PDF"), error);
        return;
    }

    QMessageBox::information(this, tr("Exporter PDF"), tr("PDF exporté: %1").arg(filePath));
}
