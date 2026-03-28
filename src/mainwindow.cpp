#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractItemView>
#include <QApplication>
#include <algorithm>
#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QCalendarWidget>
#include <QFileDialog>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QGridLayout>
#include <QHeaderView>
#include <QHash>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QMessageBox>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QImage>
#include <QPushButton>
#include <QActionGroup>
#include <QScrollArea>
#include <QStyle>
#include <QSignalBlocker>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QTableView>
#include <QTimer>
#include <QEvent>
#include <QCursor>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>

#include "widgets/circularstatwidget.h"
#include "widgets/minimapwidget.h"

#include "pdfexporter.h"
#include "quaidialogs.h"
#include "quaimodel.h"
#include "quaisearchfilterproxymodel.h"
#include "quaisortproxymodel.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

class HoverPopupMenuController final : public QObject
{
public:
    HoverPopupMenuController(QPushButton *button, QMenu *menu, QObject *parent = nullptr)
        : QObject(parent)
        , m_button(button)
        , m_menu(menu)
    {
        if (m_menu)
        {
            connect(m_menu, &QMenu::aboutToHide, this, [this]() {
                if (m_button)
                    m_button->setChecked(false);
            });
        }

        // Track the cursor globally to strictly enforce:
        // menu visible <=> cursor is on the button.
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
            m_button->setChecked(true);
            const QPoint pos = m_button->mapToGlobal(QPoint(0, m_button->height()));
            m_menu->popup(pos);
        };

        const auto hideMenu = [this]() {
            if (m_menu->isVisible())
                m_menu->hide();
        };

        // Global cursor tracking: keep open while cursor is on button OR menu.
        if (event->type() == QEvent::MouseMove)
        {
            if (isCursorInPopupZone())
            {
                // Only open when hovering the button.
                const QPoint gp = QCursor::pos();
                const QRect btnRect(m_button->mapToGlobal(QPoint(0, 0)), m_button->size());
                if (btnRect.contains(gp))
                    showMenu();
            }
            else
            {
                // Requirement: close as soon as the cursor is outside both.
                hideMenu();
            }
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
                // Close if the cursor didn't enter the menu.
                if (!isCursorInPopupZone())
                    hideMenu();
                return false;
            }
        }

        if (watched == m_menu)
        {
            if (event->type() == QEvent::Enter)
            {
                return false;
            }
            if (event->type() == QEvent::Leave)
            {
                // Close if the cursor didn't return to the button.
                if (!isCursorInPopupZone())
                    hideMenu();
                return false;
            }
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

        // QMenu::geometry() is global.
        if (m_menu->isVisible() && m_menu->geometry().contains(gp))
            return true;

        return false;
    }

private:
    QPushButton *m_button = nullptr;
    QMenu *m_menu = nullptr;
};

class YyyyMmDdValidator final : public QValidator
{
public:
    explicit YyyyMmDdValidator(QObject *parent = nullptr)
        : QValidator(parent)
    {
    }

    State validate(QString &input, int &pos) const override
    {
        Q_UNUSED(pos);
        const QString s = input.trimmed();
        if (s.isEmpty())
            return Acceptable;

        // Allow progressive typing: 2026, 2026-, 2026-0, 2026-01, 2026-01-
        static const QRegularExpression shape(QStringLiteral(R"(^\d{0,4}(-\d{0,2}(-\d{0,2})?)?$)"));
        if (!shape.match(s).hasMatch())
            return Invalid;

        if (s.size() < 10)
            return Intermediate;
        if (s.size() > 10)
            return Invalid;

        // Full date: validate calendar ranges.
        const QDate d = QDate::fromString(s, QStringLiteral("yyyy-MM-dd"));
        return d.isValid() ? Acceptable : Invalid;
    }
};

class MatriculeMqSearchValidator final : public QValidator
{
public:
    explicit MatriculeMqSearchValidator(QObject *parent = nullptr)
        : QValidator(parent)
    {
    }

    State validate(QString &input, int &pos) const override
    {
        Q_UNUSED(pos);
        const QString s = input.trimmed();
        if (s.isEmpty())
            return Acceptable;
        if (s.contains(' '))
            return Invalid;

        const QString u = s.toUpper();

        // Accept full: MQ-<digits>
        static const QRegularExpression full(QStringLiteral(R"(^MQ-\d+$)"));
        if (full.match(u).hasMatch())
            return Acceptable;

        // Allow progressive typing: M, MQ, MQ-, MQ-12
        static const QRegularExpression partial(QStringLiteral(R"(^(M|MQ|MQ-|MQ-\d*)$)"));
        if (partial.match(u).hasMatch())
            return Intermediate;

        return Invalid;
    }
};

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

static QPixmap makeCircularLogo(const QPixmap &src, int diameterPx, int insetPx)
{
    QPixmap out(diameterPx, diameterPx);
    out.fill(Qt::transparent);

    QPainter p(&out);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath clip;
    clip.addEllipse(0, 0, diameterPx, diameterPx);
    p.setClipPath(clip);

    const int target = qMax(1, diameterPx - insetPx * 2);
    const QPixmap scaled = src.scaled(target, target, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    const int x = (diameterPx - scaled.width()) / 2;
    const int y = (diameterPx - scaled.height()) / 2;
    p.drawPixmap(x, y, scaled);

    return out;
}

static QPixmap cropAndMakeCircularLogo(const QPixmap &src, int diameterPx, int insetPx)
{
    if (src.isNull())
        return {};

    QImage img = src.toImage().convertToFormat(QImage::Format_ARGB32);
    if (img.isNull() || img.width() < 2 || img.height() < 2)
        return makeCircularLogo(src, diameterPx, insetPx);

    const QColor bg = QColor::fromRgba(img.pixel(0, 0));
    const int tolerance = 18;

    int minX = img.width(), minY = img.height(), maxX = -1, maxY = -1;
    for (int y = 0; y < img.height(); ++y)
    {
        const QRgb *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
        for (int x = 0; x < img.width(); ++x)
        {
            const QColor c = QColor::fromRgba(line[x]);
            const int dr = qAbs(c.red() - bg.red());
            const int dg = qAbs(c.green() - bg.green());
            const int db = qAbs(c.blue() - bg.blue());
            if ((dr + dg + db) > tolerance)
            {
                minX = qMin(minX, x);
                minY = qMin(minY, y);
                maxX = qMax(maxX, x);
                maxY = qMax(maxY, y);
            }
        }
    }

    if (maxX <= minX || maxY <= minY)
        return makeCircularLogo(src, diameterPx, insetPx);

    const int pad = 6;
    minX = qMax(0, minX - pad);
    minY = qMax(0, minY - pad);
    maxX = qMin(img.width() - 1, maxX + pad);
    maxY = qMin(img.height() - 1, maxY + pad);

    const QRect rect(minX, minY, (maxX - minX + 1), (maxY - minY + 1));
    const QPixmap cropped = QPixmap::fromImage(img.copy(rect));
    return makeCircularLogo(cropped, diameterPx, insetPx);
}

static void addCardShadow(QFrame *f)
{
    auto *shadow = new QGraphicsDropShadowEffect(f);
    shadow->setBlurRadius(24);
    shadow->setOffset(0, 8);
    shadow->setColor(QColor(0, 0, 0, 90));
    f->setGraphicsEffect(shadow);
}

// ---------------------------------------------------------------------------
// Table delegate
// ---------------------------------------------------------------------------

class TableNoElideDelegate final : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::initStyleOption(option, index);
        option->textElideMode = Qt::ElideNone;
        option->features |= QStyleOptionViewItem::WrapText;

        if (index.column() == QuaiModel::Arrivee || index.column() == QuaiModel::Depart)
        {
            const QVariant v = index.data(QuaiModel::SortValueRole);
            if (v.canConvert<QDateTime>())
                option->text = v.toDateTime().toString(QStringLiteral("yyyy-MM-dd\nHH:mm"));
        }

        if (index.column() != QuaiModel::IdQuai)
        {
            option->state &= ~QStyle::State_Selected;
            option->state &= ~QStyle::State_HasFocus;
        }
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize s = QStyledItemDelegate::sizeHint(option, index);
        if (index.column() == QuaiModel::Arrivee || index.column() == QuaiModel::Depart)
            s.setHeight(qMax(s.height(), option.fontMetrics.height() * 2 + 18));
        return s;
    }
};

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    applyStyleNames();
    setupCardEffects();
    setupLogo();
    setupMenuIcon();
    setupFonts();
    setupModels();
    setupTable();
    setupStatWidgets();
    setupMiniMaps();
    applyTheme();
    wireSignals();

    refreshStats();
    refreshMiniMap();
    refreshTraceability();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------------------------------------------------------------
// applyStyleNames – override objectNames for stylesheet compatibility
// ---------------------------------------------------------------------------

void MainWindow::applyStyleNames()
{
    // Stylesheet now embedded in mainwindow.ui using actual objectNames
}

// ---------------------------------------------------------------------------
// setupCardEffects
// ---------------------------------------------------------------------------

void MainWindow::setupCardEffects()
{
    for (auto *f : {ui->actionsCard, ui->tableCard, ui->statsCard,
                    ui->searchCard, ui->sortCard, ui->exportCard,
                    ui->revCard, ui->occCard, ui->histCard,
                    ui->rotCard, ui->revHistCard})
        addCardShadow(f);
}

// ---------------------------------------------------------------------------
// setupLogo
// ---------------------------------------------------------------------------

void MainWindow::setupLogo()
{
    QPixmap pm(":/vision_sight.jpeg");
    if (!pm.isNull())
        ui->logo->setPixmap(cropAndMakeCircularLogo(pm, 96, 8));
}

// ---------------------------------------------------------------------------
// setupMenuIcon
// ---------------------------------------------------------------------------

void MainWindow::setupMenuIcon()
{
    ui->menuButton->setIcon(makeHamburgerIcon(QColor("#e6eef6"), 18));
}

// ---------------------------------------------------------------------------
// setupFonts
// ---------------------------------------------------------------------------

void MainWindow::setupFonts()
{
    {
        QFont ft = ui->headerH1->font();
        ft.setPixelSize(28);
        ft.setBold(true);
        ui->headerH1->setFont(ft);
    }
    {
        QFont ft = ui->rotationOcc->font();
        ft.setPixelSize(14);
        ft.setBold(true);
        ui->rotationOcc->setFont(ft);
    }
    {
        QFont ft = ui->rotationLibre->font();
        ft.setPixelSize(14);
        ft.setBold(true);
        ui->rotationLibre->setFont(ft);
    }
}

// ---------------------------------------------------------------------------
// setupModels
// ---------------------------------------------------------------------------

void MainWindow::setupModels()
{
    m_model = new QuaiModel(this);
    m_searchProxy = new QuaiSearchFilterProxyModel(this);
    m_searchProxy->setSourceModel(m_model);

    m_sortProxy = new QuaiSortProxyModel(this);
    m_sortProxy->setSourceModel(m_searchProxy);

    m_revByQuaiModel = new QStandardItemModel(this);
    m_revByQuaiModel->setColumnCount(2);
    m_revByQuaiModel->setHorizontalHeaderLabels({tr("Matricule quai"), tr("Revenu (DT)")});

    m_occByQuaiModel = new QStandardItemModel(this);
    m_occByQuaiModel->setColumnCount(2);
    m_occByQuaiModel->setHorizontalHeaderLabels({tr("Matricule quai"), tr("Taux (%)")});

    m_occHistoryModel = new QStandardItemModel(this);
    m_occHistoryModel->setColumnCount(5);
    m_occHistoryModel->setHorizontalHeaderLabels({tr("Arrivée"), tr("Départ"), tr("Matricule quai"), tr("État"), tr("Prix (DT/h)")});

    m_revHistoryModel = new QStandardItemModel(this);
    m_revHistoryModel->setColumnCount(5);
    m_revHistoryModel->setHorizontalHeaderLabels({tr("Arrivée"), tr("Départ"), tr("Heures"), tr("Prix (DT/h)"), tr("Revenu (DT)")});

    // Replace QTableWidget placeholders with real QTableView instances
    auto replaceTableInLayout = [](QWidget *placeholder, QTableView *&realPtr, const QString &name, QWidget *parent) {
        realPtr = new QTableView(parent);
        realPtr->setObjectName(name);
        if (auto *lay = placeholder->parentWidget() ? qobject_cast<QVBoxLayout*>(placeholder->parentWidget()->layout()) : nullptr) {
            int idx = lay->indexOf(placeholder);
            lay->removeWidget(placeholder);
            placeholder->hide();
            placeholder->deleteLater();
            lay->insertWidget(idx, realPtr);
        }
        realPtr->setMinimumHeight(160);
        realPtr->setSelectionMode(QAbstractItemView::NoSelection);
        realPtr->setSelectionBehavior(QAbstractItemView::SelectRows);
        realPtr->setShowGrid(false);
        realPtr->setWordWrap(false);
        realPtr->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    };

    replaceTableInLayout(ui->revByQuaiTable, m_revByQuaiTable, QStringLiteral("revByQuaiTable"), this);
    replaceTableInLayout(ui->occByQuaiTable, m_occByQuaiTable, QStringLiteral("occByQuaiTable"), this);
    replaceTableInLayout(ui->occHistoryTable, m_occHistoryTable, QStringLiteral("occHistoryTable"), this);
    replaceTableInLayout(ui->revHistoryTable, m_revHistoryTable, QStringLiteral("revHistoryTable"), this);

    m_revByQuaiTable->setModel(m_revByQuaiModel);
    m_occByQuaiTable->setModel(m_occByQuaiModel);
    m_occHistoryTable->setModel(m_occHistoryModel);
    m_revHistoryTable->setModel(m_revHistoryModel);
}

// ---------------------------------------------------------------------------
// setupTable
// ---------------------------------------------------------------------------

void MainWindow::setupTable()
{
    // Replace the QTableWidget placeholder (visible in Designer) with a real QTableView
    m_table = new QTableView(this);
    m_table->setObjectName(QStringLiteral("table"));
    if (auto *layout = qobject_cast<QVBoxLayout *>(ui->tableCard->layout())) {
        layout->removeWidget(ui->table);
        ui->table->hide();
        ui->table->deleteLater();
        layout->addWidget(m_table);
    }
    // Copy properties from the .ui placeholder
    m_table->setMinimumSize(0, 320);
    m_table->setSelectionMode(QAbstractItemView::ExtendedSelection);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setShowGrid(false);
    m_table->setSortingEnabled(true);
    m_table->setAlternatingRowColors(false);
    m_table->setWordWrap(true);
    m_table->setTextElideMode(Qt::ElideNone);
    m_table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_table->setModel(m_sortProxy);
    m_table->setItemDelegate(new TableNoElideDelegate(m_table));
    m_table->verticalHeader()->setVisible(false);
    m_table->verticalHeader()->setDefaultSectionSize(66);

    auto *hh = m_table->horizontalHeader();
    hh->setMinimumSectionSize(70);
    hh->setDefaultAlignment(Qt::AlignCenter);
    hh->setSectionResizeMode(QHeaderView::Fixed);

    m_table->setColumnWidth(QuaiModel::IdQuai, 100);
    m_table->setColumnWidth(QuaiModel::Matricule, 160);
    m_table->setColumnWidth(QuaiModel::Taille, 110);
    m_table->setColumnWidth(QuaiModel::Arrivee, 135);
    m_table->setColumnWidth(QuaiModel::Depart, 135);
    m_table->setColumnWidth(QuaiModel::Etat, 170);
    m_table->setColumnWidth(QuaiModel::Prix, 110);
    m_table->setColumnWidth(QuaiModel::Retard, 110);
    hh->setSectionResizeMode(QuaiModel::Etat, QHeaderView::Fixed);
    hh->setStretchLastSection(true);

    for (auto *tv : {m_revByQuaiTable, m_occByQuaiTable, m_occHistoryTable, m_revHistoryTable})
    {
        tv->verticalHeader()->setVisible(false);
        tv->verticalHeader()->setDefaultSectionSize(40);
        auto *h = tv->horizontalHeader();
        h->setDefaultAlignment(Qt::AlignCenter);
        h->setSectionResizeMode(QHeaderView::Stretch);
    }

    ui->availAt->setDateTime(QDateTime::currentDateTime().addSecs(24 * 3600));
    ui->rangeStart->setDateTime(QDateTime::currentDateTime());
    ui->rangeEnd->setDateTime(QDateTime::currentDateTime().addSecs(2 * 3600));

    const auto configureCalendarPopup = [](QDateTimeEdit *edit) {
        if (!edit)
            return;
        edit->setCalendarPopup(true);
        auto *cal = new QCalendarWidget(edit);
        cal->setGridVisible(true);
        cal->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
        cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        cal->setNavigationBarVisible(true);
        cal->setLocale(QLocale::system());

        // Ensure the popup is large enough to show all weeks of the month.
        const QSize minSize(340, 260);
        cal->setMinimumSize(minSize);
        cal->resize(minSize);
        edit->setCalendarWidget(cal);
    };

    configureCalendarPopup(ui->availAt);
    configureCalendarPopup(ui->rangeStart);
    configureCalendarPopup(ui->rangeEnd);

    // Input control for search fields (keep it permissive enough for partial typing).
    // Matricule: MQ-<digits> (progressive typing), no spaces.
    if (ui->searchQuai)
    {
        ui->searchQuai->setMaxLength(32);
        ui->searchQuai->setValidator(new MatriculeMqSearchValidator(ui->searchQuai));
    }
    if (ui->traceQuai)
    {
        ui->traceQuai->setMaxLength(32);
        const QRegularExpression re(QStringLiteral(R"(^[A-Za-z0-9._-]*$)"));
        ui->traceQuai->setValidator(new QRegularExpressionValidator(re, ui->traceQuai));
    }
    // Date: allow progressively typing "YYYY-MM-DD".
    if (ui->searchDate)
    {
        ui->searchDate->setMaxLength(10);
        ui->searchDate->setValidator(new YyyyMmDdValidator(ui->searchDate));
        ui->searchDate->setPlaceholderText(QStringLiteral("YYYY-MM-DD"));
    }

    // Numeric controls: enforce sensible ranges in UI.
    if (ui->minPrice)
    {
        ui->minPrice->setRange(0.0, 1e9);
        ui->minPrice->setDecimals(2);
    }
    if (ui->maxPrice)
    {
        ui->maxPrice->setRange(0.0, 1e9);
        ui->maxPrice->setDecimals(2);
    }
    if (ui->shipLength)
    {
        ui->shipLength->setRange(0.0, 1e6);
        ui->shipLength->setDecimals(1);
    }

    if (auto *g = qobject_cast<QGridLayout *>(ui->searchGrid))
    {
        g->setColumnStretch(0, 1);
        g->setColumnStretch(1, 1);
    }
    if (auto *g = qobject_cast<QGridLayout *>(ui->advancedFilterGrid))
    {
        g->setColumnStretch(0, 0);
        g->setColumnStretch(1, 1);
        g->setColumnMinimumWidth(0, 140);
    }
    if (auto *g = qobject_cast<QGridLayout *>(ui->circlesGrid))
    {
        g->setColumnStretch(0, 1);
        g->setColumnStretch(1, 1);
        g->setColumnStretch(2, 1);
        g->setColumnStretch(3, 1);
        g->setRowStretch(0, 1);
        g->setRowStretch(1, 1);
    }

    // Select-all logic
    const auto selectedRowCount = [this]() -> int {
        if (!m_table || !m_table->selectionModel())
            return 0;
        return m_table->selectionModel()->selectedRows(QuaiModel::IdQuai).size();
    };

    const auto updateSelectAllState = [this, selectedRowCount]() {
        if (!ui->selectAll || !m_sortProxy)
            return;
        const int rows = m_sortProxy->rowCount();
        const bool all = (rows > 0) && (selectedRowCount() == rows);
        m_internalSelectAllChange = true;
        ui->selectAll->setChecked(m_selectAllWanted ? true : all);
        m_internalSelectAllChange = false;
    };

    const auto applySelectAllSelection = [this, updateSelectAllState]() {
        if (!m_table || !m_table->selectionModel() || !m_sortProxy)
            return;
        if (!m_selectAllWanted)
            return;

        m_internalTableSelectionChange = true;
        m_table->selectAll();
        if (m_sortProxy->rowCount() > 0)
        {
            const QModelIndex id0 = m_sortProxy->index(0, QuaiModel::IdQuai);
            if (id0.isValid())
                m_table->selectionModel()->setCurrentIndex(id0, QItemSelectionModel::NoUpdate);
        }
        m_internalTableSelectionChange = false;
        updateSelectAllState();
    };

    const auto scheduleApplySelectAll = [this, applySelectAllSelection]() {
        if (!m_selectAllWanted)
            return;
        QTimer::singleShot(0, this, [this, applySelectAllSelection]() {
            if (!m_selectAllWanted)
                return;
            applySelectAllSelection();
        });
    };

    connect(m_table, &QTableView::clicked, this, [this](const QModelIndex &proxyIndex) {
        if (!proxyIndex.isValid() || !m_table || !m_table->selectionModel())
            return;
        if (proxyIndex.column() == QuaiModel::IdQuai)
            return;
        const QModelIndex idIdx = proxyIndex.sibling(proxyIndex.row(), QuaiModel::IdQuai);
        if (idIdx.isValid())
            m_table->selectionModel()->setCurrentIndex(idIdx, QItemSelectionModel::NoUpdate);
    });

    connect(m_table->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this, updateSelectAllState](const QItemSelection &, const QItemSelection &) {
                if (m_internalTableSelectionChange)
                    return;
                updateSelectAllState();
            });

    connect(ui->selectAll, &QCheckBox::toggled, this, [this, updateSelectAllState, scheduleApplySelectAll](bool checked) {
        if (m_internalSelectAllChange)
            return;
        if (!m_table || !m_table->selectionModel() || !m_sortProxy)
            return;

        m_selectAllWanted = checked;
        if (!checked)
        {
            m_internalTableSelectionChange = true;
            m_table->clearSelection();
            m_table->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::NoUpdate);
            m_internalTableSelectionChange = false;
            updateSelectAllState();
            return;
        }

        updateSelectAllState();
        scheduleApplySelectAll();
    });

    connect(m_sortProxy, &QAbstractItemModel::modelReset, this, [this, updateSelectAllState, scheduleApplySelectAll]() {
        updateSelectAllState();
        scheduleApplySelectAll();
    });
    connect(m_sortProxy, &QAbstractItemModel::layoutChanged, this, [this, updateSelectAllState, scheduleApplySelectAll]() {
        updateSelectAllState();
        scheduleApplySelectAll();
    });
    connect(m_sortProxy, &QAbstractItemModel::rowsInserted, this, [this, updateSelectAllState, scheduleApplySelectAll]() {
        updateSelectAllState();
        scheduleApplySelectAll();
    });
    connect(m_sortProxy, &QAbstractItemModel::rowsRemoved, this, [this, updateSelectAllState, scheduleApplySelectAll]() {
        updateSelectAllState();
        scheduleApplySelectAll();
    });
}

// ---------------------------------------------------------------------------
// setupMiniMaps
// ---------------------------------------------------------------------------

void MainWindow::setupMiniMaps()
{
    // Single integrated map: enable the port illustration and keep reference selection enabled.
    ui->miniMap->setReferenceEnabled(true);
    ui->miniMap->setPortIllustrationEnabled(true);

    // Stat titles/subtexts are now set in setupStatWidgets()
}

// ---------------------------------------------------------------------------
// setupStatWidgets  – replace .ui placeholders with real CircularStatWidgets
// ---------------------------------------------------------------------------

void MainWindow::setupStatWidgets()
{
    auto replaceInGrid = [](QGridLayout *grid, QWidget *placeholder, CircularStatWidget *real, int row, int col) {
        grid->removeWidget(placeholder);
        placeholder->hide();
        placeholder->deleteLater();
        grid->addWidget(real, row, col);
    };

    auto replaceInHBox = [](QHBoxLayout *hbox, QWidget *placeholder, CircularStatWidget *real) {
        int idx = hbox->indexOf(placeholder);
        hbox->removeWidget(placeholder);
        placeholder->hide();
        placeholder->deleteLater();
        hbox->insertWidget(idx, real);
    };

    auto *grid = ui->circlesGrid;

    m_statEtat = new CircularStatWidget(this);
    replaceInGrid(grid, ui->phStatEtat, m_statEtat, 0, 0);
    m_statEtat->setTitle(tr("Etat"));
    m_statEtat->setValueText(QString());

    m_statTauxGlobal = new CircularStatWidget(this);
    replaceInGrid(grid, ui->phStatTauxGlobal, m_statTauxGlobal, 0, 1);
    m_statTauxGlobal->setTitle(tr("Taux global"));
    m_statTauxGlobal->setSubText(tr("%"));

    m_statRevenuPeriode = new CircularStatWidget(this);
    replaceInGrid(grid, ui->phStatRevenuPeriode, m_statRevenuPeriode, 0, 2);
    m_statRevenuPeriode->setTitle(tr("Revenu période"));
    m_statRevenuPeriode->setSubText(tr("DT"));
    m_statRevenuPeriode->setProgress(-1);

    m_statRevenuMoyen = new CircularStatWidget(this);
    replaceInGrid(grid, ui->phStatRevenuMoyen, m_statRevenuMoyen, 0, 3);
    m_statRevenuMoyen->setTitle(tr("Revenu moyen"));
    m_statRevenuMoyen->setSubText(tr("/ quai"));
    m_statRevenuMoyen->setProgress(-1);

    auto *hbox = ui->bottomStatsRow;

    m_statPlusRentable = new CircularStatWidget(this);
    replaceInHBox(hbox, ui->phStatPlusRentable, m_statPlusRentable);
    m_statPlusRentable->setTitle(tr("+ rentable"));
    m_statPlusRentable->setSubText(tr("Matricule quai"));
    m_statPlusRentable->setProgress(-1);

    m_statMoinsUtilise = new CircularStatWidget(this);
    replaceInHBox(hbox, ui->phStatMoinsUtilise, m_statMoinsUtilise);
    m_statMoinsUtilise->setTitle(tr("- utilisé"));
    m_statMoinsUtilise->setSubText(tr("Matricule quai"));
    m_statMoinsUtilise->setProgress(-1);
}

// ---------------------------------------------------------------------------
// applyTheme
// ---------------------------------------------------------------------------

void MainWindow::applyTheme()
{
    // Stylesheet now embedded in mainwindow.ui using actual objectNames
}

// ---------------------------------------------------------------------------
// wireSignals
// ---------------------------------------------------------------------------

void MainWindow::wireSignals()
{
    // Sidebar: "Gestion Des Quais" dropdown
    if (ui->sideBtn2)
    {
        auto *menu = new QMenu(ui->sideBtn2);

        auto *group = new QActionGroup(menu);
        group->setExclusive(true);

        QAction *actAffichage = menu->addAction(tr("Affichage"));
        actAffichage->setCheckable(true);
        QAction *actStat = menu->addAction(tr("Statistique"));
        actStat->setCheckable(true);
        QAction *actAvancees = menu->addAction(tr("Avancées"));
        actAvancees->setCheckable(true);

        group->addAction(actAffichage);
        group->addAction(actStat);
        group->addAction(actAvancees);

        enum class QuaiViewMode
        {
            Affichage = 0,
            Statistique,
            Avancees
        };

        const auto scrollTo = [this](QWidget *w) {
            if (!ui->scroll || !w)
                return;
            ui->scroll->ensureWidgetVisible(w, 0, 24);
        };

        const auto setVisibleSafe = [](QWidget *w, bool v) {
            if (w)
                w->setVisible(v);
        };

        const auto applyMode = [this, scrollTo, setVisibleSafe](QuaiViewMode mode) {
            // Default: hide everything, then enable the needed cards.
            const QList<QWidget *> allCards = {
                ui->actionsCard,
                ui->advancedCard,
                ui->searchCard,
                ui->sortCard,
                ui->exportCard,
                ui->tableCard,
                ui->statsCard,
                ui->revCard,
                ui->occCard,
                ui->histCard,
                ui->rotCard,
                ui->revHistCard,
            };

            for (QWidget *w : allCards)
                setVisibleSafe(w, false);

            switch (mode)
            {
            case QuaiViewMode::Statistique:
                // Only the sections shown in the screenshots
                setVisibleSafe(ui->statsCard, true);
                setVisibleSafe(ui->revCard, true);
                setVisibleSafe(ui->occCard, true);
                scrollTo(ui->statsCard);
                break;
            case QuaiViewMode::Avancees:
                // Replace the page with the sections shown in the screenshots:
                // - Gestion des quais (header + mini map)
                // - Proposition de quai
                // - Historique des occupations
                // - Analyse de rotation
                // - Historique des revenus
                setVisibleSafe(ui->actionsCard, true);
                setVisibleSafe(ui->advancedCard, true);
                setVisibleSafe(ui->histCard, true);
                setVisibleSafe(ui->rotCard, true);
                setVisibleSafe(ui->revHistCard, true);
                scrollTo(ui->actionsCard);
                break;
            case QuaiViewMode::Affichage:
            default:
                setVisibleSafe(ui->searchCard, true);
                setVisibleSafe(ui->sortCard, true);
                setVisibleSafe(ui->exportCard, true);
                setVisibleSafe(ui->tableCard, true);
                scrollTo(ui->searchCard ? ui->searchCard : ui->tableCard);
                break;
            }
        };

        connect(actAffichage, &QAction::triggered, this, [applyMode]() {
            applyMode(QuaiViewMode::Affichage);
        });
        connect(actStat, &QAction::triggered, this, [applyMode]() {
            applyMode(QuaiViewMode::Statistique);
        });
        connect(actAvancees, &QAction::triggered, this, [applyMode]() {
            applyMode(QuaiViewMode::Avancees);
        });

        // Default dropdown option + default mode on startup.
        actAffichage->setChecked(true);
        applyMode(QuaiViewMode::Affichage);

        // Show dropdown on hover (not on click).
        auto *hoverCtl = new HoverPopupMenuController(ui->sideBtn2, menu, this);
        ui->sideBtn2->installEventFilter(hoverCtl);
        menu->installEventFilter(hoverCtl);
    }

    connect(ui->btnCreate, &QPushButton::clicked, this, &MainWindow::onCreate);
    connect(ui->btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdate);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::onDelete);
    connect(ui->btnPdf, &QPushButton::clicked, this, &MainWindow::onExportPdf);

    connect(ui->searchQuai, &QLineEdit::textChanged, m_searchProxy, &QuaiSearchFilterProxyModel::setSearchMatricule);
    connect(ui->searchDate, &QLineEdit::textChanged, m_searchProxy, &QuaiSearchFilterProxyModel::setSearchDate);

    connect(ui->searchEtat, &QComboBox::currentTextChanged, m_searchProxy, &QuaiSearchFilterProxyModel::setStatusFilter);

    connect(ui->chkPrice, &QCheckBox::toggled, m_searchProxy, &QuaiSearchFilterProxyModel::setPriceRangeEnabled);
    connect(ui->minPrice, qOverload<double>(&QDoubleSpinBox::valueChanged), m_searchProxy, &QuaiSearchFilterProxyModel::setMinPrice);
    connect(ui->maxPrice, qOverload<double>(&QDoubleSpinBox::valueChanged), m_searchProxy, &QuaiSearchFilterProxyModel::setMaxPrice);

    // Keep min/max coherent: if max is non-zero, it must be >= min.
    const auto clampPriceRange = [this]() {
        if (!ui->chkPrice || !ui->minPrice || !ui->maxPrice)
            return;
        if (!ui->chkPrice->isChecked())
            return;
        const double minV = ui->minPrice->value();
        const double maxV = ui->maxPrice->value();
        if (maxV > 0.0 && maxV + 1e-9 < minV)
        {
            QSignalBlocker b(ui->maxPrice);
            ui->maxPrice->setValue(minV);
        }
    };
    connect(ui->minPrice, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [clampPriceRange](double) { clampPriceRange(); });
    connect(ui->maxPrice, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [clampPriceRange](double) { clampPriceRange(); });

    connect(ui->chkAvailAt, &QCheckBox::toggled, m_searchProxy, &QuaiSearchFilterProxyModel::setAvailableAtEnabled);
    connect(ui->availAt, &QDateTimeEdit::dateTimeChanged, m_searchProxy, &QuaiSearchFilterProxyModel::setAvailableAt);

    // Range availability (auto-correction): start must be strictly before end.
    // If invalid, silently adjust end = start + 1 second.
    m_lastValidRangeStart = ui->rangeStart->dateTime();
    m_lastValidRangeEnd = ui->rangeEnd->dateTime();
    if (m_lastValidRangeStart.isValid() && m_lastValidRangeEnd.isValid() && m_lastValidRangeEnd <= m_lastValidRangeStart)
        m_lastValidRangeEnd = m_lastValidRangeStart.addSecs(1);

    const auto pushRangeToProxy = [this]() {
        if (!m_searchProxy)
            return;
        m_searchProxy->setRangeStart(ui->rangeStart->dateTime());
        m_searchProxy->setRangeEnd(ui->rangeEnd->dateTime());
    };

    const auto showInvalidRangeMessage = [this]() {
        QMessageBox::warning(this,
                             tr("Plage invalide"),
                             tr("La date/heure d'arrivée doit être avant la date/heure de départ."));
    };

    const auto validateAndCommitRange = [this, pushRangeToProxy, showInvalidRangeMessage]() {
        static bool updating = false;
        if (updating)
            return;
        if (!ui->chkRange || !ui->rangeStart || !ui->rangeEnd)
            return;

        const QDateTime s = ui->rangeStart->dateTime();
        const QDateTime e = ui->rangeEnd->dateTime();

        // When disabled, just remember the last valid values (no message, no proxy update needed).
        if (!ui->chkRange->isChecked())
        {
            if (s.isValid() && e.isValid() && e > s)
            {
                m_lastValidRangeStart = s;
                m_lastValidRangeEnd = e;
            }
            return;
        }
        if (s.isValid() && e.isValid() && e <= s)
        {
            updating = true;
            showInvalidRangeMessage();
            {
                QSignalBlocker be(ui->rangeEnd);
                ui->rangeEnd->setDateTime(s.addSecs(1));
            }
            updating = false;
        }

        const QDateTime s2 = ui->rangeStart->dateTime();
        const QDateTime e2 = ui->rangeEnd->dateTime();
        if (s2.isValid() && e2.isValid() && e2 > s2)
        {
            m_lastValidRangeStart = s2;
            m_lastValidRangeEnd = e2;
        }
        pushRangeToProxy();
    };

    connect(ui->chkRange, &QCheckBox::toggled, this, [this, validateAndCommitRange](bool enabled) {
        if (m_searchProxy)
            m_searchProxy->setRangeAvailabilityEnabled(enabled);

        if (enabled)
        {
            validateAndCommitRange();
        }
    });

    connect(ui->rangeStart, &QDateTimeEdit::dateTimeChanged, this, [validateAndCommitRange](const QDateTime &) { validateAndCommitRange(); });
    connect(ui->rangeEnd, &QDateTimeEdit::dateTimeChanged, this, [validateAndCommitRange](const QDateTime &) { validateAndCommitRange(); });

    connect(ui->sortMode, &QComboBox::currentIndexChanged, this, [this](int idx) {
        using SM = QuaiSortProxyModel::SortMode;
        SM mode = SM::TableColumn;
        switch (idx)
        {
        case 0: mode = SM::TailleAsc; break;
        case 1: mode = SM::Statut; break;
        case 2: mode = SM::LiberationBientot; break;
        case 3: mode = SM::OccupeDepuisLongtemps; break;
        case 4: mode = SM::PrixAsc; break;
        default: mode = SM::TableColumn; break;
        }
        m_sortProxy->setSortMode(mode);
        m_sortProxy->sort(0);
    });

    connect(m_model, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::modelReset, this, &MainWindow::refreshStats);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &MainWindow::refreshStats);

    connect(m_model, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshTraceability);
    connect(m_model, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshTraceability);
    connect(m_model, &QAbstractItemModel::modelReset, this, &MainWindow::refreshTraceability);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &MainWindow::refreshTraceability);

    connect(ui->revenuePeriod, &QComboBox::currentIndexChanged, this, &MainWindow::refreshStats);

    connect(ui->traceQuai, &QLineEdit::textChanged, this, [this](const QString &) {
        refreshTraceability();
    });

    const auto syncPriceEnabled = [this](bool enabled) {
        ui->minPrice->setEnabled(enabled);
        ui->maxPrice->setEnabled(enabled);
    };
    connect(ui->chkPrice, &QCheckBox::toggled, this, syncPriceEnabled);
    syncPriceEnabled(ui->chkPrice->isChecked());

    const auto syncAvailAtEnabled = [this](bool enabled) {
        ui->availAt->setEnabled(enabled);
        if (enabled)
        {
            QSignalBlocker b(ui->availAt);
            ui->availAt->setDateTime(QDateTime::currentDateTime());
        }
    };
    connect(ui->chkAvailAt, &QCheckBox::toggled, this, syncAvailAtEnabled);
    syncAvailAtEnabled(ui->chkAvailAt->isChecked());

    const auto syncRangeEnabled = [this](bool enabled) {
        ui->rangeStart->setEnabled(enabled);
        ui->rangeEnd->setEnabled(enabled);
        if (enabled)
        {
            const QDateTime now = QDateTime::currentDateTime();
            {
                QSignalBlocker bs(ui->rangeStart);
                QSignalBlocker be(ui->rangeEnd);
                ui->rangeStart->setDateTime(now);
                ui->rangeEnd->setDateTime(now.addSecs(2 * 3600));
            }
            m_lastValidRangeStart = ui->rangeStart->dateTime();
            m_lastValidRangeEnd = ui->rangeEnd->dateTime();
        }
    };
    connect(ui->chkRange, &QCheckBox::toggled, this, syncRangeEnabled);
    syncRangeEnabled(ui->chkRange->isChecked());

    // Mini map + proposals
    connect(ui->miniMap, &MiniMapWidget::referencePointChanged, this, [this](double x, double y) {
        QSignalBlocker bx(ui->refX);
        QSignalBlocker by(ui->refY);
        ui->refX->setValue(x);
        ui->refY->setValue(y);
        refreshMiniMap();
    });

    connect(ui->refX, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        refreshMiniMap();
    });
    connect(ui->refY, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [this](double) {
        refreshMiniMap();
    });

    connect(m_model, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshMiniMap);
    connect(m_model, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshMiniMap);
    connect(m_model, &QAbstractItemModel::modelReset, this, &MainWindow::refreshMiniMap);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &MainWindow::refreshMiniMap);

    connect(m_searchProxy, &QAbstractItemModel::modelReset, this, &MainWindow::refreshMiniMap);
    connect(m_searchProxy, &QAbstractItemModel::layoutChanged, this, &MainWindow::refreshMiniMap);
    connect(m_searchProxy, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshMiniMap);
    connect(m_searchProxy, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshMiniMap);

    connect(m_searchProxy, &QAbstractItemModel::modelReset, this, &MainWindow::refreshTraceability);
    connect(m_searchProxy, &QAbstractItemModel::layoutChanged, this, &MainWindow::refreshTraceability);

    connect(m_sortProxy, &QAbstractItemModel::modelReset, this, &MainWindow::refreshMiniMap);
    connect(m_sortProxy, &QAbstractItemModel::layoutChanged, this, &MainWindow::refreshMiniMap);
    connect(m_sortProxy, &QAbstractItemModel::rowsInserted, this, &MainWindow::refreshMiniMap);
    connect(m_sortProxy, &QAbstractItemModel::rowsRemoved, this, &MainWindow::refreshMiniMap);

    connect(m_sortProxy, &QAbstractItemModel::modelReset, this, &MainWindow::refreshTraceability);
    connect(m_sortProxy, &QAbstractItemModel::layoutChanged, this, &MainWindow::refreshTraceability);

    const auto isLibre = [](const QString &etat) {
        return etat.compare("Libre", Qt::CaseInsensitive) == 0;
    };
    const auto isMaint = [](const QString &etat) {
        return etat.contains("maintenance", Qt::CaseInsensitive);
    };

    const auto selectProxyRow = [this](int proxyRow) {
        if (!m_table || !m_sortProxy || !m_table->selectionModel())
            return;
        const QModelIndex idx = m_sortProxy->index(proxyRow, QuaiModel::IdQuai);
        if (!idx.isValid())
            return;
        m_table->selectionModel()->clearSelection();
        m_table->setCurrentIndex(idx);
        m_table->selectionModel()->select(idx, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        m_table->scrollTo(idx, QAbstractItemView::PositionAtCenter);
    };

    const auto sourceRowForProxyRow = [this](int proxyRow) -> int {
        if (!m_sortProxy || !m_searchProxy)
            return -1;
        const QModelIndex proxyIdx = m_sortProxy->index(proxyRow, 0);
        if (!proxyIdx.isValid())
            return -1;
        const QModelIndex searchIdx = m_sortProxy->mapToSource(proxyIdx);
        const QModelIndex sourceIdx = m_searchProxy->mapToSource(searchIdx);
        return sourceIdx.isValid() ? sourceIdx.row() : -1;
    };

    connect(ui->btnProposeCheapest, &QPushButton::clicked, this, [this, isLibre, isMaint, selectProxyRow, sourceRowForProxyRow]() {
        if (!m_sortProxy || !m_model)
            return;

        int bestProxyRow = -1;
        double bestPrice = 1e300;

        for (int r = 0; r < m_sortProxy->rowCount(); ++r)
        {
            const int srow = sourceRowForProxyRow(r);
            if (srow < 0 || srow >= m_model->items().size())
                continue;
            const Quai &q = m_model->items().at(srow);
            if (isMaint(q.etatQuai) || !isLibre(q.etatQuai))
                continue;
            if (q.prixLocation < bestPrice)
            {
                bestPrice = q.prixLocation;
                bestProxyRow = r;
            }
        }

        if (bestProxyRow < 0)
        {
            ui->proposeResult->setText(tr("Aucun quai libre visible pour proposer le moins cher."));
            return;
        }

        const int srow = sourceRowForProxyRow(bestProxyRow);
        const Quai &q = m_model->items().at(srow);
        selectProxyRow(bestProxyRow);
        ui->miniMap->setHighlightedId(q.idQuai);
        ui->proposeResult->setText(tr("Proposition (moins cher): <b>%1</b> — %2 DT/h")
                                       .arg(q.idQuai.toHtmlEscaped(), QLocale().toString(q.prixLocation, 'f', 2)));
    });

    connect(ui->btnProposeNearest, &QPushButton::clicked, this, [this, isLibre, isMaint, selectProxyRow, sourceRowForProxyRow]() {
        if (!m_sortProxy || !m_model)
            return;

        const double rx = ui->refX->value();
        const double ry = ui->refY->value();

        int bestProxyRow = -1;
        double bestD2 = 1e300;

        for (int r = 0; r < m_sortProxy->rowCount(); ++r)
        {
            const int srow = sourceRowForProxyRow(r);
            if (srow < 0 || srow >= m_model->items().size())
                continue;
            const Quai &q = m_model->items().at(srow);
            if (isMaint(q.etatQuai) || !isLibre(q.etatQuai))
                continue;
            const double dx = q.posX - rx;
            const double dy = q.posY - ry;
            const double d2 = dx * dx + dy * dy;
            if (d2 < bestD2)
            {
                bestD2 = d2;
                bestProxyRow = r;
            }
        }

        if (bestProxyRow < 0)
        {
            ui->proposeResult->setText(tr("Aucun quai libre visible pour proposer le plus proche."));
            return;
        }

        const int srow = sourceRowForProxyRow(bestProxyRow);
        const Quai &q = m_model->items().at(srow);
        selectProxyRow(bestProxyRow);
        ui->miniMap->setHighlightedId(q.idQuai);
        ui->proposeResult->setText(tr("Proposition (plus proche): <b>%1</b> — (X=%2, Y=%3)")
                                       .arg(q.idQuai.toHtmlEscaped(), QLocale().toString(q.posX, 'f', 2), QLocale().toString(q.posY, 'f', 2)));
    });

    connect(ui->btnProposeFit, &QPushButton::clicked, this, [this, isLibre, isMaint, selectProxyRow, sourceRowForProxyRow]() {
        if (!m_sortProxy || !m_model)
            return;

        const double needed = ui->shipLength->value();
        const QString wantedType = ui->shipType->currentText().trimmed();

        int bestProxyRow = -1;
        double bestTaille = 1e300;
        double bestPrice = 1e300;

        for (int r = 0; r < m_sortProxy->rowCount(); ++r)
        {
            const int srow = sourceRowForProxyRow(r);
            if (srow < 0 || srow >= m_model->items().size())
                continue;
            const Quai &q = m_model->items().at(srow);
            if (isMaint(q.etatQuai) || !isLibre(q.etatQuai))
                continue;
            if (q.taille + 1e-6 < needed)
                continue;

            if (!wantedType.isEmpty() && wantedType.compare("Tous", Qt::CaseInsensitive) != 0)
            {
                const bool matches = (q.typeQuai.compare(wantedType, Qt::CaseInsensitive) == 0) ||
                                     (q.typeQuai.compare("Général", Qt::CaseInsensitive) == 0);
                if (!matches)
                    continue;
            }

            if (q.taille < bestTaille || (qFuzzyCompare(q.taille, bestTaille) && q.prixLocation < bestPrice))
            {
                bestTaille = q.taille;
                bestPrice = q.prixLocation;
                bestProxyRow = r;
            }
        }

        if (bestProxyRow < 0)
        {
            ui->proposeResult->setText(tr("Aucun quai libre visible n'est adapté (%1 m, type: %2).")
                                           .arg(QLocale().toString(needed, 'f', 1), wantedType.isEmpty() ? tr("Tous") : wantedType));
            return;
        }

        const int srow = sourceRowForProxyRow(bestProxyRow);
        const Quai &q = m_model->items().at(srow);
        selectProxyRow(bestProxyRow);
        ui->miniMap->setHighlightedId(q.idQuai);
        ui->proposeResult->setText(tr("Proposition (adapté): <b>%1</b> — Type %2 — Taille %3 m — %4 DT/h")
                                       .arg(q.idQuai.toHtmlEscaped(),
                                            q.typeQuai.toHtmlEscaped(),
                                            QLocale().toString(q.taille, 'f', 1),
                                            QLocale().toString(q.prixLocation, 'f', 2)));
    });
}

// ---------------------------------------------------------------------------
// refreshMiniMap
// ---------------------------------------------------------------------------

void MainWindow::refreshMiniMap()
{
    if (!m_sortProxy || !m_searchProxy || !m_model)
        return;

    QVector<MiniMapWidget::Point> pts;
    pts.reserve(m_sortProxy->rowCount());

    const auto isLibre = [](const QString &etat) {
        return etat.compare("Libre", Qt::CaseInsensitive) == 0;
    };
    const auto isOcc = [](const QString &etat) {
        return etat.compare("Occupé", Qt::CaseInsensitive) == 0 || etat.compare("Occupe", Qt::CaseInsensitive) == 0;
    };
    const auto isMaint = [](const QString &etat) {
        return etat.contains("maintenance", Qt::CaseInsensitive);
    };

    for (int r = 0; r < m_sortProxy->rowCount(); ++r)
    {
        const QModelIndex proxyIdx = m_sortProxy->index(r, 0);
        if (!proxyIdx.isValid())
            continue;

        const QModelIndex searchIdx = m_sortProxy->mapToSource(proxyIdx);
        const QModelIndex sourceIdx = m_searchProxy->mapToSource(searchIdx);
        if (!sourceIdx.isValid() || sourceIdx.row() < 0 || sourceIdx.row() >= m_model->items().size())
            continue;

        const Quai &q = m_model->items().at(sourceIdx.row());

        QColor c("#39c0fa");
        if (isMaint(q.etatQuai))
            c = QColor(255, 255, 255, 60);
        else if (isLibre(q.etatQuai))
            c = QColor("#e6eef6");
        else if (isOcc(q.etatQuai))
            c = QColor("#39c0fa");

        pts.push_back({q.idQuai, q.posX, q.posY, c});
    }

    ui->miniMap->setPoints(pts);
    ui->miniMap->setReferencePoint(ui->refX->value(), ui->refY->value());
}

// ---------------------------------------------------------------------------
// refreshTraceability
// ---------------------------------------------------------------------------

void MainWindow::refreshTraceability()
{
    if (!m_occHistoryModel || !m_model)
        return;

    const QString typed = ui->traceQuai->text().trimmed();
    QString selectedId;

    if (!typed.isEmpty())
    {
        int bestIdx = -1;
        for (int i = 0; i < m_model->items().size(); ++i)
        {
            const auto &q = m_model->items().at(i);
            if (q.matricule.compare(typed, Qt::CaseInsensitive) == 0)
            {
                bestIdx = i;
                break;
            }
            if (bestIdx < 0 && q.matricule.contains(typed, Qt::CaseInsensitive))
                bestIdx = i;
        }
        if (bestIdx >= 0)
            selectedId = m_model->items().at(bestIdx).idQuai;
    }

    m_occHistoryModel->removeRows(0, m_occHistoryModel->rowCount());
    if (m_revHistoryModel)
        m_revHistoryModel->removeRows(0, m_revHistoryModel->rowCount());

    if (selectedId.trimmed().isEmpty())
    {
        ui->rotationOcc->setText(tr("Temps moyen occupé: —"));
        ui->rotationLibre->setText(tr("Temps moyen libre: —"));
        return;
    }

    auto isOcc = [&](const Quai &q) {
        return q.etatQuai.compare("Occupé", Qt::CaseInsensitive) == 0 || q.etatQuai.compare("Occupe", Qt::CaseInsensitive) == 0;
    };

    QVector<Quai> records = m_model->historyForId(selectedId);

    std::sort(records.begin(), records.end(), [](const Quai &a, const Quai &b) {
        return a.dateHeureArrivee > b.dateHeureArrivee;
    });

    const QString dateFmt = QStringLiteral("yyyy-MM-dd HH:mm");
    const QLocale loc;

    for (const auto &q : records)
    {
        QList<QStandardItem *> row;
        row << new QStandardItem(q.dateHeureArrivee.isValid() ? q.dateHeureArrivee.toString(dateFmt) : QString());
        row << new QStandardItem(q.dateHeureDepart.isValid() ? q.dateHeureDepart.toString(dateFmt) : QString());
        row << new QStandardItem(q.matricule);
        row << new QStandardItem(q.etatQuai);
        row << new QStandardItem(loc.toString(q.prixLocation, 'f', 2));

        for (auto *it : row)
            it->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

        m_occHistoryModel->appendRow(row);
    }

    if (m_revHistoryModel)
    {
        for (const auto &q : records)
        {
            if (!isOcc(q))
                continue;
            if (!q.dateHeureArrivee.isValid() || !q.dateHeureDepart.isValid() || q.dateHeureDepart <= q.dateHeureArrivee)
                continue;
            const qint64 secs = q.dateHeureArrivee.secsTo(q.dateHeureDepart);
            if (secs <= 0)
                continue;

            const double hours = static_cast<double>(secs) / 3600.0;
            const double rev = (q.prixLocation > 0.0) ? (hours * q.prixLocation) : 0.0;

            QList<QStandardItem *> row;
            row << new QStandardItem(q.dateHeureArrivee.toString(dateFmt));
            row << new QStandardItem(q.dateHeureDepart.toString(dateFmt));
            row << new QStandardItem(loc.toString(hours, 'f', 2));
            row << new QStandardItem(loc.toString(q.prixLocation, 'f', 2));
            row << new QStandardItem(loc.toString(rev, 'f', 2));

            for (auto *it : row)
                it->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

            m_revHistoryModel->appendRow(row);
        }
    }

    QVector<Quai> occ;
    for (const auto &q : records)
    {
        if (!isOcc(q))
            continue;
        if (!q.dateHeureArrivee.isValid() || !q.dateHeureDepart.isValid() || q.dateHeureDepart <= q.dateHeureArrivee)
            continue;
        occ.push_back(q);
    }
    std::sort(occ.begin(), occ.end(), [](const Quai &a, const Quai &b) {
        return a.dateHeureArrivee < b.dateHeureArrivee;
    });

    double occSumHours = 0.0;
    int occCount = 0;
    for (const auto &q : occ)
    {
        const qint64 secs = q.dateHeureArrivee.secsTo(q.dateHeureDepart);
        if (secs > 0)
        {
            occSumHours += static_cast<double>(secs) / 3600.0;
            ++occCount;
        }
    }

    double freeSumHours = 0.0;
    int freeCount = 0;
    for (int i = 0; i + 1 < occ.size(); ++i)
    {
        const qint64 gapSecs = occ[i].dateHeureDepart.secsTo(occ[i + 1].dateHeureArrivee);
        if (gapSecs > 0)
        {
            freeSumHours += static_cast<double>(gapSecs) / 3600.0;
            ++freeCount;
        }
    }

    if (occCount > 0)
        ui->rotationOcc->setText(tr("Temps moyen occupé: %1 h").arg(loc.toString(occSumHours / static_cast<double>(occCount), 'f', 2)));
    else
        ui->rotationOcc->setText(tr("Temps moyen occupé: —"));

    if (freeCount > 0)
        ui->rotationLibre->setText(tr("Temps moyen libre: %1 h").arg(loc.toString(freeSumHours / static_cast<double>(freeCount), 'f', 2)));
    else
        ui->rotationLibre->setText(tr("Temps moyen libre: —"));
}

// ---------------------------------------------------------------------------
// refreshStats
// ---------------------------------------------------------------------------

void MainWindow::refreshStats()
{
    const auto &items = m_model->items();
    const auto &history = m_model->history();

    const QDateTime now = QDateTime::currentDateTime();

    auto overlapSeconds = [](const QDateTime &a1, const QDateTime &a2, const QDateTime &b1, const QDateTime &b2) -> qint64 {
        if (!a1.isValid() || !a2.isValid() || !b1.isValid() || !b2.isValid())
            return 0;
        const QDateTime start = (a1 > b1) ? a1 : b1;
        const QDateTime end = (a2 < b2) ? a2 : b2;
        const qint64 secs = start.secsTo(end);
        return secs > 0 ? secs : 0;
    };

    auto periodStartEnd = [&](QDateTime *startOut, QDateTime *endOut) {
        QDateTime end = now;
        QDateTime start = QDateTime();
        const int idx = ui->revenuePeriod ? ui->revenuePeriod->currentIndex() : 1;
        switch (idx)
        {
        case 0: start = now.addSecs(-24 * 3600); break;
        case 1: start = now.addDays(-7); break;
        case 2: start = now.addDays(-30); break;
        case 3: default: start = QDateTime(QDate(2000, 1, 1), QTime(0, 0)); break;
        }
        if (startOut) *startOut = start;
        if (endOut) *endOut = end;
    };

    QDateTime periodStart;
    QDateTime periodEnd;
    periodStartEnd(&periodStart, &periodEnd);

    const qint64 periodSecsRaw = periodStart.isValid() && periodEnd.isValid() ? periodStart.secsTo(periodEnd) : 0;
    const double periodSecs = periodSecsRaw > 0 ? static_cast<double>(periodSecsRaw) : 0.0;

    int occ = 0;
    int libres = 0;
    int considered = 0;
    double revenuePeriod = 0.0;

    QHash<QString, double> revenueByQuai;
    QHash<QString, double> occSecondsByQuai;

    for (const auto &q : items)
    {
        const bool isOcc = q.etatQuai.compare("Occupé", Qt::CaseInsensitive) == 0 || q.etatQuai.compare("Occupe", Qt::CaseInsensitive) == 0;
        const bool isLibre = q.etatQuai.compare("Libre", Qt::CaseInsensitive) == 0;
        const bool isMaint = q.etatQuai.contains("maintenance", Qt::CaseInsensitive);

        if (!isMaint) ++considered;
        if (!isMaint && isOcc) ++occ;
        if (!isMaint && isLibre) ++libres;
    }

    for (const auto &h : history)
    {
        const bool isMaint = h.etatQuai.contains("maintenance", Qt::CaseInsensitive);
        if (isMaint) continue;

        const bool isOcc = h.etatQuai.compare("Occupé", Qt::CaseInsensitive) == 0 || h.etatQuai.compare("Occupe", Qt::CaseInsensitive) == 0;
        if (!isOcc) continue;

        if (!h.dateHeureArrivee.isValid() || !h.dateHeureDepart.isValid() || h.dateHeureDepart <= h.dateHeureArrivee)
            continue;

        const qint64 overlap = overlapSeconds(h.dateHeureArrivee, h.dateHeureDepart, periodStart, periodEnd);
        if (overlap <= 0) continue;

        occSecondsByQuai[h.idQuai] += static_cast<double>(overlap);
        if (h.prixLocation > 0.0)
        {
            const double overlapHours = overlap / 3600.0;
            const double rev = overlapHours * h.prixLocation;
            revenueByQuai[h.idQuai] += rev;
            revenuePeriod += rev;
        }
    }

    if (m_revByQuaiModel)
        m_revByQuaiModel->removeRows(0, m_revByQuaiModel->rowCount());
    if (m_occByQuaiModel)
        m_occByQuaiModel->removeRows(0, m_occByQuaiModel->rowCount());

    struct RowStat { QString id; QString matricule; double rev; double occRate; bool isMaint; };
    QVector<RowStat> per;
    per.reserve(items.size());

    for (const auto &q : items)
    {
        const bool isMaint = q.etatQuai.contains("maintenance", Qt::CaseInsensitive);
        const double rev = revenueByQuai.value(q.idQuai, 0.0);
        double occRate = 0.0;
        if (!isMaint && periodSecs > 0.0)
        {
            const double occSecs = qBound(0.0, occSecondsByQuai.value(q.idQuai, 0.0), periodSecs);
            occRate = qBound(0.0, occSecs / periodSecs, 1.0);
        }
        per.push_back({q.idQuai, q.matricule, rev, occRate, isMaint});
    }

    double bestRevenue = -1.0;
    QString bestQuaiId;
    double leastOccRate = 1e300;
    QString leastUsedQuaiId;
    for (const auto &s : per)
    {
        if (s.isMaint) continue;
        if (s.rev > bestRevenue || (qFuzzyCompare(s.rev + 1.0, bestRevenue + 1.0) && s.id < bestQuaiId))
        {
            bestRevenue = s.rev;
            bestQuaiId = s.id;
        }
        if (s.occRate < leastOccRate || (qFuzzyCompare(s.occRate + 1.0, leastOccRate + 1.0) && s.id < leastUsedQuaiId))
        {
            leastOccRate = s.occRate;
            leastUsedQuaiId = s.id;
        }
    }

    if (m_revByQuaiModel)
    {
        QVector<RowStat> revSorted = per;
        std::sort(revSorted.begin(), revSorted.end(), [](const RowStat &a, const RowStat &b) {
            if (a.rev == b.rev) return a.id < b.id;
            return a.rev > b.rev;
        });
        for (const auto &s : revSorted)
        {
            QList<QStandardItem *> row;
            auto *idItem = new QStandardItem(s.matricule);
            idItem->setTextAlignment(Qt::AlignCenter);
            auto *revItem = new QStandardItem(QLocale().toString(s.rev, 'f', 2));
            revItem->setTextAlignment(Qt::AlignCenter);
            row << idItem << revItem;
            m_revByQuaiModel->appendRow(row);
        }
    }

    if (m_occByQuaiModel)
    {
        QVector<RowStat> occSorted = per;
        std::sort(occSorted.begin(), occSorted.end(), [](const RowStat &a, const RowStat &b) {
            if (a.occRate == b.occRate) return a.id < b.id;
            return a.occRate > b.occRate;
        });
        for (const auto &s : occSorted)
        {
            const double pct = s.occRate * 100.0;
            QList<QStandardItem *> row;
            auto *idItem = new QStandardItem(s.matricule);
            idItem->setTextAlignment(Qt::AlignCenter);
            auto *pctItem = new QStandardItem(QString::number(pct, 'f', 1));
            pctItem->setTextAlignment(Qt::AlignCenter);
            row << idItem << pctItem;
            m_occByQuaiModel->appendRow(row);
        }
    }

    const double taux = considered > 0 ? (static_cast<double>(occ) / static_cast<double>(considered)) * 100.0 : 0.0;

    m_statEtat->setValueText(QString());
    m_statEtat->setDualLegendFormat(0, QString());
    m_statEtat->setDualSegments(tr("Occupés"), static_cast<double>(occ), QColor("#39c0fa"),
                                  tr("Libres"), static_cast<double>(libres), QColor("#e6eef6"));

    m_statTauxGlobal->setValueText(QString::number(taux, 'f', 1) + "%");
    m_statTauxGlobal->setProgress(taux / 100.0);

    m_statRevenuPeriode->setValueText(QLocale().toString(revenuePeriod, 'f', 2));
    m_statRevenuPeriode->setAccentColor(QColor("#39c0fa"));
    m_statRevenuPeriode->setProgress(revenuePeriod > 0.0 ? 0.85 : 0.0);

    const double avgRevenue = (considered > 0) ? (revenuePeriod / static_cast<double>(considered)) : 0.0;
    m_statRevenuMoyen->setValueText(QLocale().toString(avgRevenue, 'f', 2));
    m_statRevenuMoyen->setAccentColor(QColor("#39c0fa"));
    m_statRevenuMoyen->setProgress(avgRevenue > 0.0 ? 0.70 : 0.0);

    const auto matriculeForId = [this](const QString &id) -> QString {
        if (!m_model) return id;
        for (const auto &q : m_model->items())
        {
            if (q.idQuai.compare(id, Qt::CaseInsensitive) == 0)
                return q.matricule;
        }
        return id;
    };

    {
        const QString label = bestQuaiId.isEmpty() ? QStringLiteral("-") : matriculeForId(bestQuaiId);
        m_statPlusRentable->setValueText(label);
        m_statPlusRentable->setAccentColor(QColor("#39c0fa"));
        m_statPlusRentable->setProgress(bestRevenue >= 0.0 ? 1.0 : 0.0);
        if (bestRevenue >= 0.0)
            m_statPlusRentable->setSubText(QLocale().toString(bestRevenue, 'f', 2) + " DT");
        else
            m_statPlusRentable->setSubText(QString());
    }

    {
        const QString label = leastUsedQuaiId.isEmpty() ? QStringLiteral("-") : matriculeForId(leastUsedQuaiId);
        m_statMoinsUtilise->setValueText(label);
        m_statMoinsUtilise->setAccentColor(QColor("#39c0fa"));
        m_statMoinsUtilise->setProgress(leastOccRate < 1e299 ? qBound(0.0, 1.0 - leastOccRate, 1.0) : 0.0);
        if (leastOccRate < 1e299)
            m_statMoinsUtilise->setSubText(QString::number(leastOccRate * 100.0, 'f', 1) + "%");
        else
            m_statMoinsUtilise->setSubText(QString());
    }
}

// ---------------------------------------------------------------------------
// CRUD
// ---------------------------------------------------------------------------

void MainWindow::onCreate()
{
    QuaiFormDialog dlg(QuaiFormDialog::Mode::Create, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    QString error;
    if (!m_model->addQuai(dlg.quai(), &error))
    {
        QMessageBox::warning(this, tr("Créer"), error);
        return;
    }
}

void MainWindow::onUpdate()
{
    QModelIndex proxyIndex;
    if (m_table && m_table->selectionModel())
    {
        const QModelIndexList indexes = m_table->selectionModel()->selectedIndexes();
        for (const QModelIndex &idx : indexes)
        {
            if (idx.isValid() && idx.column() == QuaiModel::IdQuai)
            {
                proxyIndex = idx;
                break;
            }
        }
    }

    if (!proxyIndex.isValid())
    {
        QMessageBox::information(this, tr("Modifier"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (m_table && m_table->selectionModel())
    {
        int selectedIds = 0;
        const QModelIndexList indexes = m_table->selectionModel()->selectedIndexes();
        for (const QModelIndex &idx : indexes)
        {
            if (idx.isValid() && idx.column() == QuaiModel::IdQuai)
                ++selectedIds;
        }
        if (selectedIds > 1)
        {
            QMessageBox::information(this, tr("Modifier"), tr("Sélectionnez une seule ligne (ID Quai)."));
            return;
        }
    }

    const QModelIndex searchIndex = m_sortProxy->mapToSource(proxyIndex);
    const QModelIndex sourceIndex = m_searchProxy->mapToSource(searchIndex);
    const int row = sourceIndex.row();

    if (row < 0 || row >= m_model->rowCount())
        return;

    const Quai current = m_model->items().at(row);
    QuaiFormDialog dlg(QuaiFormDialog::Mode::Update, this);
    dlg.setQuai(current);

    if (dlg.exec() != QDialog::Accepted)
        return;

    const Quai q = dlg.quai();

    QString error;
    if (!m_model->updateQuai(row, q, &error))
    {
        QMessageBox::warning(this, tr("Modifier"), error);
        return;
    }
}

void MainWindow::onDelete()
{
    QModelIndex proxyIndex;
    if (m_table && m_table->selectionModel())
    {
        const QModelIndexList indexes = m_table->selectionModel()->selectedIndexes();
        for (const QModelIndex &idx : indexes)
        {
            if (idx.isValid() && idx.column() == QuaiModel::IdQuai)
            {
                proxyIndex = idx;
                break;
            }
        }
    }

    if (!proxyIndex.isValid())
    {
        QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez une ligne dans le tableau."));
        return;
    }

    if (m_table && m_table->selectionModel())
    {
        int selectedIds = 0;
        const QModelIndexList indexes = m_table->selectionModel()->selectedIndexes();
        for (const QModelIndex &idx : indexes)
        {
            if (idx.isValid() && idx.column() == QuaiModel::IdQuai)
                ++selectedIds;
        }
        if (selectedIds > 1)
        {
            QMessageBox::information(this, tr("Supprimer"), tr("Sélectionnez une seule ligne (ID Quai)."));
            return;
        }
    }

    const QModelIndex searchIndex = m_sortProxy->mapToSource(proxyIndex);
    const QModelIndex sourceIndex = m_searchProxy->mapToSource(searchIndex);
    const int row = sourceIndex.row();

    if (row < 0 || row >= m_model->rowCount())
        return;

    const auto &q = m_model->items().at(row);
    QuaiDeleteDialog dlg(this);
    dlg.setQuaiSummary(q.idQuai, q.matricule);
    if (dlg.exec() != QDialog::Accepted)
        return;

    QString error;
    if (!m_model->removeQuai(row, &error))
    {
        QMessageBox::warning(this, tr("Supprimer"), error.isEmpty() ? tr("Erreur lors de la suppression.") : error);
        return;
    }
}

// ---------------------------------------------------------------------------
// onExportPdf
// ---------------------------------------------------------------------------

void MainWindow::onExportPdf()
{
    const int exportIdx = ui->exportType ? ui->exportType->currentIndex() : 0;

    QString suggestedName = "export.pdf";
    QString title = tr("VISION SIGHT");
    switch (exportIdx)
    {
    case 0: suggestedName = "facture_quai.pdf"; title = tr("Facturation de location de quai"); break;
    case 1: suggestedName = "rapport_journalier_occupation.pdf"; title = tr("Rapport journalier d'occupation"); break;
    case 2: suggestedName = "rapport_mensuel_revenus_par_quai.pdf"; title = tr("Rapport mensuel de revenus par quai"); break;
    case 3: suggestedName = "rapport_anomalies.pdf"; title = tr("Rapport anomalies (retards, dépassements)"); break;
    default: suggestedName = "quais.pdf"; title = tr("VISION SIGHT  Quais"); break;
    }

    const QString filePath = QFileDialog::getSaveFileName(this, tr("Exporter PDF"), suggestedName, tr("PDF (*.pdf)"));
    if (filePath.isEmpty())
        return;

    QString error;
    bool ok = false;
    switch (exportIdx)
    {
    case 0: {
        QModelIndex idx;
        if (m_table && m_table->selectionModel())
        {
            const QModelIndexList indexes = m_table->selectionModel()->selectedIndexes();
            for (const QModelIndex &it : indexes)
            {
                if (it.isValid() && it.column() == QuaiModel::IdQuai)
                {
                    idx = it;
                    break;
                }
            }
            int selectedIds = 0;
            for (const QModelIndex &it : indexes)
            {
                if (it.isValid() && it.column() == QuaiModel::IdQuai)
                    ++selectedIds;
            }
            if (selectedIds > 1)
            {
                QMessageBox::information(this, tr("Exporter PDF"), tr("Pour la facturation, sélectionnez un seul quai (ID Quai)."));
                return;
            }
        }

        if (!idx.isValid())
        {
            QMessageBox::information(this, tr("Exporter PDF"), tr("Pour la facturation, sélectionnez une ligne du tableau (un quai)."));
            return;
        }
        ok = PdfExporter::exportInvoiceToPdf(filePath, title, m_sortProxy, idx.row(), &error);
        break;
    }
    case 1:
        ok = PdfExporter::exportDailyOccupationReportToPdf(filePath, title, m_sortProxy, QDate::currentDate(), &error);
        break;
    case 2: {
        const QDate d = QDate::currentDate();
        ok = PdfExporter::exportMonthlyRevenueByQuaiToPdf(filePath, title, m_sortProxy, d.year(), d.month(), &error);
        break;
    }
    case 3:
        ok = PdfExporter::exportAnomaliesReportToPdf(filePath, title, m_sortProxy, QDateTime::currentDateTime(), &error);
        break;
    default:
        ok = PdfExporter::exportTableToPdf(filePath, tr("VISION SIGHT  Quais"), m_sortProxy, &error);
        break;
    }

    if (!ok)
    {
        QMessageBox::warning(this, tr("Exporter PDF"), error.isEmpty() ? tr("Erreur lors de l'export PDF") : error);
        return;
    }

    QMessageBox::information(this, tr("Exporter PDF"), tr("PDF exporté: %1").arg(filePath));
}
