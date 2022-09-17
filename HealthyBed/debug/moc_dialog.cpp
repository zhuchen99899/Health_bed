/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Dialog_t {
    QByteArrayData data[15];
    char stringdata0[327];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dialog_t qt_meta_stringdata_Dialog = {
    {
QT_MOC_LITERAL(0, 0, 6), // "Dialog"
QT_MOC_LITERAL(1, 7, 23), // "signal_speed_to_mainwin"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 5), // "value"
QT_MOC_LITERAL(4, 38, 35), // "singal_motor1_direction0_to_m..."
QT_MOC_LITERAL(5, 74, 35), // "singal_motor1_direction1_to_m..."
QT_MOC_LITERAL(6, 110, 32), // "on_horizontalSlider_valueChanged"
QT_MOC_LITERAL(7, 143, 27), // "on_horizontalSlider_release"
QT_MOC_LITERAL(8, 171, 19), // "QPushButton_clicked"
QT_MOC_LITERAL(9, 191, 20), // "QPushButton1_clicked"
QT_MOC_LITERAL(10, 212, 20), // "button_create_widget"
QT_MOC_LITERAL(11, 233, 24), // "Edit_set_speed_toMainwin"
QT_MOC_LITERAL(12, 258, 3), // "str"
QT_MOC_LITERAL(13, 262, 35), // "Edit_set_speed_toMyhorizontal..."
QT_MOC_LITERAL(14, 298, 28) // "Edit_set_speed_toMyLcdNumber"

    },
    "Dialog\0signal_speed_to_mainwin\0\0value\0"
    "singal_motor1_direction0_to_mainwin\0"
    "singal_motor1_direction1_to_mainwin\0"
    "on_horizontalSlider_valueChanged\0"
    "on_horizontalSlider_release\0"
    "QPushButton_clicked\0QPushButton1_clicked\0"
    "button_create_widget\0Edit_set_speed_toMainwin\0"
    "str\0Edit_set_speed_toMyhorizontalSlider\0"
    "Edit_set_speed_toMyLcdNumber"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       5,    1,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   78,    2, 0x0a /* Public */,
       7,    0,   81,    2, 0x0a /* Public */,
       8,    0,   82,    2, 0x0a /* Public */,
       9,    0,   83,    2, 0x0a /* Public */,
      10,    0,   84,    2, 0x0a /* Public */,
      11,    1,   85,    2, 0x0a /* Public */,
      13,    1,   88,    2, 0x0a /* Public */,
      14,    1,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   12,

       0        // eod
};

void Dialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Dialog *_t = static_cast<Dialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_speed_to_mainwin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->singal_motor1_direction0_to_mainwin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->singal_motor1_direction1_to_mainwin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_horizontalSlider_release(); break;
        case 5: _t->QPushButton_clicked(); break;
        case 6: _t->QPushButton1_clicked(); break;
        case 7: _t->button_create_widget(); break;
        case 8: _t->Edit_set_speed_toMainwin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->Edit_set_speed_toMyhorizontalSlider((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->Edit_set_speed_toMyLcdNumber((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Dialog::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dialog::signal_speed_to_mainwin)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Dialog::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dialog::singal_motor1_direction0_to_mainwin)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Dialog::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Dialog::singal_motor1_direction1_to_mainwin)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog.data,
      qt_meta_data_Dialog,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog.stringdata0))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void Dialog::signal_speed_to_mainwin(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Dialog::singal_motor1_direction0_to_mainwin(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Dialog::singal_motor1_direction1_to_mainwin(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
