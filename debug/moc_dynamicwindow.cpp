/****************************************************************************
** Meta object code from reading C++ file 'dynamicwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dynamicwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dynamicwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DynamicWindow_t {
    QByteArrayData data[7];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DynamicWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DynamicWindow_t qt_meta_stringdata_DynamicWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DynamicWindow"
QT_MOC_LITERAL(1, 14, 10), // "timer_lock"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "timer_unlock"
QT_MOC_LITERAL(4, 39, 26), // "on_tableWidget_cellChanged"
QT_MOC_LITERAL(5, 66, 3), // "row"
QT_MOC_LITERAL(6, 70, 6) // "column"

    },
    "DynamicWindow\0timer_lock\0\0timer_unlock\0"
    "on_tableWidget_cellChanged\0row\0column"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DynamicWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    0,   30,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt,    5,    6,

       0        // eod
};

void DynamicWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DynamicWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timer_lock(); break;
        case 1: _t->timer_unlock(); break;
        case 2: _t->on_tableWidget_cellChanged((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DynamicWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DynamicWindow::timer_lock)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DynamicWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DynamicWindow::timer_unlock)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DynamicWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_DynamicWindow.data,
    qt_meta_data_DynamicWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DynamicWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DynamicWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DynamicWindow.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int DynamicWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void DynamicWindow::timer_lock()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DynamicWindow::timer_unlock()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
