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
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QCheckBox>
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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *rootLayout;
    QWidget *sidebar;
    QVBoxLayout *sidebarLayout;
    QWidget *brandBox;
    QVBoxLayout *brandBoxLayout;
    QLabel *lblLogo;
    QLabel *lblSidebarTitle;
    QSpacerItem *brandSpacer;
    QPushButton *btnGestionEmployees;
    QPushButton *btnGestionNavires;
    QWidget *naviresMenu;
    QVBoxLayout *naviresMenuLayout;
    QPushButton *btnNaviresAffichage;
    QPushButton *btnNaviresStatistique;
    QPushButton *btnNaviresAvancees;
    QPushButton *btnGestionQuais;
    QPushButton *btnGestionCaptures;
    QPushButton *btnStockageFrigorifique;
    QPushButton *btnGestionVentes;
    QSpacerItem *sidebarBottomSpacer;
    QScrollArea *scrollArea;
    QWidget *scrollContent;
    QVBoxLayout *pageLayout;
    QHBoxLayout *headerLayout;
    QVBoxLayout *headerTitleLayout;
    QLabel *lblAppTitle;
    QLabel *lblAppSubtitle;
    QSpacerItem *headerSpacer;
    QToolButton *btnMenu;
    QFrame *searchCard;
    QVBoxLayout *searchCardLayout;
    QLabel *lblSearchTitle;
    QHBoxLayout *searchRow1;
    QLineEdit *searchType;
    QLineEdit *searchNom;
    QHBoxLayout *searchRow2;
    QLineEdit *searchMatricule;
    QLineEdit *searchPosition;
    QFrame *statsCard;
    QVBoxLayout *statsCardLayout;
    QHBoxLayout *statsHeaderRow;
    QLabel *lblStatsTitle;
    QSpacerItem *statsTitleSpacer;
    QLabel *periodLabel;
    QComboBox *revenuePeriod;
    QHBoxLayout *statsCirclesRow1;
    QFrame *statCircle1;
    QVBoxLayout *circle1Layout;
    QLabel *statCircleTitle1;
    QLabel *statCircleValue1;
    QLabel *statCircleSub1;
    QFrame *statCircle2;
    QVBoxLayout *circle2Layout;
    QLabel *statCircleTitle2;
    QLabel *statCircleValue2;
    QLabel *statCircleSub2;
    QFrame *statCircle3;
    QVBoxLayout *circle3Layout;
    QLabel *statCircleTitle3;
    QLabel *statCircleValue3;
    QLabel *statCircleSub3;
    QHBoxLayout *midRow;
    QFrame *maintenanceCard;
    QVBoxLayout *maintenanceLayout;
    QLabel *lblMaintenanceTitle;
    QTableWidget *tableMaintenance;
    QFrame *revenusCard;
    QVBoxLayout *revenusLayout;
    QLabel *lblRevenusTitle;
    QTableWidget *tableRevenus;
    QHBoxLayout *bottomRow;
    QFrame *triCard;
    QVBoxLayout *triLayout;
    QLabel *lblTriTitle;
    QComboBox *comboSort;
    QFrame *exportCard;
    QVBoxLayout *exportLayout;
    QLabel *lblExportTitle;
    QLineEdit *lineQRCode;
    QPushButton *btnExportPDF;
    QFrame *gestionCard;
    QVBoxLayout *gestionLayout;
    QLabel *lblGestionTitle;
    QFrame *buttonsCard;
    QHBoxLayout *buttonsLayout;
    QPushButton *btnAjouter;
    QPushButton *btnModifier;
    QPushButton *btnSupprimer;
    QSpacerItem *btnSpacer;
    QCheckBox *checkSelectAll;
    QTableWidget *tableNavires;
    QFrame *weatherCard;
    QVBoxLayout *weatherLayout;
    QLabel *lblWeatherTitle;
    QHBoxLayout *weatherInputRow;
    QLineEdit *lineWeatherCity;
    QPushButton *btnFetchWeather;
    QLabel *lblWeatherInfo;
    QLabel *lblWeatherDetails;
    QFrame *calendarCard;
    QVBoxLayout *calendarLayout;
    QHBoxLayout *calendarHeaderRow;
    QLabel *lblCalendarTitle;
    QSpacerItem *calendarHeaderSpacer;
    QLabel *lblSelectedDate;
    QPushButton *btnShowAllMouvements;
    QPushButton *btnAddMouvement;
    QCalendarWidget *calendarMouvements;
    QTableWidget *tableMouvements;
    QSpacerItem *pageBottomSpacer;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1366, 768);
        MainWindow->setMinimumSize(QSize(1100, 700));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/vision_sight.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"/* ===== GLOBAL ===== */\n"
"QMainWindow {\n"
"    background: qlineargradient(\n"
"        x1:0, y1:0, x2:0, y2:1,\n"
"        stop:0 #ffffff,\n"
"        stop:1 #ADD8E6\n"
"    );\n"
"}\n"
"\n"
"/* ===== SCROLL AREA ===== */\n"
"QScrollArea#scrollArea {\n"
"    background: transparent;\n"
"    border: none;\n"
"}\n"
"QScrollArea#scrollArea > QWidget > QWidget {\n"
"    background: transparent;\n"
"}\n"
"QWidget#scrollContent {\n"
"    background: transparent;\n"
"}\n"
"\n"
"/* ===== MENUS (match Quais dropdown) ===== */\n"
"QMenu {\n"
"    background: #101c2c;\n"
"    border: 1px solid #23374e;\n"
"    border-radius: 10px;\n"
"    padding: 6px;\n"
"}\n"
"QMenu::item {\n"
"    padding: 8px 14px;\n"
"    color: #e6eef6;\n"
"    border-radius: 8px;\n"
"}\n"
"QMenu::item:selected {\n"
"    background: #18304a;\n"
"}\n"
"QMenu::item:checked {\n"
"    color: #39c0fa;\n"
"    background: rgba(57,192,250,22);\n"
"}\n"
"\n"
"/* ===== SIDEBAR ===== */\n"
"#sidebar {\n"
"    background: #0f1922;\n"
"    border: no"
                        "ne;\n"
"}\n"
"\n"
"#lblLogo {\n"
"    background: #ffffff;\n"
"    color: #132437;\n"
"    border: 2px solid #ffffff;\n"
"    border-radius: 39px;\n"
"    padding: 0px;\n"
"}\n"
"\n"
"#lblSidebarTitle {\n"
"    color: #e6eef6;\n"
"    font-size: 16px;\n"
"    font-weight: 800;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    letter-spacing: 1px;\n"
"}\n"
"\n"
"#sidebar QPushButton {\n"
"    text-align: left;\n"
"    color: #b8c9dc;\n"
"    background: transparent;\n"
"    border: none;\n"
"    border-radius: 8px;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    padding-left: 18px;\n"
"    margin: 0px;\n"
"}\n"
"\n"
"#sidebar QPushButton:hover {\n"
"    background: #132437;\n"
"    color: #39c0fa;\n"
"    border: none;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"#sidebar QPushButton:checked {\n"
"    color: #39c0fa;\n"
"    background: #132437;\n"
"    border-left: 3px solid #39c0fa;\n"
"    font-weight: 600;\n"
"}\n"
"\n"
"#sidebar QPushButton:pressed {\n"
"    background: #1a2f45;\n"
"}\n"
"\n"
"/* Embe"
                        "dded submenu under \"Gestion Des Navires\" (animated) */\n"
"#naviresMenu QPushButton {\n"
"    text-align: left;\n"
"    padding-left: 28px;\n"
"    color: #e6eef6;\n"
"    background: transparent;\n"
"    border: 1px solid transparent;\n"
"    border-radius: 10px;\n"
"    font-size: 16px;\n"
"    font-weight: 600;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    padding: 8px 14px;\n"
"    margin: 1px 0px;\n"
"}\n"
"#naviresMenu QPushButton:hover {\n"
"    background: #18304a;\n"
"    border-color: #23374e;\n"
"}\n"
"#naviresMenu QPushButton:checked {\n"
"    color: #39c0fa;\n"
"    background: rgba(57,192,250,22);\n"
"    border-color: rgba(57,192,250,90);\n"
"}\n"
"\n"
"/* ===== HEADER ===== */\n"
"#lblAppTitle {\n"
"    color: #132437;\n"
"    font-size: 28px;\n"
"    font-weight: bold;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#lblAppSubtitle {\n"
"    color: #132437;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#btnMenu {\n"
"    color: #e6eef6;\n"
"    back"
                        "ground: #132437;\n"
"    border: 1px solid #23374e;\n"
"    border-radius: 10px;\n"
"    font-size: 18px;\n"
"}\n"
"\n"
"#btnMenu:hover {\n"
"    background: #18304a;\n"
"}\n"
"\n"
"/* ===== CARDS ===== */\n"
"QFrame#buttonsCard, QFrame#searchCard, QFrame#statsCard,\n"
"QFrame#maintenanceCard, QFrame#revenusCard,\n"
"QFrame#triCard, QFrame#exportCard, QFrame#gestionCard,\n"
"QFrame#weatherCard, QFrame#calendarCard, QFrame#mapCard {\n"
"    background: #132437;\n"
"    border: 1px solid #23374e;\n"
"    border-radius: 16px;\n"
"}\n"
"\n"
"/* When buttonsCard is inside gestionCard, remove the extra \"card\" look */\n"
"QFrame#gestionCard QFrame#buttonsCard {\n"
"    background: transparent;\n"
"    border: none;\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
"QCalendarWidget {\n"
"    background: #0D1B2A;\n"
"    color: #e6eef6;\n"
"    border: 1px solid #23374e;\n"
"    border-radius: 12px;\n"
"}\n"
"\n"
"/* Navigation bar */\n"
"QCalendarWidget #qt_calendar_navigationbar {\n"
"    background: #0D1B2A;\n"
"    borde"
                        "r: 0px;\n"
"    padding: 6px;\n"
"    border-top-left-radius: 12px;\n"
"    border-top-right-radius: 12px;\n"
"}\n"
"\n"
"/* Prev/Next month buttons (left/right arrows) */\n"
"QCalendarWidget QToolButton#qt_calendar_prevmonth,\n"
"QCalendarWidget QToolButton#qt_calendar_nextmonth {\n"
"    color: #ffffff;\n"
"    background: #1e8de0;\n"
"    border: 0px;\n"
"    border-radius: 10px;\n"
"    padding: 4px;\n"
"    min-width: 28px;\n"
"    min-height: 28px;\n"
"}\n"
"QCalendarWidget QToolButton#qt_calendar_prevmonth:hover,\n"
"QCalendarWidget QToolButton#qt_calendar_nextmonth:hover {\n"
"    background: #2a99ec;\n"
"}\n"
"\n"
"/* Month / Year controls (center chips) */\n"
"QCalendarWidget QToolButton#qt_calendar_monthbutton,\n"
"QCalendarWidget QToolButton#qt_calendar_yearbutton,\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit {\n"
"    color: #ffffff;\n"
"    background: #1e8de0;\n"
"    border: 0px;\n"
"    border-radius: 10px;\n"
"    padding: 4px 12px;\n"
"    font-weight: 800;\n"
"}\n"
"QCalendarWidget QTo"
                        "olButton#qt_calendar_monthbutton:hover,\n"
"QCalendarWidget QToolButton#qt_calendar_yearbutton:hover,\n"
"QCalendarWidget QSpinBox#qt_calendar_yearedit:hover {\n"
"    background: #2a99ec;\n"
"}\n"
"\n"
"QCalendarWidget QMenu {\n"
"    background: #101c2c;\n"
"    color: #e6eef6;\n"
"    border: 1px solid #23374e;\n"
"}\n"
"\n"
"/* Day names row */\n"
"QCalendarWidget QHeaderView::section {\n"
"    background: #0D1B2A;\n"
"    color: #9bb0c3;\n"
"    border: 1px solid #0b1624;\n"
"    padding: 6px 0px;\n"
"    font-weight: 700;\n"
"}\n"
"\n"
"/* Calendar grid */\n"
"QCalendarWidget QAbstractItemView {\n"
"    background: #101c2c;\n"
"    color: #e6eef6;\n"
"    outline: 0;\n"
"    selection-background-color: #39c0fa;\n"
"    selection-color: #ffffff;\n"
"}\n"
"QCalendarWidget QAbstractItemView::item {\n"
"    border: 1px solid #0b1624;\n"
"    padding: 6px;\n"
"}\n"
"QCalendarWidget QAbstractItemView::item:hover {\n"
"    background: #132437;\n"
"}\n"
"QCalendarWidget QAbstractItemView::item:selected {\n"
"   "
                        " background: #39c0fa;\n"
"    color: #ffffff;\n"
"}\n"
"QCalendarWidget QAbstractItemView:disabled {\n"
"    color: #7b8fa3;\n"
"}\n"
"\n"
"#btnFetchWeather, #btnAddMouvement {\n"
"    background: #1e8de0;\n"
"    color: white;\n"
"    border: 0px;\n"
"    border-radius: 10px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"#btnFetchWeather:hover, #btnAddMouvement:hover {\n"
"    background: #2a99ec;\n"
"}\n"
"\n"
"/* ===== SECTION TITLES ===== */\n"
"#lblSearchTitle, #lblStatsTitle, #lblMaintenanceTitle,\n"
"#lblRevenusTitle, #lblTriTitle, #lblExportTitle, #lblGestionTitle,\n"
"#lblWeatherTitle, #lblCalendarTitle, #lblMapTitle {\n"
"    color: #39c0fa;\n"
"    font-size: 18px;\n"
"    font-weight: 700;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== CARD SUBTITLES ===== */\n"
"#lblMaintenanceSubtitle, #lblRevenusSubtitle, #lblGestionSubtitle,\n"
"#lblWeatherSubtitle, #lblCalendarSubtitle {\n"
"    color: #7b8fa3;\n"
""
                        "    font-size: 11px;\n"
"    font-weight: 400;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    padding-bottom: 2px;\n"
"}\n"
"\n"
"/* ===== MUTED LABELS ===== */\n"
"QLabel[objectName=\"Muted\"] {\n"
"    color: #9bb0c3;\n"
"    font-size: 12px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== FORM LABELS ===== */\n"
"QLabel#lblMuted {\n"
"    color: #9bb0c3;\n"
"}\n"
"\n"
"/* ===== INPUTS ===== */\n"
"QLineEdit, QComboBox, QDateTimeEdit, QDoubleSpinBox, QSpinBox {\n"
"    background: #101c2c;\n"
"    border: 1px solid #23374e;\n"
"    border-radius: 8px;\n"
"    min-height: 28px;\n"
"    font-size: 13px;\n"
"    padding: 4px 10px;\n"
"    color: #e6eef6;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    selection-background-color: #1e8de0;\n"
"}\n"
"\n"
"QLineEdit:focus, QComboBox:focus, QSpinBox:focus {\n"
"    border: 1px solid #1e8de0;\n"
"}\n"
"\n"
"QComboBox::drop-down {\n"
"    background: transparent;\n"
"    border: none;\n"
"    width: 24px;\n"
"    border-top-right-radius: 8px;\n"
"    "
                        "border-bottom-right-radius: 8px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 4px solid transparent;\n"
"    border-right: 4px solid transparent;\n"
"    border-top: 5px solid #9bb0c3;\n"
"    width: 0; height: 0;\n"
"}\n"
"\n"
"QComboBox QAbstractItemView {\n"
"    background: #101c2c;\n"
"    border: 1px solid #23374e;\n"
"    color: #e6eef6;\n"
"    selection-background-color: #1e8de0;\n"
"}\n"
"\n"
"QSpinBox::up-button, QSpinBox::down-button {\n"
"    background: #23374e;\n"
"    border: none;\n"
"    width: 20px;\n"
"}\n"
"\n"
"QSpinBox::up-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-bottom: 5px solid #9bb0c3;\n"
"    width: 0; height: 0;\n"
"}\n"
"\n"
"QSpinBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 5px solid #9bb0c3;\n"
"    width: 0; height: 0;\n"
"}\n"
"\n"
"/* ===== BUTTONS "
                        "===== */\n"
"#btnAjouter {\n"
"    background: #3fc1f9;\n"
"    color: #0b1622;\n"
"    border: 0px solid transparent;\n"
"    border-radius: 10px;\n"
"    padding: 8px 28px;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#btnAjouter:hover {\n"
"    background: #5ed0fc;\n"
"}\n"
"\n"
"#btnModifier {\n"
"    background: #3a4a5c;\n"
"    color: #ffffff;\n"
"    border: 0px solid transparent;\n"
"    border-radius: 10px;\n"
"    padding: 8px 28px;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#btnModifier:hover {\n"
"    background: #455a6f;\n"
"}\n"
"\n"
"#btnSupprimer {\n"
"    background: #3fc1f9;\n"
"    color: #0b1622;\n"
"    border: 0px solid transparent;\n"
"    border-radius: 10px;\n"
"    padding: 8px 28px;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#btnSupprimer:hover {\n"
"    background: #5ed0fc;\n"
"}\n"
"\n"
"#btnExport"
                        "PDF {\n"
"    background: #1e8de0;\n"
"    color: white;\n"
"    border: 0px;\n"
"    border-radius: 10px;\n"
"    padding: 10px 20px;\n"
"    font-weight: 700;\n"
"    font-size: 14px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#btnExportPDF:hover {\n"
"    background: #2a99ec;\n"
"}\n"
"\n"
"/* ===== STAT CIRCLES ===== */\n"
"#statCircle1, #statCircle2, #statCircle3 {\n"
"    background: transparent;\n"
"    border: none;\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
"#statCircleTitle1, #statCircleTitle2, #statCircleTitle3 {\n"
"    color: #39c0fa;\n"
"    font-size: 11px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#statCircleValue1, #statCircleValue2, #statCircleValue3 {\n"
"    color: #e6eef6;\n"
"    font-size: 20px;\n"
"    font-weight: 800;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"#statCircleSub1, #statCircleSub2, #statCircleSub3 {\n"
"    color: #9bb0c3;\n"
"    font-size: 10px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== PERIOD LABEL (stats) ===== "
                        "*/\n"
"#periodLabel {\n"
"    color: #7b8fa3;\n"
"    font-size: 11px;\n"
"    font-weight: 400;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== TABLES ===== */\n"
"QTableWidget {\n"
"    background: transparent;\n"
"    color: #e6eef6;\n"
"    border: 0px;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    selection-background-color: #2196F3;\n"
"    selection-color: #ffffff;\n"
"    gridline-color: transparent;\n"
"    outline: 0;\n"
"}\n"
"\n"
"QTableWidget::item {\n"
"    border-bottom: 1px solid #1a2d44;\n"
"}\n"
"\n"
"QTableWidget::item:selected {\n"
"    background-color: #2196F3;\n"
"    color: #ffffff;\n"
"}\n"
"\n"
"QHeaderView {\n"
"    background: transparent;\n"
"    border: none;\n"
"}\n"
"\n"
"QHeaderView::section {\n"
"    background: #101c2c;\n"
"    color: #9bb0c3;\n"
"    border: none;\n"
"    border-bottom: 2px solid #23374e;\n"
"    font-weight: 700;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== CHECKBOX ====="
                        " */\n"
"QCheckBox {\n"
"    color: #e6eef6;\n"
"    font-size: 13px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"    spacing: 8px;\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"    width: 18px;\n"
"    height: 18px;\n"
"    border: 2px solid #23374e;\n"
"    border-radius: 4px;\n"
"    background: #101c2c;\n"
"}\n"
"\n"
"QCheckBox::indicator:checked {\n"
"    background: #1e8de0;\n"
"    border-color: #1e8de0;\n"
"}\n"
"\n"
"/* ===== ACCENT TEXT ===== */\n"
"#lblAccent {\n"
"    color: #39c0fa;\n"
"    font-size: 12px;\n"
"    font-family: 'Segoe UI', Arial;\n"
"}\n"
"\n"
"/* ===== SCROLLBARS ===== */\n"
"QScrollBar:vertical {\n"
"    background: transparent;\n"
"    width: 8px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"    background: #23374e;\n"
"    border-radius: 4px;\n"
"    min-height: 30px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #2b4561;\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"\n"
"QScrollBar:horizont"
                        "al {\n"
"    background: transparent;\n"
"    height: 8px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"    background: #23374e;\n"
"    border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {\n"
"    width: 0px;\n"
"}\n"
"   "));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        rootLayout = new QHBoxLayout(centralwidget);
        rootLayout->setSpacing(0);
        rootLayout->setObjectName("rootLayout");
        rootLayout->setContentsMargins(0, 0, 0, 0);
        sidebar = new QWidget(centralwidget);
        sidebar->setObjectName("sidebar");
        sidebar->setMinimumSize(QSize(270, 0));
        sidebar->setMaximumSize(QSize(270, 16777215));
        sidebarLayout = new QVBoxLayout(sidebar);
        sidebarLayout->setSpacing(6);
        sidebarLayout->setObjectName("sidebarLayout");
        sidebarLayout->setContentsMargins(18, 18, 18, 18);
        brandBox = new QWidget(sidebar);
        brandBox->setObjectName("brandBox");
        brandBoxLayout = new QVBoxLayout(brandBox);
        brandBoxLayout->setSpacing(8);
        brandBoxLayout->setObjectName("brandBoxLayout");
        brandBoxLayout->setContentsMargins(0, 0, 0, 0);
        lblLogo = new QLabel(brandBox);
        lblLogo->setObjectName("lblLogo");
        lblLogo->setMinimumSize(QSize(78, 78));
        lblLogo->setMaximumSize(QSize(78, 78));
        lblLogo->setAlignment(Qt::AlignmentFlag::AlignCenter);
        lblLogo->setPixmap(QPixmap(QString::fromUtf8(":/logo_circle.png")));
        lblLogo->setScaledContents(true);

        brandBoxLayout->addWidget(lblLogo, 0, Qt::AlignmentFlag::AlignHCenter);

        lblSidebarTitle = new QLabel(brandBox);
        lblSidebarTitle->setObjectName("lblSidebarTitle");
        lblSidebarTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        brandBoxLayout->addWidget(lblSidebarTitle, 0, Qt::AlignmentFlag::AlignHCenter);


        sidebarLayout->addWidget(brandBox);

        brandSpacer = new QSpacerItem(20, 12, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        sidebarLayout->addItem(brandSpacer);

        btnGestionEmployees = new QPushButton(sidebar);
        btnGestionEmployees->setObjectName("btnGestionEmployees");
        btnGestionEmployees->setMinimumSize(QSize(0, 42));
        btnGestionEmployees->setCheckable(false);
        btnGestionEmployees->setChecked(false);

        sidebarLayout->addWidget(btnGestionEmployees);

        btnGestionNavires = new QPushButton(sidebar);
        btnGestionNavires->setObjectName("btnGestionNavires");
        btnGestionNavires->setMinimumSize(QSize(0, 42));
        btnGestionNavires->setCheckable(true);
        btnGestionNavires->setChecked(true);

        sidebarLayout->addWidget(btnGestionNavires);

        naviresMenu = new QWidget(sidebar);
        naviresMenu->setObjectName("naviresMenu");
        naviresMenu->setVisible(false);
        naviresMenu->setMaximumSize(QSize(16777215, 0));
        naviresMenuLayout = new QVBoxLayout(naviresMenu);
        naviresMenuLayout->setSpacing(2);
        naviresMenuLayout->setObjectName("naviresMenuLayout");
        naviresMenuLayout->setContentsMargins(0, 0, 0, 0);
        btnNaviresAffichage = new QPushButton(naviresMenu);
        btnNaviresAffichage->setObjectName("btnNaviresAffichage");
        btnNaviresAffichage->setMinimumSize(QSize(0, 36));
        btnNaviresAffichage->setCheckable(true);
        btnNaviresAffichage->setChecked(true);

        naviresMenuLayout->addWidget(btnNaviresAffichage);

        btnNaviresStatistique = new QPushButton(naviresMenu);
        btnNaviresStatistique->setObjectName("btnNaviresStatistique");
        btnNaviresStatistique->setMinimumSize(QSize(0, 36));
        btnNaviresStatistique->setCheckable(true);

        naviresMenuLayout->addWidget(btnNaviresStatistique);

        btnNaviresAvancees = new QPushButton(naviresMenu);
        btnNaviresAvancees->setObjectName("btnNaviresAvancees");
        btnNaviresAvancees->setMinimumSize(QSize(0, 36));
        btnNaviresAvancees->setCheckable(true);

        naviresMenuLayout->addWidget(btnNaviresAvancees);


        sidebarLayout->addWidget(naviresMenu);

        btnGestionQuais = new QPushButton(sidebar);
        btnGestionQuais->setObjectName("btnGestionQuais");
        btnGestionQuais->setMinimumSize(QSize(0, 42));
        btnGestionQuais->setCheckable(false);

        sidebarLayout->addWidget(btnGestionQuais);

        btnGestionCaptures = new QPushButton(sidebar);
        btnGestionCaptures->setObjectName("btnGestionCaptures");
        btnGestionCaptures->setMinimumSize(QSize(0, 42));
        btnGestionCaptures->setCheckable(false);

        sidebarLayout->addWidget(btnGestionCaptures);

        btnStockageFrigorifique = new QPushButton(sidebar);
        btnStockageFrigorifique->setObjectName("btnStockageFrigorifique");
        btnStockageFrigorifique->setMinimumSize(QSize(0, 42));
        btnStockageFrigorifique->setCheckable(false);

        sidebarLayout->addWidget(btnStockageFrigorifique);

        btnGestionVentes = new QPushButton(sidebar);
        btnGestionVentes->setObjectName("btnGestionVentes");
        btnGestionVentes->setMinimumSize(QSize(0, 42));
        btnGestionVentes->setCheckable(false);

        sidebarLayout->addWidget(btnGestionVentes);

        sidebarBottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        sidebarLayout->addItem(sidebarBottomSpacer);


        rootLayout->addWidget(sidebar);

        scrollArea = new QScrollArea(centralwidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setFrameShape(QFrame::Shape::NoFrame);
        scrollArea->setWidgetResizable(true);
        scrollContent = new QWidget();
        scrollContent->setObjectName("scrollContent");
        scrollContent->setGeometry(QRect(0, -819, 1148, 1587));
        pageLayout = new QVBoxLayout(scrollContent);
        pageLayout->setSpacing(14);
        pageLayout->setObjectName("pageLayout");
        pageLayout->setContentsMargins(24, 18, 24, 24);
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName("headerLayout");
        headerTitleLayout = new QVBoxLayout();
        headerTitleLayout->setSpacing(2);
        headerTitleLayout->setObjectName("headerTitleLayout");
        lblAppTitle = new QLabel(scrollContent);
        lblAppTitle->setObjectName("lblAppTitle");

        headerTitleLayout->addWidget(lblAppTitle);

        lblAppSubtitle = new QLabel(scrollContent);
        lblAppSubtitle->setObjectName("lblAppSubtitle");
        lblAppSubtitle->setMaximumSize(QSize(0, 0));

        headerTitleLayout->addWidget(lblAppSubtitle);


        headerLayout->addLayout(headerTitleLayout);

        headerSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(headerSpacer);

        btnMenu = new QToolButton(scrollContent);
        btnMenu->setObjectName("btnMenu");
        btnMenu->setMinimumSize(QSize(36, 36));
        btnMenu->setMaximumSize(QSize(36, 36));
        btnMenu->setIconSize(QSize(18, 18));

        headerLayout->addWidget(btnMenu);


        pageLayout->addLayout(headerLayout);

        searchCard = new QFrame(scrollContent);
        searchCard->setObjectName("searchCard");
        searchCard->setFrameShape(QFrame::Shape::NoFrame);
        searchCardLayout = new QVBoxLayout(searchCard);
        searchCardLayout->setSpacing(10);
        searchCardLayout->setObjectName("searchCardLayout");
        searchCardLayout->setContentsMargins(16, 14, 16, 14);
        lblSearchTitle = new QLabel(searchCard);
        lblSearchTitle->setObjectName("lblSearchTitle");

        searchCardLayout->addWidget(lblSearchTitle);

        searchRow1 = new QHBoxLayout();
        searchRow1->setSpacing(12);
        searchRow1->setObjectName("searchRow1");
        searchType = new QLineEdit(searchCard);
        searchType->setObjectName("searchType");

        searchRow1->addWidget(searchType);

        searchNom = new QLineEdit(searchCard);
        searchNom->setObjectName("searchNom");

        searchRow1->addWidget(searchNom);


        searchCardLayout->addLayout(searchRow1);

        searchRow2 = new QHBoxLayout();
        searchRow2->setSpacing(12);
        searchRow2->setObjectName("searchRow2");
        searchMatricule = new QLineEdit(searchCard);
        searchMatricule->setObjectName("searchMatricule");

        searchRow2->addWidget(searchMatricule);

        searchPosition = new QLineEdit(searchCard);
        searchPosition->setObjectName("searchPosition");

        searchRow2->addWidget(searchPosition);


        searchCardLayout->addLayout(searchRow2);


        pageLayout->addWidget(searchCard);

        statsCard = new QFrame(scrollContent);
        statsCard->setObjectName("statsCard");
        statsCard->setFrameShape(QFrame::Shape::NoFrame);
        statsCardLayout = new QVBoxLayout(statsCard);
        statsCardLayout->setSpacing(14);
        statsCardLayout->setObjectName("statsCardLayout");
        statsCardLayout->setContentsMargins(16, 14, 16, 16);
        statsHeaderRow = new QHBoxLayout();
        statsHeaderRow->setObjectName("statsHeaderRow");
        lblStatsTitle = new QLabel(statsCard);
        lblStatsTitle->setObjectName("lblStatsTitle");

        statsHeaderRow->addWidget(lblStatsTitle);

        statsTitleSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        statsHeaderRow->addItem(statsTitleSpacer);

        periodLabel = new QLabel(statsCard);
        periodLabel->setObjectName("periodLabel");

        statsHeaderRow->addWidget(periodLabel);

        revenuePeriod = new QComboBox(statsCard);
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->addItem(QString());
        revenuePeriod->setObjectName("revenuePeriod");
        revenuePeriod->setMaximumSize(QSize(140, 16777215));

        statsHeaderRow->addWidget(revenuePeriod);


        statsCardLayout->addLayout(statsHeaderRow);

        statsCirclesRow1 = new QHBoxLayout();
        statsCirclesRow1->setSpacing(16);
        statsCirclesRow1->setObjectName("statsCirclesRow1");
        statCircle1 = new QFrame(statsCard);
        statCircle1->setObjectName("statCircle1");
        statCircle1->setMinimumSize(QSize(180, 180));
        statCircle1->setMaximumSize(QSize(220, 220));
        statCircle1->setFrameShape(QFrame::Shape::NoFrame);
        circle1Layout = new QVBoxLayout(statCircle1);
        circle1Layout->setSpacing(2);
        circle1Layout->setObjectName("circle1Layout");
        circle1Layout->setContentsMargins(30, 45, 30, 35);
        statCircleTitle1 = new QLabel(statCircle1);
        statCircleTitle1->setObjectName("statCircleTitle1");
        statCircleTitle1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle1Layout->addWidget(statCircleTitle1);

        statCircleValue1 = new QLabel(statCircle1);
        statCircleValue1->setObjectName("statCircleValue1");
        statCircleValue1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle1Layout->addWidget(statCircleValue1);

        statCircleSub1 = new QLabel(statCircle1);
        statCircleSub1->setObjectName("statCircleSub1");
        statCircleSub1->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle1Layout->addWidget(statCircleSub1);


        statsCirclesRow1->addWidget(statCircle1);

        statCircle2 = new QFrame(statsCard);
        statCircle2->setObjectName("statCircle2");
        statCircle2->setMinimumSize(QSize(180, 180));
        statCircle2->setMaximumSize(QSize(220, 220));
        statCircle2->setFrameShape(QFrame::Shape::NoFrame);
        circle2Layout = new QVBoxLayout(statCircle2);
        circle2Layout->setSpacing(2);
        circle2Layout->setObjectName("circle2Layout");
        circle2Layout->setContentsMargins(30, 45, 30, 35);
        statCircleTitle2 = new QLabel(statCircle2);
        statCircleTitle2->setObjectName("statCircleTitle2");
        statCircleTitle2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle2Layout->addWidget(statCircleTitle2);

        statCircleValue2 = new QLabel(statCircle2);
        statCircleValue2->setObjectName("statCircleValue2");
        statCircleValue2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle2Layout->addWidget(statCircleValue2);

        statCircleSub2 = new QLabel(statCircle2);
        statCircleSub2->setObjectName("statCircleSub2");
        statCircleSub2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle2Layout->addWidget(statCircleSub2);


        statsCirclesRow1->addWidget(statCircle2);

        statCircle3 = new QFrame(statsCard);
        statCircle3->setObjectName("statCircle3");
        statCircle3->setMinimumSize(QSize(180, 180));
        statCircle3->setMaximumSize(QSize(220, 220));
        statCircle3->setFrameShape(QFrame::Shape::NoFrame);
        circle3Layout = new QVBoxLayout(statCircle3);
        circle3Layout->setSpacing(2);
        circle3Layout->setObjectName("circle3Layout");
        circle3Layout->setContentsMargins(30, 45, 30, 35);
        statCircleTitle3 = new QLabel(statCircle3);
        statCircleTitle3->setObjectName("statCircleTitle3");
        statCircleTitle3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle3Layout->addWidget(statCircleTitle3);

        statCircleValue3 = new QLabel(statCircle3);
        statCircleValue3->setObjectName("statCircleValue3");
        statCircleValue3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle3Layout->addWidget(statCircleValue3);

        statCircleSub3 = new QLabel(statCircle3);
        statCircleSub3->setObjectName("statCircleSub3");
        statCircleSub3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        circle3Layout->addWidget(statCircleSub3);


        statsCirclesRow1->addWidget(statCircle3);


        statsCardLayout->addLayout(statsCirclesRow1);


        pageLayout->addWidget(statsCard);

        midRow = new QHBoxLayout();
        midRow->setSpacing(14);
        midRow->setObjectName("midRow");
        maintenanceCard = new QFrame(scrollContent);
        maintenanceCard->setObjectName("maintenanceCard");
        maintenanceCard->setFrameShape(QFrame::Shape::NoFrame);
        maintenanceLayout = new QVBoxLayout(maintenanceCard);
        maintenanceLayout->setSpacing(10);
        maintenanceLayout->setObjectName("maintenanceLayout");
        maintenanceLayout->setContentsMargins(16, 14, 16, 14);
        lblMaintenanceTitle = new QLabel(maintenanceCard);
        lblMaintenanceTitle->setObjectName("lblMaintenanceTitle");

        maintenanceLayout->addWidget(lblMaintenanceTitle);

        tableMaintenance = new QTableWidget(maintenanceCard);
        if (tableMaintenance->columnCount() < 4)
            tableMaintenance->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableMaintenance->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableMaintenance->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableMaintenance->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableMaintenance->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        if (tableMaintenance->rowCount() < 1)
            tableMaintenance->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableMaintenance->setVerticalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableMaintenance->setItem(0, 0, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableMaintenance->setItem(0, 1, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableMaintenance->setItem(0, 2, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableMaintenance->setItem(0, 3, __qtablewidgetitem8);
        tableMaintenance->setObjectName("tableMaintenance");
        tableMaintenance->setMinimumSize(QSize(0, 120));
        tableMaintenance->setFrameShape(QFrame::Shape::NoFrame);
        tableMaintenance->setShowGrid(false);
        tableMaintenance->setWordWrap(false);
        tableMaintenance->setRowCount(1);
        tableMaintenance->setColumnCount(4);
        tableMaintenance->horizontalHeader()->setDefaultSectionSize(120);
        tableMaintenance->horizontalHeader()->setHighlightSections(false);
        tableMaintenance->horizontalHeader()->setStretchLastSection(true);
        tableMaintenance->verticalHeader()->setVisible(false);
        tableMaintenance->verticalHeader()->setDefaultSectionSize(32);

        maintenanceLayout->addWidget(tableMaintenance);


        midRow->addWidget(maintenanceCard);

        revenusCard = new QFrame(scrollContent);
        revenusCard->setObjectName("revenusCard");
        revenusCard->setFrameShape(QFrame::Shape::NoFrame);
        revenusLayout = new QVBoxLayout(revenusCard);
        revenusLayout->setSpacing(10);
        revenusLayout->setObjectName("revenusLayout");
        revenusLayout->setContentsMargins(16, 14, 16, 14);
        lblRevenusTitle = new QLabel(revenusCard);
        lblRevenusTitle->setObjectName("lblRevenusTitle");

        revenusLayout->addWidget(lblRevenusTitle);

        tableRevenus = new QTableWidget(revenusCard);
        if (tableRevenus->columnCount() < 4)
            tableRevenus->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableRevenus->setHorizontalHeaderItem(0, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableRevenus->setHorizontalHeaderItem(1, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableRevenus->setHorizontalHeaderItem(2, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableRevenus->setHorizontalHeaderItem(3, __qtablewidgetitem12);
        if (tableRevenus->rowCount() < 3)
            tableRevenus->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableRevenus->setVerticalHeaderItem(0, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableRevenus->setVerticalHeaderItem(1, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableRevenus->setVerticalHeaderItem(2, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setTextAlignment(Qt::AlignCenter);
        tableRevenus->setItem(0, 0, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableRevenus->setItem(0, 1, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableRevenus->setItem(0, 2, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableRevenus->setItem(0, 3, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setTextAlignment(Qt::AlignCenter);
        tableRevenus->setItem(1, 0, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        tableRevenus->setItem(1, 1, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        tableRevenus->setItem(1, 2, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        tableRevenus->setItem(1, 3, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        tableRevenus->setItem(2, 0, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        tableRevenus->setItem(2, 1, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        tableRevenus->setItem(2, 2, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        tableRevenus->setItem(2, 3, __qtablewidgetitem27);
        tableRevenus->setObjectName("tableRevenus");
        tableRevenus->setMinimumSize(QSize(0, 120));
        tableRevenus->setFrameShape(QFrame::Shape::NoFrame);
        tableRevenus->setShowGrid(false);
        tableRevenus->setWordWrap(false);
        tableRevenus->setRowCount(3);
        tableRevenus->setColumnCount(4);
        tableRevenus->horizontalHeader()->setDefaultSectionSize(120);
        tableRevenus->horizontalHeader()->setHighlightSections(false);
        tableRevenus->horizontalHeader()->setStretchLastSection(true);
        tableRevenus->verticalHeader()->setVisible(false);
        tableRevenus->verticalHeader()->setDefaultSectionSize(32);

        revenusLayout->addWidget(tableRevenus);


        midRow->addWidget(revenusCard);


        pageLayout->addLayout(midRow);

        bottomRow = new QHBoxLayout();
        bottomRow->setSpacing(14);
        bottomRow->setObjectName("bottomRow");
        triCard = new QFrame(scrollContent);
        triCard->setObjectName("triCard");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(triCard->sizePolicy().hasHeightForWidth());
        triCard->setSizePolicy(sizePolicy);
        triCard->setFrameShape(QFrame::Shape::NoFrame);
        triLayout = new QVBoxLayout(triCard);
        triLayout->setSpacing(10);
        triLayout->setObjectName("triLayout");
        triLayout->setContentsMargins(16, 14, 16, 14);
        lblTriTitle = new QLabel(triCard);
        lblTriTitle->setObjectName("lblTriTitle");

        triLayout->addWidget(lblTriTitle);

        comboSort = new QComboBox(triCard);
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->addItem(QString());
        comboSort->setObjectName("comboSort");
        comboSort->setMinimumSize(QSize(0, 38));

        triLayout->addWidget(comboSort);


        bottomRow->addWidget(triCard);

        exportCard = new QFrame(scrollContent);
        exportCard->setObjectName("exportCard");
        sizePolicy.setHeightForWidth(exportCard->sizePolicy().hasHeightForWidth());
        exportCard->setSizePolicy(sizePolicy);
        exportCard->setFrameShape(QFrame::Shape::NoFrame);
        exportLayout = new QVBoxLayout(exportCard);
        exportLayout->setSpacing(10);
        exportLayout->setObjectName("exportLayout");
        exportLayout->setContentsMargins(16, 14, 16, 14);
        lblExportTitle = new QLabel(exportCard);
        lblExportTitle->setObjectName("lblExportTitle");

        exportLayout->addWidget(lblExportTitle);

        lineQRCode = new QLineEdit(exportCard);
        lineQRCode->setObjectName("lineQRCode");

        exportLayout->addWidget(lineQRCode);

        btnExportPDF = new QPushButton(exportCard);
        btnExportPDF->setObjectName("btnExportPDF");
        btnExportPDF->setMinimumSize(QSize(0, 38));
        btnExportPDF->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        exportLayout->addWidget(btnExportPDF);


        bottomRow->addWidget(exportCard);


        pageLayout->addLayout(bottomRow);

        gestionCard = new QFrame(scrollContent);
        gestionCard->setObjectName("gestionCard");
        gestionCard->setFrameShape(QFrame::Shape::NoFrame);
        gestionLayout = new QVBoxLayout(gestionCard);
        gestionLayout->setSpacing(10);
        gestionLayout->setObjectName("gestionLayout");
        gestionLayout->setContentsMargins(16, 14, 16, 14);
        lblGestionTitle = new QLabel(gestionCard);
        lblGestionTitle->setObjectName("lblGestionTitle");

        gestionLayout->addWidget(lblGestionTitle);

        buttonsCard = new QFrame(gestionCard);
        buttonsCard->setObjectName("buttonsCard");
        buttonsCard->setMinimumSize(QSize(0, 60));
        buttonsCard->setMaximumSize(QSize(16777215, 70));
        buttonsCard->setFrameShape(QFrame::Shape::NoFrame);
        buttonsLayout = new QHBoxLayout(buttonsCard);
        buttonsLayout->setSpacing(12);
        buttonsLayout->setObjectName("buttonsLayout");
        buttonsLayout->setContentsMargins(0, 8, 0, 6);
        btnAjouter = new QPushButton(buttonsCard);
        btnAjouter->setObjectName("btnAjouter");
        btnAjouter->setMinimumSize(QSize(140, 40));
        btnAjouter->setMaximumSize(QSize(140, 40));
        btnAjouter->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnAjouter);

        btnModifier = new QPushButton(buttonsCard);
        btnModifier->setObjectName("btnModifier");
        btnModifier->setMinimumSize(QSize(140, 40));
        btnModifier->setMaximumSize(QSize(140, 40));
        btnModifier->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnModifier);

        btnSupprimer = new QPushButton(buttonsCard);
        btnSupprimer->setObjectName("btnSupprimer");
        btnSupprimer->setMinimumSize(QSize(140, 40));
        btnSupprimer->setMaximumSize(QSize(140, 40));
        btnSupprimer->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        buttonsLayout->addWidget(btnSupprimer);

        btnSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonsLayout->addItem(btnSpacer);


        gestionLayout->addWidget(buttonsCard);

        checkSelectAll = new QCheckBox(gestionCard);
        checkSelectAll->setObjectName("checkSelectAll");

        gestionLayout->addWidget(checkSelectAll);

        tableNavires = new QTableWidget(gestionCard);
        if (tableNavires->columnCount() < 7)
            tableNavires->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(0, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(1, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(2, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(3, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(4, __qtablewidgetitem32);
        QTableWidgetItem *__qtablewidgetitem33 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(5, __qtablewidgetitem33);
        QTableWidgetItem *__qtablewidgetitem34 = new QTableWidgetItem();
        tableNavires->setHorizontalHeaderItem(6, __qtablewidgetitem34);
        if (tableNavires->rowCount() < 3)
            tableNavires->setRowCount(3);
        QTableWidgetItem *__qtablewidgetitem35 = new QTableWidgetItem();
        tableNavires->setVerticalHeaderItem(0, __qtablewidgetitem35);
        QTableWidgetItem *__qtablewidgetitem36 = new QTableWidgetItem();
        tableNavires->setVerticalHeaderItem(1, __qtablewidgetitem36);
        QTableWidgetItem *__qtablewidgetitem37 = new QTableWidgetItem();
        tableNavires->setVerticalHeaderItem(2, __qtablewidgetitem37);
        QTableWidgetItem *__qtablewidgetitem38 = new QTableWidgetItem();
        tableNavires->setItem(0, 0, __qtablewidgetitem38);
        QTableWidgetItem *__qtablewidgetitem39 = new QTableWidgetItem();
        tableNavires->setItem(0, 1, __qtablewidgetitem39);
        QTableWidgetItem *__qtablewidgetitem40 = new QTableWidgetItem();
        tableNavires->setItem(0, 2, __qtablewidgetitem40);
        QTableWidgetItem *__qtablewidgetitem41 = new QTableWidgetItem();
        tableNavires->setItem(0, 3, __qtablewidgetitem41);
        QTableWidgetItem *__qtablewidgetitem42 = new QTableWidgetItem();
        tableNavires->setItem(0, 4, __qtablewidgetitem42);
        QTableWidgetItem *__qtablewidgetitem43 = new QTableWidgetItem();
        tableNavires->setItem(0, 5, __qtablewidgetitem43);
        QTableWidgetItem *__qtablewidgetitem44 = new QTableWidgetItem();
        tableNavires->setItem(0, 6, __qtablewidgetitem44);
        QTableWidgetItem *__qtablewidgetitem45 = new QTableWidgetItem();
        tableNavires->setItem(1, 0, __qtablewidgetitem45);
        QTableWidgetItem *__qtablewidgetitem46 = new QTableWidgetItem();
        tableNavires->setItem(1, 1, __qtablewidgetitem46);
        QTableWidgetItem *__qtablewidgetitem47 = new QTableWidgetItem();
        tableNavires->setItem(1, 2, __qtablewidgetitem47);
        QTableWidgetItem *__qtablewidgetitem48 = new QTableWidgetItem();
        tableNavires->setItem(1, 3, __qtablewidgetitem48);
        QTableWidgetItem *__qtablewidgetitem49 = new QTableWidgetItem();
        tableNavires->setItem(1, 4, __qtablewidgetitem49);
        QTableWidgetItem *__qtablewidgetitem50 = new QTableWidgetItem();
        tableNavires->setItem(1, 5, __qtablewidgetitem50);
        QTableWidgetItem *__qtablewidgetitem51 = new QTableWidgetItem();
        tableNavires->setItem(1, 6, __qtablewidgetitem51);
        QTableWidgetItem *__qtablewidgetitem52 = new QTableWidgetItem();
        tableNavires->setItem(2, 0, __qtablewidgetitem52);
        QTableWidgetItem *__qtablewidgetitem53 = new QTableWidgetItem();
        tableNavires->setItem(2, 1, __qtablewidgetitem53);
        QTableWidgetItem *__qtablewidgetitem54 = new QTableWidgetItem();
        tableNavires->setItem(2, 2, __qtablewidgetitem54);
        QTableWidgetItem *__qtablewidgetitem55 = new QTableWidgetItem();
        tableNavires->setItem(2, 3, __qtablewidgetitem55);
        QTableWidgetItem *__qtablewidgetitem56 = new QTableWidgetItem();
        tableNavires->setItem(2, 4, __qtablewidgetitem56);
        QTableWidgetItem *__qtablewidgetitem57 = new QTableWidgetItem();
        tableNavires->setItem(2, 5, __qtablewidgetitem57);
        QTableWidgetItem *__qtablewidgetitem58 = new QTableWidgetItem();
        tableNavires->setItem(2, 6, __qtablewidgetitem58);
        tableNavires->setObjectName("tableNavires");
        tableNavires->setMinimumSize(QSize(0, 180));
        tableNavires->setFrameShape(QFrame::Shape::NoFrame);
        tableNavires->setShowGrid(false);
        tableNavires->setWordWrap(false);
        tableNavires->setRowCount(3);
        tableNavires->setColumnCount(7);
        tableNavires->horizontalHeader()->setDefaultSectionSize(110);
        tableNavires->horizontalHeader()->setHighlightSections(false);
        tableNavires->horizontalHeader()->setStretchLastSection(true);
        tableNavires->verticalHeader()->setVisible(false);
        tableNavires->verticalHeader()->setDefaultSectionSize(32);

        gestionLayout->addWidget(tableNavires);


        pageLayout->addWidget(gestionCard);

        weatherCard = new QFrame(scrollContent);
        weatherCard->setObjectName("weatherCard");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(weatherCard->sizePolicy().hasHeightForWidth());
        weatherCard->setSizePolicy(sizePolicy1);
        weatherCard->setFrameShape(QFrame::Shape::NoFrame);
        weatherLayout = new QVBoxLayout(weatherCard);
        weatherLayout->setSpacing(10);
        weatherLayout->setObjectName("weatherLayout");
        weatherLayout->setContentsMargins(16, 14, 16, 14);
        lblWeatherTitle = new QLabel(weatherCard);
        lblWeatherTitle->setObjectName("lblWeatherTitle");

        weatherLayout->addWidget(lblWeatherTitle);

        weatherInputRow = new QHBoxLayout();
        weatherInputRow->setSpacing(8);
        weatherInputRow->setObjectName("weatherInputRow");
        lineWeatherCity = new QLineEdit(weatherCard);
        lineWeatherCity->setObjectName("lineWeatherCity");
        lineWeatherCity->setMinimumSize(QSize(0, 38));

        weatherInputRow->addWidget(lineWeatherCity);

        btnFetchWeather = new QPushButton(weatherCard);
        btnFetchWeather->setObjectName("btnFetchWeather");
        btnFetchWeather->setMinimumSize(QSize(120, 38));
        btnFetchWeather->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        weatherInputRow->addWidget(btnFetchWeather);


        weatherLayout->addLayout(weatherInputRow);

        lblWeatherInfo = new QLabel(weatherCard);
        lblWeatherInfo->setObjectName("lblWeatherInfo");

        weatherLayout->addWidget(lblWeatherInfo);

        lblWeatherDetails = new QLabel(weatherCard);
        lblWeatherDetails->setObjectName("lblWeatherDetails");

        weatherLayout->addWidget(lblWeatherDetails);


        pageLayout->addWidget(weatherCard);

        calendarCard = new QFrame(scrollContent);
        calendarCard->setObjectName("calendarCard");
        sizePolicy1.setHeightForWidth(calendarCard->sizePolicy().hasHeightForWidth());
        calendarCard->setSizePolicy(sizePolicy1);
        calendarCard->setFrameShape(QFrame::Shape::NoFrame);
        calendarLayout = new QVBoxLayout(calendarCard);
        calendarLayout->setSpacing(10);
        calendarLayout->setObjectName("calendarLayout");
        calendarLayout->setContentsMargins(16, 14, 16, 14);
        calendarHeaderRow = new QHBoxLayout();
        calendarHeaderRow->setObjectName("calendarHeaderRow");
        lblCalendarTitle = new QLabel(calendarCard);
        lblCalendarTitle->setObjectName("lblCalendarTitle");

        calendarHeaderRow->addWidget(lblCalendarTitle);

        calendarHeaderSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        calendarHeaderRow->addItem(calendarHeaderSpacer);

        lblSelectedDate = new QLabel(calendarCard);
        lblSelectedDate->setObjectName("lblSelectedDate");
        lblSelectedDate->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignVCenter);

        calendarHeaderRow->addWidget(lblSelectedDate);

        btnShowAllMouvements = new QPushButton(calendarCard);
        btnShowAllMouvements->setObjectName("btnShowAllMouvements");
        btnShowAllMouvements->setMinimumSize(QSize(90, 32));
        btnShowAllMouvements->setMaximumSize(QSize(110, 32));
        btnShowAllMouvements->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        calendarHeaderRow->addWidget(btnShowAllMouvements);

        btnAddMouvement = new QPushButton(calendarCard);
        btnAddMouvement->setObjectName("btnAddMouvement");
        btnAddMouvement->setMinimumSize(QSize(160, 38));
        btnAddMouvement->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        calendarHeaderRow->addWidget(btnAddMouvement);


        calendarLayout->addLayout(calendarHeaderRow);

        calendarMouvements = new QCalendarWidget(calendarCard);
        calendarMouvements->setObjectName("calendarMouvements");
        calendarMouvements->setMinimumSize(QSize(0, 230));
        calendarMouvements->setMaximumSize(QSize(16777215, 260));
        calendarMouvements->setGridVisible(true);

        calendarLayout->addWidget(calendarMouvements);

        tableMouvements = new QTableWidget(calendarCard);
        if (tableMouvements->columnCount() < 5)
            tableMouvements->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem59 = new QTableWidgetItem();
        tableMouvements->setHorizontalHeaderItem(0, __qtablewidgetitem59);
        QTableWidgetItem *__qtablewidgetitem60 = new QTableWidgetItem();
        tableMouvements->setHorizontalHeaderItem(1, __qtablewidgetitem60);
        QTableWidgetItem *__qtablewidgetitem61 = new QTableWidgetItem();
        tableMouvements->setHorizontalHeaderItem(2, __qtablewidgetitem61);
        QTableWidgetItem *__qtablewidgetitem62 = new QTableWidgetItem();
        tableMouvements->setHorizontalHeaderItem(3, __qtablewidgetitem62);
        QTableWidgetItem *__qtablewidgetitem63 = new QTableWidgetItem();
        tableMouvements->setHorizontalHeaderItem(4, __qtablewidgetitem63);
        tableMouvements->setObjectName("tableMouvements");
        tableMouvements->setMinimumSize(QSize(0, 180));
        tableMouvements->setFrameShape(QFrame::Shape::NoFrame);
        tableMouvements->setShowGrid(false);
        tableMouvements->setWordWrap(false);
        tableMouvements->setColumnCount(5);
        tableMouvements->horizontalHeader()->setHighlightSections(false);
        tableMouvements->horizontalHeader()->setStretchLastSection(true);
        tableMouvements->verticalHeader()->setVisible(false);
        tableMouvements->verticalHeader()->setDefaultSectionSize(34);

        calendarLayout->addWidget(tableMouvements);


        pageLayout->addWidget(calendarCard);

        pageBottomSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        pageLayout->addItem(pageBottomSpacer);

        scrollArea->setWidget(scrollContent);

        rootLayout->addWidget(scrollArea);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        revenuePeriod->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        lblLogo->setText(QString());
        lblSidebarTitle->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        btnGestionEmployees->setText(QCoreApplication::translate("MainWindow", "Gestion Des employ\303\251es", nullptr));
        btnGestionNavires->setText(QCoreApplication::translate("MainWindow", "Gestion Des Navires", nullptr));
        btnNaviresAffichage->setText(QCoreApplication::translate("MainWindow", "Affichage", nullptr));
        btnNaviresStatistique->setText(QCoreApplication::translate("MainWindow", "Statistique", nullptr));
        btnNaviresAvancees->setText(QCoreApplication::translate("MainWindow", "Avanc\303\251es", nullptr));
        btnGestionQuais->setText(QCoreApplication::translate("MainWindow", "Gestion Des Quais", nullptr));
        btnGestionCaptures->setText(QCoreApplication::translate("MainWindow", "Gestion Des Captures", nullptr));
        btnStockageFrigorifique->setText(QCoreApplication::translate("MainWindow", "Stockage Frigorifique", nullptr));
        btnGestionVentes->setText(QCoreApplication::translate("MainWindow", "Gestion Des Ventes", nullptr));
        lblAppTitle->setText(QCoreApplication::translate("MainWindow", "VISION SIGHT", nullptr));
        lblAppSubtitle->setText(QString());
        btnMenu->setText(QString());
        lblSearchTitle->setText(QCoreApplication::translate("MainWindow", "Recherche / Filtres", nullptr));
        searchType->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par type navire", nullptr));
        searchNom->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par nom navire", nullptr));
        searchMatricule->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par matricule navire", nullptr));
        searchPosition->setPlaceholderText(QCoreApplication::translate("MainWindow", "Recherche par Position", nullptr));
        lblStatsTitle->setText(QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        periodLabel->setText(QCoreApplication::translate("MainWindow", "P\303\251riode:", nullptr));
        revenuePeriod->setItemText(0, QCoreApplication::translate("MainWindow", "24h", nullptr));
        revenuePeriod->setItemText(1, QCoreApplication::translate("MainWindow", "7j", nullptr));
        revenuePeriod->setItemText(2, QCoreApplication::translate("MainWindow", "30j", nullptr));
        revenuePeriod->setItemText(3, QCoreApplication::translate("MainWindow", "Tout", nullptr));

        statCircleTitle1->setText(QCoreApplication::translate("MainWindow", "Types", nullptr));
        statCircleValue1->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        statCircleSub1->setText(QCoreApplication::translate("MainWindow", "types", nullptr));
        statCircleTitle2->setText(QCoreApplication::translate("MainWindow", "Taux actifs", nullptr));
        statCircleValue2->setText(QCoreApplication::translate("MainWindow", "66.7%", nullptr));
        statCircleSub2->setText(QCoreApplication::translate("MainWindow", "%", nullptr));
        statCircleTitle3->setText(QCoreApplication::translate("MainWindow", "Distance totale", nullptr));
        statCircleValue3->setText(QCoreApplication::translate("MainWindow", "147.5", nullptr));
        statCircleSub3->setText(QCoreApplication::translate("MainWindow", "km", nullptr));
        lblMaintenanceTitle->setText(QCoreApplication::translate("MainWindow", "Maintenances pr\303\251vues", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableMaintenance->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableMaintenance->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableMaintenance->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Co\303\273t estim\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableMaintenance->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e", nullptr));

        const bool __sortingEnabled = tableMaintenance->isSortingEnabled();
        tableMaintenance->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem4 = tableMaintenance->item(0, 0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableMaintenance->item(0, 1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "En maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableMaintenance->item(0, 2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableMaintenance->item(0, 3);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("MainWindow", "\342\200\224", nullptr));
        tableMaintenance->setSortingEnabled(__sortingEnabled);

        lblRevenusTitle->setText(QCoreApplication::translate("MainWindow", "Revenus par navire ", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableRevenus->horizontalHeaderItem(0);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("MainWindow", "Navire", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableRevenus->horizontalHeaderItem(1);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("MainWindow", "Revenu p\303\251riode", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableRevenus->horizontalHeaderItem(2);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("MainWindow", "Co\303\273ts", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableRevenus->horizontalHeaderItem(3);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("MainWindow", "B\303\251n\303\251fice", nullptr));

        const bool __sortingEnabled1 = tableRevenus->isSortingEnabled();
        tableRevenus->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem12 = tableRevenus->item(0, 0);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("MainWindow", "AQUA", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableRevenus->item(0, 1);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableRevenus->item(0, 2);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableRevenus->item(0, 3);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableRevenus->item(1, 0);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("MainWindow", "MARIN", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableRevenus->item(1, 1);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableRevenus->item(1, 2);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableRevenus->item(1, 3);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableRevenus->item(2, 0);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("MainWindow", "OC\303\211AN", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableRevenus->item(2, 1);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableRevenus->item(2, 2);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableRevenus->item(2, 3);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("MainWindow", "0,00", nullptr));
        tableRevenus->setSortingEnabled(__sortingEnabled1);

        lblTriTitle->setText(QCoreApplication::translate("MainWindow", "Tri", nullptr));
        comboSort->setItemText(0, QCoreApplication::translate("MainWindow", "Par d\303\251faut", nullptr));
        comboSort->setItemText(1, QCoreApplication::translate("MainWindow", "Nom \342\206\221", nullptr));
        comboSort->setItemText(2, QCoreApplication::translate("MainWindow", "Nom \342\206\223", nullptr));
        comboSort->setItemText(3, QCoreApplication::translate("MainWindow", "Capacit\303\251 \342\206\221", nullptr));
        comboSort->setItemText(4, QCoreApplication::translate("MainWindow", "Capacit\303\251 \342\206\223", nullptr));
        comboSort->setItemText(5, QCoreApplication::translate("MainWindow", "Statut \342\206\221", nullptr));
        comboSort->setItemText(6, QCoreApplication::translate("MainWindow", "Statut \342\206\223", nullptr));

        lblExportTitle->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        lineQRCode->setPlaceholderText(QCoreApplication::translate("MainWindow", "Ins\303\251rer le QR code de la fiche navire", nullptr));
        btnExportPDF->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        lblGestionTitle->setText(QCoreApplication::translate("MainWindow", "Gestion des Navires", nullptr));
        btnAjouter->setText(QCoreApplication::translate("MainWindow", "Cr\303\251er", nullptr));
        btnModifier->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        btnSupprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        checkSelectAll->setText(QCoreApplication::translate("MainWindow", "Tout s\303\251lectionner", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableNavires->horizontalHeaderItem(0);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableNavires->horizontalHeaderItem(1);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("MainWindow", "ID Navire", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableNavires->horizontalHeaderItem(2);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("MainWindow", "Matricule", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableNavires->horizontalHeaderItem(3);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tableNavires->horizontalHeaderItem(4);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("MainWindow", "Capacit\303\251", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = tableNavires->horizontalHeaderItem(5);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = tableNavires->horizontalHeaderItem(6);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("MainWindow", "Position", nullptr));

        const bool __sortingEnabled2 = tableNavires->isSortingEnabled();
        tableNavires->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem31 = tableNavires->item(0, 0);
        ___qtablewidgetitem31->setText(QCoreApplication::translate("MainWindow", "Oc\303\251an Bleu", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = tableNavires->item(0, 1);
        ___qtablewidgetitem32->setText(QCoreApplication::translate("MainWindow", "NAV-001", nullptr));
        QTableWidgetItem *___qtablewidgetitem33 = tableNavires->item(0, 2);
        ___qtablewidgetitem33->setText(QCoreApplication::translate("MainWindow", "M-1234", nullptr));
        QTableWidgetItem *___qtablewidgetitem34 = tableNavires->item(0, 3);
        ___qtablewidgetitem34->setText(QCoreApplication::translate("MainWindow", "P\303\252che", nullptr));
        QTableWidgetItem *___qtablewidgetitem35 = tableNavires->item(0, 4);
        ___qtablewidgetitem35->setText(QCoreApplication::translate("MainWindow", "800", nullptr));
        QTableWidgetItem *___qtablewidgetitem36 = tableNavires->item(0, 5);
        ___qtablewidgetitem36->setText(QCoreApplication::translate("MainWindow", "Actif", nullptr));
        QTableWidgetItem *___qtablewidgetitem37 = tableNavires->item(0, 6);
        ___qtablewidgetitem37->setText(QCoreApplication::translate("MainWindow", "Port A", nullptr));
        QTableWidgetItem *___qtablewidgetitem38 = tableNavires->item(1, 0);
        ___qtablewidgetitem38->setText(QCoreApplication::translate("MainWindow", "Mistral", nullptr));
        QTableWidgetItem *___qtablewidgetitem39 = tableNavires->item(1, 1);
        ___qtablewidgetitem39->setText(QCoreApplication::translate("MainWindow", "NAV-002", nullptr));
        QTableWidgetItem *___qtablewidgetitem40 = tableNavires->item(1, 2);
        ___qtablewidgetitem40->setText(QCoreApplication::translate("MainWindow", "M-1235", nullptr));
        QTableWidgetItem *___qtablewidgetitem41 = tableNavires->item(1, 3);
        ___qtablewidgetitem41->setText(QCoreApplication::translate("MainWindow", "Transport", nullptr));
        QTableWidgetItem *___qtablewidgetitem42 = tableNavires->item(1, 4);
        ___qtablewidgetitem42->setText(QCoreApplication::translate("MainWindow", "1200", nullptr));
        QTableWidgetItem *___qtablewidgetitem43 = tableNavires->item(1, 5);
        ___qtablewidgetitem43->setText(QCoreApplication::translate("MainWindow", "En maintenance", nullptr));
        QTableWidgetItem *___qtablewidgetitem44 = tableNavires->item(1, 6);
        ___qtablewidgetitem44->setText(QCoreApplication::translate("MainWindow", "Port B", nullptr));
        QTableWidgetItem *___qtablewidgetitem45 = tableNavires->item(2, 0);
        ___qtablewidgetitem45->setText(QCoreApplication::translate("MainWindow", "Albatros", nullptr));
        QTableWidgetItem *___qtablewidgetitem46 = tableNavires->item(2, 1);
        ___qtablewidgetitem46->setText(QCoreApplication::translate("MainWindow", "NAV-003", nullptr));
        QTableWidgetItem *___qtablewidgetitem47 = tableNavires->item(2, 2);
        ___qtablewidgetitem47->setText(QCoreApplication::translate("MainWindow", "M-1236", nullptr));
        QTableWidgetItem *___qtablewidgetitem48 = tableNavires->item(2, 3);
        ___qtablewidgetitem48->setText(QCoreApplication::translate("MainWindow", "P\303\252che", nullptr));
        QTableWidgetItem *___qtablewidgetitem49 = tableNavires->item(2, 4);
        ___qtablewidgetitem49->setText(QCoreApplication::translate("MainWindow", "500", nullptr));
        QTableWidgetItem *___qtablewidgetitem50 = tableNavires->item(2, 5);
        ___qtablewidgetitem50->setText(QCoreApplication::translate("MainWindow", "Actif", nullptr));
        QTableWidgetItem *___qtablewidgetitem51 = tableNavires->item(2, 6);
        ___qtablewidgetitem51->setText(QCoreApplication::translate("MainWindow", "Port A", nullptr));
        tableNavires->setSortingEnabled(__sortingEnabled2);

        lblWeatherTitle->setText(QCoreApplication::translate("MainWindow", "M\303\251t\303\251o Maritime", nullptr));
        lineWeatherCity->setPlaceholderText(QCoreApplication::translate("MainWindow", "Entrez le nom d'une ville portuaire...", nullptr));
        btnFetchWeather->setText(QCoreApplication::translate("MainWindow", "Recherchez", nullptr));
        lblWeatherInfo->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #ffffff; font-size: 16px; font-weight: 700;", nullptr));
        lblWeatherInfo->setText(QCoreApplication::translate("MainWindow", "Entrez une ville pour voir la m\303\251t\303\251o", nullptr));
        lblWeatherDetails->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #9bb0c3; font-size: 13px;", nullptr));
        lblWeatherDetails->setText(QCoreApplication::translate("MainWindow", "Temp\303\251rature \302\267 Humidit\303\251 \302\267 Vent", nullptr));
        lblCalendarTitle->setText(QCoreApplication::translate("MainWindow", "Calendrier des Mouvements", nullptr));
        lblSelectedDate->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #39c0fa; font-size: 14px; font-weight: 600;", nullptr));
        lblSelectedDate->setText(QCoreApplication::translate("MainWindow", "Tous les mouvements", nullptr));
        btnShowAllMouvements->setStyleSheet(QCoreApplication::translate("MainWindow", "background: #23374e; color: #e6eef6; border: 0; border-radius: 8px; padding: 6px 14px; font-weight: 600; font-size: 12px;", nullptr));
        btnShowAllMouvements->setText(QCoreApplication::translate("MainWindow", "Voir tout", nullptr));
        btnAddMouvement->setText(QCoreApplication::translate("MainWindow", "+ Ajoutez un mouvement", nullptr));
        QTableWidgetItem *___qtablewidgetitem52 = tableMouvements->horizontalHeaderItem(0);
        ___qtablewidgetitem52->setText(QCoreApplication::translate("MainWindow", "Navire", nullptr));
        QTableWidgetItem *___qtablewidgetitem53 = tableMouvements->horizontalHeaderItem(1);
        ___qtablewidgetitem53->setText(QCoreApplication::translate("MainWindow", "Date/Heure Arriv\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem54 = tableMouvements->horizontalHeaderItem(2);
        ___qtablewidgetitem54->setText(QCoreApplication::translate("MainWindow", "Date/Heure D\303\251part", nullptr));
        QTableWidgetItem *___qtablewidgetitem55 = tableMouvements->horizontalHeaderItem(3);
        ___qtablewidgetitem55->setText(QCoreApplication::translate("MainWindow", "Cargaison rapport\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem56 = tableMouvements->horizontalHeaderItem(4);
        ___qtablewidgetitem56->setText(QCoreApplication::translate("MainWindow", "Distance (km)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
