/****************************************************************************
** Meta object code from reading C++ file 'sessionmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/sessionmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessionmanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14SessionManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto SessionManager::qt_create_metaobjectdata<qt_meta_tag_ZN14SessionManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SessionManager",
        "logMessage",
        "",
        "message",
        "pollSessions",
        "onMonitorDiscovered",
        "sessionId",
        "localStreamId",
        "name",
        "QSize",
        "size",
        "isWindow",
        "onFrameReady",
        "QImage",
        "image",
        "onMetadataChanged",
        "application",
        "idleText",
        "inputEvents",
        "url",
        "onKeystrokeReceived",
        "windowTitle",
        "text",
        "onIngestDisconnected",
        "onViewerCountChanged",
        "count"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Slot 'pollSessions'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onMonitorDiscovered'
        QtMocHelpers::SlotData<void(quint32, quint32, const QString &, const QSize &, bool)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 6 }, { QMetaType::UInt, 7 }, { QMetaType::QString, 8 }, { 0x80000000 | 9, 10 },
            { QMetaType::Bool, 11 },
        }}),
        // Slot 'onFrameReady'
        QtMocHelpers::SlotData<void(quint32, quint32, const QImage &)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 6 }, { QMetaType::UInt, 7 }, { 0x80000000 | 13, 14 },
        }}),
        // Slot 'onMetadataChanged'
        QtMocHelpers::SlotData<void(quint32, quint32, const QString &, const QString &, int, const QString &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 6 }, { QMetaType::UInt, 7 }, { QMetaType::QString, 16 }, { QMetaType::QString, 17 },
            { QMetaType::Int, 18 }, { QMetaType::QString, 19 },
        }}),
        // Slot 'onKeystrokeReceived'
        QtMocHelpers::SlotData<void(quint32, const QString &, const QString &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 6 }, { QMetaType::QString, 21 }, { QMetaType::QString, 22 },
        }}),
        // Slot 'onIngestDisconnected'
        QtMocHelpers::SlotData<void(quint32)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::UInt, 6 },
        }}),
        // Slot 'onViewerCountChanged'
        QtMocHelpers::SlotData<void(int)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 25 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SessionManager, qt_meta_tag_ZN14SessionManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SessionManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14SessionManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14SessionManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14SessionManagerE_t>.metaTypes,
    nullptr
} };

void SessionManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SessionManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->pollSessions(); break;
        case 2: _t->onMonitorDiscovered((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QSize>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[5]))); break;
        case 3: _t->onFrameReady((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QImage>>(_a[3]))); break;
        case 4: _t->onMetadataChanged((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[6]))); break;
        case 5: _t->onKeystrokeReceived((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 6: _t->onIngestDisconnected((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1]))); break;
        case 7: _t->onViewerCountChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SessionManager::*)(const QString & )>(_a, &SessionManager::logMessage, 0))
            return;
    }
}

const QMetaObject *SessionManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14SessionManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SessionManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SessionManager::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
