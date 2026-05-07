/********************************************************************************
** Form generated from reading UI file 'employe_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYE_MAINWINDOW_H
#define UI_EMPLOYE_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EmployeMainWindow
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
    QSpacerItem *sidebarBottomSpacer;
    QWidget *mainContent;
    QVBoxLayout *mainContentLayout;
    QFrame *headerFrame;
    QHBoxLayout *headerLayout;
    QLabel *lblTitle;
    QSpacerItem *headerSpacer;
    QPushButton *btnMenu;
    QStackedWidget *stackedWidget;
    QWidget *pageEmployees;
    QVBoxLayout *pageEmpOuterLayout;
    QScrollArea *empScrollArea;
    QWidget *empScrollContents;
    QVBoxLayout *empContentLayout;
    QFrame *empCrudFrame;
    QVBoxLayout *vboxLayout;
    QLabel *lblEmpCrud;
    QHBoxLayout *hboxLayout;
    QPushButton *btnCreerUser;
    QPushButton *btnModifierUser;
    QPushButton *btnSupprimerUser;
    QPushButton *btnActiverDesactiver;
    QSpacerItem *spacerItem;
    QFrame *empSearchFrame;
    QVBoxLayout *vboxLayout1;
    QLabel *lblEmpRecherche;
    QHBoxLayout *hboxLayout1;
    QLineEdit *lineRechercheNomUser;
    QLineEdit *lineRechercheEmailUser;
    QComboBox *comboFilterRole;
    QComboBox *comboFilterStatut;
    QPushButton *btnRechercherUser;
    QFrame *empSortFrame;
    QVBoxLayout *vboxLayout2;
    QLabel *lblEmpTri;
    QHBoxLayout *hboxLayout2;
    QLabel *label;
    QComboBox *comboTriUser;
    QLabel *label1;
    QComboBox *comboOrdreUser;
    QSpacerItem *spacerItem1;
    QFrame *empTableFrame;
    QVBoxLayout *vboxLayout3;
    QLabel *lblEmpList;
    QTableWidget *tableEmployees;
    QFrame *empPayslipFrame;
    QVBoxLayout *vboxLayout4;
    QLabel *lblEmpPayslip;
    QHBoxLayout *hboxLayout3;
    QComboBox *comboPayrollUser;
    QComboBox *comboPayrollMonth;
    QPushButton *btnPayslipPDF;
    QPushButton *btnPayslipExcel;
    QPushButton *btnPayslipEmail;
    QSpacerItem *spacerItem2;
    QFrame *empStatsFrame;
    QVBoxLayout *vboxLayout5;
    QLabel *lblEmpStats;
    QHBoxLayout *hboxLayout4;
    QVBoxLayout *vboxLayout6;
    QLabel *label2;
    QTableWidget *tableStatRole;
    QVBoxLayout *vboxLayout7;
    QLabel *label3;
    QTableWidget *tableStatStatut;
    QVBoxLayout *vboxLayout8;
    QLabel *label4;
    QTableWidget *tableStatService;
    QFrame *empSessionFrame;
    QVBoxLayout *vboxLayout9;
    QLabel *lblEmpSession;
    QTableWidget *tableSessions;
    QHBoxLayout *hboxLayout5;
    QSpacerItem *spacerItem3;
    QPushButton *btnTerminerSession;
    QSpacerItem *empBottomSpacer;

    void setupUi(QMainWindow *EmployeMainWindow)
    {
        if (EmployeMainWindow->objectName().isEmpty())
            EmployeMainWindow->setObjectName("EmployeMainWindow");
        EmployeMainWindow->resize(1500, 900);
        EmployeMainWindow->setMinimumSize(QSize(1200, 700));
        centralWidget = new QWidget(EmployeMainWindow);
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
"  background: #ffffff; color: #132437;\n"
"  border: 2px solid #ffffff; border-radius: 39px; padding: 0px;\n"
"}\n"
"#lblVisionSight {\n"
"  color: #e6eef6;\n"
"  font-size: 16px;\n"
"  font-weight: 800;\n"
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
"  font-weight: 600;\n"
"}\n"
"#"
                        "sidebar QPushButton:checked {\n"
"  color: #39c0fa;\n"
"  background: #132437;\n"
"  border-left: 3px solid #39c0fa;\n"
"  font-weight: 600;\n"
"}\n"
"#sidebar QPushButton:pressed {\n"
"  background: #1a2f45;\n"
"}\n"
"#btnEmployees, #btnNavires, #btnQuais, #btnCaptures, #btnStockage, #btnVentes {\n"
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
"#btnMenu:hover {\n"
"  background-color: #112b41;\n"
"}\n"
"QFrame#transactionsFrame, QFrame#rechercheFrame, QFrame#triFrame,\n"
"QFrame#affichageFrame, QFrame#statsFrame, QFrame#optionsFrame,\n"
"QFrame#empCrudFrame, QFrame#empSearchFrame, QFrame#empSortFrame,\n"
"QFrame#empTableFrame, QFrame#empPayslipFrame, QFrame#empStatsFrame, QFrame#empSessionFrame {\n"
"  background-color: #1"
                        "32437;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 16px;\n"
"}\n"
"#lblTransactions, #lblRecherche, #lblTri, #lblAffichage,\n"
"#lblStatistiquesVentes, #lblOptionsAvancees,\n"
"#lblEmpCrud, #lblEmpRecherche, #lblEmpTri, #lblEmpList,\n"
"#lblEmpPayslip, #lblEmpStats, #lblEmpSession {\n"
"  color: #39c0fa;\n"
"  font-size: 18px;\n"
"  font-weight: 700;\n"
"}\n"
"#lblDetection, #lblPrevision {\n"
"  color: #9bb0c3;\n"
"  font-size: 13px;\n"
"}\n"
"#btnCreer, #btnCreerUser, #btnPayslipEmail {\n"
"  background-color: #39c0fa;\n"
"  color: #0c1a29;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  font-size: 14px;\n"
"  font-weight: 800;\n"
"}\n"
"#btnCreer:hover, #btnCreerUser:hover, #btnPayslipEmail:hover { background-color: #56cdfc; }\n"
"#btnModifier, #btnModifierUser, #btnActiverDesactiver, #btnPayslipExcel, #btnTerminerSession {\n"
"  background-color: #23374e;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #2b4561;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  f"
                        "ont-size: 14px;\n"
"  font-weight: 800;\n"
"}\n"
"#btnModifier:hover, #btnModifierUser:hover, #btnActiverDesactiver:hover,\n"
"#btnPayslipExcel:hover, #btnTerminerSession:hover { background-color: #2b4561; }\n"
"#btnSupprimer, #btnSupprimerUser {\n"
"  background-color: #39c0fa;\n"
"  color: #0c1a29;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 18px;\n"
"  font-size: 14px;\n"
"  font-weight: 700;\n"
"}\n"
"#btnSupprimer:hover, #btnSupprimerUser:hover { background-color: #56cdfc; }\n"
"#btnExportPDF, #btnRechercherUser, #btnPayslipPDF {\n"
"  background-color: #1e8de0;\n"
"  color: white;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 14px;\n"
"  font-size: 13px;\n"
"  font-weight: 700;\n"
"}\n"
"#btnExportPDF:hover, #btnRechercherUser:hover, #btnPayslipPDF:hover { background-color: #2a99ec; }\n"
"QLineEdit {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  min-height: 22px;\n"
""
                        "  font-size: 11px;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QDateTimeEdit {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  padding: 4px 10px;\n"
"  padding-right: 28px;\n"
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
"  selection-background-color:"
                        " #39c0fa;\n"
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
"QTableWidget::item {\n"
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
"}\n"
"#chartQTE, #chartREV {\n"
"  background-color: #101c2c;\n"
"  border: 10px solid #39c0fa;\n"
"  border-radius: 75px;\n"
"  min-width: 150px; min-height: 150px;\n"
"  max-width: 150px; max-height: 150px;\n"
"}\n"
"#lblChartTitle_jour, #lblChartTitle_mois, #lblChartTitle_annee, #lblChartTitle_moyen,\n"
"#lblChartTitleQTE, #lblChartTitleREV {\n"
"  color: #39c0fa;\n"
"  font-size:"
                        " 11px;\n"
"  font-weight: 600;\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"#lblChartValue_jour, #lblChartValue_mois, #lblChartValue_annee, #lblChartValue_moyen,\n"
"#lblChartQTE, #lblChartREV {\n"
"  color: #e6eef6;\n"
"  font-size: 20px;\n"
"  font-weight: bold;\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"#lblChartUnit_jour, #lblChartUnit_mois, #lblChartUnit_annee, #lblChartUnit_moyen,\n"
"#lblChartUnitQTE, #lblChartUnitREV {\n"
"  color: #6a7f93;\n"
"  font-size: 10px;\n"
"  background: transparent;\n"
"  border: none;\n"
"}\n"
"#lblPeriode {\n"
"  color: #9bb0c3;\n"
"  font-size: 13px;\n"
"}\n"
"#comboPeriode {\n"
"  background-color: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
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
""
                        "}\n"
"QScrollBar:vertical {\n"
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
        sidebarLayout->setSpacing(10);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(18, 18, 18, 18);
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

        sidebarTopSpacer = new QSpacerItem(20, 12, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(sidebarTopSpacer);

        btnEmployees = new QPushButton(sidebar);
        btnEmployees->setObjectName("btnEmployees");
        btnEmployees->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnEmployees->setCheckable(true);
        btnEmployees->setAutoExclusive(true);
        btnEmployees->setChecked(true);

        sidebarLayout->addWidget(btnEmployees);

        btnNavires = new QPushButton(sidebar);
        btnNavires->setObjectName("btnNavires");
        btnNavires->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnNavires->setCheckable(true);
        btnNavires->setAutoExclusive(true);

        sidebarLayout->addWidget(btnNavires);

        btnQuais = new QPushButton(sidebar);
        btnQuais->setObjectName("btnQuais");
        btnQuais->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnQuais->setCheckable(true);
        btnQuais->setAutoExclusive(true);

        sidebarLayout->addWidget(btnQuais);

        btnCaptures = new QPushButton(sidebar);
        btnCaptures->setObjectName("btnCaptures");
        btnCaptures->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnCaptures->setCheckable(true);
        btnCaptures->setAutoExclusive(true);

        sidebarLayout->addWidget(btnCaptures);

        btnStockage = new QPushButton(sidebar);
        btnStockage->setObjectName("btnStockage");
        btnStockage->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnStockage->setCheckable(true);
        btnStockage->setAutoExclusive(true);

        sidebarLayout->addWidget(btnStockage);

        btnVentes = new QPushButton(sidebar);
        btnVentes->setObjectName("btnVentes");
        btnVentes->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnVentes->setCheckable(true);
        btnVentes->setAutoExclusive(true);

        sidebarLayout->addWidget(btnVentes);

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
        pageEmployees = new QWidget();
        pageEmployees->setObjectName("pageEmployees");
        pageEmpOuterLayout = new QVBoxLayout(pageEmployees);
        pageEmpOuterLayout->setSpacing(0);
        pageEmpOuterLayout->setObjectName("pageEmpOuterLayout");
        pageEmpOuterLayout->setContentsMargins(0, 0, 0, 0);
        empScrollArea = new QScrollArea(pageEmployees);
        empScrollArea->setObjectName("empScrollArea");
        empScrollArea->setWidgetResizable(true);
        empScrollArea->setFrameShape(QFrame::NoFrame);
        empScrollContents = new QWidget();
        empScrollContents->setObjectName("empScrollContents");
        empScrollContents->setGeometry(QRect(0, 0, 1260, 3200));
        empContentLayout = new QVBoxLayout(empScrollContents);
        empContentLayout->setSpacing(14);
        empContentLayout->setObjectName("empContentLayout");
        empContentLayout->setContentsMargins(16, 14, 16, 14);
        empCrudFrame = new QFrame(empScrollContents);
        empCrudFrame->setObjectName("empCrudFrame");
        empCrudFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout = new QVBoxLayout(empCrudFrame);
        vboxLayout->setSpacing(12);
        vboxLayout->setObjectName("vboxLayout");
        vboxLayout->setContentsMargins(20, 14, 20, 16);
        lblEmpCrud = new QLabel(empCrudFrame);
        lblEmpCrud->setObjectName("lblEmpCrud");

        vboxLayout->addWidget(lblEmpCrud);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(12);
        hboxLayout->setObjectName("hboxLayout");
        btnCreerUser = new QPushButton(empCrudFrame);
        btnCreerUser->setObjectName("btnCreerUser");
        btnCreerUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnCreerUser->setMinimumSize(QSize(100, 36));

        hboxLayout->addWidget(btnCreerUser);

        btnModifierUser = new QPushButton(empCrudFrame);
        btnModifierUser->setObjectName("btnModifierUser");
        btnModifierUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnModifierUser->setMinimumSize(QSize(100, 36));

        hboxLayout->addWidget(btnModifierUser);

        btnSupprimerUser = new QPushButton(empCrudFrame);
        btnSupprimerUser->setObjectName("btnSupprimerUser");
        btnSupprimerUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnSupprimerUser->setMinimumSize(QSize(100, 36));

        hboxLayout->addWidget(btnSupprimerUser);

        btnActiverDesactiver = new QPushButton(empCrudFrame);
        btnActiverDesactiver->setObjectName("btnActiverDesactiver");
        btnActiverDesactiver->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnActiverDesactiver->setMinimumSize(QSize(140, 36));

        hboxLayout->addWidget(btnActiverDesactiver);

        spacerItem = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(spacerItem);


        vboxLayout->addLayout(hboxLayout);


        empContentLayout->addWidget(empCrudFrame);

        empSearchFrame = new QFrame(empScrollContents);
        empSearchFrame->setObjectName("empSearchFrame");
        empSearchFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout1 = new QVBoxLayout(empSearchFrame);
        vboxLayout1->setSpacing(12);
        vboxLayout1->setObjectName("vboxLayout1");
        vboxLayout1->setContentsMargins(20, 14, 20, 16);
        lblEmpRecherche = new QLabel(empSearchFrame);
        lblEmpRecherche->setObjectName("lblEmpRecherche");

        vboxLayout1->addWidget(lblEmpRecherche);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(10);
        hboxLayout1->setObjectName("hboxLayout1");
        lineRechercheNomUser = new QLineEdit(empSearchFrame);
        lineRechercheNomUser->setObjectName("lineRechercheNomUser");
        lineRechercheNomUser->setMinimumSize(QSize(0, 36));

        hboxLayout1->addWidget(lineRechercheNomUser);

        lineRechercheEmailUser = new QLineEdit(empSearchFrame);
        lineRechercheEmailUser->setObjectName("lineRechercheEmailUser");
        lineRechercheEmailUser->setMinimumSize(QSize(0, 36));

        hboxLayout1->addWidget(lineRechercheEmailUser);

        comboFilterRole = new QComboBox(empSearchFrame);
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->addItem(QString());
        comboFilterRole->setObjectName("comboFilterRole");
        comboFilterRole->setMinimumSize(QSize(150, 36));

        hboxLayout1->addWidget(comboFilterRole);

        comboFilterStatut = new QComboBox(empSearchFrame);
        comboFilterStatut->addItem(QString());
        comboFilterStatut->addItem(QString());
        comboFilterStatut->addItem(QString());
        comboFilterStatut->setObjectName("comboFilterStatut");
        comboFilterStatut->setMinimumSize(QSize(140, 36));

        hboxLayout1->addWidget(comboFilterStatut);

        btnRechercherUser = new QPushButton(empSearchFrame);
        btnRechercherUser->setObjectName("btnRechercherUser");
        btnRechercherUser->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        btnRechercherUser->setMinimumSize(QSize(120, 36));

        hboxLayout1->addWidget(btnRechercherUser);


        vboxLayout1->addLayout(hboxLayout1);


        empContentLayout->addWidget(empSearchFrame);

        empSortFrame = new QFrame(empScrollContents);
        empSortFrame->setObjectName("empSortFrame");
        empSortFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout2 = new QVBoxLayout(empSortFrame);
        vboxLayout2->setSpacing(12);
        vboxLayout2->setObjectName("vboxLayout2");
        vboxLayout2->setContentsMargins(20, 14, 20, 16);
        lblEmpTri = new QLabel(empSortFrame);
        lblEmpTri->setObjectName("lblEmpTri");

        vboxLayout2->addWidget(lblEmpTri);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(10);
        hboxLayout2->setObjectName("hboxLayout2");
        label = new QLabel(empSortFrame);
        label->setObjectName("label");

        hboxLayout2->addWidget(label);

        comboTriUser = new QComboBox(empSortFrame);
        comboTriUser->addItem(QString());
        comboTriUser->addItem(QString());
        comboTriUser->setObjectName("comboTriUser");
        comboTriUser->setMinimumSize(QSize(160, 36));

        hboxLayout2->addWidget(comboTriUser);

        label1 = new QLabel(empSortFrame);
        label1->setObjectName("label1");

        hboxLayout2->addWidget(label1);

        comboOrdreUser = new QComboBox(empSortFrame);
        comboOrdreUser->addItem(QString());
        comboOrdreUser->addItem(QString());
        comboOrdreUser->setObjectName("comboOrdreUser");
        comboOrdreUser->setMinimumSize(QSize(150, 36));

        hboxLayout2->addWidget(comboOrdreUser);

        spacerItem1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout2->addItem(spacerItem1);


        vboxLayout2->addLayout(hboxLayout2);


        empContentLayout->addWidget(empSortFrame);

        empTableFrame = new QFrame(empScrollContents);
        empTableFrame->setObjectName("empTableFrame");
        empTableFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout3 = new QVBoxLayout(empTableFrame);
        vboxLayout3->setSpacing(12);
        vboxLayout3->setObjectName("vboxLayout3");
        vboxLayout3->setContentsMargins(20, 14, 20, 20);
        lblEmpList = new QLabel(empTableFrame);
        lblEmpList->setObjectName("lblEmpList");

        vboxLayout3->addWidget(lblEmpList);

        tableEmployees = new QTableWidget(empTableFrame);
        if (tableEmployees->columnCount() < 8)
            tableEmployees->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableEmployees->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableEmployees->setObjectName("tableEmployees");
        tableEmployees->setMinimumSize(QSize(0, 300));
        tableEmployees->setAlternatingRowColors(true);
        tableEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableEmployees->setSortingEnabled(true);
        tableEmployees->horizontalHeader()->setStretchLastSection(true);
        tableEmployees->verticalHeader()->setVisible(false);

        vboxLayout3->addWidget(tableEmployees);


        empContentLayout->addWidget(empTableFrame);

        empPayslipFrame = new QFrame(empScrollContents);
        empPayslipFrame->setObjectName("empPayslipFrame");
        empPayslipFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout4 = new QVBoxLayout(empPayslipFrame);
        vboxLayout4->setSpacing(12);
        vboxLayout4->setObjectName("vboxLayout4");
        vboxLayout4->setContentsMargins(20, 14, 20, 16);
        lblEmpPayslip = new QLabel(empPayslipFrame);
        lblEmpPayslip->setObjectName("lblEmpPayslip");

        vboxLayout4->addWidget(lblEmpPayslip);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(10);
        hboxLayout3->setObjectName("hboxLayout3");
        comboPayrollUser = new QComboBox(empPayslipFrame);
        comboPayrollUser->addItem(QString());
        comboPayrollUser->setObjectName("comboPayrollUser");
        comboPayrollUser->setMinimumSize(QSize(180, 36));

        hboxLayout3->addWidget(comboPayrollUser);

        comboPayrollMonth = new QComboBox(empPayslipFrame);
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->addItem(QString());
        comboPayrollMonth->setObjectName("comboPayrollMonth");
        comboPayrollMonth->setMinimumSize(QSize(140, 36));

        hboxLayout3->addWidget(comboPayrollMonth);

        btnPayslipPDF = new QPushButton(empPayslipFrame);
        btnPayslipPDF->setObjectName("btnPayslipPDF");
        btnPayslipPDF->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout3->addWidget(btnPayslipPDF);

        btnPayslipExcel = new QPushButton(empPayslipFrame);
        btnPayslipExcel->setObjectName("btnPayslipExcel");
        btnPayslipExcel->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout3->addWidget(btnPayslipExcel);

        btnPayslipEmail = new QPushButton(empPayslipFrame);
        btnPayslipEmail->setObjectName("btnPayslipEmail");
        btnPayslipEmail->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout3->addWidget(btnPayslipEmail);

        spacerItem2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout3->addItem(spacerItem2);


        vboxLayout4->addLayout(hboxLayout3);


        empContentLayout->addWidget(empPayslipFrame);

        empStatsFrame = new QFrame(empScrollContents);
        empStatsFrame->setObjectName("empStatsFrame");
        empStatsFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout5 = new QVBoxLayout(empStatsFrame);
        vboxLayout5->setSpacing(14);
        vboxLayout5->setObjectName("vboxLayout5");
        vboxLayout5->setContentsMargins(20, 14, 20, 20);
        lblEmpStats = new QLabel(empStatsFrame);
        lblEmpStats->setObjectName("lblEmpStats");

        vboxLayout5->addWidget(lblEmpStats);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(14);
        hboxLayout4->setObjectName("hboxLayout4");
        vboxLayout6 = new QVBoxLayout();
        vboxLayout6->setObjectName("vboxLayout6");
        label2 = new QLabel(empStatsFrame);
        label2->setObjectName("label2");
        label2->setStyleSheet(QString::fromUtf8("color:#e6eef6;font-size:15px;font-weight:700;"));

        vboxLayout6->addWidget(label2);

        tableStatRole = new QTableWidget(empStatsFrame);
        if (tableStatRole->columnCount() < 3)
            tableStatRole->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableStatRole->setHorizontalHeaderItem(0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableStatRole->setHorizontalHeaderItem(1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableStatRole->setHorizontalHeaderItem(2, __qtablewidgetitem10);
        tableStatRole->setObjectName("tableStatRole");
        tableStatRole->setMinimumSize(QSize(0, 180));
        tableStatRole->setAlternatingRowColors(true);
        tableStatRole->horizontalHeader()->setStretchLastSection(true);
        tableStatRole->verticalHeader()->setVisible(false);

        vboxLayout6->addWidget(tableStatRole);


        hboxLayout4->addLayout(vboxLayout6);

        vboxLayout7 = new QVBoxLayout();
        vboxLayout7->setObjectName("vboxLayout7");
        label3 = new QLabel(empStatsFrame);
        label3->setObjectName("label3");
        label3->setStyleSheet(QString::fromUtf8("color:#e6eef6;font-size:15px;font-weight:700;"));

        vboxLayout7->addWidget(label3);

        tableStatStatut = new QTableWidget(empStatsFrame);
        if (tableStatStatut->columnCount() < 3)
            tableStatStatut->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableStatStatut->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableStatStatut->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableStatStatut->setHorizontalHeaderItem(2, __qtablewidgetitem13);
        tableStatStatut->setObjectName("tableStatStatut");
        tableStatStatut->setMinimumSize(QSize(0, 180));
        tableStatStatut->setAlternatingRowColors(true);
        tableStatStatut->horizontalHeader()->setStretchLastSection(true);
        tableStatStatut->verticalHeader()->setVisible(false);

        vboxLayout7->addWidget(tableStatStatut);


        hboxLayout4->addLayout(vboxLayout7);

        vboxLayout8 = new QVBoxLayout();
        vboxLayout8->setObjectName("vboxLayout8");
        label4 = new QLabel(empStatsFrame);
        label4->setObjectName("label4");
        label4->setStyleSheet(QString::fromUtf8("color:#e6eef6;font-size:15px;font-weight:700;"));

        vboxLayout8->addWidget(label4);

        tableStatService = new QTableWidget(empStatsFrame);
        if (tableStatService->columnCount() < 3)
            tableStatService->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableStatService->setHorizontalHeaderItem(0, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableStatService->setHorizontalHeaderItem(1, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableStatService->setHorizontalHeaderItem(2, __qtablewidgetitem16);
        tableStatService->setObjectName("tableStatService");
        tableStatService->setMinimumSize(QSize(0, 180));
        tableStatService->setAlternatingRowColors(true);
        tableStatService->horizontalHeader()->setStretchLastSection(true);
        tableStatService->verticalHeader()->setVisible(false);

        vboxLayout8->addWidget(tableStatService);


        hboxLayout4->addLayout(vboxLayout8);


        vboxLayout5->addLayout(hboxLayout4);


        empContentLayout->addWidget(empStatsFrame);

        empSessionFrame = new QFrame(empScrollContents);
        empSessionFrame->setObjectName("empSessionFrame");
        empSessionFrame->setFrameShape(QFrame::StyledPanel);
        vboxLayout9 = new QVBoxLayout(empSessionFrame);
        vboxLayout9->setSpacing(12);
        vboxLayout9->setObjectName("vboxLayout9");
        vboxLayout9->setContentsMargins(20, 14, 20, 20);
        lblEmpSession = new QLabel(empSessionFrame);
        lblEmpSession->setObjectName("lblEmpSession");

        vboxLayout9->addWidget(lblEmpSession);

        tableSessions = new QTableWidget(empSessionFrame);
        if (tableSessions->columnCount() < 5)
            tableSessions->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableSessions->setHorizontalHeaderItem(0, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableSessions->setHorizontalHeaderItem(1, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableSessions->setHorizontalHeaderItem(2, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        tableSessions->setHorizontalHeaderItem(3, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableSessions->setHorizontalHeaderItem(4, __qtablewidgetitem21);
        tableSessions->setObjectName("tableSessions");
        tableSessions->setMinimumSize(QSize(0, 200));
        tableSessions->setAlternatingRowColors(true);
        tableSessions->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableSessions->horizontalHeader()->setStretchLastSection(true);
        tableSessions->verticalHeader()->setVisible(false);

        vboxLayout9->addWidget(tableSessions);

        hboxLayout5 = new QHBoxLayout();
        hboxLayout5->setObjectName("hboxLayout5");
        spacerItem3 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout5->addItem(spacerItem3);

        btnTerminerSession = new QPushButton(empSessionFrame);
        btnTerminerSession->setObjectName("btnTerminerSession");
        btnTerminerSession->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        hboxLayout5->addWidget(btnTerminerSession);


        vboxLayout9->addLayout(hboxLayout5);


        empContentLayout->addWidget(empSessionFrame);

        empBottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        empContentLayout->addItem(empBottomSpacer);

        empScrollArea->setWidget(empScrollContents);

        pageEmpOuterLayout->addWidget(empScrollArea);

        stackedWidget->addWidget(pageEmployees);

        mainContentLayout->addWidget(stackedWidget);


        mainLayout->addWidget(mainContent);

        EmployeMainWindow->setCentralWidget(centralWidget);

        retranslateUi(EmployeMainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(EmployeMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EmployeMainWindow)
    {
        EmployeMainWindow->setWindowTitle(QCoreApplication::translate("EmployeMainWindow", "VISION SIGHT - Gestion Des Ventes", nullptr));
        logoLabel->setText(QString());
        lblVisionSight->setText(QCoreApplication::translate("EmployeMainWindow", "VISION SIGHT", nullptr));
        btnEmployees->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion Des employ\303\251es", nullptr));
        btnNavires->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion Des Navires", nullptr));
        btnQuais->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion Des Quais", nullptr));
        btnCaptures->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion Des Captures", nullptr));
        btnStockage->setText(QCoreApplication::translate("EmployeMainWindow", "Stockage Frigorifique", nullptr));
        btnVentes->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion Des Ventes", nullptr));
        lblTitle->setText(QCoreApplication::translate("EmployeMainWindow", "VISION SIGHT", nullptr));
        btnMenu->setText(QCoreApplication::translate("EmployeMainWindow", "\342\230\260", nullptr));
        lblEmpCrud->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion des Employ\303\251s", nullptr));
        btnCreerUser->setText(QCoreApplication::translate("EmployeMainWindow", "Ajouter", nullptr));
        btnModifierUser->setText(QCoreApplication::translate("EmployeMainWindow", "Modifier", nullptr));
        btnSupprimerUser->setText(QCoreApplication::translate("EmployeMainWindow", "Supprimer", nullptr));
        btnActiverDesactiver->setText(QCoreApplication::translate("EmployeMainWindow", "Activer/D\303\251sactiver", nullptr));
        lblEmpRecherche->setText(QCoreApplication::translate("EmployeMainWindow", "Recherche Employ\303\251s", nullptr));
        lineRechercheNomUser->setPlaceholderText(QCoreApplication::translate("EmployeMainWindow", "Recherche par nom...", nullptr));
        lineRechercheEmailUser->setPlaceholderText(QCoreApplication::translate("EmployeMainWindow", "Recherche par email...", nullptr));
        comboFilterRole->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "Tous les r\303\264les", nullptr));
        comboFilterRole->setItemText(1, QCoreApplication::translate("EmployeMainWindow", "RH (Ressources Humaines)", nullptr));
        comboFilterRole->setItemText(2, QCoreApplication::translate("EmployeMainWindow", "Agent d'exploitation", nullptr));
        comboFilterRole->setItemText(3, QCoreApplication::translate("EmployeMainWindow", "Responsable stock", nullptr));
        comboFilterRole->setItemText(4, QCoreApplication::translate("EmployeMainWindow", "Responsable finance", nullptr));
        comboFilterRole->setItemText(5, QCoreApplication::translate("EmployeMainWindow", "ouvrier", nullptr));
        comboFilterRole->setItemText(6, QCoreApplication::translate("EmployeMainWindow", "admin", nullptr));

        comboFilterStatut->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "Tous les statuts", nullptr));
        comboFilterStatut->setItemText(1, QCoreApplication::translate("EmployeMainWindow", "Actif", nullptr));
        comboFilterStatut->setItemText(2, QCoreApplication::translate("EmployeMainWindow", "Inactif", nullptr));

        btnRechercherUser->setText(QCoreApplication::translate("EmployeMainWindow", "Rechercher", nullptr));
        lblEmpTri->setText(QCoreApplication::translate("EmployeMainWindow", "Tri", nullptr));
        label->setText(QCoreApplication::translate("EmployeMainWindow", "Trier par", nullptr));
        comboTriUser->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "CIN", nullptr));
        comboTriUser->setItemText(1, QCoreApplication::translate("EmployeMainWindow", "R\303\264le", nullptr));

        label1->setText(QCoreApplication::translate("EmployeMainWindow", "Ordre", nullptr));
        comboOrdreUser->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "Croissant", nullptr));
        comboOrdreUser->setItemText(1, QCoreApplication::translate("EmployeMainWindow", "D\303\251croissant", nullptr));

        lblEmpList->setText(QCoreApplication::translate("EmployeMainWindow", "Liste des Employ\303\251s", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableEmployees->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("EmployeMainWindow", "CIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableEmployees->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("EmployeMainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableEmployees->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("EmployeMainWindow", "Pr\303\251nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableEmployees->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("EmployeMainWindow", "Email", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableEmployees->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("EmployeMainWindow", "Login", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableEmployees->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("EmployeMainWindow", "R\303\264le", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableEmployees->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("EmployeMainWindow", "Heures", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableEmployees->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("EmployeMainWindow", "Statut", nullptr));
        lblEmpPayslip->setText(QCoreApplication::translate("EmployeMainWindow", "Fiches de Paie & Export", nullptr));
        comboPayrollUser->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "S\303\251lectionner employ\303\251...", nullptr));

        comboPayrollMonth->setItemText(0, QCoreApplication::translate("EmployeMainWindow", "Janvier", nullptr));
        comboPayrollMonth->setItemText(1, QCoreApplication::translate("EmployeMainWindow", "F\303\251vrier", nullptr));
        comboPayrollMonth->setItemText(2, QCoreApplication::translate("EmployeMainWindow", "Mars", nullptr));
        comboPayrollMonth->setItemText(3, QCoreApplication::translate("EmployeMainWindow", "Avril", nullptr));
        comboPayrollMonth->setItemText(4, QCoreApplication::translate("EmployeMainWindow", "Mai", nullptr));
        comboPayrollMonth->setItemText(5, QCoreApplication::translate("EmployeMainWindow", "Juin", nullptr));
        comboPayrollMonth->setItemText(6, QCoreApplication::translate("EmployeMainWindow", "Juillet", nullptr));
        comboPayrollMonth->setItemText(7, QCoreApplication::translate("EmployeMainWindow", "Ao\303\273t", nullptr));
        comboPayrollMonth->setItemText(8, QCoreApplication::translate("EmployeMainWindow", "Septembre", nullptr));
        comboPayrollMonth->setItemText(9, QCoreApplication::translate("EmployeMainWindow", "Octobre", nullptr));
        comboPayrollMonth->setItemText(10, QCoreApplication::translate("EmployeMainWindow", "Novembre", nullptr));
        comboPayrollMonth->setItemText(11, QCoreApplication::translate("EmployeMainWindow", "D\303\251cembre", nullptr));

        btnPayslipPDF->setText(QCoreApplication::translate("EmployeMainWindow", "Export PDF", nullptr));
        btnPayslipExcel->setText(QCoreApplication::translate("EmployeMainWindow", "Export Excel", nullptr));
        btnPayslipEmail->setText(QCoreApplication::translate("EmployeMainWindow", "Envoyer Email", nullptr));
        lblEmpStats->setText(QCoreApplication::translate("EmployeMainWindow", "Statistiques des Employ\303\251s", nullptr));
        label2->setText(QCoreApplication::translate("EmployeMainWindow", "R\303\251partition par R\303\264le", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableStatRole->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("EmployeMainWindow", "R\303\264le", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableStatRole->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("EmployeMainWindow", "Nombre", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableStatRole->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("EmployeMainWindow", "%", nullptr));
        label3->setText(QCoreApplication::translate("EmployeMainWindow", "R\303\251partition par Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableStatStatut->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("EmployeMainWindow", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableStatStatut->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("EmployeMainWindow", "Nombre", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableStatStatut->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("EmployeMainWindow", "%", nullptr));
        label4->setText(QCoreApplication::translate("EmployeMainWindow", "R\303\251partition par Service", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableStatService->horizontalHeaderItem(0);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("EmployeMainWindow", "Service", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableStatService->horizontalHeaderItem(1);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("EmployeMainWindow", "Nombre", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableStatService->horizontalHeaderItem(2);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("EmployeMainWindow", "%", nullptr));
        lblEmpSession->setText(QCoreApplication::translate("EmployeMainWindow", "Gestion des Sessions", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableSessions->horizontalHeaderItem(0);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("EmployeMainWindow", "Utilisateur", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableSessions->horizontalHeaderItem(1);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("EmployeMainWindow", "Date connexion", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableSessions->horizontalHeaderItem(2);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("EmployeMainWindow", "Heure", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableSessions->horizontalHeaderItem(3);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("EmployeMainWindow", "IP", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableSessions->horizontalHeaderItem(4);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("EmployeMainWindow", "Action", nullptr));
        btnTerminerSession->setText(QCoreApplication::translate("EmployeMainWindow", "Terminer la Session", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeMainWindow: public Ui_EmployeMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYE_MAINWINDOW_H
