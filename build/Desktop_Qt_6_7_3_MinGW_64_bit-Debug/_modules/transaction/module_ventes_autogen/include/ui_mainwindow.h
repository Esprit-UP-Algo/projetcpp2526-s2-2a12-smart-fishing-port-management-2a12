/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QLabel *logoLabel;
    QLabel *lblVisionSight;
    QSpacerItem *sidebarTopSpacer;
    QPushButton *btnEmployees;
    QPushButton *btnNavires;
    QPushButton *btnQuais;
    QPushButton *btnCaptures;
    QPushButton *btnStockage;
    QPushButton *btnVentes;
    QFrame *ventesMenu;
    QVBoxLayout *ventesMenuLayout;
    QPushButton *btnVentesAffichage;
    QPushButton *btnVentesStatistique;
    QPushButton *btnVentesAvancees;
    QSpacerItem *sidebarBottomSpacer;
    QWidget *mainContent;
    QVBoxLayout *mainContentLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QLabel *lblTitle;
    QSpacerItem *headerSpacer;
    QPushButton *btnMenu;
    QStackedWidget *stackedWidget;
    QWidget *pageTransactions;
    QVBoxLayout *pageTransOuterLayout;
    QScrollArea *mainScrollArea;
    QWidget *scrollContents;
    QVBoxLayout *contentLayout;
    QFrame *rechercheFrame;
    QVBoxLayout *rechercheFrameLayout;
    QLabel *lblRecherche;
    QHBoxLayout *searchLayout;
    QLineEdit *lineRecherchePecheur;
    QLineEdit *lineRechercheFacture;
    QLineEdit *lineRechercheLot;
    QDateTimeEdit *dateDebut;
    QDateTimeEdit *dateFin;
    QPushButton *btnExportPDF;
    QFrame *triFrame;
    QVBoxLayout *triFrameLayout;
    QLabel *lblTri;
    QHBoxLayout *triRowLayout;
    QLabel *lblTrierPar;
    QComboBox *comboTri;
    QLabel *lblOrdre;
    QComboBox *comboOrdre;
    QSpacerItem *triSpacer;
    QFrame *affichageFrame;
    QVBoxLayout *affichageFrameLayout;
    QLabel *lblAffichage;
    QHBoxLayout *buttonsLayout;
    QPushButton *btnCreer;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QSpacerItem *buttonsSpacer;
    QTableWidget *tableTransactions;
    QFrame *statsFrame;
    QVBoxLayout *statsFrameLayout;
    QHBoxLayout *statsHeaderLayout;
    QLabel *lblStatistiquesVentes;
    QSpacerItem *statsHeaderSpacer;
    QLabel *lblPeriode;
    QComboBox *comboPeriode;
    QHBoxLayout *chartsRow1Layout;
    QVBoxLayout *caJourLayout;
    QFrame *chartCA_jour;
    QVBoxLayout *chartCA_jourInner;
    QLabel *lblChartTitle_jour;
    QLabel *lblChartValue_jour;
    QLabel *lblChartUnit_jour;
    QVBoxLayout *caMoisLayout;
    QFrame *chartCA_mois;
    QVBoxLayout *chartCA_moisInner;
    QLabel *lblChartTitle_mois;
    QLabel *lblChartValue_mois;
    QLabel *lblChartUnit_mois;
    QVBoxLayout *caAnneeLayout;
    QFrame *chartCA_annee;
    QVBoxLayout *chartCA_anneeInner;
    QLabel *lblChartTitle_annee;
    QLabel *lblChartValue_annee;
    QLabel *lblChartUnit_annee;
    QVBoxLayout *caMoyenLayout;
    QFrame *chartCA_moyen;
    QVBoxLayout *chartCA_moyenInner;
    QLabel *lblChartTitle_moyen;
    QLabel *lblChartValue_moyen;
    QLabel *lblChartUnit_moyen;
    QHBoxLayout *chartsRow2Layout;
    QSpacerItem *chartsRow2SpacerL;
    QVBoxLayout *qteLayout;
    QFrame *chartQTE;
    QVBoxLayout *chartQTEInner;
    QLabel *lblChartTitleQTE;
    QLabel *lblChartQTE;
    QLabel *lblChartUnitQTE;
    QVBoxLayout *revLayout;
    QFrame *chartREV;
    QVBoxLayout *chartREVInner;
    QLabel *lblChartTitleREV;
    QLabel *lblChartREV;
    QLabel *lblChartUnitREV;
    QSpacerItem *chartsRow2SpacerR;
    QFrame *optionsFrame;
    QVBoxLayout *optionsFrameLayout;
    QLabel *lblOptionsAvancees;
    QFrame *priceEstimateFrame;
    QVBoxLayout *priceEstimateLayout;
    QLabel *priceEstimateLabel;
    QHBoxLayout *fishPriceSelectLayout;
    QComboBox *fishPriceComboBox;
    QPushButton *btnPrixEstime;
    QFrame *priceResultDisplayFrame;
    QVBoxLayout *priceResultLayout;
    QLabel *priceResultDisplayLabel;
    QLabel *lblDetection;
    QFrame *separator1;
    QFrame *chatFrame;
    QVBoxLayout *chatFrameLayout;
    QTextBrowser *legacyChatDisplay;
    QHBoxLayout *inputLayout;
    QFrame *chatbotFrame;
    QVBoxLayout *chatbotLayout;
    QLabel *chatbotLabel;
    QTextEdit *chatDisplay;
    QHBoxLayout *suggestionsLayout;
    QPushButton *suggestPrice;
    QPushButton *suggestAnomaly;
    QPushButton *suggestStats;
    QPushButton *suggestRevenue;
    QPushButton *suggestAlerts;
    QSpacerItem *suggestSpacer;
    QHBoxLayout *chatInputLayout;
    QLineEdit *chatInput;
    QPushButton *chatSendButton;
    QSpacerItem *contentBottomSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1500, 900);
        MainWindow->setMinimumSize(QSize(1200, 700));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName("centralWidget");
        centralWidget->setStyleSheet(QString::fromUtf8("\n"
"#centralWidget {\n"
"  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #ADD8E6);\n"
"}\n"
"QScrollArea#mainScrollArea,\n"
"QScrollArea#mainScrollArea QWidget#qt_scrollarea_viewport {\n"
"  background: transparent;\n"
"}\n"
"#scrollContents {\n"
"  background: transparent;\n"
"}\n"
"#sidebar {\n"
"  background-color: #0f1922;\n"
"  border: none;\n"
"}\n"
"#logoLabel {\n"
"  background: white;\n"
"  color: #101c2c;\n"
"  border: 2px solid #ffffff;\n"
"  border-radius: 39px;\n"
"}\n"
"#lblVisionSight {\n"
"  color: #d9e3ee;\n"
"  font-size: 16px;\n"
"  font-weight: 800;\n"
"  letter-spacing: 1px;\n"
"}\n"
"#sidebar QPushButton {\n"
"  background: transparent;\n"
"  color: #b8c9dc;\n"
"  border: none;\n"
"  border-radius: 8px;\n"
"  text-align: left;\n"
"  padding-left: 18px;\n"
"  font-size: 13px;\n"
"  font-weight: 500;\n"
"  margin: 0px;\n"
"  min-height: 42px;\n"
"}\n"
"#sidebar QPushButton:hover {\n"
"  background: #132437;\n"
"  color: #39c0fa;\n"
"  border: none;\n"
"  font-w"
                        "eight: 600;\n"
"}\n"
"#sidebar QPushButton[active=\"true\"] {\n"
"  background: #132437;\n"
"  color: #39c0fa;\n"
"  border-left: 3px solid #39c0fa;\n"
"  font-weight: 600;\n"
"}\n"
"#sidebar QPushButton[subitem=\"true\"] {\n"
"  background: transparent;\n"
"  color: #b8c9dc;\n"
"  border: none;\n"
"  border-radius: 8px;\n"
"  text-align: left;\n"
"  padding-left: 34px;\n"
"  font-size: 12px;\n"
"  font-weight: 500;\n"
"  margin: 0px;\n"
"  min-height: 36px;\n"
"}\n"
"#sidebar QPushButton[subitem=\"true\"]:hover {\n"
"  background: #132437;\n"
"  color: #39c0fa;\n"
"}\n"
"#sidebar QPushButton[subitem=\"true\"][active=\"true\"] {\n"
"  color: #39c0fa;\n"
"  font-weight: 600;\n"
"}\n"
"#headerFrame {\n"
"  background: transparent;\n"
"}\n"
"#lblTitle {\n"
"  color: #132437;\n"
"  font-size: 28px;\n"
"  font-weight: bold;\n"
"}\n"
"#btnMenu {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 10px;\n"
"  font-size: 14px;\n"
"  font-weight: 800;\n"
"}\n"
"#"
                        "btnMenu:hover {\n"
"  background-color: #112b41;\n"
"}\n"
"QFrame#transactionsFrame, QFrame#rechercheFrame, QFrame#triFrame,\n"
"QFrame#affichageFrame, QFrame#statsFrame, QFrame#optionsFrame {\n"
"  background-color: #132437;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 16px;\n"
"}\n"
"#lblTransactions, #lblRecherche, #lblTri, #lblAffichage,\n"
"#lblStatistiquesVentes, #lblOptionsAvancees {\n"
"  color: #64e4ff;\n"
"  font-size: 18px;\n"
"  font-weight: 700;\n"
"  text-shadow: 0 2px 4px rgba(0,0,0,0.3);\n"
"}\n"
"#lblDetection, #lblPrevision {\n"
"  color: #9bb0c3;\n"
"  font-size: 13px;\n"
"}\n"
"#btnCreer {\n"
"  background-color: #39c0fa;\n"
"  color: #0c1a29;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  font-size: 14px;\n"
"  font-weight: 800;\n"
"}\n"
"#btnCreer:hover { background-color: #56cdfc; }\n"
"#btnModifier {\n"
"  background-color: #23374e;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2b4561;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  font"
                        "-size: 14px;\n"
"  font-weight: 800;\n"
"}\n"
"#btnModifier:hover { background-color: #2b4561; }\n"
"#btnSupprimer {\n"
"  background-color: #39c0fa;\n"
"  color: #0c1a29;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"}\n"
"#btnSupprimer:hover { background-color: #56cdfc; }\n"
"#btnExportPDF {\n"
"  background-color: #1e8de0;\n"
"  color: white;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 14px;\n"
"  font-size: 13px;\n"
"  font-weight: 700;\n"
"}\n"
"#btnExportPDF:hover { background-color: #2a99ec; }\n"
"QLineEdit {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  min-height: 22px;\n"
"  font-size: 11px;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QDateTimeEdit {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  paddin"
                        "g-right: 28px;\n"
"  min-height: 22px;\n"
"  font-size: 11px;\n"
"}\n"
"QDateTimeEdit::drop-down { border: 0px; width: 20px; }\n"
"QComboBox {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  min-height: 22px;\n"
"  font-size: 11px;\n"
"}\n"
"QComboBox::drop-down { border: 0px; width: 20px; }\n"
"QComboBox QAbstractItemView {\n"
"  background-color: #132437;\n"
"  color: #e6eef6;\n"
"  selection-background-color: #1e8de0;\n"
"  border: 1px solid #23374e;\n"
"}\n"
"QTableWidget {\n"
"  background: transparent;\n"
"  alternate-background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 0px;\n"
"  gridline-color: transparent;\n"
"  font-size: 13px;\n"
"  selection-background-color: #39c0fa;\n"
"  selection-color: #0c1a29;\n"
"}\n"
"QHeaderView::section {\n"
"  background-color: #101c2c;\n"
"  color: #9bb0c3;\n"
"  border: 0px;\n"
"  padding: 8px 10px;\n"
"  font-weight: 700;\n"
"  font-size: 13px;\n"
"}\n"
"QTableWi"
                        "dget::item {\n"
"  padding: 6px 10px;\n"
"  border-radius: 6px;\n"
"}\n"
"QTableWidget::item:selected { background-color: #39c0fa; color: #0c1a29; }\n"
"#chartCA_jour, #chartCA_mois, #chartCA_annee, #chartCA_moyen {\n"
"  background-color: #101c2c;\n"
"  border: 10px solid #2a3f55;\n"
"  border-radius: 75px;\n"
"  min-width: 150px; min-height: 150px;\n"
"  max-width: 150px; max-height: 150px;\n"
"  box-shadow: 0 8px 16px rgba(57,192,250,0.15), inset 0 1px 2px rgba(255,255,255,0.1);\n"
"}\n"
"#chartQTE, #chartREV {\n"
"  background-color: #101c2c;\n"
"  border: 10px solid #39c0fa;\n"
"  border-radius: 75px;\n"
"  min-width: 150px; min-height: 150px;\n"
"  max-width: 150px; max-height: 150px;\n"
"  box-shadow: 0 12px 24px rgba(57,192,250,0.25), 0 0 20px rgba(57,192,250,0.15);\n"
"}\n"
"#lblChartTitle_jour, #lblChartTitle_mois, #lblChartTitle_annee, #lblChartTitle_moyen,\n"
"#lblChartTitleQTE, #lblChartTitleREV {\n"
"  color: #64e4ff;\n"
"  font-size: 11px;\n"
"  font-weight: 700;\n"
"  background: transparent;\n"
""
                        "  border: none;\n"
"}\n"
"#lblChartValue_jour, #lblChartValue_mois, #lblChartValue_annee, #lblChartValue_moyen,\n"
"#lblChartQTE, #lblChartREV {\n"
"  color: #ffffff;\n"
"  font-size: 20px;\n"
"  font-weight: bold;\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"#lblChartUnit_jour, #lblChartUnit_mois, #lblChartUnit_annee, #lblChartUnit_moyen,\n"
"#lblChartUnitQTE, #lblChartUnitREV {\n"
"  color: #9fc5d8;\n"
"  font-size: 10px;\n"
"  font-weight: 600;\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"#lblPeriode {\n"
"  color: #ffffff;\n"
"  font-size: 13px;\n"
"  font-weight: 600;\n"
"}\n"
"#comboPeriode {\n"
"  background-color: #0d1b2a;\n"
"  color: #ffffff;\n"
"  border: 2px solid #39c0fa;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  min-height: 22px;\n"
"  font-size: 13px;\n"
"  font-weight: 700;\n"
"}\n"
"#lblTrierPar, #lblOrdre {\n"
"  color: #9bb0c3;\n"
"  font-size: 13px;\n"
"}\n"
"QScrollArea {\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"QScrollBar:"
                        "vertical {\n"
"  background: #101c2c;\n"
"  width: 8px;\n"
"  border-radius: 4px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"  background: #23374e;\n"
"  border-radius: 4px;\n"
"  min-height: 30px;\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }\n"
"#separator1, #separator2 {\n"
"  background-color: #23374e;\n"
"  border: none;\n"
"  max-height: 1px;\n"
"  min-height: 1px;\n"
"}\n"
"    "));
        mainLayout = new QHBoxLayout(centralWidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(centralWidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(270, 0));
        sidebar->setMaximumSize(QSize(270, 16777215));
        sidebar->setFrameShape(QFrame::NoFrame);
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(4);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(0, 20, 0, 20);
        logoLabel = new QLabel(sidebar);
        logoLabel->setObjectName("logoLabel");
        logoLabel->setMinimumSize(QSize(78, 78));
        logoLabel->setMaximumSize(QSize(78, 78));
        logoLabel->setPixmap(QPixmap(QString::fromUtf8(":/vision_sight.png")));
        logoLabel->setScaledContents(true);

        sidebarLayout->addWidget(logoLabel, 0, Qt::AlignHCenter);

        lblVisionSight = new QLabel(sidebar);
        lblVisionSight->setObjectName("lblVisionSight");
        lblVisionSight->setAlignment(Qt::AlignCenter);

        sidebarLayout->addWidget(lblVisionSight, 0, Qt::AlignHCenter);

        sidebarTopSpacer = new QSpacerItem(20, 24, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(sidebarTopSpacer);

        btnEmployees = new QPushButton(sidebar);
        btnEmployees->setObjectName("btnEmployees");
        btnEmployees->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnEmployees);

        btnNavires = new QPushButton(sidebar);
        btnNavires->setObjectName("btnNavires");
        btnNavires->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnNavires);

        btnQuais = new QPushButton(sidebar);
        btnQuais->setObjectName("btnQuais");
        btnQuais->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnQuais);

        btnCaptures = new QPushButton(sidebar);
        btnCaptures->setObjectName("btnCaptures");
        btnCaptures->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnCaptures);

        btnStockage = new QPushButton(sidebar);
        btnStockage->setObjectName("btnStockage");
        btnStockage->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnStockage);

        btnVentes = new QPushButton(sidebar);
        btnVentes->setObjectName("btnVentes");
        btnVentes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(btnVentes);

        ventesMenu = new QFrame(sidebar);
        ventesMenu->setObjectName("ventesMenu");
        ventesMenu->setVisible(false);
        ventesMenu->setMinimumSize(QSize(0, 0));
        ventesMenu->setMaximumSize(QSize(16777215, 0));
        ventesMenu->setFrameShape(QFrame::NoFrame);
        ventesMenuLayout = new QVBoxLayout(ventesMenu);
        ventesMenuLayout->setSpacing(2);
        ventesMenuLayout->setObjectName("ventesMenuLayout");
        ventesMenuLayout->setContentsMargins(0, 0, 0, 0);
        btnVentesAffichage = new QPushButton(ventesMenu);
        btnVentesAffichage->setObjectName("btnVentesAffichage");
        btnVentesAffichage->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        ventesMenuLayout->addWidget(btnVentesAffichage);

        btnVentesStatistique = new QPushButton(ventesMenu);
        btnVentesStatistique->setObjectName("btnVentesStatistique");
        btnVentesStatistique->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        ventesMenuLayout->addWidget(btnVentesStatistique);

        btnVentesAvancees = new QPushButton(ventesMenu);
        btnVentesAvancees->setObjectName("btnVentesAvancees");
        btnVentesAvancees->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        ventesMenuLayout->addWidget(btnVentesAvancees);


        sidebarLayout->addWidget(ventesMenu);

        sidebarBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarBottomSpacer);


        mainLayout->addWidget(sidebar);

        mainContent = new QWidget(centralWidget);
        mainContent->setObjectName("mainContent");
        mainContentLayout = new QVBoxLayout(mainContent);
        mainContentLayout->setSpacing(0);
        mainContentLayout->setObjectName("mainContentLayout");
        mainContentLayout->setContentsMargins(0, 0, 0, 0);
        headerFrame = new QFrame(mainContent);
        headerFrame->setObjectName("headerFrame");
        headerFrame->setMinimumSize(QSize(0, 60));
        headerFrame->setMaximumSize(QSize(16777215, 60));
        headerFrame->setFrameShape(QFrame::NoFrame);
        headerLayout = new QHBoxLayout(headerFrame);
        headerLayout->setObjectName("headerLayout");
        headerLayout->setContentsMargins(24, 10, 24, 10);
        lblTitle = new QLabel(headerFrame);
        lblTitle->setObjectName("lblTitle");

        headerLayout->addWidget(lblTitle);

        headerSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        btnMenu = new QPushButton(headerFrame);
        btnMenu->setObjectName("btnMenu");
        btnMenu->setMinimumSize(QSize(40, 36));
        btnMenu->setMaximumSize(QSize(40, 36));
        btnMenu->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        headerLayout->addWidget(btnMenu);


        mainContentLayout->addWidget(headerFrame);

        stackedWidget = new QStackedWidget(mainContent);
        stackedWidget->setObjectName("stackedWidget");
        pageTransactions = new QWidget();
        pageTransactions->setObjectName("pageTransactions");
        pageTransOuterLayout = new QVBoxLayout(pageTransactions);
        pageTransOuterLayout->setSpacing(0);
        pageTransOuterLayout->setObjectName("pageTransOuterLayout");
        pageTransOuterLayout->setContentsMargins(0, 0, 0, 0);
        mainScrollArea = new QScrollArea(pageTransactions);
        mainScrollArea->setObjectName("mainScrollArea");
        mainScrollArea->setWidgetResizable(true);
        mainScrollArea->setFrameShape(QFrame::NoFrame);
        scrollContents = new QWidget();
        scrollContents->setObjectName("scrollContents");
        scrollContents->setGeometry(QRect(0, 0, 1260, 2600));
        contentLayout = new QVBoxLayout(scrollContents);
        contentLayout->setSpacing(14);
        contentLayout->setObjectName("contentLayout");
        contentLayout->setContentsMargins(16, 14, 16, 14);
        rechercheFrame = new QFrame(scrollContents);
        rechercheFrame->setObjectName("rechercheFrame");
        rechercheFrame->setFrameShape(QFrame::StyledPanel);
        rechercheFrameLayout = new QVBoxLayout(rechercheFrame);
        rechercheFrameLayout->setSpacing(12);
        rechercheFrameLayout->setObjectName("rechercheFrameLayout");
        rechercheFrameLayout->setContentsMargins(20, 14, 20, 16);
        lblRecherche = new QLabel(rechercheFrame);
        lblRecherche->setObjectName("lblRecherche");

        rechercheFrameLayout->addWidget(lblRecherche);

        searchLayout = new QHBoxLayout();
        searchLayout->setSpacing(10);
        searchLayout->setObjectName("searchLayout");
        lineRecherchePecheur = new QLineEdit(rechercheFrame);
        lineRecherchePecheur->setObjectName("lineRecherchePecheur");
        lineRecherchePecheur->setMinimumSize(QSize(0, 36));

        searchLayout->addWidget(lineRecherchePecheur);

        lineRechercheFacture = new QLineEdit(rechercheFrame);
        lineRechercheFacture->setObjectName("lineRechercheFacture");
        lineRechercheFacture->setMinimumSize(QSize(0, 36));

        searchLayout->addWidget(lineRechercheFacture);

        lineRechercheLot = new QLineEdit(rechercheFrame);
        lineRechercheLot->setObjectName("lineRechercheLot");
        lineRechercheLot->setMinimumSize(QSize(0, 36));

        searchLayout->addWidget(lineRechercheLot);

        dateDebut = new QDateTimeEdit(rechercheFrame);
        dateDebut->setObjectName("dateDebut");
        dateDebut->setMinimumSize(QSize(160, 36));
        dateDebut->setCalendarPopup(true);

        searchLayout->addWidget(dateDebut);

        dateFin = new QDateTimeEdit(rechercheFrame);
        dateFin->setObjectName("dateFin");
        dateFin->setMinimumSize(QSize(160, 36));
        dateFin->setCalendarPopup(true);

        searchLayout->addWidget(dateFin);

        btnExportPDF = new QPushButton(rechercheFrame);
        btnExportPDF->setObjectName("btnExportPDF");
        btnExportPDF->setMinimumSize(QSize(120, 36));
        btnExportPDF->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        searchLayout->addWidget(btnExportPDF);


        rechercheFrameLayout->addLayout(searchLayout);


        contentLayout->addWidget(rechercheFrame);

        triFrame = new QFrame(scrollContents);
        triFrame->setObjectName("triFrame");
        triFrame->setFrameShape(QFrame::StyledPanel);
        triFrameLayout = new QVBoxLayout(triFrame);
        triFrameLayout->setSpacing(12);
        triFrameLayout->setObjectName("triFrameLayout");
        triFrameLayout->setContentsMargins(20, 14, 20, 16);
        lblTri = new QLabel(triFrame);
        lblTri->setObjectName("lblTri");

        triFrameLayout->addWidget(lblTri);

        triRowLayout = new QHBoxLayout();
        triRowLayout->setSpacing(10);
        triRowLayout->setObjectName("triRowLayout");
        lblTrierPar = new QLabel(triFrame);
        lblTrierPar->setObjectName("lblTrierPar");

        triRowLayout->addWidget(lblTrierPar);

        comboTri = new QComboBox(triFrame);
        comboTri->addItem(QString());
        comboTri->addItem(QString());
        comboTri->addItem(QString());
        comboTri->addItem(QString());
        comboTri->setObjectName("comboTri");
        comboTri->setMinimumSize(QSize(160, 36));

        triRowLayout->addWidget(comboTri);

        lblOrdre = new QLabel(triFrame);
        lblOrdre->setObjectName("lblOrdre");

        triRowLayout->addWidget(lblOrdre);

        comboOrdre = new QComboBox(triFrame);
        comboOrdre->addItem(QString());
        comboOrdre->addItem(QString());
        comboOrdre->setObjectName("comboOrdre");
        comboOrdre->setMinimumSize(QSize(150, 36));

        triRowLayout->addWidget(comboOrdre);

        triSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        triRowLayout->addItem(triSpacer);


        triFrameLayout->addLayout(triRowLayout);


        contentLayout->addWidget(triFrame);

        affichageFrame = new QFrame(scrollContents);
        affichageFrame->setObjectName("affichageFrame");
        affichageFrame->setFrameShape(QFrame::StyledPanel);
        affichageFrameLayout = new QVBoxLayout(affichageFrame);
        affichageFrameLayout->setSpacing(12);
        affichageFrameLayout->setObjectName("affichageFrameLayout");
        affichageFrameLayout->setContentsMargins(20, 14, 20, 20);
        lblAffichage = new QLabel(affichageFrame);
        lblAffichage->setObjectName("lblAffichage");

        affichageFrameLayout->addWidget(lblAffichage);

        buttonsLayout = new QHBoxLayout();
        buttonsLayout->setSpacing(12);
        buttonsLayout->setObjectName("buttonsLayout");
        btnCreer = new QPushButton(affichageFrame);
        btnCreer->setObjectName("btnCreer");
        btnCreer->setMinimumSize(QSize(100, 36));
        btnCreer->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnCreer);

        btnModifier = new QPushButton(affichageFrame);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setMinimumSize(QSize(100, 36));
        btnModifier->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnModifier);

        btnSupprimer = new QPushButton(affichageFrame);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setMinimumSize(QSize(100, 36));
        btnSupprimer->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnSupprimer);

        buttonsSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(buttonsSpacer);


        affichageFrameLayout->addLayout(buttonsLayout);

        tableTransactions = new QTableWidget(affichageFrame);
        if (tableTransactions->columnCount() < 10)
            tableTransactions->setColumnCount(10);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableTransactions->setHorizontalHeaderItem(9, __qtablewidgetitem9);
        tableTransactions->setObjectName("tableTransactions");
        tableTransactions->setMinimumSize(QSize(0, 300));
        tableTransactions->setAlternatingRowColors(true);
        tableTransactions->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableTransactions->setSortingEnabled(true);
        tableTransactions->horizontalHeader()->setStretchLastSection(true);
        tableTransactions->verticalHeader()->setVisible(false);

        affichageFrameLayout->addWidget(tableTransactions);


        contentLayout->addWidget(affichageFrame);

        statsFrame = new QFrame(scrollContents);
        statsFrame->setObjectName("statsFrame");
        statsFrame->setFrameShape(QFrame::StyledPanel);
        statsFrameLayout = new QVBoxLayout(statsFrame);
        statsFrameLayout->setSpacing(24);
        statsFrameLayout->setObjectName("statsFrameLayout");
        statsFrameLayout->setContentsMargins(20, 16, 20, 24);
        statsHeaderLayout = new QHBoxLayout();
        statsHeaderLayout->setObjectName("statsHeaderLayout");
        lblStatistiquesVentes = new QLabel(statsFrame);
        lblStatistiquesVentes->setObjectName("lblStatistiquesVentes");

        statsHeaderLayout->addWidget(lblStatistiquesVentes);

        statsHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statsHeaderLayout->addItem(statsHeaderSpacer);

        lblPeriode = new QLabel(statsFrame);
        lblPeriode->setObjectName("lblPeriode");

        statsHeaderLayout->addWidget(lblPeriode);

        comboPeriode = new QComboBox(statsFrame);
        comboPeriode->addItem(QString());
        comboPeriode->addItem(QString());
        comboPeriode->addItem(QString());
        comboPeriode->addItem(QString());
        comboPeriode->setObjectName("comboPeriode");

        statsHeaderLayout->addWidget(comboPeriode);


        statsFrameLayout->addLayout(statsHeaderLayout);

        chartsRow1Layout = new QHBoxLayout();
        chartsRow1Layout->setSpacing(20);
        chartsRow1Layout->setObjectName("chartsRow1Layout");
        caJourLayout = new QVBoxLayout();
        caJourLayout->setObjectName("caJourLayout");
        chartCA_jour = new QFrame(statsFrame);
        chartCA_jour->setObjectName("chartCA_jour");
        chartCA_jour->setFrameShape(QFrame::NoFrame);
        chartCA_jourInner = new QVBoxLayout(chartCA_jour);
        chartCA_jourInner->setSpacing(2);
        chartCA_jourInner->setObjectName("chartCA_jourInner");
        lblChartTitle_jour = new QLabel(chartCA_jour);
        lblChartTitle_jour->setObjectName("lblChartTitle_jour");
        lblChartTitle_jour->setAlignment(Qt::AlignCenter);

        chartCA_jourInner->addWidget(lblChartTitle_jour, 0, Qt::AlignCenter);

        lblChartValue_jour = new QLabel(chartCA_jour);
        lblChartValue_jour->setObjectName("lblChartValue_jour");
        lblChartValue_jour->setAlignment(Qt::AlignCenter);

        chartCA_jourInner->addWidget(lblChartValue_jour, 0, Qt::AlignCenter);

        lblChartUnit_jour = new QLabel(chartCA_jour);
        lblChartUnit_jour->setObjectName("lblChartUnit_jour");
        lblChartUnit_jour->setAlignment(Qt::AlignCenter);

        chartCA_jourInner->addWidget(lblChartUnit_jour, 0, Qt::AlignCenter);


        caJourLayout->addWidget(chartCA_jour, 0, Qt::AlignHCenter);


        chartsRow1Layout->addLayout(caJourLayout);

        caMoisLayout = new QVBoxLayout();
        caMoisLayout->setObjectName("caMoisLayout");
        chartCA_mois = new QFrame(statsFrame);
        chartCA_mois->setObjectName("chartCA_mois");
        chartCA_mois->setFrameShape(QFrame::NoFrame);
        chartCA_moisInner = new QVBoxLayout(chartCA_mois);
        chartCA_moisInner->setSpacing(2);
        chartCA_moisInner->setObjectName("chartCA_moisInner");
        lblChartTitle_mois = new QLabel(chartCA_mois);
        lblChartTitle_mois->setObjectName("lblChartTitle_mois");
        lblChartTitle_mois->setAlignment(Qt::AlignCenter);

        chartCA_moisInner->addWidget(lblChartTitle_mois, 0, Qt::AlignCenter);

        lblChartValue_mois = new QLabel(chartCA_mois);
        lblChartValue_mois->setObjectName("lblChartValue_mois");
        lblChartValue_mois->setAlignment(Qt::AlignCenter);

        chartCA_moisInner->addWidget(lblChartValue_mois, 0, Qt::AlignCenter);

        lblChartUnit_mois = new QLabel(chartCA_mois);
        lblChartUnit_mois->setObjectName("lblChartUnit_mois");
        lblChartUnit_mois->setAlignment(Qt::AlignCenter);

        chartCA_moisInner->addWidget(lblChartUnit_mois, 0, Qt::AlignCenter);


        caMoisLayout->addWidget(chartCA_mois, 0, Qt::AlignHCenter);


        chartsRow1Layout->addLayout(caMoisLayout);

        caAnneeLayout = new QVBoxLayout();
        caAnneeLayout->setObjectName("caAnneeLayout");
        chartCA_annee = new QFrame(statsFrame);
        chartCA_annee->setObjectName("chartCA_annee");
        chartCA_annee->setFrameShape(QFrame::NoFrame);
        chartCA_anneeInner = new QVBoxLayout(chartCA_annee);
        chartCA_anneeInner->setSpacing(2);
        chartCA_anneeInner->setObjectName("chartCA_anneeInner");
        lblChartTitle_annee = new QLabel(chartCA_annee);
        lblChartTitle_annee->setObjectName("lblChartTitle_annee");
        lblChartTitle_annee->setAlignment(Qt::AlignCenter);

        chartCA_anneeInner->addWidget(lblChartTitle_annee, 0, Qt::AlignCenter);

        lblChartValue_annee = new QLabel(chartCA_annee);
        lblChartValue_annee->setObjectName("lblChartValue_annee");
        lblChartValue_annee->setAlignment(Qt::AlignCenter);

        chartCA_anneeInner->addWidget(lblChartValue_annee, 0, Qt::AlignCenter);

        lblChartUnit_annee = new QLabel(chartCA_annee);
        lblChartUnit_annee->setObjectName("lblChartUnit_annee");
        lblChartUnit_annee->setAlignment(Qt::AlignCenter);

        chartCA_anneeInner->addWidget(lblChartUnit_annee, 0, Qt::AlignCenter);


        caAnneeLayout->addWidget(chartCA_annee, 0, Qt::AlignHCenter);


        chartsRow1Layout->addLayout(caAnneeLayout);

        caMoyenLayout = new QVBoxLayout();
        caMoyenLayout->setObjectName("caMoyenLayout");
        chartCA_moyen = new QFrame(statsFrame);
        chartCA_moyen->setObjectName("chartCA_moyen");
        chartCA_moyen->setFrameShape(QFrame::NoFrame);
        chartCA_moyenInner = new QVBoxLayout(chartCA_moyen);
        chartCA_moyenInner->setSpacing(2);
        chartCA_moyenInner->setObjectName("chartCA_moyenInner");
        lblChartTitle_moyen = new QLabel(chartCA_moyen);
        lblChartTitle_moyen->setObjectName("lblChartTitle_moyen");
        lblChartTitle_moyen->setAlignment(Qt::AlignCenter);

        chartCA_moyenInner->addWidget(lblChartTitle_moyen, 0, Qt::AlignCenter);

        lblChartValue_moyen = new QLabel(chartCA_moyen);
        lblChartValue_moyen->setObjectName("lblChartValue_moyen");
        lblChartValue_moyen->setAlignment(Qt::AlignCenter);

        chartCA_moyenInner->addWidget(lblChartValue_moyen, 0, Qt::AlignCenter);

        lblChartUnit_moyen = new QLabel(chartCA_moyen);
        lblChartUnit_moyen->setObjectName("lblChartUnit_moyen");
        lblChartUnit_moyen->setAlignment(Qt::AlignCenter);

        chartCA_moyenInner->addWidget(lblChartUnit_moyen, 0, Qt::AlignCenter);


        caMoyenLayout->addWidget(chartCA_moyen, 0, Qt::AlignHCenter);


        chartsRow1Layout->addLayout(caMoyenLayout);


        statsFrameLayout->addLayout(chartsRow1Layout);

        chartsRow2Layout = new QHBoxLayout();
        chartsRow2Layout->setSpacing(40);
        chartsRow2Layout->setObjectName("chartsRow2Layout");
        chartsRow2SpacerL = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        chartsRow2Layout->addItem(chartsRow2SpacerL);

        qteLayout = new QVBoxLayout();
        qteLayout->setObjectName("qteLayout");
        chartQTE = new QFrame(statsFrame);
        chartQTE->setObjectName("chartQTE");
        chartQTE->setFrameShape(QFrame::NoFrame);
        chartQTEInner = new QVBoxLayout(chartQTE);
        chartQTEInner->setSpacing(2);
        chartQTEInner->setObjectName("chartQTEInner");
        lblChartTitleQTE = new QLabel(chartQTE);
        lblChartTitleQTE->setObjectName("lblChartTitleQTE");
        lblChartTitleQTE->setAlignment(Qt::AlignCenter);

        chartQTEInner->addWidget(lblChartTitleQTE, 0, Qt::AlignCenter);

        lblChartQTE = new QLabel(chartQTE);
        lblChartQTE->setObjectName("lblChartQTE");
        lblChartQTE->setAlignment(Qt::AlignCenter);

        chartQTEInner->addWidget(lblChartQTE, 0, Qt::AlignCenter);

        lblChartUnitQTE = new QLabel(chartQTE);
        lblChartUnitQTE->setObjectName("lblChartUnitQTE");
        lblChartUnitQTE->setAlignment(Qt::AlignCenter);

        chartQTEInner->addWidget(lblChartUnitQTE, 0, Qt::AlignCenter);


        qteLayout->addWidget(chartQTE, 0, Qt::AlignHCenter);


        chartsRow2Layout->addLayout(qteLayout);

        revLayout = new QVBoxLayout();
        revLayout->setObjectName("revLayout");
        chartREV = new QFrame(statsFrame);
        chartREV->setObjectName("chartREV");
        chartREV->setFrameShape(QFrame::NoFrame);
        chartREVInner = new QVBoxLayout(chartREV);
        chartREVInner->setSpacing(2);
        chartREVInner->setObjectName("chartREVInner");
        lblChartTitleREV = new QLabel(chartREV);
        lblChartTitleREV->setObjectName("lblChartTitleREV");
        lblChartTitleREV->setAlignment(Qt::AlignCenter);

        chartREVInner->addWidget(lblChartTitleREV, 0, Qt::AlignCenter);

        lblChartREV = new QLabel(chartREV);
        lblChartREV->setObjectName("lblChartREV");
        lblChartREV->setAlignment(Qt::AlignCenter);

        chartREVInner->addWidget(lblChartREV, 0, Qt::AlignCenter);

        lblChartUnitREV = new QLabel(chartREV);
        lblChartUnitREV->setObjectName("lblChartUnitREV");
        lblChartUnitREV->setAlignment(Qt::AlignCenter);

        chartREVInner->addWidget(lblChartUnitREV, 0, Qt::AlignCenter);


        revLayout->addWidget(chartREV, 0, Qt::AlignHCenter);


        chartsRow2Layout->addLayout(revLayout);

        chartsRow2SpacerR = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        chartsRow2Layout->addItem(chartsRow2SpacerR);


        statsFrameLayout->addLayout(chartsRow2Layout);


        contentLayout->addWidget(statsFrame);

        optionsFrame = new QFrame(scrollContents);
        optionsFrame->setObjectName("optionsFrame");
        optionsFrame->setFrameShape(QFrame::StyledPanel);
        optionsFrameLayout = new QVBoxLayout(optionsFrame);
        optionsFrameLayout->setSpacing(14);
        optionsFrameLayout->setObjectName("optionsFrameLayout");
        optionsFrameLayout->setContentsMargins(20, 16, 20, 20);
        lblOptionsAvancees = new QLabel(optionsFrame);
        lblOptionsAvancees->setObjectName("lblOptionsAvancees");

        optionsFrameLayout->addWidget(lblOptionsAvancees);

        priceEstimateFrame = new QFrame(optionsFrame);
        priceEstimateFrame->setObjectName("priceEstimateFrame");
        priceEstimateFrame->setStyleSheet(QString::fromUtf8("\n"
"QFrame#priceEstimateFrame {\n"
"  border: 2px solid #39c0fa;\n"
"  border-radius: 8px;\n"
"  padding: 16px;\n"
"  background-color: #101c2c;\n"
"}\n"
"                     "));
        priceEstimateLayout = new QVBoxLayout(priceEstimateFrame);
        priceEstimateLayout->setSpacing(12);
        priceEstimateLayout->setObjectName("priceEstimateLayout");
        priceEstimateLabel = new QLabel(priceEstimateFrame);
        priceEstimateLabel->setObjectName("priceEstimateLabel");
        priceEstimateLabel->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"  color: #39c0fa;\n"
"  font-size: 14px;\n"
"  font-weight: bold;\n"
"}\n"
"                        "));

        priceEstimateLayout->addWidget(priceEstimateLabel);

        fishPriceSelectLayout = new QHBoxLayout();
        fishPriceSelectLayout->setSpacing(8);
        fishPriceSelectLayout->setObjectName("fishPriceSelectLayout");
        fishPriceComboBox = new QComboBox(priceEstimateFrame);
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->addItem(QString());
        fishPriceComboBox->setObjectName("fishPriceComboBox");
        fishPriceComboBox->setMinimumHeight(40);
        fishPriceComboBox->setStyleSheet(QString::fromUtf8("\n"
"QComboBox {\n"
"  background-color: #0d1b2a;\n"
"  color: #39c0fa;\n"
"  border: 1px solid #39c0fa;\n"
"  border-radius: 6px;\n"
"  padding: 6px 10px;\n"
"  font-size: 12px;\n"
"  font-weight: 500;\n"
"}\n"
"QComboBox:hover {\n"
"  border-color: #56cdfc;\n"
"  background-color: #132437;\n"
"}\n"
"QComboBox:focus {\n"
"  border-color: #56cdfc;\n"
"  background-color: #132437;\n"
"}\n"
"QComboBox::drop-down {\n"
"  border-left: 1px solid #39c0fa;\n"
"  background-color: #0d1b2a;\n"
"  width: 30px;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"  background-color: #132437;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #39c0fa;\n"
"  selection-background-color: #39c0fa;\n"
"  selection-color: #0c1a29;\n"
"  padding: 4px 0px;\n"
"}\n"
"                          "));

        fishPriceSelectLayout->addWidget(fishPriceComboBox);

        btnPrixEstime = new QPushButton(priceEstimateFrame);
        btnPrixEstime->setObjectName("btnPrixEstime");
        btnPrixEstime->setMinimumHeight(40);
        btnPrixEstime->setMaximumWidth(140);
        btnPrixEstime->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1e90ff;\n"
"  color: #ffffff;\n"
"  border: 2px solid #0033cc;\n"
"  border-radius: 6px;\n"
"  padding: 8px 16px;\n"
"  font-size: 12px;\n"
"  font-weight: 600;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #4169e1;\n"
"  border-color: #1e90ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0033cc;\n"
"  border-color: #ffffff;\n"
"}\n"
"                          "));

        fishPriceSelectLayout->addWidget(btnPrixEstime);


        priceEstimateLayout->addLayout(fishPriceSelectLayout);

        priceResultDisplayFrame = new QFrame(priceEstimateFrame);
        priceResultDisplayFrame->setObjectName("priceResultDisplayFrame");
        priceResultDisplayFrame->setFrameShape(QFrame::StyledPanel);
        priceResultDisplayFrame->setStyleSheet(QString::fromUtf8("\n"
"QFrame {\n"
"  background-color: #ffffff;\n"
"  border: 1px solid #1e90ff;\n"
"  border-radius: 6px;\n"
"  padding: 12px;\n"
"}\n"
"                        "));
        priceResultLayout = new QVBoxLayout(priceResultDisplayFrame);
        priceResultLayout->setObjectName("priceResultLayout");
        priceResultLayout->setContentsMargins(0, 0, 0, 0);
        priceResultDisplayLabel = new QLabel(priceResultDisplayFrame);
        priceResultDisplayLabel->setObjectName("priceResultDisplayLabel");
        priceResultDisplayLabel->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"  color: #0033cc;\n"
"  font-size: 12px;\n"
"  font-weight: 500;\n"
"  padding: 8px;\n"
"}\n"
"                           "));
        priceResultDisplayLabel->setAlignment(Qt::AlignCenter);
        priceResultDisplayLabel->setMinimumHeight(60);

        priceResultLayout->addWidget(priceResultDisplayLabel);


        priceEstimateLayout->addWidget(priceResultDisplayFrame);


        optionsFrameLayout->addWidget(priceEstimateFrame);

        lblDetection = new QLabel(optionsFrame);
        lblDetection->setObjectName("lblDetection");

        optionsFrameLayout->addWidget(lblDetection);

        separator1 = new QFrame(optionsFrame);
        separator1->setObjectName("separator1");
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);

        optionsFrameLayout->addWidget(separator1);

        chatFrame = new QFrame(optionsFrame);
        chatFrame->setObjectName("chatFrame");
        chatFrame->setMinimumSize(QSize(0, 300));
        chatFrame->setFrameShape(QFrame::StyledPanel);
        chatFrame->setStyleSheet(QString::fromUtf8("\n"
"QFrame#chatFrame {\n"
"  background-color: transparent;\n"
"  border: none;\n"
"}\n"
"                     "));
        chatFrameLayout = new QVBoxLayout(chatFrame);
        chatFrameLayout->setSpacing(8);
        chatFrameLayout->setObjectName("chatFrameLayout");
        chatFrameLayout->setContentsMargins(0, 0, 0, 0);
        legacyChatDisplay = new QTextBrowser(chatFrame);
        legacyChatDisplay->setObjectName("legacyChatDisplay");
        legacyChatDisplay->setMinimumSize(QSize(0, 200));
        legacyChatDisplay->setStyleSheet(QString::fromUtf8("\n"
"QTextBrowser {\n"
"  background-color: transparent;\n"
"  color: #e6eef6;\n"
"  border: none;\n"
"  border-radius: 8px;\n"
"  padding: 12px;\n"
"  font-size: 13px;\n"
"}\n"
"                        "));

        chatFrameLayout->addWidget(legacyChatDisplay);

        inputLayout = new QHBoxLayout();
        inputLayout->setSpacing(8);
        inputLayout->setObjectName("inputLayout");

        chatFrameLayout->addLayout(inputLayout);


        optionsFrameLayout->addWidget(chatFrame);

        chatbotFrame = new QFrame(optionsFrame);
        chatbotFrame->setObjectName("chatbotFrame");
        chatbotFrame->setStyleSheet(QString::fromUtf8("\n"
"QFrame#chatbotFrame {\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 8px;\n"
"  padding: 16px;\n"
"}\n"
"                     "));
        chatbotLayout = new QVBoxLayout(chatbotFrame);
        chatbotLayout->setSpacing(12);
        chatbotLayout->setObjectName("chatbotLayout");
        chatbotLabel = new QLabel(chatbotFrame);
        chatbotLabel->setObjectName("chatbotLabel");
        chatbotLabel->setStyleSheet(QString::fromUtf8("\n"
"QLabel {\n"
"  color: #00bcd4;\n"
"  font-size: 14px;\n"
"  font-weight: bold;\n"
"}\n"
"                        "));

        chatbotLayout->addWidget(chatbotLabel);

        chatDisplay = new QTextEdit(chatbotFrame);
        chatDisplay->setObjectName("chatDisplay");
        chatDisplay->setReadOnly(true);
        chatDisplay->setMinimumHeight(120);
        chatDisplay->setMaximumHeight(400);
        chatDisplay->setStyleSheet(QString::fromUtf8("\n"
"QTextEdit {\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 12px;\n"
"  background-color: #0d1b2a;\n"
"  color: #ecf0f1;\n"
"  font-family: 'Segoe UI', Arial;\n"
"  font-size: 11px;\n"
"  line-height: 1.6;\n"
"  selection-background-color: #00bcd4;\n"
"  selection-color: #0d1b2a;\n"
"}\n"
"QTextEdit:focus {\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                        "));

        chatbotLayout->addWidget(chatDisplay);

        suggestionsLayout = new QHBoxLayout();
        suggestionsLayout->setSpacing(6);
        suggestionsLayout->setObjectName("suggestionsLayout");
        suggestPrice = new QPushButton(chatbotFrame);
        suggestPrice->setObjectName("suggestPrice");
        suggestPrice->setMaximumWidth(130);
        suggestPrice->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1a237e;\n"
"  color: #00bcd4;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  font-size: 10px;\n"
"  font-weight: bold;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #283593;\n"
"  color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0d1b6b;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                          "));

        suggestionsLayout->addWidget(suggestPrice);

        suggestAnomaly = new QPushButton(chatbotFrame);
        suggestAnomaly->setObjectName("suggestAnomaly");
        suggestAnomaly->setMaximumWidth(130);
        suggestAnomaly->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1a237e;\n"
"  color: #00bcd4;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  font-size: 10px;\n"
"  font-weight: bold;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #283593;\n"
"  color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0d1b6b;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                          "));

        suggestionsLayout->addWidget(suggestAnomaly);

        suggestStats = new QPushButton(chatbotFrame);
        suggestStats->setObjectName("suggestStats");
        suggestStats->setMaximumWidth(130);
        suggestStats->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1a237e;\n"
"  color: #00bcd4;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  font-size: 10px;\n"
"  font-weight: bold;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #283593;\n"
"  color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0d1b6b;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                          "));

        suggestionsLayout->addWidget(suggestStats);

        suggestRevenue = new QPushButton(chatbotFrame);
        suggestRevenue->setObjectName("suggestRevenue");
        suggestRevenue->setMaximumWidth(130);
        suggestRevenue->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1a237e;\n"
"  color: #00bcd4;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  font-size: 10px;\n"
"  font-weight: bold;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #283593;\n"
"  color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0d1b6b;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                          "));

        suggestionsLayout->addWidget(suggestRevenue);

        suggestAlerts = new QPushButton(chatbotFrame);
        suggestAlerts->setObjectName("suggestAlerts");
        suggestAlerts->setMaximumWidth(130);
        suggestAlerts->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #1a237e;\n"
"  color: #00bcd4;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  font-size: 10px;\n"
"  font-weight: bold;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #283593;\n"
"  color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0d1b6b;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"                          "));

        suggestionsLayout->addWidget(suggestAlerts);

        suggestSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        suggestionsLayout->addItem(suggestSpacer);


        chatbotLayout->addLayout(suggestionsLayout);

        chatInputLayout = new QHBoxLayout();
        chatInputLayout->setSpacing(8);
        chatInputLayout->setObjectName("chatInputLayout");
        chatInput = new QLineEdit(chatbotFrame);
        chatInput->setObjectName("chatInput");
        chatInput->setMinimumHeight(36);
        chatInput->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit {\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 12px;\n"
"  background-color: #0d1b2a;\n"
"  color: #00bcd4;\n"
"  font-weight: bold;\n"
"  font-size: 11px;\n"
"  letter-spacing: 0.3px;\n"
"}\n"
"QLineEdit:focus {\n"
"  border: 2px solid #00e5ff;\n"
"  background-color: #0d1b2a;\n"
"  color: #00e5ff;\n"
"}\n"
"QLineEdit::placeholder {\n"
"  color: #64b5f6;\n"
"}\n"
"                         "));

        chatInputLayout->addWidget(chatInput);

        chatSendButton = new QPushButton(chatbotFrame);
        chatSendButton->setObjectName("chatSendButton");
        chatSendButton->setMinimumHeight(36);
        chatSendButton->setMinimumWidth(100);
        chatSendButton->setStyleSheet(QString::fromUtf8("\n"
"QPushButton {\n"
"  background-color: #00bcd4;\n"
"  color: white;\n"
"  border: 2px solid #00bcd4;\n"
"  border-radius: 6px;\n"
"  padding: 8px 18px;\n"
"  font-weight: bold;\n"
"  font-size: 11px;\n"
"  letter-spacing: 0.5px;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #00e5ff;\n"
"  border: 2px solid #00e5ff;\n"
"}\n"
"QPushButton:pressed {\n"
"  background-color: #0097a7;\n"
"  border: 2px solid #00bcd4;\n"
"}\n"
"QPushButton:focus {\n"
"  outline: none;\n"
"  border: 2px solid white;\n"
"}\n"
"                         "));

        chatInputLayout->addWidget(chatSendButton);


        chatbotLayout->addLayout(chatInputLayout);


        optionsFrameLayout->addWidget(chatbotFrame);


        contentLayout->addWidget(optionsFrame);

        contentBottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        contentLayout->addItem(contentBottomSpacer);

        mainScrollArea->setWidget(scrollContents);

        pageTransOuterLayout->addWidget(mainScrollArea);

        stackedWidget->addWidget(pageTransactions);

        mainContentLayout->addWidget(stackedWidget);


        mainLayout->addWidget(mainContent);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VISION SIGHT - Gestion Des Ventes", nullptr));
        logoLabel->setText(QString());
        lblVisionSight->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        btnEmployees->setText(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251es", nullptr));
        btnNavires->setText(QCoreApplication::translate("MainWindow", "Gestion Des Navires", nullptr));
        btnQuais->setText(QCoreApplication::translate("MainWindow", "Gestion Des Quais", nullptr));
        btnCaptures->setText(QCoreApplication::translate("MainWindow", "Gestion Des Captures", nullptr));
        btnStockage->setText(QCoreApplication::translate("MainWindow", "Stockage Frigorifique", nullptr));
        btnVentes->setText(QCoreApplication::translate("MainWindow", "Gestion Des Ventes", nullptr));
        btnVentesAffichage->setText(QCoreApplication::translate("MainWindow", "Affichage", nullptr));
        btnVentesStatistique->setText(QCoreApplication::translate("MainWindow", "Statistique", nullptr));
        btnVentesAvancees->setText(QCoreApplication::translate("MainWindow", "Avanc\303\251es", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        btnMenu->setText(QCoreApplication::translate("MainWindow", "\342\230\260", nullptr));
        lblRecherche->setText(QCoreApplication::translate("MainWindow", "Recherche", nullptr));
        lineRecherchePecheur->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche p\303\252cheur", nullptr));
        lineRechercheFacture->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche Num Facture", nullptr));
        lineRechercheLot->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche lot", nullptr));
        dateDebut->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd HH:mm", nullptr));
        dateFin->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd HH:mm", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        lblTri->setText(QCoreApplication::translate("MainWindow", "Tri", nullptr));
        lblTrierPar->setText(QCoreApplication::translate("MainWindow", "Trier par", nullptr));
        comboTri->setItemText(0, QCoreApplication::translate("MainWindow", "Date transaction", nullptr));
        comboTri->setItemText(1, QCoreApplication::translate("MainWindow", "Prix/kg", nullptr));
        comboTri->setItemText(2, QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        comboTri->setItemText(3, QCoreApplication::translate("MainWindow", "Total", nullptr));

        lblOrdre->setText(QCoreApplication::translate("MainWindow", "Ordre", nullptr));
        comboOrdre->setItemText(0, QCoreApplication::translate("MainWindow", "D\303\251croissant", nullptr));
        comboOrdre->setItemText(1, QCoreApplication::translate("MainWindow", "Croissant", nullptr));

        lblAffichage->setText(QCoreApplication::translate("MainWindow", "Gestion Transactions", nullptr));
        btnCreer->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableTransactions->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Num Facture", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableTransactions->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "P\303\252cheur", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableTransactions->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "R\303\251f\303\251rence", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableTransactions->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Lot", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableTransactions->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Prix/kg", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableTransactions->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Quantit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableTransactions->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Total", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableTransactions->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Date transaction", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableTransactions->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Mode paiement", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableTransactions->horizontalHeaderItem(9);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Statut paiement", nullptr));
        lblStatistiquesVentes->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        lblPeriode->setText(QCoreApplication::translate("MainWindow", "P\303\251riode:", nullptr));
        comboPeriode->setItemText(0, QCoreApplication::translate("MainWindow", "7j", nullptr));
        comboPeriode->setItemText(1, QCoreApplication::translate("MainWindow", "30j", nullptr));
        comboPeriode->setItemText(2, QCoreApplication::translate("MainWindow", "90j", nullptr));
        comboPeriode->setItemText(3, QCoreApplication::translate("MainWindow", "365j", nullptr));

        lblChartTitle_jour->setText(QCoreApplication::translate("MainWindow", "CA jour", nullptr));
        lblChartValue_jour->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnit_jour->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        lblChartTitle_mois->setText(QCoreApplication::translate("MainWindow", "CA mois", nullptr));
        lblChartValue_mois->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnit_mois->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        lblChartTitle_annee->setText(QCoreApplication::translate("MainWindow", "CA ann\303\251e", nullptr));
        lblChartValue_annee->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnit_annee->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        lblChartTitle_moyen->setText(QCoreApplication::translate("MainWindow", "Revenu moyen", nullptr));
        lblChartValue_moyen->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnit_moyen->setText(QCoreApplication::translate("MainWindow", "/ transaction", nullptr));
        lblChartTitleQTE->setText(QCoreApplication::translate("MainWindow", "+ vendu", nullptr));
        lblChartQTE->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnitQTE->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        lblChartTitleREV->setText(QCoreApplication::translate("MainWindow", "- vendu", nullptr));
        lblChartREV->setText(QCoreApplication::translate("MainWindow", "--", nullptr));
        lblChartUnitREV->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        lblOptionsAvancees->setText(QCoreApplication::translate("MainWindow", "Options avanc\303\251es", nullptr));
        priceEstimateLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\223\212 Prix Estim\303\251 de Poisson", nullptr));
        fishPriceComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "-- S\303\251lectionner un poisson --", nullptr));
        fishPriceComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "Sardine", nullptr));
        fishPriceComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "Anchois", nullptr));
        fishPriceComboBox->setItemText(3, QCoreApplication::translate("MainWindow", "Maquereau", nullptr));
        fishPriceComboBox->setItemText(4, QCoreApplication::translate("MainWindow", "Dorade", nullptr));
        fishPriceComboBox->setItemText(5, QCoreApplication::translate("MainWindow", "Rouget", nullptr));
        fishPriceComboBox->setItemText(6, QCoreApplication::translate("MainWindow", "Sole", nullptr));
        fishPriceComboBox->setItemText(7, QCoreApplication::translate("MainWindow", "Poulpe", nullptr));
        fishPriceComboBox->setItemText(8, QCoreApplication::translate("MainWindow", "Crevette", nullptr));
        fishPriceComboBox->setItemText(9, QCoreApplication::translate("MainWindow", "Lieu", nullptr));
        fishPriceComboBox->setItemText(10, QCoreApplication::translate("MainWindow", "Encornet", nullptr));
        fishPriceComboBox->setItemText(11, QCoreApplication::translate("MainWindow", "Merlan", nullptr));
        fishPriceComboBox->setItemText(12, QCoreApplication::translate("MainWindow", "Cabillaud", nullptr));
        fishPriceComboBox->setItemText(13, QCoreApplication::translate("MainWindow", "Trout", nullptr));
        fishPriceComboBox->setItemText(14, QCoreApplication::translate("MainWindow", "Turbot", nullptr));
        fishPriceComboBox->setItemText(15, QCoreApplication::translate("MainWindow", "Hareng", nullptr));
        fishPriceComboBox->setItemText(16, QCoreApplication::translate("MainWindow", "Moule", nullptr));
        fishPriceComboBox->setItemText(17, QCoreApplication::translate("MainWindow", "Bar", nullptr));
        fishPriceComboBox->setItemText(18, QCoreApplication::translate("MainWindow", "Basse", nullptr));
        fishPriceComboBox->setItemText(19, QCoreApplication::translate("MainWindow", "Seiche", nullptr));
        fishPriceComboBox->setItemText(20, QCoreApplication::translate("MainWindow", "Calmar", nullptr));
        fishPriceComboBox->setItemText(21, QCoreApplication::translate("MainWindow", "Saumon", nullptr));
        fishPriceComboBox->setItemText(22, QCoreApplication::translate("MainWindow", "Esturgeon", nullptr));
        fishPriceComboBox->setItemText(23, QCoreApplication::translate("MainWindow", "Tilapia", nullptr));
        fishPriceComboBox->setItemText(24, QCoreApplication::translate("MainWindow", "Goberge", nullptr));
        fishPriceComboBox->setItemText(25, QCoreApplication::translate("MainWindow", "Fl\303\251tan", nullptr));
        fishPriceComboBox->setItemText(26, QCoreApplication::translate("MainWindow", "Morue", nullptr));
        fishPriceComboBox->setItemText(27, QCoreApplication::translate("MainWindow", "Truite", nullptr));
        fishPriceComboBox->setItemText(28, QCoreApplication::translate("MainWindow", "Daurade royale", nullptr));
        fishPriceComboBox->setItemText(29, QCoreApplication::translate("MainWindow", "Bar ray\303\251", nullptr));
        fishPriceComboBox->setItemText(30, QCoreApplication::translate("MainWindow", "Anguille", nullptr));
        fishPriceComboBox->setItemText(31, QCoreApplication::translate("MainWindow", "Mulet", nullptr));
        fishPriceComboBox->setItemText(32, QCoreApplication::translate("MainWindow", "Rouget barbarin", nullptr));
        fishPriceComboBox->setItemText(33, QCoreApplication::translate("MainWindow", "Br\303\250me", nullptr));
        fishPriceComboBox->setItemText(34, QCoreApplication::translate("MainWindow", "Daurade rose", nullptr));
        fishPriceComboBox->setItemText(35, QCoreApplication::translate("MainWindow", "Sar", nullptr));
        fishPriceComboBox->setItemText(36, QCoreApplication::translate("MainWindow", "Loup de mer", nullptr));
        fishPriceComboBox->setItemText(37, QCoreApplication::translate("MainWindow", "Poutine", nullptr));
        fishPriceComboBox->setItemText(38, QCoreApplication::translate("MainWindow", "Crapet", nullptr));
        fishPriceComboBox->setItemText(39, QCoreApplication::translate("MainWindow", "Carpe", nullptr));
        fishPriceComboBox->setItemText(40, QCoreApplication::translate("MainWindow", "Silure", nullptr));

        btnPrixEstime->setText(QCoreApplication::translate("MainWindow", "Prix Estim\303\251", nullptr));
        priceResultDisplayLabel->setText(QCoreApplication::translate("MainWindow", "S\303\251lectionnez un poisson et cliquez sur \"Prix Estim\303\251\"", nullptr));
        lblDetection->setText(QCoreApplication::translate("MainWindow", "D\303\251tection des transactions anormales", nullptr));
        chatbotLabel->setText(QCoreApplication::translate("MainWindow", "\360\237\244\226 Assistant M\303\251tier - Chatbot", nullptr));
        suggestPrice->setText(QCoreApplication::translate("MainWindow", "Quel prix?", nullptr));
        suggestAnomaly->setText(QCoreApplication::translate("MainWindow", "Anomalies?", nullptr));
        suggestStats->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        suggestRevenue->setText(QCoreApplication::translate("MainWindow", "Revenu", nullptr));
        suggestAlerts->setText(QCoreApplication::translate("MainWindow", "Alertes", nullptr));
        chatInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Posez une question...", nullptr));
        chatSendButton->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
