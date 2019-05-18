#include "ClassCollection.h"

#include <QObject>
#include <QMap>
#include <functional>

#include <QDebug>

#include "QMetaObjectHelper.h"

QMap<QString,QMetaObject>* ClassCollection::_metaObjectMap = nullptr;

ClassCollection::ClassCollection()
{

}

void ClassCollection::initMetaObjectMap()
{
    static QMap<QString,QMetaObject> metaObject =
    {
        {QObject::staticMetaObject.className(),QObject::staticMetaObject},
        //TODO add Qt QObject
    };
    _metaObjectMap = &metaObject;
}

void ClassCollection::registerClass(const QMetaObject &metaObject, const bool &isOverride)
{
    if(!_metaObjectMap)
    {
        initMetaObjectMap();
    }

    if(!_metaObjectMap->contains(metaObject.className()) || isOverride)
    {
        (*_metaObjectMap)[metaObject.className()] = metaObject;
        qDebug()<<__FILE__<< "Register New Class:"<< metaObject.className();
    }
}

bool ClassCollection::getClass(const QString &className, QMetaObject &retMetaObject, bool showAssert)
{
    if(!_metaObjectMap)
    {
        initMetaObjectMap();
    }

    if(_metaObjectMap->contains(className))
    {
        retMetaObject = (*_metaObjectMap)[className];
        return true;
    }
    else if(showAssert)
    {
        qt_assert(QString("没有找到类型: %1,可能是没有注册! ").arg(className).toStdString().c_str(),__FILE__,__LINE__);
    }
    return false;
}

bool ClassCollection::getClassByFlag(const QString &key, QMetaObject &retMetaObject, bool showAssert)
{
    bool ret = false;
    iterateMetaObjectMap([&](const QMetaObject& metaObject)
    {
        if(QMetaObjectHelper::hasClaaInfoKey(metaObject, key))
        {
            retMetaObject = metaObject;
            ret = true;
            return true;
        }
        return false;
    });

    if(!ret && showAssert)
    {
        QString msg = QString("没有找到有标记 %1 的类,可能是没有注册或者没有添加 %1 标记").arg(key);
        qt_assert(msg.toStdString().c_str(), __FILE__, __LINE__);
    }

    return ret;
}

const QMap<QString, QMetaObject> ClassCollection::getMetaObjectMap()
{
    if(!_metaObjectMap)
    {
        initMetaObjectMap();
    }

    return *_metaObjectMap;
}

void ClassCollection::iterateMetaObjectMap(std::function<bool (const QMetaObject &)> callBack)
{
    for(const auto& metaObject : *_metaObjectMap)
    {
        if(callBack(metaObject))
        {
            break;
        }
    }
}

