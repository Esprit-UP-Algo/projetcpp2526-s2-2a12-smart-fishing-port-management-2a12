/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../transaction/mainwindow.h"
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
    "setActiveSidebarButton",
    "",
    "QPushButton*",
    "btn",
    "onToggleVentesMenu",
    "onVentesAffichage",
    "onVentesStatistique",
    "onVentesAvancees",
    "onCreer",
    "onModifier",
    "onSupprimer",
    "onSearchChanged",
    "onSortChanged",
    "onExportPDF",
    "onSendChatMessage",
    "setupChatBotUI",
    "onPrixEstimeButtonClicked",
    "loadFishPricesFromFile"
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
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  104,    2, 0x08,    1 /* Private */,
       5,    0,  107,    2, 0x08,    3 /* Private */,
       6,    0,  108,    2, 0x08,    4 /* Private */,
       7,    0,  109,    2, 0x08,    5 /* Private */,
       8,    0,  110,    2, 0x08,    6 /* Private */,
       9,    0,  111,    2, 0x08,    7 /* Private */,
      10,    0,  112,    2, 0x08,    8 /* Private */,
      11,    0,  113,    2, 0x08,    9 /* Private */,
      12,    0,  114,    2, 0x08,   10 /* Private */,
      13,    0,  115,    2, 0x08,   11 /* Private */,
      14,    0,  116,    2, 0x08,   12 /* Private */,
      15,    0,  117,    2, 0x08,   13 /* Private */,
      16,    0,  118,    2, 0x08,   14 /* Private */,
      17,    0,  119,    2, 0x08,   15 /* Private */,
      18,    0,  120,    2, 0x08,   16 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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
        // method 'setActiveSidebarButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'onToggleVentesMenu'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVentesAffichage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVentesStatistique'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onVentesAvancees'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onCreer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onModifier'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSupprimer'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSortChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onExportPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSendChatMessage'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setupChatBotUI'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onPrixEstimeButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'loadFishPricesFromFile'
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
        case 0: _t->setActiveSidebarButton((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 1: _t->onToggleVentesMenu(); break;
        case 2: _t->onVentesAffichage(); break;
        case 3: _t->onVentesStatistique(); break;
        case 4: _t->onVentesAvancees(); break;
        case 5: _t->onCreer(); break;
        case 6: _t->onModifier(); break;
        case 7: _t->onSupprimer(); break;
        case 8: _t->onSearchChanged(); break;
        case 9: _t->onSortChanged(); break;
        case 10: _t->onExportPDF(); break;
        case 11: _t->onSendChatMessage(); break;
        case 12: _t->setupChatBotUI(); break;
        case 13: _t->onPrixEstimeButtonClicked(); break;
        case 14: _t->loadFishPricesFromFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
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
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
