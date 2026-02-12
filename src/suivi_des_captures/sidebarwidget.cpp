#include "sidebarwidget.h"

#include <QFrame>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QResizeEvent>
#include <QStyle>
#include <QVBoxLayout>

SidebarWidget::SidebarWidget(QWidget *parent) : QWidget(parent) {
    setObjectName("CapturesSidebar");
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);
    // Match the global app sidebar proportions (see Quais/Transactions/Navires)
    setFixedWidth(270);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    setStyleSheet(R"(
        QWidget#CapturesSidebar {
            background: #101c2c;
            border-top-right-radius: 16px;
            border-bottom-right-radius: 16px;
        }

        QFrame#SidebarHighlight {
            background: rgba(57,192,250,22);
            border: 1px solid rgba(57,192,250,90);
            border-radius: 10px;
        }

        QFrame#LogoCircle {
            background: #ffffff;
            border: 2px solid #ffffff;
            border-radius: 48px;
        }

        QLabel#LogoImage {
            background: transparent;
        }

        QLabel#SidebarTitle {
            color: #e6eef6;
            font-size: 16px;
            font-weight: 800;
            margin-top: 0px;
            margin-bottom: 0px;
        }

        QPushButton#SidebarItem {
            color: #e6eef6;
            background: transparent;
            border: 1px solid transparent;
            text-align: left;
            padding-left: 12px;
            font-size: 16px;
            font-weight: 600;
            border-radius: 10px;
            min-height: 42px;
        }

        QPushButton#SidebarItem:hover {
            background: #18304a;
            border-color: #23374e;
        }

        QPushButton#SidebarItem[selected="true"] {
            color: #39c0fa;
            font-weight: 600;
        }
    )");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 18, 18, 18);
    layout->setSpacing(10);

    // Highlight (animated selected item background)
    m_highlight = new QFrame(this);
    m_highlight->setObjectName("SidebarHighlight");
    m_highlight->setAttribute(Qt::WA_StyledBackground, true);
    m_highlight->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_highlight->hide();
    m_highlightAnim = new QPropertyAnimation(m_highlight, "geometry", this);
    m_highlightAnim->setDuration(180);
    m_highlightAnim->setEasingCurve(QEasingCurve::OutCubic);

    // Logo in white circle
    auto *logoCircle = new QFrame(this);
    logoCircle->setObjectName("LogoCircle");
    logoCircle->setAttribute(Qt::WA_StyledBackground, true);
    logoCircle->setFixedSize(96, 96);

    auto *logoCircleLayout = new QVBoxLayout(logoCircle);
    logoCircleLayout->setContentsMargins(0, 0, 0, 0);
    logoCircleLayout->setSpacing(0);

    logoLabel = new QLabel(logoCircle);
    logoLabel->setObjectName("LogoImage");
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setFixedSize(96, 96);
    QPixmap logoPixmap(":/logo_circle.png");
    if (!logoPixmap.isNull())
        logoLabel->setPixmap(logoPixmap.scaled(96, 96, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoCircleLayout->addWidget(logoLabel);

    layout->addWidget(logoCircle, 0, Qt::AlignHCenter);

    titleLabel = new QLabel("VISION SIGHT", this);
    titleLabel->setObjectName("SidebarTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Spacer below brand, like the other modules' sidebars
    layout->addSpacing(12);

    QStringList menuItems = {
        "Gestion Des employés",
        "Gestion Des Navires",
        "Gestion Des Quais",
        "Gestion Des Captures",
        "Stockage Frigorifique",
        "Gestion Des Ventes"
    };

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(true);

    for (int i = 0; i < menuItems.size(); ++i) {
        QPushButton *btn = new QPushButton(menuItems[i], this);
        btn->setObjectName("SidebarItem");
        btn->setProperty("selected", "false");
        btn->setCheckable(true);
        btn->setChecked(false);
        layout->addWidget(btn);
        menuButtons.append(btn);
        buttonGroup->addButton(btn, i);
    }

    layout->addStretch();

    connect(buttonGroup, &QButtonGroup::idClicked, this, [=](int idx) {
        setSelected(idx);
        emit menuClicked(idx);
    });

    // Default selection (captures) will usually be enforced by the host
    setSelected(3);
}

void SidebarWidget::setSelected(int index) {
    if (index < 0 || index >= menuButtons.size())
        return;

    m_selectedIndex = index;
    for (int i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i]->setProperty("selected", i == index ? "true" : "false");
        menuButtons[i]->setChecked(i == index);
        menuButtons[i]->style()->unpolish(menuButtons[i]);
        menuButtons[i]->style()->polish(menuButtons[i]);
    }

    moveHighlightTo(index, /*animate*/ isVisible() && m_highlight->isVisible());
}

void SidebarWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_selectedIndex >= 0)
        moveHighlightTo(m_selectedIndex, /*animate*/ false);
}

QRect SidebarWidget::highlightTargetRect(const QPushButton *btn) const
{
    if (!btn)
        return {};

    QRect r = btn->geometry();
    // Keep highlight aligned with the full button rect
    return r;
}

void SidebarWidget::moveHighlightTo(int index, bool animate)
{
    if (index < 0 || index >= menuButtons.size())
        return;

    const QRect target = highlightTargetRect(menuButtons[index]);
    if (target.isEmpty())
        return;

    if (!m_highlight->isVisible()) {
        m_highlight->setGeometry(target);
        m_highlight->show();
        m_highlight->lower();
        return;
    }

    if (!animate) {
        m_highlightAnim->stop();
        m_highlight->setGeometry(target);
        m_highlight->lower();
        return;
    }

    m_highlightAnim->stop();
    m_highlightAnim->setStartValue(m_highlight->geometry());
    m_highlightAnim->setEndValue(target);
    m_highlightAnim->start();
    m_highlight->lower();
}
