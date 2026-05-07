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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "widgets/minimapwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *rootLayout;
    QFrame *sidebar;
    QVBoxLayout *sidebarLayout;
    QWidget *brandBox;
    QVBoxLayout *brandBoxLayout;
    QLabel *logo;
    QLabel *brandTitle;
    QSpacerItem *sidebarSpacer1;
    QPushButton *sideBtn0;
    QPushButton *sideBtn1;
    QPushButton *sideBtn2;
    QPushButton *sideBtn3;
    QPushButton *sideBtn4;
    QPushButton *sideBtn5;
    QSpacerItem *sidebarStretch;
    QScrollArea *scroll;
    QWidget *page;
    QVBoxLayout *pageLayout;
    QHBoxLayout *headerRow;
    QVBoxLayout *titleCol;
    QLabel *headerH1;
    QSpacerItem *headerSpacer;
    QToolButton *menuButton;
    QFrame *actionsCard;
    QVBoxLayout *actionsCardOuter;
    QHBoxLayout *actionsTopRow;
    QVBoxLayout *actionsLeftCol;
    QFrame *mapBox;
    QVBoxLayout *mapBoxLayout;
    MiniMapWidget *miniMap;
    QLabel *mapCaption;
    QLabel *mapHint;
    QLabel *sectionTitle;
    QSpacerItem *actionsLeftStretch;
    QFrame *advancedCard;
    QHBoxLayout *proposalRow;
    QVBoxLayout *advLeft;
    QLabel *advTitle;
    QLabel *advSub;
    QGridLayout *advGrid;
    QLabel *labNavireName;
    QComboBox *proposeNavire;
    QCheckBox *chkRef;
    QWidget *refRow;
    QHBoxLayout *refRowLayout;
    QDoubleSpinBox *refX;
    QDoubleSpinBox *refY;
    QCheckBox *chkShipLength;
    QDoubleSpinBox *shipLength;
    QCheckBox *chkShipType;
    QComboBox *shipType;
    QHBoxLayout *advBtnRow;
    QPushButton *btnProposeCheapest;
    QPushButton *btnProposeNearest;
    QPushButton *btnProposeFit;
    QSpacerItem *advBtnSpacer;
    QLabel *proposeResult;
    QSpacerItem *advLeftStretch;
    QVBoxLayout *toolsCol;
    QFrame *searchCard;
    QVBoxLayout *searchOuter;
    QLabel *searchTitle;
    QGridLayout *searchGrid;
    QLineEdit *searchQuai;
    QLineEdit *searchDate;
    QComboBox *searchEtat;
    QSpacerItem *searchSpacer;
    QGridLayout *advancedFilterGrid;
    QCheckBox *chkPrice;
    QWidget *priceRowWidget;
    QHBoxLayout *priceRowLayout;
    QDoubleSpinBox *minPrice;
    QDoubleSpinBox *maxPrice;
    QCheckBox *chkAvailAt;
    QDateTimeEdit *availAt;
    QCheckBox *chkRange;
    QWidget *rangeRowWidget;
    QHBoxLayout *rangeRowLayout;
    QDateTimeEdit *rangeStart;
    QDateTimeEdit *rangeEnd;
    QHBoxLayout *belowRow;
    QFrame *sortCard;
    QVBoxLayout *sortCardLayout;
    QLabel *sortTitle;
    QComboBox *sortMode;
    QSpacerItem *sortStretch;
    QFrame *exportCard;
    QVBoxLayout *exportCardLayout;
    QLabel *exportTitle;
    QComboBox *exportType;
    QPushButton *btnPdf;
    QSpacerItem *exportStretch;
    QFrame *tableCard;
    QVBoxLayout *tableCardLayout;
    QLabel *tableTitle;
    QHBoxLayout *tableTopRow;
    QCheckBox *selectAll;
    QSpacerItem *tableTopSpacer;
    QHBoxLayout *btnRow;
    QPushButton *btnCreate;
    QPushButton *btnUpdate;
    QPushButton *btnDelete;
    QTableWidget *table;
    QFrame *statsCard;
    QVBoxLayout *statsOuter;
    QHBoxLayout *statsHeader;
    QLabel *statsTitle;
    QSpacerItem *statsHeaderSpacer;
    QLabel *periodLabel;
    QComboBox *revenuePeriod;
    QGridLayout *circlesGrid;
    QFrame *phStatEtat;
    QVBoxLayout *phStatEtatLayout;
    QLabel *phStatEtatTitle;
    QLabel *phStatEtatVal;
    QFrame *phStatTauxGlobal;
    QVBoxLayout *phStatTauxLayout;
    QLabel *phStatTauxTitle;
    QLabel *phStatTauxVal;
    QLabel *phStatTauxSub;
    QFrame *phStatRevenuPeriode;
    QVBoxLayout *phStatRevPLayout;
    QLabel *phStatRevPTitle;
    QLabel *phStatRevPVal;
    QLabel *phStatRevPSub;
    QFrame *phStatRevenuMoyen;
    QVBoxLayout *phStatRevMLayout;
    QLabel *phStatRevMTitle;
    QLabel *phStatRevMVal;
    QLabel *phStatRevMSub;
    QHBoxLayout *bottomStatsRow;
    QSpacerItem *bottomStatsSpacer1;
    QFrame *phStatPlusRentable;
    QVBoxLayout *phStatPRLayout;
    QLabel *phStatPRTitle;
    QLabel *phStatPRVal;
    QLabel *phStatPRSub;
    QFrame *phStatMoinsUtilise;
    QVBoxLayout *phStatMULayout;
    QLabel *phStatMUTitle;
    QLabel *phStatMUVal;
    QLabel *phStatMUSub;
    QSpacerItem *bottomStatsSpacer2;
    QHBoxLayout *perQuaiRow;
    QFrame *revCard;
    QVBoxLayout *revCardLayout;
    QLabel *revTitle;
    QTableWidget *revByQuaiTable;
    QFrame *occCard;
    QVBoxLayout *occCardLayout;
    QLabel *occTitle;
    QTableWidget *occByQuaiTable;
    QHBoxLayout *traceRow;
    QFrame *histCard;
    QVBoxLayout *histCardLayout;
    QLabel *histTitle;
    QLabel *histHint;
    QWidget *pickRowWidget;
    QHBoxLayout *pickRowLayout;
    QLabel *pickLab;
    QLineEdit *traceQuai;
    QTableWidget *occHistoryTable;
    QFrame *rotCard;
    QVBoxLayout *rotCardLayout;
    QLabel *rotTitle;
    QLabel *rotSub;
    QLabel *rotationOcc;
    QLabel *rotationLibre;
    QSpacerItem *rotStretch;
    QFrame *revHistCard;
    QVBoxLayout *revHistCardLayout;
    QLabel *revHistTitle;
    QLabel *revHistHint;
    QTableWidget *revHistoryTable;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1366, 768);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"QMainWindow, QDialog {\n"
"  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #ADD8E6);\n"
"}\n"
"QScrollArea#scroll { background: transparent; }\n"
"QWidget#page { background: transparent; }\n"
"\n"
"QMenu {\n"
"  background: #101c2c;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 10px;\n"
"  padding: 6px;\n"
"}\n"
"QMenu::item {\n"
"  padding: 8px 14px;\n"
"  color: #e6eef6;\n"
"  border-radius: 8px;\n"
"}\n"
"QMenu::item:selected {\n"
"  background: #18304a;\n"
"}\n"
"QMenu::item:checked {\n"
"  color: #39c0fa;\n"
"  background: rgba(57,192,250,22);\n"
"}\n"
"\n"
"QFrame#sidebar {\n"
"  background: #0f1922;\n"
"  border: none;\n"
"}\n"
"QLabel#logo {\n"
"  background: #ffffff; color: #132437;\n"
"  border: 2px solid #ffffff; border-radius: 39px; padding: 0px;\n"
"}\n"
"QLabel#brandTitle {\n"
"  color: #e6eef6; font-size: 14px; font-weight: 800; letter-spacing: 2px;\n"
"}\n"
"\n"
"QPushButton#sideBtn0, QPushButton#sideBtn1, QPushButton#sideBtn2,\n"
"QPushButton#sideBtn3,"
                        " QPushButton#sideBtn4, QPushButton#sideBtn5 {\n"
"  text-align: left; padding-left: 18px; color: #b8c9dc;\n"
"  background: transparent; border: none;\n"
"  border-radius: 8px; font-size: 13px; font-weight: 500;\n"
"  min-height: 42px;\n"
"}\n"
"QPushButton#sideBtn0:hover, QPushButton#sideBtn1:hover, QPushButton#sideBtn2:hover,\n"
"QPushButton#sideBtn3:hover, QPushButton#sideBtn4:hover, QPushButton#sideBtn5:hover {\n"
"  background: #132437; color: #39c0fa; font-weight: 600;\n"
"}\n"
"QPushButton#sideBtn0:checked, QPushButton#sideBtn1:checked, QPushButton#sideBtn2:checked,\n"
"QPushButton#sideBtn3:checked, QPushButton#sideBtn4:checked, QPushButton#sideBtn5:checked {\n"
"  color: #39c0fa; background: #132437; border-left: 3px solid #39c0fa; font-weight: 600;\n"
"}\n"
"QPushButton#sideBtn0:pressed, QPushButton#sideBtn1:pressed, QPushButton#sideBtn2:pressed,\n"
"QPushButton#sideBtn3:pressed, QPushButton#sideBtn4:pressed, QPushButton#sideBtn5:pressed {\n"
"  background: #1a2f45;\n"
"}\n"
"\n"
"QLabel#headerH1 { co"
                        "lor: #132437; font-size: 18px; font-weight: 700; }\n"
"\n"
"QLabel#sectionTitle, QLabel#tableTitle, QLabel#advTitle, QLabel#searchTitle, QLabel#sortTitle,\n"
"QLabel#exportTitle, QLabel#statsTitle, QLabel#revTitle, QLabel#occTitle,\n"
"QLabel#histTitle, QLabel#rotTitle, QLabel#revHistTitle {\n"
"  color: #39c0fa; font-size: 18px; font-weight: 700;\n"
"}\n"
"\n"
"QLabel#mapCaption, QLabel#advSub, QLabel#labRef, QLabel#labShip,\n"
"QLabel#labShipType, QLabel#labNavireName, QLabel#proposeResult, QLabel#mapHint, QLabel#histHint,\n"
"QLabel#rotSub, QLabel#revHistHint, QLabel#periodLabel, QLabel#pickLab,\n"
"QLabel#rotationOcc, QLabel#rotationLibre {\n"
"  color: #9bb0c3;\n"
"}\n"
"\n"
"QToolButton#menuButton {\n"
"  color: #e6eef6; background: #132437;\n"
"  border: 1px solid #23374e; border-radius: 10px; font-size: 18px;\n"
"}\n"
"QToolButton#menuButton:hover { background: #18304a; }\n"
"\n"
"QFrame#actionsCard, QFrame#tableCard, QFrame#statsCard {\n"
"  background: #132437; border: 1px solid #23374e; border-radiu"
                        "s: 16px;\n"
"}\n"
"\n"
"QFrame#advancedCard {\n"
"  background: transparent;\n"
"  border: 0px;\n"
"}\n"
"QFrame#searchCard, QFrame#sortCard, QFrame#exportCard,\n"
"QFrame#revCard, QFrame#occCard, QFrame#histCard,\n"
"QFrame#rotCard, QFrame#revHistCard {\n"
"  background: #132437; border: 1px solid #23374e; border-radius: 12px;\n"
"}\n"
"\n"
"QFrame#mapBox {\n"
"  background: #101c2c; border: 1px solid #23374e; border-radius: 12px;\n"
"}\n"
"\n"
"QLineEdit#searchQuai, QLineEdit#searchDate, QLineEdit#traceQuai,\n"
"QComboBox#searchEtat, QComboBox#sortMode, QComboBox#exportType,\n"
"QComboBox#revenuePeriod, QComboBox#shipType, QComboBox#proposeNavire,\n"
"QDateTimeEdit#availAt, QDateTimeEdit#rangeStart, QDateTimeEdit#rangeEnd,\n"
"QDoubleSpinBox#minPrice, QDoubleSpinBox#maxPrice,\n"
"QDoubleSpinBox#refX, QDoubleSpinBox#refY, QDoubleSpinBox#shipLength {\n"
"  background: #101c2c; border: 1px solid #23374e; border-radius: 8px;\n"
"  min-height: 44px; font-size: 15px; padding: 8px 14px;\n"
"  color: #e6eef6; select"
                        "ion-background-color: #1e8de0;\n"
"}\n"
"QLineEdit#searchQuai:disabled, QLineEdit#searchDate:disabled, QLineEdit#traceQuai:disabled,\n"
"QComboBox#searchEtat:disabled, QComboBox#sortMode:disabled, QComboBox#exportType:disabled,\n"
"QComboBox#revenuePeriod:disabled, QComboBox#shipType:disabled, QComboBox#proposeNavire:disabled,\n"
"QDateTimeEdit#availAt:disabled, QDateTimeEdit#rangeStart:disabled, QDateTimeEdit#rangeEnd:disabled,\n"
"QDoubleSpinBox#minPrice:disabled, QDoubleSpinBox#maxPrice:disabled,\n"
"QDoubleSpinBox#refX:disabled, QDoubleSpinBox#refY:disabled, QDoubleSpinBox#shipLength:disabled {\n"
"  color: rgba(230,238,246,120); background: rgba(16,28,44,160);\n"
"}\n"
"\n"
"QCheckBox { color: #9bb0c3; spacing: 10px; font-size: 14px; }\n"
"QCheckBox::indicator {\n"
"  width: 16px; height: 16px; border-radius: 4px;\n"
"  background: #101c2c; border: 1px solid #23374e;\n"
"}\n"
"QCheckBox::indicator:checked {\n"
"  background: #1e8de0; border: 1px solid #1e8de0;\n"
"}\n"
"QComboBox::drop-down { border: 0px;"
                        " width: 20px; }\n"
"\n"
"/* Ensure the NAVIRE proposal combobox always matches the dark inputs,\n"
"   including the popup list and (if it ever becomes editable) its line edit. */\n"
"QComboBox#proposeNavire {\n"
"  background: #101c2c;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  min-height: 44px;\n"
"  font-size: 15px;\n"
"  padding: 8px 14px;\n"
"  color: #e6eef6;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QComboBox#proposeNavire QAbstractItemView {\n"
"  background: #101c2c;\n"
"  color: #e6eef6;\n"
"  border: 1px solid #23374e;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QComboBox#proposeNavire QLineEdit {\n"
"  background: transparent;\n"
"  border: 0px;\n"
"  color: #e6eef6;\n"
"}\n"
"QComboBox#proposeNavire:disabled {\n"
"  color: rgba(230,238,246,120);\n"
"  background: rgba(16,28,44,160);\n"
"}\n"
"QComboBox#proposeNavire QLineEdit:disabled {\n"
"  color: rgba(230,238,246,120);\n"
"}\n"
"\n"
"QPushButton#btnCreate, QPushButton#btnProposeFit, QPushButton#btnPdf {"
                        "\n"
"  background: #1e8de0; color: white; border: 0px;\n"
"  border-radius: 10px; padding: 9px 14px; font-weight: 700;\n"
"}\n"
"QPushButton#btnCreate:hover, QPushButton#btnProposeFit:hover, QPushButton#btnPdf:hover {\n"
"  background: #2a99ec;\n"
"}\n"
"\n"
"QPushButton#btnUpdate, QPushButton#btnProposeCheapest, QPushButton#btnProposeNearest {\n"
"  background: #23374e; color: #e6eef6; border: 1px solid #2b4561;\n"
"  border-radius: 10px; padding: 9px 14px;\n"
"}\n"
"QPushButton#btnUpdate:hover, QPushButton#btnProposeCheapest:hover, QPushButton#btnProposeNearest:hover {\n"
"  background: #2b4561;\n"
"}\n"
"\n"
"QPushButton#btnDelete {\n"
"  background: #39c0fa; color: #0c1a29; border: 0px;\n"
"  border-radius: 10px; padding: 9px 14px; font-weight: 700;\n"
"}\n"
"QPushButton#btnDelete:hover { background: #56cdfc; }\n"
"\n"
"QTableView#table, QTableWidget#table,\n"
"QTableView#revByQuaiTable, QTableWidget#revByQuaiTable,\n"
"QTableView#occByQuaiTable, QTableWidget#occByQuaiTable,\n"
"QTableView#occHistoryTable,"
                        " QTableWidget#occHistoryTable,\n"
"QTableView#revHistoryTable, QTableWidget#revHistoryTable {\n"
"  background: transparent; color: #e6eef6; border: 0px;\n"
"  selection-background-color: #1e3c5c; selection-color: #e6eef6; font-size: 16px;\n"
"}\n"
"QTableView::item:selected { background: #1e8de0; color: #ffffff; }\n"
"QTableView::item:selected:!active { background: #1769a8; color: #ffffff; }\n"
"QTableView::item:focus { outline: none; border: 0px; }\n"
"QTableView:focus { outline: none; }\n"
"QHeaderView::section {\n"
"  background: #101c2c; color: #9bb0c3; border: 0px;\n"
"  padding: 10px 12px; font-weight: 700; font-size: 15px;\n"
"}\n"
"QTableCornerButton::section { background: #101c2c; border: 0px; }\n"
"QTableView::item { padding: 10px 12px; border-radius: 6px; }\n"
"\n"
"QLabel#HeaderH1 { color: #132437; font-size: 18px; font-weight: 700; }\n"
"QDialog QLabel#Muted { color: #5c6e80; font-size: 13px; }\n"
"QDialog QLabel#FormLabel { color: #132437; font-weight: 700; font-size: 13px; }\n"
"QDialog QLabel#"
                        "ErrorText { color: #e74c3c; font-size: 12px; }\n"
"\n"
"QDialog QLineEdit#Input,\n"
"QDialog QDoubleSpinBox#Input,\n"
"QDialog QDateTimeEdit#Input,\n"
"QDialog QComboBox#Input {\n"
"  background: #132437;\n"
"  border: 1px solid #23374e;\n"
"  border-radius: 8px;\n"
"  min-height: 36px;\n"
"  padding: 4px 10px;\n"
"  font-size: 13px;\n"
"  color: #e6eef6;\n"
"  selection-background-color: #1e8de0;\n"
"}\n"
"QDialog QLineEdit#Input:focus,\n"
"QDialog QDoubleSpinBox#Input:focus,\n"
"QDialog QDateTimeEdit#Input:focus,\n"
"QDialog QComboBox#Input:focus {\n"
"  border-color: #1e8de0;\n"
"}\n"
"\n"
"QPushButton#AltPrimaryButton {\n"
"  background: transparent;\n"
"  color: #1e8de0;\n"
"  border: 2px solid #1e8de0;\n"
"  border-radius: 10px;\n"
"  padding: 9px 14px;\n"
"  font-weight: 700;\n"
"  font-size: 14px;\n"
"}\n"
"QPushButton#AltPrimaryButton:hover {\n"
"  background: rgba(30,141,224,30);\n"
"}\n"
"QPushButton#AltPrimaryButton:pressed {\n"
"  background: rgba(30,141,224,60);\n"
"}\n"
"\n"
"QPushButton#Primary"
                        "Button {\n"
"  background: #1e8de0;\n"
"  color: #ffffff;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 14px;\n"
"  font-weight: 700;\n"
"  font-size: 14px;\n"
"}\n"
"QPushButton#PrimaryButton:hover {\n"
"  background: #2a99ec;\n"
"}\n"
"QPushButton#PrimaryButton:pressed {\n"
"  background: #1a7cc5;\n"
"}\n"
"\n"
"QPushButton#DangerButton {\n"
"  background: #e74c3c;\n"
"  color: #ffffff;\n"
"  border: 0px;\n"
"  border-radius: 10px;\n"
"  padding: 9px 14px;\n"
"  font-weight: 700;\n"
"  font-size: 14px;\n"
"}\n"
"QPushButton#DangerButton:hover {\n"
"  background: #f05a4a;\n"
"}\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        rootLayout = new QHBoxLayout(centralwidget);
        rootLayout->setSpacing(0);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QFrame(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(270, 0));
        sidebar->setMaximumSize(QSize(270, 16777215));
        sidebar->setFrameShape(QFrame::NoFrame);
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(10);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(18, 18, 18, 18);
        brandBox = new QWidget(sidebar);
        brandBox->setObjectName("brandBox");
        brandBoxLayout = new QVBoxLayout(brandBox);
        brandBoxLayout->setSpacing(8);
        brandBoxLayout->setObjectName("brandBoxLayout");
        brandBoxLayout->setContentsMargins(0, 0, 0, 0);
        logo = new QLabel(brandBox);
        logo->setObjectName("logo");
        logo->setMinimumSize(QSize(78, 78));
        logo->setMaximumSize(QSize(78, 78));
        logo->setAlignment(Qt::AlignCenter);
        logo->setPixmap(QPixmap(QString::fromUtf8(":/logo_circle.png")));
        logo->setScaledContents(true);

        brandBoxLayout->addWidget(logo, 0, Qt::AlignHCenter);

        brandTitle = new QLabel(brandBox);
        brandTitle->setObjectName("brandTitle");
        brandTitle->setAlignment(Qt::AlignCenter);

        brandBoxLayout->addWidget(brandTitle, 0, Qt::AlignHCenter);


        sidebarLayout->addWidget(brandBox);

        sidebarSpacer1 = new QSpacerItem(0, 12, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(sidebarSpacer1);

        sideBtn0 = new QPushButton(sidebar);
        sideBtn0->setObjectName("sideBtn0");
        sideBtn0->setMinimumSize(QSize(0, 42));
        sideBtn0->setCheckable(true);
        sideBtn0->setAutoExclusive(true);
        sideBtn0->setChecked(false);
        sideBtn0->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn0);

        sideBtn1 = new QPushButton(sidebar);
        sideBtn1->setObjectName("sideBtn1");
        sideBtn1->setMinimumSize(QSize(0, 42));
        sideBtn1->setCheckable(true);
        sideBtn1->setAutoExclusive(true);
        sideBtn1->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn1);

        sideBtn2 = new QPushButton(sidebar);
        sideBtn2->setObjectName("sideBtn2");
        sideBtn2->setMinimumSize(QSize(0, 42));
        sideBtn2->setCheckable(true);
        sideBtn2->setAutoExclusive(true);
        sideBtn2->setChecked(true);
        sideBtn2->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn2);

        sideBtn3 = new QPushButton(sidebar);
        sideBtn3->setObjectName("sideBtn3");
        sideBtn3->setMinimumSize(QSize(0, 42));
        sideBtn3->setCheckable(true);
        sideBtn3->setAutoExclusive(true);
        sideBtn3->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn3);

        sideBtn4 = new QPushButton(sidebar);
        sideBtn4->setObjectName("sideBtn4");
        sideBtn4->setMinimumSize(QSize(0, 42));
        sideBtn4->setCheckable(true);
        sideBtn4->setAutoExclusive(true);
        sideBtn4->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn4);

        sideBtn5 = new QPushButton(sidebar);
        sideBtn5->setObjectName("sideBtn5");
        sideBtn5->setMinimumSize(QSize(0, 42));
        sideBtn5->setCheckable(true);
        sideBtn5->setAutoExclusive(true);
        sideBtn5->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        sidebarLayout->addWidget(sideBtn5);

        sidebarStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarStretch);


        rootLayout->addWidget(sidebar);

        scroll = new QScrollArea(centralwidget);
        scroll->setObjectName("scroll");
        scroll->setWidgetResizable(true);
        scroll->setFrameShape(QFrame::NoFrame);
        page = new QWidget();
        page->setObjectName("page");
        page->setGeometry(QRect(0, 0, 1090, 4000));
        pageLayout = new QVBoxLayout(page);
        pageLayout->setSpacing(14);
        pageLayout->setObjectName("pageLayout");
        pageLayout->setContentsMargins(24, 18, 24, 24);
        headerRow = new QHBoxLayout();
        headerRow->setObjectName("headerRow");
        titleCol = new QVBoxLayout();
        titleCol->setSpacing(2);
        titleCol->setObjectName("titleCol");
        headerH1 = new QLabel(page);
        headerH1->setObjectName("headerH1");
        QFont font;
        font.setBold(true);
        headerH1->setFont(font);

        titleCol->addWidget(headerH1);


        headerRow->addLayout(titleCol);

        headerSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerRow->addItem(headerSpacer);

        menuButton = new QToolButton(page);
        menuButton->setObjectName("menuButton");
        menuButton->setMinimumSize(QSize(36, 36));
        menuButton->setMaximumSize(QSize(36, 36));
        menuButton->setIconSize(QSize(18, 18));

        headerRow->addWidget(menuButton);


        pageLayout->addLayout(headerRow);

        actionsCard = new QFrame(page);
        actionsCard->setObjectName("actionsCard");
        actionsCard->setMinimumSize(QSize(0, 170));
        actionsCard->setFrameShape(QFrame::NoFrame);
        actionsCardOuter = new QVBoxLayout(actionsCard);
        actionsCardOuter->setSpacing(18);
        actionsCardOuter->setObjectName("actionsCardOuter");
        actionsCardOuter->setContentsMargins(18, 18, 18, 18);
        actionsTopRow = new QHBoxLayout();
        actionsTopRow->setSpacing(18);
        actionsTopRow->setObjectName("actionsTopRow");
        actionsTopRow->setContentsMargins(0, 0, 0, 0);
        actionsLeftCol = new QVBoxLayout();
        actionsLeftCol->setSpacing(10);
        actionsLeftCol->setObjectName("actionsLeftCol");
        mapBox = new QFrame(actionsCard);
        mapBox->setObjectName("mapBox");
        mapBox->setMinimumSize(QSize(0, 420));
        mapBox->setFrameShape(QFrame::NoFrame);
        mapBoxLayout = new QVBoxLayout(mapBox);
        mapBoxLayout->setObjectName("mapBoxLayout");
        mapBoxLayout->setContentsMargins(12, 12, 12, 12);
        miniMap = new MiniMapWidget(mapBox);
        miniMap->setObjectName("miniMap");
        miniMap->setMinimumSize(QSize(0, 260));

        mapBoxLayout->addWidget(miniMap);

        mapCaption = new QLabel(mapBox);
        mapCaption->setObjectName("mapCaption");
        mapCaption->setAlignment(Qt::AlignCenter);

        mapBoxLayout->addWidget(mapCaption);

        mapHint = new QLabel(mapBox);
        mapHint->setObjectName("mapHint");
        mapHint->setAlignment(Qt::AlignCenter);

        mapBoxLayout->addWidget(mapHint);


        actionsLeftCol->addWidget(mapBox);

        sectionTitle = new QLabel(actionsCard);
        sectionTitle->setObjectName("sectionTitle");

        actionsLeftCol->addWidget(sectionTitle);

        actionsLeftStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        actionsLeftCol->addItem(actionsLeftStretch);

        actionsLeftCol->setStretch(2, 1);

        actionsTopRow->addLayout(actionsLeftCol);

        actionsTopRow->setStretch(0, 1);

        actionsCardOuter->addLayout(actionsTopRow);

        advancedCard = new QFrame(actionsCard);
        advancedCard->setObjectName("advancedCard");
        advancedCard->setFrameShape(QFrame::NoFrame);
        proposalRow = new QHBoxLayout(advancedCard);
        proposalRow->setSpacing(18);
        proposalRow->setObjectName("proposalRow");
        proposalRow->setContentsMargins(0, 28, 0, 0);
        advLeft = new QVBoxLayout();
        advLeft->setSpacing(10);
        advLeft->setObjectName("advLeft");
        advTitle = new QLabel(advancedCard);
        advTitle->setObjectName("advTitle");

        advLeft->addWidget(advTitle);

        advSub = new QLabel(advancedCard);
        advSub->setObjectName("advSub");
        advSub->setWordWrap(true);

        advLeft->addWidget(advSub);

        advGrid = new QGridLayout();
        advGrid->setObjectName("advGrid");
        advGrid->setHorizontalSpacing(12);
        advGrid->setVerticalSpacing(10);
        advGrid->setContentsMargins(0, 0, 0, 0);
        labNavireName = new QLabel(advancedCard);
        labNavireName->setObjectName("labNavireName");

        advGrid->addWidget(labNavireName, 0, 0, 1, 1);

        proposeNavire = new QComboBox(advancedCard);
        proposeNavire->setObjectName("proposeNavire");

        advGrid->addWidget(proposeNavire, 0, 1, 1, 1);

        chkRef = new QCheckBox(advancedCard);
        chkRef->setObjectName("chkRef");
        chkRef->setChecked(true);

        advGrid->addWidget(chkRef, 1, 0, 1, 1);

        refRow = new QWidget(advancedCard);
        refRow->setObjectName("refRow");
        refRowLayout = new QHBoxLayout(refRow);
        refRowLayout->setSpacing(10);
        refRowLayout->setObjectName("refRowLayout");
        refRowLayout->setContentsMargins(0, 0, 0, 0);
        refX = new QDoubleSpinBox(refRow);
        refX->setObjectName("refX");
        refX->setMinimum(-1000000000.000000000000000);
        refX->setMaximum(1000000000.000000000000000);
        refX->setDecimals(2);
        refX->setValue(0.000000000000000);
        refX->setButtonSymbols(QAbstractSpinBox::NoButtons);
        refX->setPrefix(QString::fromUtf8("X "));

        refRowLayout->addWidget(refX);

        refY = new QDoubleSpinBox(refRow);
        refY->setObjectName("refY");
        refY->setMinimum(-1000000000.000000000000000);
        refY->setMaximum(1000000000.000000000000000);
        refY->setDecimals(2);
        refY->setValue(0.000000000000000);
        refY->setButtonSymbols(QAbstractSpinBox::NoButtons);
        refY->setPrefix(QString::fromUtf8("Y "));

        refRowLayout->addWidget(refY);


        advGrid->addWidget(refRow, 1, 1, 1, 1);

        chkShipLength = new QCheckBox(advancedCard);
        chkShipLength->setObjectName("chkShipLength");
        chkShipLength->setChecked(true);

        advGrid->addWidget(chkShipLength, 2, 0, 1, 1);

        shipLength = new QDoubleSpinBox(advancedCard);
        shipLength->setObjectName("shipLength");
        shipLength->setMinimum(0.000000000000000);
        shipLength->setMaximum(1000000.000000000000000);
        shipLength->setDecimals(1);
        shipLength->setSingleStep(1.000000000000000);
        shipLength->setValue(40.000000000000000);
        shipLength->setButtonSymbols(QAbstractSpinBox::NoButtons);

        advGrid->addWidget(shipLength, 2, 1, 1, 1);

        chkShipType = new QCheckBox(advancedCard);
        chkShipType->setObjectName("chkShipType");
        chkShipType->setChecked(true);

        advGrid->addWidget(chkShipType, 3, 0, 1, 1);

        shipType = new QComboBox(advancedCard);
        shipType->addItem(QString());
        shipType->addItem(QString());
        shipType->addItem(QString());
        shipType->addItem(QString());
        shipType->addItem(QString());
        shipType->addItem(QString());
        shipType->setObjectName("shipType");

        advGrid->addWidget(shipType, 3, 1, 1, 1);


        advLeft->addLayout(advGrid);

        advBtnRow = new QHBoxLayout();
        advBtnRow->setSpacing(10);
        advBtnRow->setObjectName("advBtnRow");
        btnProposeCheapest = new QPushButton(advancedCard);
        btnProposeCheapest->setObjectName("btnProposeCheapest");
        btnProposeCheapest->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        advBtnRow->addWidget(btnProposeCheapest);

        btnProposeNearest = new QPushButton(advancedCard);
        btnProposeNearest->setObjectName("btnProposeNearest");
        btnProposeNearest->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        advBtnRow->addWidget(btnProposeNearest);

        btnProposeFit = new QPushButton(advancedCard);
        btnProposeFit->setObjectName("btnProposeFit");
        btnProposeFit->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        advBtnRow->addWidget(btnProposeFit);

        advBtnSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        advBtnRow->addItem(advBtnSpacer);


        advLeft->addLayout(advBtnRow);

        proposeResult = new QLabel(advancedCard);
        proposeResult->setObjectName("proposeResult");
        proposeResult->setWordWrap(true);

        advLeft->addWidget(proposeResult);

        advLeftStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        advLeft->addItem(advLeftStretch);

        advLeft->setStretch(5, 1);

        proposalRow->addLayout(advLeft);

        proposalRow->setStretch(0, 1);

        actionsCardOuter->addWidget(advancedCard);

        actionsCardOuter->setStretch(1, 1);

        pageLayout->addWidget(actionsCard);

        toolsCol = new QVBoxLayout();
        toolsCol->setSpacing(12);
        toolsCol->setObjectName("toolsCol");
        searchCard = new QFrame(page);
        searchCard->setObjectName("searchCard");
        searchCard->setMinimumSize(QSize(0, 380));
        searchCard->setFrameShape(QFrame::NoFrame);
        searchOuter = new QVBoxLayout(searchCard);
        searchOuter->setSpacing(8);
        searchOuter->setObjectName("searchOuter");
        searchOuter->setContentsMargins(14, 12, 14, 12);
        searchTitle = new QLabel(searchCard);
        searchTitle->setObjectName("searchTitle");

        searchOuter->addWidget(searchTitle);

        searchGrid = new QGridLayout();
        searchGrid->setObjectName("searchGrid");
        searchGrid->setHorizontalSpacing(14);
        searchGrid->setVerticalSpacing(8);
        searchGrid->setContentsMargins(0, 0, 0, 0);
        searchQuai = new QLineEdit(searchCard);
        searchQuai->setObjectName("searchQuai");

        searchGrid->addWidget(searchQuai, 0, 0, 1, 2);

        searchDate = new QLineEdit(searchCard);
        searchDate->setObjectName("searchDate");

        searchGrid->addWidget(searchDate, 1, 0, 1, 1);

        searchEtat = new QComboBox(searchCard);
        searchEtat->addItem(QString());
        searchEtat->addItem(QString());
        searchEtat->addItem(QString());
        searchEtat->addItem(QString());
        searchEtat->setObjectName("searchEtat");

        searchGrid->addWidget(searchEtat, 1, 1, 1, 1);

        searchSpacer = new QSpacerItem(0, 2, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        searchGrid->addItem(searchSpacer, 2, 0, 1, 2);

        advancedFilterGrid = new QGridLayout();
        advancedFilterGrid->setObjectName("advancedFilterGrid");
        advancedFilterGrid->setHorizontalSpacing(12);
        advancedFilterGrid->setVerticalSpacing(14);
        advancedFilterGrid->setContentsMargins(0, 0, 0, 0);
        chkPrice = new QCheckBox(searchCard);
        chkPrice->setObjectName("chkPrice");
        chkPrice->setChecked(false);

        advancedFilterGrid->addWidget(chkPrice, 0, 0, 1, 1, Qt::AlignVCenter);

        priceRowWidget = new QWidget(searchCard);
        priceRowWidget->setObjectName("priceRowWidget");
        priceRowLayout = new QHBoxLayout(priceRowWidget);
        priceRowLayout->setSpacing(12);
        priceRowLayout->setObjectName("priceRowLayout");
        priceRowLayout->setContentsMargins(0, 0, 0, 0);
        minPrice = new QDoubleSpinBox(priceRowWidget);
        minPrice->setObjectName("minPrice");
        minPrice->setEnabled(false);
        minPrice->setMinimum(0.000000000000000);
        minPrice->setMaximum(1000000000.000000000000000);
        minPrice->setDecimals(2);
        minPrice->setButtonSymbols(QAbstractSpinBox::NoButtons);

        priceRowLayout->addWidget(minPrice);

        maxPrice = new QDoubleSpinBox(priceRowWidget);
        maxPrice->setObjectName("maxPrice");
        maxPrice->setEnabled(false);
        maxPrice->setMinimum(0.000000000000000);
        maxPrice->setMaximum(1000000000.000000000000000);
        maxPrice->setDecimals(2);
        maxPrice->setButtonSymbols(QAbstractSpinBox::NoButtons);

        priceRowLayout->addWidget(maxPrice);


        advancedFilterGrid->addWidget(priceRowWidget, 0, 1, 1, 1, Qt::AlignVCenter);

        chkAvailAt = new QCheckBox(searchCard);
        chkAvailAt->setObjectName("chkAvailAt");
        chkAvailAt->setChecked(false);

        advancedFilterGrid->addWidget(chkAvailAt, 1, 0, 1, 1, Qt::AlignVCenter);

        availAt = new QDateTimeEdit(searchCard);
        availAt->setObjectName("availAt");
        availAt->setEnabled(false);
        availAt->setCalendarPopup(true);

        advancedFilterGrid->addWidget(availAt, 1, 1, 1, 1, Qt::AlignVCenter);

        chkRange = new QCheckBox(searchCard);
        chkRange->setObjectName("chkRange");
        chkRange->setChecked(false);

        advancedFilterGrid->addWidget(chkRange, 2, 0, 1, 1, Qt::AlignVCenter);

        rangeRowWidget = new QWidget(searchCard);
        rangeRowWidget->setObjectName("rangeRowWidget");
        rangeRowLayout = new QHBoxLayout(rangeRowWidget);
        rangeRowLayout->setSpacing(12);
        rangeRowLayout->setObjectName("rangeRowLayout");
        rangeRowLayout->setContentsMargins(0, 0, 0, 0);
        rangeStart = new QDateTimeEdit(rangeRowWidget);
        rangeStart->setObjectName("rangeStart");
        rangeStart->setEnabled(false);
        rangeStart->setCalendarPopup(true);

        rangeRowLayout->addWidget(rangeStart);

        rangeEnd = new QDateTimeEdit(rangeRowWidget);
        rangeEnd->setObjectName("rangeEnd");
        rangeEnd->setEnabled(false);
        rangeEnd->setCalendarPopup(true);

        rangeRowLayout->addWidget(rangeEnd);


        advancedFilterGrid->addWidget(rangeRowWidget, 2, 1, 1, 1, Qt::AlignVCenter);


        searchGrid->addLayout(advancedFilterGrid, 3, 0, 1, 2);


        searchOuter->addLayout(searchGrid);


        toolsCol->addWidget(searchCard);

        belowRow = new QHBoxLayout();
        belowRow->setSpacing(12);
        belowRow->setObjectName("belowRow");
        sortCard = new QFrame(page);
        sortCard->setObjectName("sortCard");
        sortCard->setMinimumSize(QSize(0, 150));
        sortCard->setFrameShape(QFrame::NoFrame);
        sortCardLayout = new QVBoxLayout(sortCard);
        sortCardLayout->setSpacing(10);
        sortCardLayout->setObjectName("sortCardLayout");
        sortCardLayout->setContentsMargins(16, 14, 16, 14);
        sortTitle = new QLabel(sortCard);
        sortTitle->setObjectName("sortTitle");

        sortCardLayout->addWidget(sortTitle);

        sortMode = new QComboBox(sortCard);
        sortMode->addItem(QString());
        sortMode->addItem(QString());
        sortMode->addItem(QString());
        sortMode->addItem(QString());
        sortMode->addItem(QString());
        sortMode->setObjectName("sortMode");

        sortCardLayout->addWidget(sortMode);

        sortStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sortCardLayout->addItem(sortStretch);

        sortCardLayout->setStretch(2, 1);

        belowRow->addWidget(sortCard);

        exportCard = new QFrame(page);
        exportCard->setObjectName("exportCard");
        exportCard->setMinimumSize(QSize(0, 150));
        exportCard->setFrameShape(QFrame::NoFrame);
        exportCardLayout = new QVBoxLayout(exportCard);
        exportCardLayout->setSpacing(10);
        exportCardLayout->setObjectName("exportCardLayout");
        exportCardLayout->setContentsMargins(16, 14, 16, 14);
        exportTitle = new QLabel(exportCard);
        exportTitle->setObjectName("exportTitle");

        exportCardLayout->addWidget(exportTitle);

        exportType = new QComboBox(exportCard);
        exportType->addItem(QString());
        exportType->addItem(QString());
        exportType->addItem(QString());
        exportType->addItem(QString());
        exportType->setObjectName("exportType");

        exportCardLayout->addWidget(exportType);

        btnPdf = new QPushButton(exportCard);
        btnPdf->setObjectName("btnPdf");
        btnPdf->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        exportCardLayout->addWidget(btnPdf);

        exportStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        exportCardLayout->addItem(exportStretch);

        exportCardLayout->setStretch(3, 1);

        belowRow->addWidget(exportCard);


        toolsCol->addLayout(belowRow);


        pageLayout->addLayout(toolsCol);

        tableCard = new QFrame(page);
        tableCard->setObjectName("tableCard");
        tableCard->setMinimumSize(QSize(0, 360));
        tableCard->setFrameShape(QFrame::NoFrame);
        tableCardLayout = new QVBoxLayout(tableCard);
        tableCardLayout->setObjectName("tableCardLayout");
        tableCardLayout->setContentsMargins(12, 12, 12, 12);
        tableTitle = new QLabel(tableCard);
        tableTitle->setObjectName("tableTitle");

        tableCardLayout->addWidget(tableTitle);

        tableTopRow = new QHBoxLayout();
        tableTopRow->setSpacing(12);
        tableTopRow->setObjectName("tableTopRow");
        selectAll = new QCheckBox(tableCard);
        selectAll->setObjectName("selectAll");
        selectAll->setChecked(false);

        tableTopRow->addWidget(selectAll);

        tableTopSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        tableTopRow->addItem(tableTopSpacer);

        btnRow = new QHBoxLayout();
        btnRow->setSpacing(10);
        btnRow->setObjectName("btnRow");
        btnRow->setContentsMargins(0, 0, 0, 0);
        btnCreate = new QPushButton(tableCard);
        btnCreate->setObjectName("btnCreate");
        btnCreate->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        btnRow->addWidget(btnCreate);

        btnUpdate = new QPushButton(tableCard);
        btnUpdate->setObjectName("btnUpdate");
        btnUpdate->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        btnRow->addWidget(btnUpdate);

        btnDelete = new QPushButton(tableCard);
        btnDelete->setObjectName("btnDelete");
        btnDelete->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        btnRow->addWidget(btnDelete);


        tableTopRow->addLayout(btnRow);


        tableCardLayout->addLayout(tableTopRow);

        table = new QTableWidget(tableCard);
        if (table->columnCount() < 8)
            table->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        table->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        if (table->rowCount() < 3)
            table->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        __qtablewidgetitem8->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 0, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 1, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 2, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        __qtablewidgetitem11->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 3, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        __qtablewidgetitem12->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 4, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        __qtablewidgetitem13->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 5, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        __qtablewidgetitem14->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 6, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        __qtablewidgetitem15->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 7, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        __qtablewidgetitem17->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        __qtablewidgetitem18->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 2, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        __qtablewidgetitem19->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 3, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 4, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        __qtablewidgetitem21->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 5, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        __qtablewidgetitem22->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 6, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        __qtablewidgetitem23->setTextAlignment(Qt::AlignCenter);
        table->setItem(1, 7, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        __qtablewidgetitem24->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        __qtablewidgetitem25->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        __qtablewidgetitem26->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 2, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        __qtablewidgetitem27->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 3, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        __qtablewidgetitem28->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 4, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        __qtablewidgetitem29->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 5, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        __qtablewidgetitem30->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 6, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        __qtablewidgetitem31->setTextAlignment(Qt::AlignCenter);
        table->setItem(2, 7, __qtablewidgetitem31);
        table->setObjectName("table");
        table->setMinimumSize(QSize(0, 320));
        table->setSelectionMode(QAbstractItemView::ExtendedSelection);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setShowGrid(false);
        table->setSortingEnabled(true);
        table->setAlternatingRowColors(false);
        table->setWordWrap(true);
        table->setTextElideMode(Qt::ElideNone);
        table->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        table->setRowCount(3);
        table->setColumnCount(8);
        table->horizontalHeader()->setVisible(true);
        table->horizontalHeader()->setMinimumSectionSize(70);
        table->horizontalHeader()->setStretchLastSection(true);
        table->verticalHeader()->setVisible(false);
        table->verticalHeader()->setDefaultSectionSize(66);

        tableCardLayout->addWidget(table);

        tableCardLayout->setStretch(2, 1);

        pageLayout->addWidget(tableCard);

        statsCard = new QFrame(page);
        statsCard->setObjectName("statsCard");
        statsCard->setFrameShape(QFrame::NoFrame);
        statsOuter = new QVBoxLayout(statsCard);
        statsOuter->setSpacing(12);
        statsOuter->setObjectName("statsOuter");
        statsOuter->setContentsMargins(16, 14, 16, 14);
        statsHeader = new QHBoxLayout();
        statsHeader->setObjectName("statsHeader");
        statsTitle = new QLabel(statsCard);
        statsTitle->setObjectName("statsTitle");

        statsHeader->addWidget(statsTitle);

        statsHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statsHeader->addItem(statsHeaderSpacer);

        periodLabel = new QLabel(statsCard);
        periodLabel->setObjectName("periodLabel");

        statsHeader->addWidget(periodLabel);

        revenuePeriod = new QComboBox(statsCard);
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->setObjectName("revenuePeriod");
        revenuePeriod->setMaximumSize(QSize(140, 16777215));

        statsHeader->addWidget(revenuePeriod);


        statsOuter->addLayout(statsHeader);

        circlesGrid = new QGridLayout();
        circlesGrid->setObjectName("circlesGrid");
        circlesGrid->setHorizontalSpacing(14);
        circlesGrid->setVerticalSpacing(14);
        phStatEtat = new QFrame(statsCard);
        phStatEtat->setObjectName("phStatEtat");
        phStatEtat->setMinimumSize(QSize(160, 180));
        phStatEtat->setFrameShape(QFrame::NoFrame);
        phStatEtatLayout = new QVBoxLayout(phStatEtat);
        phStatEtatLayout->setSpacing(2);
        phStatEtatLayout->setObjectName("phStatEtatLayout");
        phStatEtatTitle = new QLabel(phStatEtat);
        phStatEtatTitle->setObjectName("phStatEtatTitle");
        phStatEtatTitle->setAlignment(Qt::AlignCenter);

        phStatEtatLayout->addWidget(phStatEtatTitle, 0, Qt::AlignCenter);

        phStatEtatVal = new QLabel(phStatEtat);
        phStatEtatVal->setObjectName("phStatEtatVal");
        phStatEtatVal->setAlignment(Qt::AlignCenter);

        phStatEtatLayout->addWidget(phStatEtatVal, 0, Qt::AlignCenter);


        circlesGrid->addWidget(phStatEtat, 0, 0, 1, 1);

        phStatTauxGlobal = new QFrame(statsCard);
        phStatTauxGlobal->setObjectName("phStatTauxGlobal");
        phStatTauxGlobal->setMinimumSize(QSize(160, 180));
        phStatTauxGlobal->setFrameShape(QFrame::NoFrame);
        phStatTauxLayout = new QVBoxLayout(phStatTauxGlobal);
        phStatTauxLayout->setSpacing(2);
        phStatTauxLayout->setObjectName("phStatTauxLayout");
        phStatTauxTitle = new QLabel(phStatTauxGlobal);
        phStatTauxTitle->setObjectName("phStatTauxTitle");
        phStatTauxTitle->setAlignment(Qt::AlignCenter);

        phStatTauxLayout->addWidget(phStatTauxTitle, 0, Qt::AlignCenter);

        phStatTauxVal = new QLabel(phStatTauxGlobal);
        phStatTauxVal->setObjectName("phStatTauxVal");
        phStatTauxVal->setAlignment(Qt::AlignCenter);

        phStatTauxLayout->addWidget(phStatTauxVal, 0, Qt::AlignCenter);

        phStatTauxSub = new QLabel(phStatTauxGlobal);
        phStatTauxSub->setObjectName("phStatTauxSub");
        phStatTauxSub->setAlignment(Qt::AlignCenter);

        phStatTauxLayout->addWidget(phStatTauxSub, 0, Qt::AlignCenter);


        circlesGrid->addWidget(phStatTauxGlobal, 0, 1, 1, 1);

        phStatRevenuPeriode = new QFrame(statsCard);
        phStatRevenuPeriode->setObjectName("phStatRevenuPeriode");
        phStatRevenuPeriode->setMinimumSize(QSize(160, 180));
        phStatRevenuPeriode->setFrameShape(QFrame::NoFrame);
        phStatRevPLayout = new QVBoxLayout(phStatRevenuPeriode);
        phStatRevPLayout->setSpacing(2);
        phStatRevPLayout->setObjectName("phStatRevPLayout");
        phStatRevPTitle = new QLabel(phStatRevenuPeriode);
        phStatRevPTitle->setObjectName("phStatRevPTitle");
        phStatRevPTitle->setAlignment(Qt::AlignCenter);

        phStatRevPLayout->addWidget(phStatRevPTitle, 0, Qt::AlignCenter);

        phStatRevPVal = new QLabel(phStatRevenuPeriode);
        phStatRevPVal->setObjectName("phStatRevPVal");
        phStatRevPVal->setAlignment(Qt::AlignCenter);

        phStatRevPLayout->addWidget(phStatRevPVal, 0, Qt::AlignCenter);

        phStatRevPSub = new QLabel(phStatRevenuPeriode);
        phStatRevPSub->setObjectName("phStatRevPSub");
        phStatRevPSub->setAlignment(Qt::AlignCenter);

        phStatRevPLayout->addWidget(phStatRevPSub, 0, Qt::AlignCenter);


        circlesGrid->addWidget(phStatRevenuPeriode, 0, 2, 1, 1);

        phStatRevenuMoyen = new QFrame(statsCard);
        phStatRevenuMoyen->setObjectName("phStatRevenuMoyen");
        phStatRevenuMoyen->setMinimumSize(QSize(160, 180));
        phStatRevenuMoyen->setFrameShape(QFrame::NoFrame);
        phStatRevMLayout = new QVBoxLayout(phStatRevenuMoyen);
        phStatRevMLayout->setSpacing(2);
        phStatRevMLayout->setObjectName("phStatRevMLayout");
        phStatRevMTitle = new QLabel(phStatRevenuMoyen);
        phStatRevMTitle->setObjectName("phStatRevMTitle");
        phStatRevMTitle->setAlignment(Qt::AlignCenter);

        phStatRevMLayout->addWidget(phStatRevMTitle, 0, Qt::AlignCenter);

        phStatRevMVal = new QLabel(phStatRevenuMoyen);
        phStatRevMVal->setObjectName("phStatRevMVal");
        phStatRevMVal->setAlignment(Qt::AlignCenter);

        phStatRevMLayout->addWidget(phStatRevMVal, 0, Qt::AlignCenter);

        phStatRevMSub = new QLabel(phStatRevenuMoyen);
        phStatRevMSub->setObjectName("phStatRevMSub");
        phStatRevMSub->setAlignment(Qt::AlignCenter);

        phStatRevMLayout->addWidget(phStatRevMSub, 0, Qt::AlignCenter);


        circlesGrid->addWidget(phStatRevenuMoyen, 0, 3, 1, 1);

        bottomStatsRow = new QHBoxLayout();
        bottomStatsRow->setSpacing(14);
        bottomStatsRow->setObjectName("bottomStatsRow");
        bottomStatsSpacer1 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomStatsRow->addItem(bottomStatsSpacer1);

        phStatPlusRentable = new QFrame(statsCard);
        phStatPlusRentable->setObjectName("phStatPlusRentable");
        phStatPlusRentable->setMinimumSize(QSize(160, 180));
        phStatPlusRentable->setFrameShape(QFrame::NoFrame);
        phStatPRLayout = new QVBoxLayout(phStatPlusRentable);
        phStatPRLayout->setSpacing(2);
        phStatPRLayout->setObjectName("phStatPRLayout");
        phStatPRTitle = new QLabel(phStatPlusRentable);
        phStatPRTitle->setObjectName("phStatPRTitle");
        phStatPRTitle->setAlignment(Qt::AlignCenter);

        phStatPRLayout->addWidget(phStatPRTitle, 0, Qt::AlignCenter);

        phStatPRVal = new QLabel(phStatPlusRentable);
        phStatPRVal->setObjectName("phStatPRVal");
        phStatPRVal->setAlignment(Qt::AlignCenter);

        phStatPRLayout->addWidget(phStatPRVal, 0, Qt::AlignCenter);

        phStatPRSub = new QLabel(phStatPlusRentable);
        phStatPRSub->setObjectName("phStatPRSub");
        phStatPRSub->setAlignment(Qt::AlignCenter);

        phStatPRLayout->addWidget(phStatPRSub, 0, Qt::AlignCenter);


        bottomStatsRow->addWidget(phStatPlusRentable);

        phStatMoinsUtilise = new QFrame(statsCard);
        phStatMoinsUtilise->setObjectName("phStatMoinsUtilise");
        phStatMoinsUtilise->setMinimumSize(QSize(160, 180));
        phStatMoinsUtilise->setFrameShape(QFrame::NoFrame);
        phStatMULayout = new QVBoxLayout(phStatMoinsUtilise);
        phStatMULayout->setSpacing(2);
        phStatMULayout->setObjectName("phStatMULayout");
        phStatMUTitle = new QLabel(phStatMoinsUtilise);
        phStatMUTitle->setObjectName("phStatMUTitle");
        phStatMUTitle->setAlignment(Qt::AlignCenter);

        phStatMULayout->addWidget(phStatMUTitle, 0, Qt::AlignCenter);

        phStatMUVal = new QLabel(phStatMoinsUtilise);
        phStatMUVal->setObjectName("phStatMUVal");
        phStatMUVal->setAlignment(Qt::AlignCenter);

        phStatMULayout->addWidget(phStatMUVal, 0, Qt::AlignCenter);

        phStatMUSub = new QLabel(phStatMoinsUtilise);
        phStatMUSub->setObjectName("phStatMUSub");
        phStatMUSub->setAlignment(Qt::AlignCenter);

        phStatMULayout->addWidget(phStatMUSub, 0, Qt::AlignCenter);


        bottomStatsRow->addWidget(phStatMoinsUtilise);

        bottomStatsSpacer2 = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bottomStatsRow->addItem(bottomStatsSpacer2);

        bottomStatsRow->setStretch(0, 1);
        bottomStatsRow->setStretch(1, 1);
        bottomStatsRow->setStretch(2, 1);
        bottomStatsRow->setStretch(3, 1);

        circlesGrid->addLayout(bottomStatsRow, 1, 0, 1, 4);


        statsOuter->addLayout(circlesGrid);


        pageLayout->addWidget(statsCard);

        perQuaiRow = new QHBoxLayout();
        perQuaiRow->setSpacing(12);
        perQuaiRow->setObjectName("perQuaiRow");
        revCard = new QFrame(page);
        revCard->setObjectName("revCard");
        revCard->setMinimumSize(QSize(0, 240));
        revCard->setFrameShape(QFrame::NoFrame);
        revCardLayout = new QVBoxLayout(revCard);
        revCardLayout->setSpacing(10);
        revCardLayout->setObjectName("revCardLayout");
        revCardLayout->setContentsMargins(16, 14, 16, 14);
        revTitle = new QLabel(revCard);
        revTitle->setObjectName("revTitle");

        revCardLayout->addWidget(revTitle);

        revByQuaiTable = new QTableWidget(revCard);
        if (revByQuaiTable->columnCount() < 2)
            revByQuaiTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        revByQuaiTable->setHorizontalHeaderItem(0, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        revByQuaiTable->setHorizontalHeaderItem(1, __qtablewidgetitem33);
        if (revByQuaiTable->rowCount() < 3)
            revByQuaiTable->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        __qtablewidgetitem34->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(0, 0, __qtablewidgetitem34);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        __qtablewidgetitem35->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(0, 1, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        __qtablewidgetitem36->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(1, 0, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        __qtablewidgetitem37->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(1, 1, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        __qtablewidgetitem38->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(2, 0, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        __qtablewidgetitem39->setTextAlignment(Qt::AlignCenter);
        revByQuaiTable->setItem(2, 1, __qtablewidgetitem39);
        revByQuaiTable->setObjectName("revByQuaiTable");
        revByQuaiTable->setMinimumSize(QSize(0, 160));
        revByQuaiTable->setSelectionMode(QAbstractItemView::NoSelection);
        revByQuaiTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        revByQuaiTable->setShowGrid(false);
        revByQuaiTable->setWordWrap(false);
        revByQuaiTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        revByQuaiTable->setRowCount(3);
        revByQuaiTable->setColumnCount(2);
        revByQuaiTable->horizontalHeader()->setVisible(true);
        revByQuaiTable->horizontalHeader()->setDefaultSectionSize(200);
        revByQuaiTable->horizontalHeader()->setStretchLastSection(true);
        revByQuaiTable->verticalHeader()->setVisible(false);
        revByQuaiTable->verticalHeader()->setDefaultSectionSize(40);

        revCardLayout->addWidget(revByQuaiTable);

        revCardLayout->setStretch(1, 1);

        perQuaiRow->addWidget(revCard);

        occCard = new QFrame(page);
        occCard->setObjectName("occCard");
        occCard->setMinimumSize(QSize(0, 240));
        occCard->setFrameShape(QFrame::NoFrame);
        occCardLayout = new QVBoxLayout(occCard);
        occCardLayout->setSpacing(10);
        occCardLayout->setObjectName("occCardLayout");
        occCardLayout->setContentsMargins(16, 14, 16, 14);
        occTitle = new QLabel(occCard);
        occTitle->setObjectName("occTitle");

        occCardLayout->addWidget(occTitle);

        occByQuaiTable = new QTableWidget(occCard);
        if (occByQuaiTable->columnCount() < 2)
            occByQuaiTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        occByQuaiTable->setHorizontalHeaderItem(0, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        occByQuaiTable->setHorizontalHeaderItem(1, __qtablewidgetitem41);
        if (occByQuaiTable->rowCount() < 3)
            occByQuaiTable->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        __qtablewidgetitem42->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(0, 0, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        __qtablewidgetitem43->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(0, 1, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        __qtablewidgetitem44->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(1, 0, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        __qtablewidgetitem45->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(1, 1, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        __qtablewidgetitem46->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(2, 0, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        __qtablewidgetitem47->setTextAlignment(Qt::AlignCenter);
        occByQuaiTable->setItem(2, 1, __qtablewidgetitem47);
        occByQuaiTable->setObjectName("occByQuaiTable");
        occByQuaiTable->setMinimumSize(QSize(0, 160));
        occByQuaiTable->setSelectionMode(QAbstractItemView::NoSelection);
        occByQuaiTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        occByQuaiTable->setShowGrid(false);
        occByQuaiTable->setWordWrap(false);
        occByQuaiTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        occByQuaiTable->setRowCount(3);
        occByQuaiTable->setColumnCount(2);
        occByQuaiTable->horizontalHeader()->setVisible(true);
        occByQuaiTable->horizontalHeader()->setDefaultSectionSize(200);
        occByQuaiTable->horizontalHeader()->setStretchLastSection(true);
        occByQuaiTable->verticalHeader()->setVisible(false);
        occByQuaiTable->verticalHeader()->setDefaultSectionSize(40);

        occCardLayout->addWidget(occByQuaiTable);

        occCardLayout->setStretch(1, 1);

        perQuaiRow->addWidget(occCard);


        pageLayout->addLayout(perQuaiRow);

        traceRow = new QHBoxLayout();
        traceRow->setSpacing(12);
        traceRow->setObjectName("traceRow");
        histCard = new QFrame(page);
        histCard->setObjectName("histCard");
        histCard->setMinimumSize(QSize(0, 260));
        histCard->setFrameShape(QFrame::NoFrame);
        histCardLayout = new QVBoxLayout(histCard);
        histCardLayout->setSpacing(10);
        histCardLayout->setObjectName("histCardLayout");
        histCardLayout->setContentsMargins(16, 14, 16, 14);
        histTitle = new QLabel(histCard);
        histTitle->setObjectName("histTitle");

        histCardLayout->addWidget(histTitle);

        histHint = new QLabel(histCard);
        histHint->setObjectName("histHint");
        histHint->setWordWrap(true);

        histCardLayout->addWidget(histHint);

        pickRowWidget = new QWidget(histCard);
        pickRowWidget->setObjectName("pickRowWidget");
        pickRowLayout = new QHBoxLayout(pickRowWidget);
        pickRowLayout->setSpacing(12);
        pickRowLayout->setObjectName("pickRowLayout");
        pickRowLayout->setContentsMargins(0, 0, 0, 0);
        pickLab = new QLabel(pickRowWidget);
        pickLab->setObjectName("pickLab");

        pickRowLayout->addWidget(pickLab);

        traceQuai = new QLineEdit(pickRowWidget);
        traceQuai->setObjectName("traceQuai");

        pickRowLayout->addWidget(traceQuai);


        histCardLayout->addWidget(pickRowWidget);

        occHistoryTable = new QTableWidget(histCard);
        if (occHistoryTable->columnCount() < 5)
            occHistoryTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        occHistoryTable->setHorizontalHeaderItem(0, __qtablewidgetitem48);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        occHistoryTable->setHorizontalHeaderItem(1, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        occHistoryTable->setHorizontalHeaderItem(2, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        occHistoryTable->setHorizontalHeaderItem(3, __qtablewidgetitem51);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        occHistoryTable->setHorizontalHeaderItem(4, __qtablewidgetitem52);
        occHistoryTable->setObjectName("occHistoryTable");
        occHistoryTable->setMinimumSize(QSize(0, 160));
        occHistoryTable->setSelectionMode(QAbstractItemView::NoSelection);
        occHistoryTable->setShowGrid(false);
        occHistoryTable->setWordWrap(false);
        occHistoryTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        occHistoryTable->setRowCount(0);
        occHistoryTable->setColumnCount(5);
        occHistoryTable->horizontalHeader()->setVisible(true);
        occHistoryTable->horizontalHeader()->setStretchLastSection(true);
        occHistoryTable->verticalHeader()->setVisible(false);
        occHistoryTable->verticalHeader()->setDefaultSectionSize(40);

        histCardLayout->addWidget(occHistoryTable);

        histCardLayout->setStretch(3, 1);

        traceRow->addWidget(histCard);

        rotCard = new QFrame(page);
        rotCard->setObjectName("rotCard");
        rotCard->setMinimumSize(QSize(0, 260));
        rotCard->setFrameShape(QFrame::NoFrame);
        rotCardLayout = new QVBoxLayout(rotCard);
        rotCardLayout->setSpacing(10);
        rotCardLayout->setObjectName("rotCardLayout");
        rotCardLayout->setContentsMargins(16, 14, 16, 14);
        rotTitle = new QLabel(rotCard);
        rotTitle->setObjectName("rotTitle");

        rotCardLayout->addWidget(rotTitle);

        rotSub = new QLabel(rotCard);
        rotSub->setObjectName("rotSub");
        rotSub->setWordWrap(true);

        rotCardLayout->addWidget(rotSub);

        rotationOcc = new QLabel(rotCard);
        rotationOcc->setObjectName("rotationOcc");
        rotationOcc->setFont(font);

        rotCardLayout->addWidget(rotationOcc);

        rotationLibre = new QLabel(rotCard);
        rotationLibre->setObjectName("rotationLibre");
        rotationLibre->setFont(font);

        rotCardLayout->addWidget(rotationLibre);

        rotStretch = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        rotCardLayout->addItem(rotStretch);

        rotCardLayout->setStretch(4, 1);

        traceRow->addWidget(rotCard);

        traceRow->setStretch(0, 2);
        traceRow->setStretch(1, 1);

        pageLayout->addLayout(traceRow);

        revHistCard = new QFrame(page);
        revHistCard->setObjectName("revHistCard");
        revHistCard->setMinimumSize(QSize(0, 240));
        revHistCard->setFrameShape(QFrame::NoFrame);
        revHistCardLayout = new QVBoxLayout(revHistCard);
        revHistCardLayout->setSpacing(10);
        revHistCardLayout->setObjectName("revHistCardLayout");
        revHistCardLayout->setContentsMargins(16, 14, 16, 14);
        revHistTitle = new QLabel(revHistCard);
        revHistTitle->setObjectName("revHistTitle");

        revHistCardLayout->addWidget(revHistTitle);

        revHistHint = new QLabel(revHistCard);
        revHistHint->setObjectName("revHistHint");
        revHistHint->setWordWrap(true);

        revHistCardLayout->addWidget(revHistHint);

        revHistoryTable = new QTableWidget(revHistCard);
        if (revHistoryTable->columnCount() < 5)
            revHistoryTable->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        revHistoryTable->setHorizontalHeaderItem(0, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        revHistoryTable->setHorizontalHeaderItem(1, __qtablewidgetitem54);
        QTableWidgetItem *__qtablewidgetitem55 = new QTableWidgetItem();
        revHistoryTable->setHorizontalHeaderItem(2, __qtablewidgetitem55);
        QTableWidgetItem *__qtablewidgetitem56 = new QTableWidgetItem();
        revHistoryTable->setHorizontalHeaderItem(3, __qtablewidgetitem56);
        QTableWidgetItem *__qtablewidgetitem57 = new QTableWidgetItem();
        revHistoryTable->setHorizontalHeaderItem(4, __qtablewidgetitem57);
        revHistoryTable->setObjectName("revHistoryTable");
        revHistoryTable->setMinimumSize(QSize(0, 160));
        revHistoryTable->setSelectionMode(QAbstractItemView::NoSelection);
        revHistoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        revHistoryTable->setShowGrid(false);
        revHistoryTable->setWordWrap(false);
        revHistoryTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        revHistoryTable->setRowCount(0);
        revHistoryTable->setColumnCount(5);
        revHistoryTable->horizontalHeader()->setVisible(true);
        revHistoryTable->horizontalHeader()->setStretchLastSection(true);
        revHistoryTable->verticalHeader()->setVisible(false);
        revHistoryTable->verticalHeader()->setDefaultSectionSize(40);

        revHistCardLayout->addWidget(revHistoryTable);

        revHistCardLayout->setStretch(2, 1);

        pageLayout->addWidget(revHistCard);

        pageLayout->setStretch(4, 1);
        scroll->setWidget(page);

        rootLayout->addWidget(scroll);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        revenuePeriod->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        logo->setText(QString());
        brandTitle->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        sideBtn0->setText(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251es", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn0->setToolTip(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251es", nullptr));
#endif // QT_CONFIG(tooltip)
        sideBtn1->setText(QCoreApplication::translate("MainWindow", "Gestion Des Navires", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn1->setToolTip(QCoreApplication::translate("MainWindow", "Gestion Des Navires", nullptr));
#endif // QT_CONFIG(tooltip)
        sideBtn2->setText(QCoreApplication::translate("MainWindow", "Gestion Des Quais", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn2->setToolTip(QCoreApplication::translate("MainWindow", "Gestion Des Quais", nullptr));
#endif // QT_CONFIG(tooltip)
        sideBtn3->setText(QCoreApplication::translate("MainWindow", "Gestion Des Captures", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn3->setToolTip(QCoreApplication::translate("MainWindow", "Gestion Des Captures", nullptr));
#endif // QT_CONFIG(tooltip)
        sideBtn4->setText(QCoreApplication::translate("MainWindow", "Stockage Frigorifique", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn4->setToolTip(QCoreApplication::translate("MainWindow", "Stockage Frigorifique", nullptr));
#endif // QT_CONFIG(tooltip)
        sideBtn5->setText(QCoreApplication::translate("MainWindow", "Gestion Des Ventes", nullptr));
#if QT_CONFIG(tooltip)
        sideBtn5->setToolTip(QCoreApplication::translate("MainWindow", "Gestion Des Ventes", nullptr));
#endif // QT_CONFIG(tooltip)
        headerH1->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        menuButton->setText(QString());
        mapCaption->setText(QCoreApplication::translate("MainWindow", "Occupation des quais", nullptr));
        mapHint->setText(QCoreApplication::translate("MainWindow", "Cliquez sur la mini map pour d\303\251finir la r\303\251f\303\251rence.", nullptr));
        sectionTitle->setText(QCoreApplication::translate("MainWindow", "Infos du quai", nullptr));
        advTitle->setText(QCoreApplication::translate("MainWindow", "Proposition de quai", nullptr));
        advSub->setText(QCoreApplication::translate("MainWindow", "Propositions automatiques: moins cher, plus proche (mini map), et adapt\303\251 au navire.", nullptr));
        labNavireName->setText(QCoreApplication::translate("MainWindow", "Nom navire", nullptr));
        proposeNavire->setPlaceholderText(QCoreApplication::translate("MainWindow", "S\303\251lectionner un navire", nullptr));
        chkRef->setText(QCoreApplication::translate("MainWindow", "R\303\251f\303\251rence (X,Y)", nullptr));
        chkShipLength->setText(QCoreApplication::translate("MainWindow", "Longueur navire", nullptr));
        shipLength->setSuffix(QCoreApplication::translate("MainWindow", " m", nullptr));
        chkShipType->setText(QCoreApplication::translate("MainWindow", "Type navire", nullptr));
        shipType->setItemText(0, QCoreApplication::translate("MainWindow", "Tous", nullptr));
        shipType->setItemText(1, QCoreApplication::translate("MainWindow", "G\303\251n\303\251ral", nullptr));
        shipType->setItemText(2, QCoreApplication::translate("MainWindow", "Cargo", nullptr));
        shipType->setItemText(3, QCoreApplication::translate("MainWindow", "Plaisance", nullptr));
        shipType->setItemText(4, QCoreApplication::translate("MainWindow", "P\303\252che", nullptr));
        shipType->setItemText(5, QCoreApplication::translate("MainWindow", "Ferry", nullptr));

        btnProposeCheapest->setText(QCoreApplication::translate("MainWindow", "Proposer le moins cher", nullptr));
        btnProposeNearest->setText(QCoreApplication::translate("MainWindow", "Proposer le plus proche", nullptr));
        btnProposeFit->setText(QCoreApplication::translate("MainWindow", "Proposer adapt\303\251", nullptr));
        proposeResult->setText(QString());
        searchTitle->setText(QCoreApplication::translate("MainWindow", "Recherche / Filtres", nullptr));
        searchQuai->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par matricule quai", nullptr));
        searchDate->setPlaceholderText(QCoreApplication::translate("MainWindow", "Arriv\303\251e: AAAA-MM-JJ", nullptr));
        searchEtat->setItemText(0, QCoreApplication::translate("MainWindow", "Tous", nullptr));
        searchEtat->setItemText(1, QCoreApplication::translate("MainWindow", "Libre", nullptr));
        searchEtat->setItemText(2, QCoreApplication::translate("MainWindow", "Occup\303\251", nullptr));
        searchEtat->setItemText(3, QCoreApplication::translate("MainWindow", "En maintenance", nullptr));

        chkPrice->setText(QCoreApplication::translate("MainWindow", "Prix/h", nullptr));
        minPrice->setPrefix(QCoreApplication::translate("MainWindow", "Min ", nullptr));
        maxPrice->setPrefix(QCoreApplication::translate("MainWindow", "Max ", nullptr));
        chkAvailAt->setText(QCoreApplication::translate("MainWindow", "Dispo \303\240", nullptr));
        availAt->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd HH:mm", nullptr));
        chkRange->setText(QCoreApplication::translate("MainWindow", "Dispo plage", nullptr));
        rangeStart->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd HH:mm", nullptr));
        rangeEnd->setDisplayFormat(QCoreApplication::translate("MainWindow", "yyyy-MM-dd HH:mm", nullptr));
        sortTitle->setText(QCoreApplication::translate("MainWindow", "Tri", nullptr));
        sortMode->setItemText(0, QCoreApplication::translate("MainWindow", "Par taille de quai", nullptr));
        sortMode->setItemText(1, QCoreApplication::translate("MainWindow", "Statut", nullptr));
        sortMode->setItemText(2, QCoreApplication::translate("MainWindow", "Qui se lib\303\250rent bient\303\264t", nullptr));
        sortMode->setItemText(3, QCoreApplication::translate("MainWindow", "Qui sont occup\303\251s depuis longtemps", nullptr));
        sortMode->setItemText(4, QCoreApplication::translate("MainWindow", "Prix de location par heure", nullptr));

        exportTitle->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        exportType->setItemText(0, QCoreApplication::translate("MainWindow", "Facturation de location de quai", nullptr));
        exportType->setItemText(1, QCoreApplication::translate("MainWindow", "Rapport journalier d'occupation", nullptr));
        exportType->setItemText(2, QCoreApplication::translate("MainWindow", "Rapport mensuel de revenus par quai", nullptr));
        exportType->setItemText(3, QCoreApplication::translate("MainWindow", "Rapport anomalies (retards, d\303\251passements)", nullptr));

        btnPdf->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        tableTitle->setText(QCoreApplication::translate("MainWindow", "Gestion des quais", nullptr));
        selectAll->setText(QCoreApplication::translate("MainWindow", "Tout s\303\251lectionner", nullptr));
        btnCreate->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er", nullptr));
        btnUpdate->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnDelete->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID Quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Matricule quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = table->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Taille (m)", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = table->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Arriv\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = table->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "D\303\251part", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = table->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = table->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Prix (DT/h)", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = table->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "Retard (min)", nullptr));

        const bool __sortingEnabled = table->isSortingEnabled();
        table->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem8 = table->item(0, 0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Q-101", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = table->item(0, 1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "MQ-401", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = table->item(0, 2);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "45,0", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = table->item(0, 3);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "2026-02-01 08:30", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = table->item(0, 4);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "2026-02-01 18:30", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = table->item(0, 5);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "Occup\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = table->item(0, 6);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "120,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = table->item(0, 7);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "120", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = table->item(1, 0);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "Q-102", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = table->item(1, 1);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "MQ-233", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = table->item(1, 2);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "32,5", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = table->item(1, 3);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "2026-02-01 10:15", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = table->item(1, 4);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "2026-02-01 22:00", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = table->item(1, 5);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "Libre", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = table->item(1, 6);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "95,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = table->item(1, 7);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "225", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = table->item(2, 0);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "Q-103", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = table->item(2, 1);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "MQ-778", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = table->item(2, 2);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "60,0", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = table->item(2, 3);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "2026-02-03 06:00", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = table->item(2, 4);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "2026-02-03 20:30", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = table->item(2, 5);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "En maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = table->item(2, 6);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem31 = table->item(2, 7);
        ___qtablewidgetitem31->setText(QCoreApplication::translate("MainWindow", "390", nullptr));
        table->setSortingEnabled(__sortingEnabled);

        statsTitle->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        periodLabel->setText(QCoreApplication::translate("MainWindow", "P\303\251riode:", nullptr));
        revenuePeriod->setItemText(0, QCoreApplication::translate("MainWindow", "24h", nullptr));
        revenuePeriod->setItemText(1, QCoreApplication::translate("MainWindow", "7j", nullptr));
        revenuePeriod->setItemText(2, QCoreApplication::translate("MainWindow", "30j", nullptr));
        revenuePeriod->setItemText(3, QCoreApplication::translate("MainWindow", "Tout", nullptr));

        phStatEtat->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatEtat{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatEtatTitle->setText(QCoreApplication::translate("MainWindow", "Etat", nullptr));
        phStatEtatTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatEtatVal->setText(QCoreApplication::translate("MainWindow", "Occup\303\251s 1\n"
"Libres 1", nullptr));
        phStatEtatVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:16px;font-weight:bold;", nullptr));
        phStatTauxGlobal->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatTauxGlobal{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatTauxTitle->setText(QCoreApplication::translate("MainWindow", "Taux global", nullptr));
        phStatTauxTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatTauxVal->setText(QCoreApplication::translate("MainWindow", "50.0%", nullptr));
        phStatTauxVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:22px;font-weight:bold;", nullptr));
        phStatTauxSub->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        phStatTauxSub->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#8899aa;font-size:11px;", nullptr));
        phStatRevenuPeriode->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatRevenuPeriode{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatRevPTitle->setText(QCoreApplication::translate("MainWindow", "Revenu p\303\251riode", nullptr));
        phStatRevPTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatRevPVal->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        phStatRevPVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:22px;font-weight:bold;", nullptr));
        phStatRevPSub->setText(QCoreApplication::translate("MainWindow", "DT", nullptr));
        phStatRevPSub->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#8899aa;font-size:11px;", nullptr));
        phStatRevenuMoyen->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatRevenuMoyen{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatRevMTitle->setText(QCoreApplication::translate("MainWindow", "Revenu moyen", nullptr));
        phStatRevMTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatRevMVal->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        phStatRevMVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:22px;font-weight:bold;", nullptr));
        phStatRevMSub->setText(QCoreApplication::translate("MainWindow", "/ quai", nullptr));
        phStatRevMSub->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#8899aa;font-size:11px;", nullptr));
        phStatPlusRentable->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatPlusRentable{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatPRTitle->setText(QCoreApplication::translate("MainWindow", "+ rentable", nullptr));
        phStatPRTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatPRVal->setText(QCoreApplication::translate("MainWindow", "MQ-401", nullptr));
        phStatPRVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:22px;font-weight:bold;", nullptr));
        phStatPRSub->setText(QCoreApplication::translate("MainWindow", "0,00 DT", nullptr));
        phStatPRSub->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#8899aa;font-size:11px;", nullptr));
        phStatMoinsUtilise->setStyleSheet(QCoreApplication::translate("MainWindow", "QFrame#phStatMoinsUtilise{background:#253040;border-radius:80px;border:6px solid #39c0fa;}QLabel{color:white;}", nullptr));
        phStatMUTitle->setText(QCoreApplication::translate("MainWindow", "- utilis\303\251", nullptr));
        phStatMUTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#39c0fa;font-size:13px;font-weight:bold;", nullptr));
        phStatMUVal->setText(QCoreApplication::translate("MainWindow", "MQ-401", nullptr));
        phStatMUVal->setStyleSheet(QCoreApplication::translate("MainWindow", "color:white;font-size:22px;font-weight:bold;", nullptr));
        phStatMUSub->setText(QCoreApplication::translate("MainWindow", "0.0%", nullptr));
        phStatMUSub->setStyleSheet(QCoreApplication::translate("MainWindow", "color:#8899aa;font-size:11px;", nullptr));
        revTitle->setText(QCoreApplication::translate("MainWindow", "Historique des revenus par quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = revByQuaiTable->horizontalHeaderItem(0);
        ___qtablewidgetitem32->setText(QCoreApplication::translate("MainWindow", "Matricule quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem33 = revByQuaiTable->horizontalHeaderItem(1);
        ___qtablewidgetitem33->setText(QCoreApplication::translate("MainWindow", "Revenu (DT)", nullptr));

        const bool __sortingEnabled1 = revByQuaiTable->isSortingEnabled();
        revByQuaiTable->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem34 = revByQuaiTable->item(0, 0);
        ___qtablewidgetitem34->setText(QCoreApplication::translate("MainWindow", "MQ-401", nullptr));
        QTableWidgetItem *___qtablewidgetitem35 = revByQuaiTable->item(0, 1);
        ___qtablewidgetitem35->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem36 = revByQuaiTable->item(1, 0);
        ___qtablewidgetitem36->setText(QCoreApplication::translate("MainWindow", "MQ-233", nullptr));
        QTableWidgetItem *___qtablewidgetitem37 = revByQuaiTable->item(1, 1);
        ___qtablewidgetitem37->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem38 = revByQuaiTable->item(2, 0);
        ___qtablewidgetitem38->setText(QCoreApplication::translate("MainWindow", "MQ-778", nullptr));
        QTableWidgetItem *___qtablewidgetitem39 = revByQuaiTable->item(2, 1);
        ___qtablewidgetitem39->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        revByQuaiTable->setSortingEnabled(__sortingEnabled1);

        occTitle->setText(QCoreApplication::translate("MainWindow", "Taux d'occupation par quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem40 = occByQuaiTable->horizontalHeaderItem(0);
        ___qtablewidgetitem40->setText(QCoreApplication::translate("MainWindow", "Matricule quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem41 = occByQuaiTable->horizontalHeaderItem(1);
        ___qtablewidgetitem41->setText(QCoreApplication::translate("MainWindow", "Taux (%)", nullptr));

        const bool __sortingEnabled2 = occByQuaiTable->isSortingEnabled();
        occByQuaiTable->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem42 = occByQuaiTable->item(0, 0);
        ___qtablewidgetitem42->setText(QCoreApplication::translate("MainWindow", "MQ-401", nullptr));
        QTableWidgetItem *___qtablewidgetitem43 = occByQuaiTable->item(0, 1);
        ___qtablewidgetitem43->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        QTableWidgetItem *___qtablewidgetitem44 = occByQuaiTable->item(1, 0);
        ___qtablewidgetitem44->setText(QCoreApplication::translate("MainWindow", "MQ-233", nullptr));
        QTableWidgetItem *___qtablewidgetitem45 = occByQuaiTable->item(1, 1);
        ___qtablewidgetitem45->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        QTableWidgetItem *___qtablewidgetitem46 = occByQuaiTable->item(2, 0);
        ___qtablewidgetitem46->setText(QCoreApplication::translate("MainWindow", "MQ-778", nullptr));
        QTableWidgetItem *___qtablewidgetitem47 = occByQuaiTable->item(2, 1);
        ___qtablewidgetitem47->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        occByQuaiTable->setSortingEnabled(__sortingEnabled2);

        histTitle->setText(QCoreApplication::translate("MainWindow", "Historique des occupations (quai s\303\251lectionn\303\251)", nullptr));
        histHint->setText(QCoreApplication::translate("MainWindow", "Tapez un matricule quai (ex: MQ-) pour voir l'historique.", nullptr));
        pickLab->setText(QCoreApplication::translate("MainWindow", "Matricule quai:", nullptr));
        traceQuai->setPlaceholderText(QCoreApplication::translate("MainWindow", "MQ-", nullptr));
        QTableWidgetItem *___qtablewidgetitem48 = occHistoryTable->horizontalHeaderItem(0);
        ___qtablewidgetitem48->setText(QCoreApplication::translate("MainWindow", "Arriv\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem49 = occHistoryTable->horizontalHeaderItem(1);
        ___qtablewidgetitem49->setText(QCoreApplication::translate("MainWindow", "D\303\251part", nullptr));
        QTableWidgetItem *___qtablewidgetitem50 = occHistoryTable->horizontalHeaderItem(2);
        ___qtablewidgetitem50->setText(QCoreApplication::translate("MainWindow", "Matricule quai", nullptr));
        QTableWidgetItem *___qtablewidgetitem51 = occHistoryTable->horizontalHeaderItem(3);
        ___qtablewidgetitem51->setText(QCoreApplication::translate("MainWindow", "\303\211tat", nullptr));
        QTableWidgetItem *___qtablewidgetitem52 = occHistoryTable->horizontalHeaderItem(4);
        ___qtablewidgetitem52->setText(QCoreApplication::translate("MainWindow", "Prix (DT/h)", nullptr));
        rotTitle->setText(QCoreApplication::translate("MainWindow", "Analyse de rotation (quai s\303\251lectionn\303\251)", nullptr));
        rotSub->setText(QCoreApplication::translate("MainWindow", "Moyennes calcul\303\251es \303\240 partir des occupations (Arriv\303\251e \342\206\222 D\303\251part) tri\303\251es par date.", nullptr));
        rotationOcc->setText(QCoreApplication::translate("MainWindow", "Temps moyen occup\303\251: \342\200\224", nullptr));
        rotationLibre->setText(QCoreApplication::translate("MainWindow", "Temps moyen libre: \342\200\224", nullptr));
        revHistTitle->setText(QCoreApplication::translate("MainWindow", "Historique des revenus (quai s\303\251lectionn\303\251)", nullptr));
        revHistHint->setText(QCoreApplication::translate("MainWindow", "Revenus calcul\303\251s \303\240 partir des occupations (Arriv\303\251e \342\206\222 D\303\251part) et du prix (DT/h).", nullptr));
        QTableWidgetItem *___qtablewidgetitem53 = revHistoryTable->horizontalHeaderItem(0);
        ___qtablewidgetitem53->setText(QCoreApplication::translate("MainWindow", "Arriv\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem54 = revHistoryTable->horizontalHeaderItem(1);
        ___qtablewidgetitem54->setText(QCoreApplication::translate("MainWindow", "D\303\251part", nullptr));
        QTableWidgetItem *___qtablewidgetitem55 = revHistoryTable->horizontalHeaderItem(2);
        ___qtablewidgetitem55->setText(QCoreApplication::translate("MainWindow", "Heures", nullptr));
        QTableWidgetItem *___qtablewidgetitem56 = revHistoryTable->horizontalHeaderItem(3);
        ___qtablewidgetitem56->setText(QCoreApplication::translate("MainWindow", "Prix (DT/h)", nullptr));
        QTableWidgetItem *___qtablewidgetitem57 = revHistoryTable->horizontalHeaderItem(4);
        ___qtablewidgetitem57->setText(QCoreApplication::translate("MainWindow", "Revenu (DT)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
