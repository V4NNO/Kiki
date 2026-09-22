/****************************************************************************
** Meta object code from reading C++ file 'screencapture.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/screencapture.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screencapture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
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
struct qt_meta_tag_ZN20ScreenCaptureManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto ScreenCaptureManager::qt_create_metaobjectdata<qt_meta_tag_ZN20ScreenCaptureManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ScreenCaptureManager",
        "frameCaptured",
        "",
        "streamId",
        "QImage",
        "image",
        "monitorListChanged",
        "QList<MonitorInfo>",
        "monitors",
        "captureBackendReady",
        "screenName",
        "usingDxgi",
        "detail",
        "captureTick",
        "refreshMonitorList"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'frameCaptured'
        QtMocHelpers::SignalData<void(quint32, const QImage &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { 0x80000000 | 4, 5 },
        }}),
        // Signal 'monitorListChanged'
        QtMocHelpers::SignalData<void(const QList<MonitorInfo> &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'captureBackendReady'
        QtMocHelpers::SignalData<void(const QString &, bool, const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 }, { QMetaType::Bool, 11 }, { QMetaType::QString, 12 },
        }}),
        // Slot 'captureTick'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'refreshMonitorList'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ScreenCaptureManager, qt_meta_tag_ZN20ScreenCaptureManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ScreenCaptureManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ScreenCaptureManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ScreenCaptureManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20ScreenCaptureManagerE_t>.metaTypes,
    nullptr
} };

void ScreenCaptureManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ScreenCaptureManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->frameCaptured((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QImage>>(_a[2]))); break;
        case 1: _t->monitorListChanged((*reinterpret_cast<std::add_pointer_t<QList<MonitorInfo>>>(_a[1]))); break;
        case 2: _t->captureBackendReady((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 3: _t->captureTick(); break;
        case 4: _t->refreshMonitorList(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ScreenCaptureManager::*)(quint32 , const QImage & )>(_a, &ScreenCaptureManager::frameCaptured, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenCaptureManager::*)(const QList<MonitorInfo> & )>(_a, &ScreenCaptureManager::monitorListChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ScreenCaptureManager::*)(const QString & , bool , const QString & )>(_a, &ScreenCaptureManager::captureBackendReady, 2))
            return;
    }
}

const QMetaObject *ScreenCaptureManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScreenCaptureManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ScreenCaptureManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ScreenCaptureManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ScreenCaptureManager::frameCaptured(quint32 _t1, const QImage & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void ScreenCaptureManager::monitorListChanged(const QList<MonitorInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ScreenCaptureManager::captureBackendReady(const QString & _t1, bool _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
