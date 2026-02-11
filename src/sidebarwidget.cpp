#include "sidebarwidget.h"
#include <QStyle>
#include <QMouseEvent>

SidebarWidget::SidebarWidget(QWidget *parent) : QWidget(parent) {
    setFixedWidth(240);
    setStyleSheet(R"(
        SidebarWidget {
            background: #14243A;
            border-top-right-radius: 24px;
            border-bottom-right-radius: 24px;
        }
        QLabel#Logo {
            margin-top: 24px;
            margin-bottom: 8px;
        }
        QLabel#Title {
            color: #FFFFFF;
            font-size: 18px;
            font-weight: bold;
            margin-bottom: 24px;
        }
        QPushButton {
            color: #FFFFFF;
            background: transparent;
            border: none;
            text-align: left;
            padding: 12px 24px;
            font-size: 16px;
            border-radius: 8px;
            margin-bottom: 4px;
        }
        QPushButton:hover {
            background: #1CC6FF;
            color: #14243A;
        }
        QPushButton[selected="true"] {
            background: #1CC6FF;
            color: #14243A;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    logoLabel = new QLabel(this);
    logoLabel->setObjectName("Logo");
    QPixmap logoPixmap(":/resources/logo.png"); // Place your logo in resources
    logoLabel->setPixmap(logoPixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(logoLabel);

    titleLabel = new QLabel("VISION SIGHT", this);
    titleLabel->setObjectName("Title");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    layout->addSpacing(16);

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
        btn->setProperty("selected", i == 1 ? "true" : "false"); // Default: Navires selected
        btn->setCheckable(true);
        btn->setChecked(i == 1);
        layout->addWidget(btn);
        menuButtons.append(btn);
        buttonGroup->addButton(btn, i);
    }

    layout->addStretch();

    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [=](int idx) {
        setSelected(idx);
        emit menuClicked(idx);
    });
}

void SidebarWidget::setSelected(int index) {
    for (int i = 0; i < menuButtons.size(); ++i) {
        menuButtons[i]->setProperty("selected", i == index ? "true" : "false");
        menuButtons[i]->setChecked(i == index);
        menuButtons[i]->style()->unpolish(menuButtons[i]);
        menuButtons[i]->style()->polish(menuButtons[i]);
    }
}
