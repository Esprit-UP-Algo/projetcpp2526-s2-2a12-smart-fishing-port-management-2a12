#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "quaiwindow.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QApplication>
#include <QCryptographicHash>
#include "qrcodegen.hpp"
#include <QStackedWidget>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
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
#include <QActionGroup>
#include <QScrollBar>
#include <QRegularExpression>
#include <QPropertyAnimation>
#include <QMenu>
#include <QEvent>
#include <QCursor>
#include <QCoreApplication>
#include <algorithm>

namespace {

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

class HoverPopupMenuController final : public QObject
{
public:
    HoverPopupMenuController(QPushButton *button, QMenu *menu, QObject *parent = nullptr)
        : QObject(parent)
        , m_button(button)
        , m_menu(menu)
    {
        if (auto *app = QCoreApplication::instance())
            app->installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (!m_button || !m_menu)
            return QObject::eventFilter(watched, event);

        const auto showMenu = [this]() {
            if (m_menu->isVisible())
                return;
            const QPoint pos = m_button->mapToGlobal(QPoint(0, m_button->height()));
            m_menu->popup(pos);
        };

        const auto hideMenu = [this]() {
            if (m_menu->isVisible())
                m_menu->hide();
        };

        if (event->type() == QEvent::MouseMove)
        {
            if (m_menu->isVisible() && !isCursorInPopupZone())
                hideMenu();
            return QObject::eventFilter(watched, event);
        }

        if (watched == m_button)
        {
            if (event->type() == QEvent::Enter)
            {
                showMenu();
                return false;
            }
            if (event->type() == QEvent::Leave)
            {
                if (!isCursorInPopupZone())
                    hideMenu();
                return false;
            }
        }

        if (watched == m_menu)
        {
            if (event->type() == QEvent::Leave)
            {
                if (!isCursorInPopupZone())
                    hideMenu();
                return false;
            }
            if (event->type() == QEvent::Enter)
                return false;
        }

        return QObject::eventFilter(watched, event);
    }

private:
    bool isCursorInPopupZone() const
    {
        if (!m_button || !m_menu)
            return false;

        const QPoint gp = QCursor::pos();
        const QRect btnRect(m_button->mapToGlobal(QPoint(0, 0)), m_button->size());
        if (btnRect.contains(gp))
            return true;

        if (m_menu->isVisible() && m_menu->geometry().contains(gp))
            return true;

        return false;
    }

private:
    QPushButton *m_button = nullptr;
    QMenu *m_menu = nullptr;
};

struct NavireDbColumns {
    QString id;
    QString nom;
    QString matricule;
    QString type;
    QString capacite;
    QString statut;
    QString position;
    QString nbSorties;
};

static QString upperTrim(const QString &s)
{
    return s.trimmed().toUpper();
}

static QString findColumnName(const QSqlRecord &rec, const QStringList &candidates)
{
    // Compare sans tenir compte de la casse.
    for (const QString &cand : candidates) {
        const QString target = upperTrim(cand);
        for (int i = 0; i < rec.count(); ++i) {
            if (upperTrim(rec.fieldName(i)) == target)
                return rec.fieldName(i);
        }
    }
    return {};
}

static bool resolveNavireColumns(NavireDbColumns *cols, QString *errorMessage)
{
    if (!cols) return false;

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isValid() || !db.isOpen()) {
        if (errorMessage)
            *errorMessage = QStringLiteral("Connexion BD indisponible (db invalide ou fermée).");
        return false;
    }

    QSqlQuery q(db);
    if (!q.exec(QStringLiteral("SELECT * FROM NAVIRE WHERE 1=0"))) {
        if (errorMessage)
            *errorMessage = QStringLiteral("Impossible de lire la structure de NAVIRE: %1").arg(q.lastError().text());
        return false;
    }

    const QSqlRecord rec = q.record();
    cols->id        = findColumnName(rec, {"ID", "ID_NAVIRE", "IDNAVIRE"});
    cols->nom       = findColumnName(rec, {"NOM", "NOM_NAVIRE", "NOMNAVIRE"});
    cols->matricule = findColumnName(rec, {"MATRICULE", "MATRICULE_NAVIRE", "MATRICULENAVIRE"});
    cols->type      = findColumnName(rec, {"TYPE", "TYPE_NAVIRE", "TYPENAVIRE"});
    cols->capacite  = findColumnName(rec, {"CAPACITE", "CAPACITE_T", "CAPACITE_NAVIRE"});
    cols->statut    = findColumnName(rec, {"STATUT", "ETAT", "STATUS"});
    cols->position  = findColumnName(rec, {"POSITION", "PORT", "LOCALISATION", "EMPLACEMENT"});
    cols->nbSorties = findColumnName(rec, {"NB_SORTIES", "NBSORTIES", "NB_SORTIE", "SORTIES"});

    // Colonnes minimales pour le CRUD.
    if (cols->nom.isEmpty() || cols->matricule.isEmpty()) {
        if (errorMessage)
            *errorMessage = QStringLiteral("Colonnes NAVIRE non reconnues: NOM/MATRICULE introuvables.\n"
                                          "Colonnes détectées: %1")
                                .arg([&]() {
                                    QStringList fields;
                                    for (int i = 0; i < rec.count(); ++i) fields << rec.fieldName(i);
                                    return fields.join(", ");
                                }());
        return false;
    }
    return true;
}

static bool isValidMatricule(const QString &mat)
{
    // Format attendu: M-1234 (lettre, tiret, 3 à 6 chiffres)
    static const QRegularExpression re(QStringLiteral("^[A-Za-z]\\-\\d{3,6}$"));
    return re.match(mat.trimmed()).hasMatch();
}

}

static QTableWidgetItem* centeredItem(const QString &text) {
    auto *item = new QTableWidgetItem(text);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , nextId(1)
{
    ui->setupUi(this);

    if (ui->btnMenu) {
        ui->btnMenu->setText(QString());
        ui->btnMenu->setIcon(makeHamburgerIcon(QColor("#e6eef6"), 18));
        ui->btnMenu->setIconSize(QSize(18, 18));
    }

    ensureIntegratedPages();

    // Force explicit startup state: Gestion Des Navires only.
    ui->btnGestionNavires->setChecked(true);
    ui->btnNaviresAffichage->setChecked(true);
    ui->btnNaviresStatistique->setChecked(false);
    ui->btnNaviresAvancees->setChecked(false);

    // Modules not used in this app: fully neutralize interactions.
    for (auto *btn : {ui->btnGestionEmployees,
                      ui->btnGestionCaptures,
                      ui->btnGestionVentes,
                      ui->btnStockageFrigorifique}) {
        if (!btn)
            continue;
        btn->setChecked(false);
        btn->setCheckable(false);
        btn->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &MainWindow::onWeatherReply);

    connect(ui->btnAjouter,    &QPushButton::clicked, this, &MainWindow::onAjouterClicked);
    connect(ui->btnModifier,   &QPushButton::clicked, this, &MainWindow::onModifierClicked);
    connect(ui->btnSupprimer,  &QPushButton::clicked, this, &MainWindow::onSupprimerClicked);

    // Embedded animated submenu for "Gestion Des Navires" (matches other modules).
    // Keep the legacy embedded widget hidden (we now use a popup QMenu like the Quais interface).
    if (ui->naviresMenu) {
        ui->naviresMenu->setVisible(false);
        ui->naviresMenu->setMaximumHeight(0);
    }

    if (ui->btnGestionNavires)
    {
        m_naviresPopupMenu = new QMenu(ui->btnGestionNavires);

        m_naviresActAffichage = m_naviresPopupMenu->addAction(tr("Affichage"));
        m_naviresActAffichage->setCheckable(true);
        m_naviresActStatistique = m_naviresPopupMenu->addAction(tr("Statistique"));
        m_naviresActStatistique->setCheckable(true);
        m_naviresActAvancees = m_naviresPopupMenu->addAction(tr("Avancées"));
        m_naviresActAvancees->setCheckable(true);

        auto *group = new QActionGroup(m_naviresPopupMenu);
        group->setExclusive(true);
        group->addAction(m_naviresActAffichage);
        group->addAction(m_naviresActStatistique);
        group->addAction(m_naviresActAvancees);
        m_naviresActAffichage->setChecked(true);

        connect(m_naviresActAffichage, &QAction::triggered, this, &MainWindow::onNaviresAffichage);
        connect(m_naviresActStatistique, &QAction::triggered, this, &MainWindow::onNaviresStatistique);
        connect(m_naviresActAvancees, &QAction::triggered, this, &MainWindow::onNaviresAvancees);

        // Match the Quais dropdown behavior: popup on hover and on click.
        auto *hoverCtl = new HoverPopupMenuController(ui->btnGestionNavires, m_naviresPopupMenu, this);
        ui->btnGestionNavires->installEventFilter(hoverCtl);
        m_naviresPopupMenu->installEventFilter(hoverCtl);
        connect(ui->btnGestionNavires, &QPushButton::clicked, this, [this]() {
            showNaviresPage();
            if (m_naviresPopupMenu)
                m_naviresPopupMenu->popup(ui->btnGestionNavires->mapToGlobal(QPoint(0, ui->btnGestionNavires->height())));
        });
    }

    if (ui->btnGestionQuais) {
        ui->btnGestionQuais->setCheckable(true);
        ui->btnGestionQuais->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        connect(ui->btnGestionQuais, &QPushButton::clicked, this, &MainWindow::onGestionQuaisClicked);
    }

    connect(ui->searchType,      &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchNom,       &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchMatricule, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(ui->searchPosition,  &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);

    connect(ui->comboSort, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSortChanged);

    if (ui->revenuePeriod) {
        connect(ui->revenuePeriod, &QComboBox::currentIndexChanged, this, &MainWindow::updateStats);
    }

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

    // Autres modules (Employés, Quais, Captures, Stockage, Ventes) neutralisés ci-dessus.

    loadSampleData();

    QString dbErr;
    if (!reloadNaviresFromDb(&dbErr)) {
        // On évite d'injecter des données “sample” qui masquent un problème BD.
        navires.clear();
        QMessageBox::warning(this,
                             QStringLiteral("Oracle / NAVIRE"),
                             QStringLiteral("Les navires n'ont pas pu être chargés depuis Oracle.\n\n%1")
                                 .arg(dbErr));
    }

    refreshTable();
    refreshMouvementsTable();
    updateStats();

    // Default view: Affichage (navires management only)
    setNaviresAffichageOnly();
    setActiveNaviresOption(ui->btnNaviresAffichage);

    // Style table row selection
    ui->tableNavires->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableNavires->setSelectionMode(QAbstractItemView::MultiSelection);

    // Center all table headers
    ui->tableNavires->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableMaintenance->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableRevenus->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableMouvements->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

void MainWindow::onGestionQuaisClicked()
{
    showQuaisPage();
}

void MainWindow::ensureIntegratedPages()
{
    if (m_pages)
        return;

    m_naviresPage = takeCentralWidget();
    m_pages = new QStackedWidget(this);
    setCentralWidget(m_pages);

    if (m_naviresPage)
    {
        m_naviresPage->setParent(m_pages);
        m_pages->addWidget(m_naviresPage);
    }

    m_quaiPage = new QuaiWindow(m_pages);
    m_quaiPage->setWindowFlags(Qt::Widget);
    m_pages->addWidget(m_quaiPage);

    connect(m_quaiPage, &QuaiWindow::requestShowNavires, this, [this]() {
        showNaviresPage();
    });
}

void MainWindow::showNaviresPage()
{
    ensureIntegratedPages();
    if (m_pages && m_naviresPage)
        m_pages->setCurrentWidget(m_naviresPage);

    if (ui->btnGestionNavires)
        ui->btnGestionNavires->setChecked(true);
    if (ui->btnGestionQuais)
        ui->btnGestionQuais->setChecked(false);
}

void MainWindow::showQuaisPage()
{
    ensureIntegratedPages();
    if (m_pages && m_quaiPage)
        m_pages->setCurrentWidget(m_quaiPage);

    if (ui->btnGestionNavires)
        ui->btnGestionNavires->setChecked(false);
    if (ui->btnGestionQuais)
        ui->btnGestionQuais->setChecked(true);
}

void MainWindow::setNaviresMenuExpanded(bool expanded, bool animated)
{
    if (!ui->naviresMenu)
        return;

    if (m_naviresMenuExpanded == expanded && ui->naviresMenu->isVisible() == expanded)
        return;

    m_naviresMenuExpanded = expanded;

    if (m_naviresMenuAnim) {
        m_naviresMenuAnim->stop();
        delete m_naviresMenuAnim;
        m_naviresMenuAnim = nullptr;
    }

    const int startH = ui->naviresMenu->maximumHeight();
    int endH = 0;
    if (expanded) {
        ui->naviresMenu->setVisible(true);
        ui->naviresMenu->setMaximumHeight(0);
        endH = ui->naviresMenu->layout() ? ui->naviresMenu->layout()->sizeHint().height()
                                         : ui->naviresMenu->sizeHint().height();
        if (endH < 0) endH = 0;
    } else {
        endH = 0;
    }

    if (!animated) {
        ui->naviresMenu->setMaximumHeight(endH);
        if (!expanded)
            ui->naviresMenu->setVisible(false);
        return;
    }

    m_naviresMenuAnim = new QPropertyAnimation(ui->naviresMenu, "maximumHeight", this);
    m_naviresMenuAnim->setDuration(160);
    m_naviresMenuAnim->setEasingCurve(QEasingCurve::InOutCubic);
    m_naviresMenuAnim->setStartValue(startH);
    m_naviresMenuAnim->setEndValue(endH);
    connect(m_naviresMenuAnim, &QPropertyAnimation::finished, this, [this, expanded]() {
        if (!expanded) {
            ui->naviresMenu->setVisible(false);
            ui->naviresMenu->setMaximumHeight(0);
        }
    });
    m_naviresMenuAnim->start();
}

void MainWindow::setActiveNaviresOption(QPushButton *active)
{
    for (auto *btn : {ui->btnNaviresAffichage, ui->btnNaviresStatistique, ui->btnNaviresAvancees}) {
        if (!btn) continue;
        btn->setChecked(btn == active);
    }
}

void MainWindow::onToggleNaviresMenu()
{
    showNaviresPage();

    // Root selection
    ui->btnGestionEmployees->setChecked(false);
    ui->btnGestionNavires->setChecked(true);
    ui->btnGestionQuais->setChecked(false);
    ui->btnGestionCaptures->setChecked(false);
    ui->btnStockageFrigorifique->setChecked(false);
    ui->btnGestionVentes->setChecked(false);

    // Keep Affichage visible when opening the submenu (same behavior as Ventes).
    setNaviresAffichageOnly();
    setActiveNaviresOption(ui->btnNaviresAffichage);
    setNaviresMenuExpanded(!m_naviresMenuExpanded, true);
}

void MainWindow::onNaviresAffichage()
{
    showNaviresPage();
    ui->btnGestionNavires->setChecked(true);
    setNaviresAffichageOnly();
    setActiveNaviresOption(ui->btnNaviresAffichage);
    if (m_naviresActAffichage)
        m_naviresActAffichage->setChecked(true);
    setNaviresMenuExpanded(false, true);
}

void MainWindow::onNaviresStatistique()
{
    showNaviresPage();
    ui->btnGestionNavires->setChecked(true);
    setNaviresStatsOnly(true);
    setActiveNaviresOption(ui->btnNaviresStatistique);
    if (m_naviresActStatistique)
        m_naviresActStatistique->setChecked(true);
    setNaviresMenuExpanded(false, true);
}

void MainWindow::onNaviresAvancees()
{
    showNaviresPage();
    ui->btnGestionNavires->setChecked(true);
    setNaviresAdvancedOnly(true);
    setActiveNaviresOption(ui->btnNaviresAvancees);
    if (m_naviresActAvancees)
        m_naviresActAvancees->setChecked(true);
    setNaviresMenuExpanded(false, true);
}

void MainWindow::setNaviresAffichageOnly()
{
    // Affichage section (navires management)
    ui->buttonsCard->setVisible(true);
    ui->searchCard->setVisible(true);
    ui->triCard->setVisible(true);
    ui->exportCard->setVisible(true);
    ui->gestionCard->setVisible(true);

    // Hide other sections
    ui->statsCard->setVisible(false);
    ui->maintenanceCard->setVisible(false);
    ui->revenusCard->setVisible(false);
    ui->weatherCard->setVisible(false);
    ui->calendarCard->setVisible(false);

    QTimer::singleShot(0, this, [this]() {
        if (ui->scrollArea && ui->scrollArea->verticalScrollBar())
            ui->scrollArea->verticalScrollBar()->setValue(0);
    });
}

void MainWindow::setNaviresStatsOnly(bool enabled)
{
    // Stats section
    ui->statsCard->setVisible(true);
    ui->maintenanceCard->setVisible(true);
    ui->revenusCard->setVisible(true);

    // Other sections
    ui->buttonsCard->setVisible(!enabled);
    ui->searchCard->setVisible(!enabled);
    ui->triCard->setVisible(!enabled);
    ui->exportCard->setVisible(!enabled);
    ui->gestionCard->setVisible(!enabled);
    ui->weatherCard->setVisible(!enabled);
    ui->calendarCard->setVisible(!enabled);

    // Ensure we land on the visible section.
    QTimer::singleShot(0, this, [this]() {
        if (ui->scrollArea && ui->scrollArea->verticalScrollBar())
            ui->scrollArea->verticalScrollBar()->setValue(0);
    });
}

void MainWindow::setNaviresAdvancedOnly(bool enabled)
{
    // Advanced section
    ui->weatherCard->setVisible(true);
    ui->calendarCard->setVisible(true);

    // Other sections
    ui->buttonsCard->setVisible(!enabled);
    ui->searchCard->setVisible(!enabled);
    ui->statsCard->setVisible(!enabled);
    ui->maintenanceCard->setVisible(!enabled);
    ui->revenusCard->setVisible(!enabled);
    ui->triCard->setVisible(!enabled);
    ui->exportCard->setVisible(!enabled);
    ui->gestionCard->setVisible(!enabled);

    QTimer::singleShot(0, this, [this]() {
        if (ui->scrollArea && ui->scrollArea->verticalScrollBar())
            ui->scrollArea->verticalScrollBar()->setValue(0);
    });
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

void MainWindow::loadSampleData()
{
    // L'application fonctionne désormais sur données réelles (Oracle).
    // On laisse les listes vides au démarrage.
}

bool MainWindow::reloadNaviresFromDb(QString *errorMessage)
{
    NavireDbColumns cols;
    QString err;
    if (!resolveNavireColumns(&cols, &err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery q(db);

    QStringList sel;
    sel << cols.nom << cols.matricule;
    if (!cols.id.isEmpty()) sel << cols.id;
    if (!cols.type.isEmpty()) sel << cols.type;
    if (!cols.capacite.isEmpty()) sel << cols.capacite;
    if (!cols.statut.isEmpty()) sel << cols.statut;
    if (!cols.position.isEmpty()) sel << cols.position;
    if (!cols.nbSorties.isEmpty()) sel << cols.nbSorties;

    QString sql = QStringLiteral("SELECT %1 FROM NAVIRE").arg(sel.join(QStringLiteral(", ")));
    if (!cols.id.isEmpty())
        sql += QStringLiteral(" ORDER BY %1").arg(cols.id);

    if (!q.exec(sql)) {
        if (errorMessage)
            *errorMessage = QStringLiteral("Erreur SELECT NAVIRE: %1").arg(q.lastError().text());
        return false;
    }

    QList<Navire> loaded;
    int maxId = 0;
    const QSqlRecord rec = q.record();
    const int idxId        = cols.id.isEmpty() ? -1 : rec.indexOf(cols.id);
    const int idxNom       = rec.indexOf(cols.nom);
    const int idxMat       = rec.indexOf(cols.matricule);
    const int idxType      = cols.type.isEmpty() ? -1 : rec.indexOf(cols.type);
    const int idxCap       = cols.capacite.isEmpty() ? -1 : rec.indexOf(cols.capacite);
    const int idxStatut    = cols.statut.isEmpty() ? -1 : rec.indexOf(cols.statut);
    const int idxPosition  = cols.position.isEmpty() ? -1 : rec.indexOf(cols.position);
    const int idxSorties   = cols.nbSorties.isEmpty() ? -1 : rec.indexOf(cols.nbSorties);

    while (q.next()) {
        Navire n;
        n.nom = q.value(idxNom).toString();
        n.matricule = q.value(idxMat).toString();
        if (idxId >= 0) {
            n.id = q.value(idxId).toInt();
            maxId = qMax(maxId, n.id);
        }
        if (idxType >= 0) n.type = q.value(idxType).toString();
        if (idxCap >= 0) n.capacite = q.value(idxCap).toInt();
        if (idxStatut >= 0) n.statut = q.value(idxStatut).toString();
        if (idxPosition >= 0) n.position = q.value(idxPosition).toString();
        if (idxSorties >= 0) n.nbSorties = q.value(idxSorties).toInt();
        loaded.append(n);
    }

    navires = loaded;
    nextId = (maxId > 0) ? (maxId + 1) : 1;
    return true;
}

bool MainWindow::insertNavireToDb(Navire &navire, QString *errorMessage)
{
    NavireDbColumns cols;
    QString err;
    if (!resolveNavireColumns(&cols, &err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database();

    const QString nom = navire.nom.trimmed();
    const QString mat = navire.matricule.trimmed();
    if (nom.isEmpty() || mat.isEmpty()) {
        if (errorMessage) *errorMessage = QStringLiteral("Nom et matricule sont obligatoires.");
        return false;
    }
    if (!isValidMatricule(mat)) {
        if (errorMessage) *errorMessage = QStringLiteral("Matricule invalide. Format attendu: M-1234.");
        return false;
    }

    // Unicité matricule (si la table n'a pas de contrainte unique, on le force côté app).
    {
        QSqlQuery check(db);
        const QString sqlCheck = QStringLiteral("SELECT COUNT(1) FROM NAVIRE WHERE %1 = :mat")
                                     .arg(cols.matricule);
        if (check.prepare(sqlCheck)) {
            check.bindValue(QStringLiteral(":mat"), mat);
            if (check.exec() && check.next()) {
                if (check.value(0).toInt() > 0) {
                    if (errorMessage) *errorMessage = QStringLiteral("Ce matricule existe déjà dans la base.");
                    return false;
                }
            }
            // Si ce contrôle échoue, on laisse l'INSERT remonter l'erreur (contrainte unique, etc.).
        }
    }

    // Calcul d'un ID si la colonne existe.
    if (!cols.id.isEmpty()) {
        QSqlQuery qMax(db);
        const QString sqlMax = QStringLiteral("SELECT NVL(MAX(%1),0) FROM NAVIRE").arg(cols.id);
        int id = nextId;
        if (qMax.exec(sqlMax) && qMax.next())
            id = qMax.value(0).toInt() + 1;
        navire.id = id;
    }

    QStringList fields;
    QStringList values;
    if (!cols.id.isEmpty())     { fields << cols.id; values << QStringLiteral(":id"); }
    fields << cols.nom;          values << QStringLiteral(":nom");
    fields << cols.matricule;    values << QStringLiteral(":matricule");
    if (!cols.type.isEmpty())    { fields << cols.type; values << QStringLiteral(":type"); }
    if (!cols.capacite.isEmpty()){ fields << cols.capacite; values << QStringLiteral(":capacite"); }
    if (!cols.statut.isEmpty())  { fields << cols.statut; values << QStringLiteral(":statut"); }
    if (!cols.position.isEmpty()){ fields << cols.position; values << QStringLiteral(":position"); }
    if (!cols.nbSorties.isEmpty()){ fields << cols.nbSorties; values << QStringLiteral(":nb_sorties"); }

    const bool txStarted = db.transaction();

    QSqlQuery ins(db);
    const QString sql = QStringLiteral("INSERT INTO NAVIRE (%1) VALUES (%2)")
                            .arg(fields.join(QStringLiteral(", ")),
                                 values.join(QStringLiteral(", ")));

    if (!ins.prepare(sql)) {
        if (errorMessage) *errorMessage = ins.lastError().text();
        return false;
    }

    if (!cols.id.isEmpty()) ins.bindValue(QStringLiteral(":id"), navire.id);
    ins.bindValue(QStringLiteral(":nom"), nom);
    ins.bindValue(QStringLiteral(":matricule"), mat);
    if (!cols.type.isEmpty()) ins.bindValue(QStringLiteral(":type"), navire.type);
    if (!cols.capacite.isEmpty()) ins.bindValue(QStringLiteral(":capacite"), navire.capacite);
    if (!cols.statut.isEmpty()) ins.bindValue(QStringLiteral(":statut"), navire.statut);
    if (!cols.position.isEmpty()) ins.bindValue(QStringLiteral(":position"), navire.position);
    if (!cols.nbSorties.isEmpty()) ins.bindValue(QStringLiteral(":nb_sorties"), navire.nbSorties);

    if (!ins.exec()) {
        if (txStarted) db.rollback();
        if (errorMessage)
            *errorMessage = QStringLiteral("INSERT NAVIRE échoué: %1").arg(ins.lastError().text());
        return false;
    }

    if (txStarted && !db.commit()) {
        if (errorMessage)
            *errorMessage = QStringLiteral("INSERT exécuté mais commit a échoué: %1").arg(db.lastError().text());
        return false;
    }

    nextId = qMax(nextId, navire.id + 1);
    return true;
}

bool MainWindow::updateNavireInDb(const Navire &navire, const QString &whereMatricule, QString *errorMessage)
{
    NavireDbColumns cols;
    QString err;
    if (!resolveNavireColumns(&cols, &err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database();

    const QString nom = navire.nom.trimmed();
    const QString mat = navire.matricule.trimmed();
    if (nom.isEmpty() || mat.isEmpty()) {
        if (errorMessage) *errorMessage = QStringLiteral("Nom et matricule sont obligatoires.");
        return false;
    }
    if (!isValidMatricule(mat)) {
        if (errorMessage) *errorMessage = QStringLiteral("Matricule invalide. Format attendu: M-1234.");
        return false;
    }

    QStringList sets;
    sets << QStringLiteral("%1 = :nom").arg(cols.nom);
    sets << QStringLiteral("%1 = :matricule").arg(cols.matricule);
    if (!cols.type.isEmpty()) sets << QStringLiteral("%1 = :type").arg(cols.type);
    if (!cols.capacite.isEmpty()) sets << QStringLiteral("%1 = :capacite").arg(cols.capacite);
    if (!cols.statut.isEmpty()) sets << QStringLiteral("%1 = :statut").arg(cols.statut);
    if (!cols.position.isEmpty()) sets << QStringLiteral("%1 = :position").arg(cols.position);
    if (!cols.nbSorties.isEmpty()) sets << QStringLiteral("%1 = :nb_sorties").arg(cols.nbSorties);

    QString where;
    if (!cols.id.isEmpty() && navire.id > 0) {
        where = QStringLiteral("%1 = :id").arg(cols.id);
    } else {
        where = QStringLiteral("%1 = :where_matricule").arg(cols.matricule);
    }

    const QString sql = QStringLiteral("UPDATE NAVIRE SET %1 WHERE %2")
                            .arg(sets.join(QStringLiteral(", ")), where);

    const bool txStarted = db.transaction();

    QSqlQuery upd(db);
    if (!upd.prepare(sql)) {
        if (txStarted) db.rollback();
        if (errorMessage) *errorMessage = upd.lastError().text();
        return false;
    }
    upd.bindValue(QStringLiteral(":nom"), nom);
    upd.bindValue(QStringLiteral(":matricule"), mat);
    if (!cols.type.isEmpty()) upd.bindValue(QStringLiteral(":type"), navire.type);
    if (!cols.capacite.isEmpty()) upd.bindValue(QStringLiteral(":capacite"), navire.capacite);
    if (!cols.statut.isEmpty()) upd.bindValue(QStringLiteral(":statut"), navire.statut);
    if (!cols.position.isEmpty()) upd.bindValue(QStringLiteral(":position"), navire.position);
    if (!cols.nbSorties.isEmpty()) upd.bindValue(QStringLiteral(":nb_sorties"), navire.nbSorties);
    if (!cols.id.isEmpty() && navire.id > 0) {
        upd.bindValue(QStringLiteral(":id"), navire.id);
    } else {
        const QString keyMat = whereMatricule.trimmed().isEmpty() ? mat : whereMatricule.trimmed();
        upd.bindValue(QStringLiteral(":where_matricule"), keyMat);
    }

    if (!upd.exec()) {
        if (txStarted) db.rollback();
        if (errorMessage)
            *errorMessage = QStringLiteral("UPDATE NAVIRE échoué: %1").arg(upd.lastError().text());
        return false;
    }
    if (upd.numRowsAffected() <= 0) {
        if (txStarted) db.rollback();
        if (errorMessage)
            *errorMessage = QStringLiteral("Aucune ligne mise à jour (clé introuvable).");
        return false;
    }

    if (txStarted && !db.commit()) {
        if (errorMessage)
            *errorMessage = QStringLiteral("UPDATE exécuté mais commit a échoué: %1").arg(db.lastError().text());
        return false;
    }
    return true;
}

bool MainWindow::deleteNavireFromDb(int id, const QString &matricule, QString *errorMessage)
{
    NavireDbColumns cols;
    QString err;
    if (!resolveNavireColumns(&cols, &err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }
    QSqlDatabase db = QSqlDatabase::database();

    QString where;
    if (!cols.id.isEmpty() && id > 0)
        where = QStringLiteral("%1 = :id").arg(cols.id);
    else
        where = QStringLiteral("%1 = :mat").arg(cols.matricule);

    const QString sql = QStringLiteral("DELETE FROM NAVIRE WHERE %1").arg(where);
    QSqlQuery del(db);
    if (!del.prepare(sql)) {
        if (errorMessage) *errorMessage = del.lastError().text();
        return false;
    }
    if (!cols.id.isEmpty() && id > 0)
        del.bindValue(QStringLiteral(":id"), id);
    else
        del.bindValue(QStringLiteral(":mat"), matricule.trimmed());

    if (!del.exec()) {
        if (errorMessage)
            *errorMessage = QStringLiteral("DELETE NAVIRE échoué: %1").arg(del.lastError().text());
        return false;
    }
    if (del.numRowsAffected() <= 0) {
        if (errorMessage)
            *errorMessage = QStringLiteral("Aucune ligne supprimée (clé introuvable).");
        return false;
    }
    return true;
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
        n.nbSorties = 0;

        QString err;
        if (!insertNavireToDb(n, &err)) {
            QMessageBox::critical(this, QStringLiteral("Ajout navire"), err);
            return;
        }
        if (!reloadNaviresFromDb(&err)) {
            QMessageBox::warning(this, QStringLiteral("Oracle / NAVIRE"), err);
        }
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

    QTableWidgetItem *idItem = ui->tableNavires->item(row, 1);
    int id = idItem ? idItem->data(Qt::UserRole).toInt() : 0;
    QString matricule = ui->tableNavires->item(row, 2) ? ui->tableNavires->item(row, 2)->text() : QString();
    const QString oldMatricule = matricule;

    int idx = -1;
    for (int i = 0; i < navires.size(); ++i) {
        if (id > 0 && navires[i].id == id) { idx = i; break; }
        if (id <= 0 && !matricule.isEmpty() && navires[i].matricule == matricule) { idx = i; break; }
    }
    if (idx < 0) {
        QMessageBox::warning(this, QStringLiteral("Modifier"), QStringLiteral("Navire introuvable en mémoire. Rechargez la liste."));
        return;
    }

    Navire n = navires[idx];
    DialogNavire dlg(n, this);
    if (dlg.exec() == QDialog::Accepted) {
        Navire updated = dlg.getNavire();
        // Conserver les champs techniques
        updated.id = n.id;
        updated.nbSorties = n.nbSorties;

        QString err;
        if (!updateNavireInDb(updated, oldMatricule, &err)) {
            QMessageBox::critical(this, QStringLiteral("Modification navire"), err);
            return;
        }
        if (!reloadNaviresFromDb(&err)) {
            QMessageBox::warning(this, QStringLiteral("Oracle / NAVIRE"), err);
        }
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
        QSqlDatabase db = QSqlDatabase::database();
        bool useTx = db.isValid() && db.isOpen();
        if (useTx) db.transaction();

        QString firstErr;
        int okCount = 0;

        for (int row : std::as_const(selectedRows)) {
            QTableWidgetItem *idItem = ui->tableNavires->item(row, 1);
            int id = idItem ? idItem->data(Qt::UserRole).toInt() : 0;
            QString mat = ui->tableNavires->item(row, 2) ? ui->tableNavires->item(row, 2)->text() : QString();

            QString err;
            if (!deleteNavireFromDb(id, mat, &err)) {
                if (firstErr.isEmpty()) firstErr = err;
            } else {
                okCount++;
            }
        }

        if (useTx) {
            if (firstErr.isEmpty()) db.commit();
            else db.rollback();
        }

        if (!firstErr.isEmpty()) {
            QMessageBox::critical(this, QStringLiteral("Suppression navire"),
                                  QStringLiteral("Suppression incomplète (%1/%2).\n\n%3")
                                      .arg(okCount)
                                      .arg(selectedRows.size())
                                      .arg(firstErr));
            return;
        }

        QString err;
        if (!reloadNaviresFromDb(&err))
            QMessageBox::warning(this, QStringLiteral("Oracle / NAVIRE"), err);

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
        {
            QTableWidgetItem *idItem = centeredItem(QString("NAV-%1").arg(n.id, 3, 10, QChar('0')));
            idItem->setData(Qt::UserRole, n.id);
            ui->tableNavires->setItem(row, 1, idItem);
        }
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
    const QDateTime now = QDateTime::currentDateTime();

    auto overlapSeconds = [](const QDateTime &a1, const QDateTime &a2, const QDateTime &b1, const QDateTime &b2) -> qint64 {
        if (!a1.isValid() || !a2.isValid() || !b1.isValid() || !b2.isValid())
            return 0;
        const QDateTime start = (a1 > b1) ? a1 : b1;
        const QDateTime end = (a2 < b2) ? a2 : b2;
        const qint64 secs = start.secsTo(end);
        return secs > 0 ? secs : 0;
    };

    const int periodIdx = ui->revenuePeriod ? ui->revenuePeriod->currentIndex() : 1;
    const QDateTime periodEnd = now;
    QDateTime periodStart;
    switch (periodIdx) {
    case 0: periodStart = now.addSecs(-24 * 3600); break;
    case 1: periodStart = now.addDays(-7); break;
    case 2: periodStart = now.addDays(-30); break;
    case 3:
    default:
        periodStart = QDateTime(QDate(2000, 1, 1), QTime(0, 0));
        break;
    }

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
    QMap<QString, int> sortiesByNavire;
    int totalSortiesPeriod = 0;
    for (const Mouvement &m : std::as_const(mouvements)) {
        if (!m.heureDepart.isValid() || !m.heureRetour.isValid() || m.heureRetour <= m.heureDepart)
            continue;
        const qint64 overlap = overlapSeconds(m.heureDepart, m.heureRetour, periodStart, periodEnd);
        if (overlap <= 0)
            continue;
        sortiesByNavire[m.navireNom] += 1;
        totalSortiesPeriod += 1;
    }

    int maxSorties = 0;
    QString plusActif = QStringLiteral("\u2014");
    for (auto it = sortiesByNavire.constBegin(); it != sortiesByNavire.constEnd(); ++it) {
        if (it.value() > maxSorties || (it.value() == maxSorties && it.key() < plusActif)) {
            maxSorties = it.value();
            plusActif = it.key();
        }
    }
    ui->statCircleTitle2->setText("+ actif");
    ui->statCircleValue2->setText(plusActif);
    ui->statCircleSub2->setText(QString("%1 sorties").arg(maxSorties));
    double r2 = totalSortiesPeriod > 0 ? qMin(1.0, (double)maxSorties / totalSortiesPeriod * 2) : 0;
    paintDonutRing(ui->statCircle2, r2);

    // ---- Circle 3: Distance moyenne parcourue ----
    double totalDist = 0.0;
    int nbMouvements = 0;
    for (const Mouvement &m : std::as_const(mouvements)) {
        if (!m.heureDepart.isValid() || !m.heureRetour.isValid() || m.heureRetour <= m.heureDepart)
            continue;
        const qint64 duration = m.heureDepart.secsTo(m.heureRetour);
        const qint64 overlap = overlapSeconds(m.heureDepart, m.heureRetour, periodStart, periodEnd);
        if (duration <= 0 || overlap <= 0)
            continue;
        const double fraction = qBound(0.0, static_cast<double>(overlap) / static_cast<double>(duration), 1.0);
        totalDist += m.distanceKm * fraction;
        nbMouvements += 1;
    }
    const double moyenne = nbMouvements > 0 ? totalDist / nbMouvements : 0.0;
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

// Fonction utilitaire pour dessiner le QImage du QRCode
QImage generateQrImage(const QString& text) {
    using namespace qrcodegen;
    QrCode qr = QrCode::encodeText(text.toUtf8().constData(), QrCode::Ecc::LOW);

    int size = qr.getSize();
    QImage image(size, size, QImage::Format_RGB32);
    image.fill(Qt::white);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (qr.getModule(x, y)) {
                image.setPixel(x, y, qRgb(0, 0, 0));
            }
        }
    }
    return image.scaled(300, 300, Qt::KeepAspectRatio);
}

void MainWindow::onExportPDF()
{
    // Récupérer le navire sélectionné
    auto ranges = ui->tableNavires->selectedRanges();
    QList<int> selectedRows;
    for (auto &r : ranges) {
        for (int row = r.topRow(); row <= r.bottomRow(); ++row) {
            if (!selectedRows.contains(row)) selectedRows.append(row);
        }
    }

    if (selectedRows.isEmpty()) {
        QMessageBox::warning(this, "Export", "Veuillez sélectionner un navire à exporter.");
        return;
    }

    int row = selectedRows.first();
    QTableWidgetItem *idItem = ui->tableNavires->item(row, 1);
    if (!idItem) return;

    int navireId = idItem->data(Qt::UserRole).toInt();
    Navire selectedNavire;
    bool found = false;
    for (const Navire &n : std::as_const(navires)) {
        if (n.id == navireId) {
            selectedNavire = n;
            found = true;
            break;
        }
    }
    if (!found) return;

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF",
                                                    QString("Fiche_Navire_%1.pdf").arg(selectedNavire.matricule),
                                                    "PDF (*.pdf)");
    if (fileName.isEmpty()) return;

    // --- Contenu du QR Code (Texte direct pour lecture hors-ligne) ---
    // Au lieu d'une URL qui fait 404, on encode directement le certificat dans le QR Code !
    QString qrContent = QString("CERTIFICAT VÉRIFIÉ\nNom: %1\nMatricule: %2\nStatut: %3\nDate d'export: %4")
                            .arg(selectedNavire.nom)
                            .arg(selectedNavire.matricule)
                            .arg(selectedNavire.statut)
                            .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy"));

    if (selectedNavire.statut.toLower() == "actif") {
        qrContent.prepend("✅ AUTORISATION VALIDE\n\n");
    } else {
        qrContent.prepend("❌ AUTORISATION REFUSÉE\n\n");
    }

    // --- Génération de l'image du QR Code en Local ---
    QImage qrImg = generateQrImage(qrContent);

    // --- Génération du PDF ---
    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&printer);
    int pageW = printer.pageRect(QPrinter::DevicePixel).width();
    int y = 60;

    QFont titleFont("Segoe UI", 22, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QColor(0x13, 0x24, 0x37));
    painter.drawText(100, y, QString("Fiche d'Autorisation Navire"));
    y += 50;

    QFont subFont("Segoe UI", 12);
    painter.setFont(subFont);
    painter.drawText(100, y, QStringLiteral("Généré le %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")));
    y += 40;

    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), 3));
    painter.drawLine(100, y, pageW - 100, y);
    y += 40;

    QFont dataFont("Segoe UI", 12);
    QFont boldDataFont("Segoe UI", 12, QFont::Bold);

    auto drawLineProp = [&](const QString &label, const QString &val) {
        painter.setFont(boldDataFont);
        painter.setPen(QColor(0x13, 0x24, 0x37));
        painter.drawText(100, y, label);
        painter.setFont(dataFont);
        painter.drawText(350, y, val);
        y += 40;
    };

    drawLineProp("Nom du Navire :", selectedNavire.nom);
    drawLineProp("Matricule :", selectedNavire.matricule);
    drawLineProp("Type :", selectedNavire.type);
    drawLineProp("Capacité :", QString::number(selectedNavire.capacite));
    
    // Mettre en évidence le statut
    painter.setFont(boldDataFont);
    painter.setPen(QColor(0x13, 0x24, 0x37));
    painter.drawText(100, y, "Statut :");
    painter.setFont(dataFont);
    if (selectedNavire.statut.toLower() == "actif") {
        painter.setPen(QColor(0, 150, 0)); // Vert
    } else {
        painter.setPen(QColor(200, 0, 0)); // Rouge
    }
    painter.drawText(350, y, selectedNavire.statut);
    y += 40;

    painter.setPen(QColor(0x13, 0x24, 0x37)); // Reset color
    drawLineProp("Position :", selectedNavire.position);

    y += 20;
    painter.setPen(QPen(QColor(0x39, 0xc0, 0xfa), 3));
    painter.drawLine(100, y, pageW - 100, y);
    y += 40;

    // --- Dessin du QR Code ---
    painter.setFont(boldDataFont);
    painter.drawText(100, y, "Code d'Autorisation (Scannez pour valider)");
    y += 30;
    
    int qrSize = 250; 
    int qrX = (pageW - qrSize) / 2; // Centré
    painter.drawImage(QRect(qrX, y, qrSize, qrSize), qrImg);
    y += qrSize + 40;
    
    painter.setFont(dataFont);
    painter.drawText(100, y, pageW - 200, 50, Qt::AlignCenter, "Ce token généré permet de vérifier l'autorisation et les détails en ligne.");

    painter.end();
    QMessageBox::information(this, "Export", QString("Fiche PDF et QRCode générés avec succès :\n%1").arg(fileName));
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

void MainWindow::onSelectAllChanged(int state)
{
    if (state == Qt::Checked)
        ui->tableNavires->selectAll();
    else
        ui->tableNavires->clearSelection();
}




