/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../integration/integration/GestionDesNavires/mainwindow.h"
#include <QtNetwork/QSslError>
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "onAjouterClicked",
    "",
    "onModifierClicked",
    "onSupprimerClicked",
    "onSearchChanged",
    "onSortChanged",
    "index",
    "onExportPDF",
    "onFetchWeather",
    "fetchMarineWeatherData",
    "latitude",
    "longitude",
    "onWeatherReply",
    "QNetworkReply*",
    "reply",
    "onGeocodingReply",
    "onAddMouvement",
    "onTableSelectionChanged",
    "onSelectAllChanged",
    "state",
    "onTableCellModified",
    "row",
    "col",
    "onTableContextMenu",
    "pos",
    "onTableRowDelete",
    "updateStats",
    "onCalendarDateClicked",
    "date",
    "fetchWeatherForDate",
    "onWeatherCalendarDateSelected",
    "updateWeatherForDate",
    "onWeatherDataLoaded"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  146,    2, 0x08,    1 /* Private */,
       3,    0,  147,    2, 0x08,    2 /* Private */,
       4,    0,  148,    2, 0x08,    3 /* Private */,
       5,    0,  149,    2, 0x08,    4 /* Private */,
       6,    1,  150,    2, 0x08,    5 /* Private */,
       8,    0,  153,    2, 0x08,    7 /* Private */,
       9,    0,  154,    2, 0x08,    8 /* Private */,
      10,    2,  155,    2, 0x08,    9 /* Private */,
      13,    1,  160,    2, 0x08,   12 /* Private */,
      16,    1,  163,    2, 0x08,   14 /* Private */,
      17,    0,  166,    2, 0x08,   16 /* Private */,
      18,    0,  167,    2, 0x08,   17 /* Private */,
      19,    1,  168,    2, 0x08,   18 /* Private */,
      21,    2,  171,    2, 0x08,   20 /* Private */,
      24,    1,  176,    2, 0x08,   23 /* Private */,
      26,    1,  179,    2, 0x08,   25 /* Private */,
      27,    0,  182,    2, 0x08,   27 /* Private */,
      28,    1,  183,    2, 0x08,   28 /* Private */,
      30,    1,  186,    2, 0x08,   30 /* Private */,
      31,    1,  189,    2, 0x08,   32 /* Private */,
      32,    1,  192,    2, 0x08,   34 /* Private */,
      33,    0,  195,    2, 0x08,   36 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   22,   23,
    QMetaType::Void, QMetaType::QPoint,   25,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QDate,   29,
    QMetaType::Void, QMetaType::QDate,   29,
    QMetaType::Void, QMetaType::QDate,   29,
    QMetaType::Void, QMetaType::QDate,   29,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'onAjouterClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onModifierClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSupprimerClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSortChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onExportPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFetchWeather'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fetchMarineWeatherData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        QtPrivate::TypeAndForceComplete<double, std::false_type>,
        // method 'onWeatherReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'onGeocodingReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'onAddMouvement'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTableSelectionChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSelectAllChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTableCellModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onTableContextMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPoint &, std::false_type>,
        // method 'onTableRowDelete'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'updateStats'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCalendarDateClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'fetchWeatherForDate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QDate, std::false_type>,
        // method 'onWeatherCalendarDateSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'updateWeatherForDate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QDate &, std::false_type>,
        // method 'onWeatherDataLoaded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAjouterClicked(); break;
        case 1: _t->onModifierClicked(); break;
        case 2: _t->onSupprimerClicked(); break;
        case 3: _t->onSearchChanged(); break;
        case 4: _t->onSortChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onExportPDF(); break;
        case 6: _t->onFetchWeather(); break;
        case 7: _t->fetchMarineWeatherData((*reinterpret_cast< std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[2]))); break;
        case 8: _t->onWeatherReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 9: _t->onGeocodingReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 10: _t->onAddMouvement(); break;
        case 11: _t->onTableSelectionChanged(); break;
        case 12: _t->onSelectAllChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 13: _t->onTableCellModified((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 14: _t->onTableContextMenu((*reinterpret_cast< std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 15: _t->onTableRowDelete((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->updateStats(); break;
        case 17: _t->onCalendarDateClicked((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 18: _t->fetchWeatherForDate((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 19: _t->onWeatherCalendarDateSelected((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 20: _t->updateWeatherForDate((*reinterpret_cast< std::add_pointer_t<QDate>>(_a[1]))); break;
        case 21: _t->onWeatherDataLoaded(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}
QT_WARNING_POP
