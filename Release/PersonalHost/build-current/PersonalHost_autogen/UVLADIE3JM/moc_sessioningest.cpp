/****************************************************************************
** Meta object code from reading C++ file 'sessioningest.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/sessioningest.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sessioningest.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13SessionIngestE_t {};
} // unnamed namespace

template <> constexpr inline auto SessionIngest::qt_create_metaobjectdata<qt_meta_tag_ZN13SessionIngestE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SessionIngest",
        "monitorDiscovered",
        "",
        "sessionId",
        "localStreamId",
        "name",
        "QSize",
        "size",
        "isWindow",
        "frameReady",
        "QImage",
        "image",
        "metadataChanged",
        "application",
        "idleText",
        "inputEvents",
        "url",
        "keystrokeReceived",
        "windowTitle",
        "text",
        "ingestDisconnected",
        "connectFailed",
        "logMessage",
        "message",
        "onConnected",
        "onReadyRead",
        "onDisconnected",
        "onErrorOccurred",
        "retryConnect"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'monitorDiscovered'
        QtMocHelpers::SignalData<void(quint32, quint32, const QString &, const QSize &, bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { QMetaType::UInt, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 },
            { QMetaType::Bool, 8 },
        }}),
        // Signal 'frameReady'
        QtMocHelpers::SignalData<void(quint32, quint32, const QImage &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { QMetaType::UInt, 4 }, { 0x80000000 | 10, 11 },
        }}),
        // Signal 'metadataChanged'
        QtMocHelpers::SignalData<void(quint32, quint32, const QString &, const QString &, int, const QString &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { QMetaType::UInt, 4 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
            { QMetaType::Int, 15 }, { QMetaType::QString, 16 },
        }}),
        // Signal 'keystrokeReceived'
        QtMocHelpers::SignalData<void(quint32, const QString &, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 }, { QMetaType::QString, 18 }, { QMetaType::QString, 19 },
        }}),
        // Signal 'ingestDisconnected'
        QtMocHelpers::SignalData<void(quint32)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 },
        }}),
        // Signal 'connectFailed'
        QtMocHelpers::SignalData<void(quint32)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::UInt, 3 },
        }}),
        // Signal 'logMessage'
        QtMocHelpers::SignalData<void(const QString &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 23 },
        }}),
        // Slot 'onConnected'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReadyRead'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDisconnected'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onErrorOccurred'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'retryConnect'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SessionIngest, qt_meta_tag_ZN13SessionIngestE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SessionIngest::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionIngestE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionIngestE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SessionIngestE_t>.metaTypes,
    nullptr
} };

void SessionIngest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SessionIngest *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->monitorDiscovered((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QSize>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[5]))); break;
        case 1: _t->frameReady((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QImage>>(_a[3]))); break;
        case 2: _t->metadataChanged((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<quint32>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[5])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[6]))); break;
        case 3: _t->keystrokeReceived((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 4: _t->ingestDisconnected((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1]))); break;
        case 5: _t->connectFailed((*reinterpret_cast<std::add_pointer_t<quint32>>(_a[1]))); break;
        case 6: _t->logMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onConnected(); break;
        case 8: _t->onReadyRead(); break;
        case 9: _t->onDisconnected(); break;
        case 10: _t->onErrorOccurred(); break;
        case 11: _t->retryConnect(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 , quint32 , const QString & , const QSize & , bool )>(_a, &SessionIngest::monitorDiscovered, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 , quint32 , const QImage & )>(_a, &SessionIngest::frameReady, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 , quint32 , const QString & , const QString & , int , const QString & )>(_a, &SessionIngest::metadataChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 , const QString & , const QString & )>(_a, &SessionIngest::keystrokeReceived, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 )>(_a, &SessionIngest::ingestDisconnected, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(quint32 )>(_a, &SessionIngest::connectFailed, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionIngest::*)(const QString & )>(_a, &SessionIngest::logMessage, 6))
            return;
    }
}

const QMetaObject *SessionIngest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionIngest::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionIngestE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SessionIngest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SessionIngest::monitorDiscovered(quint32 _t1, quint32 _t2, const QString & _t3, const QSize & _t4, bool _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 1
void SessionIngest::frameReady(quint32 _t1, quint32 _t2, const QImage & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}

// SIGNAL 2
void SessionIngest::metadataChanged(quint32 _t1, quint32 _t2, const QString & _t3, const QString & _t4, int _t5, const QString & _t6)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6);
}

// SIGNAL 3
void SessionIngest::keystrokeReceived(quint32 _t1, const QString & _t2, const QString & _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void SessionIngest::ingestDisconnected(quint32 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void SessionIngest::connectFailed(quint32 _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void SessionIngest::logMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}
QT_WARNING_POP
