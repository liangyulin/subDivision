/****************************************************************************
** Meta object code from reading C++ file 'subdivision_new.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../subdivision_new.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'subdivision_new.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_subDivision_new_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_subDivision_new_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_subDivision_new_t qt_meta_stringdata_subDivision_new = {
    {
QT_MOC_LITERAL(0, 0, 15), // "subDivision_new"
QT_MOC_LITERAL(1, 16, 7), // "addData"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "save"
QT_MOC_LITERAL(4, 30, 7), // "refresh"
QT_MOC_LITERAL(5, 38, 12), // "radioChange1"
QT_MOC_LITERAL(6, 51, 12) // "radioChange2"

    },
    "subDivision_new\0addData\0\0save\0refresh\0"
    "radioChange1\0radioChange2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_subDivision_new[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x08 /* Private */,
       3,    0,   40,    2, 0x08 /* Private */,
       4,    0,   41,    2, 0x08 /* Private */,
       5,    0,   42,    2, 0x08 /* Private */,
       6,    0,   43,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void subDivision_new::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        subDivision_new *_t = static_cast<subDivision_new *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addData(); break;
        case 1: _t->save(); break;
        case 2: _t->refresh(); break;
        case 3: _t->radioChange1(); break;
        case 4: _t->radioChange2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject subDivision_new::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_subDivision_new.data,
      qt_meta_data_subDivision_new,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *subDivision_new::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *subDivision_new::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_subDivision_new.stringdata0))
        return static_cast<void*>(const_cast< subDivision_new*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int subDivision_new::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
