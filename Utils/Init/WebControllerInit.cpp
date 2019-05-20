#include "WebControllerInit.h"

#ifdef IS_WEBSERVER

#include <QMetaObject>
#include <functional>
#include <QDebug>
#include <QMetaMethod>

#include "ReflectionFramework/Reflection.h"
#include "ReflectionFramework/ClassCollection.h"
#include "ReflectionFramework/QMetaObjectHelper.h"
#include "Utils/SystemGateway.h"
#include "Utils/ModelCollection.h"
#include "Utils/Interface/ISystem.h"
#include "Utils/Interface/AModel.h"

REG_QCLASS(WebControllerInit)

WebControllerInit::WebControllerInit(QObject *parent)
    : QObject(parent)
{
    initModels();
    initControllers();
}

WebControllerInit::~WebControllerInit()
{
    ModelCollection::instance()->saveSettings();
}

void WebControllerInit::initModels()
{
    ClassCollection::iterateMetaObjectMap([&](const QMetaObject & metaObject)
    {
        if(QMetaObjectHelper::hasClaaInfoKey(metaObject, KEY_MODEL))
        {
            QObject* object = QMetaObjectHelper::createQObject(true, metaObject);
            AModel* model = qobject_cast<AModel*>(object);
            if(!model)
            {
                qt_assert(QString("类型 %1 没有继承自 AModel").arg(metaObject.className()).toStdString().c_str(), __FILE__, __LINE__);
            }
            ModelCollection::instance()->insertModel(metaObject.className(), model);
            qDebug()<<"Add Model:"<< metaObject.className();
        }
        return false;
    });

    ModelCollection::instance()->loadSettings();
}


void WebControllerInit::initControllers()
{
    ClassCollection::iterateMetaObjectMap([&](const QMetaObject & metaObject)
    {
        if(QMetaObjectHelper::hasClaaInfoKey(metaObject, KEY_SYSTEM))
        {
            QObject* object = QMetaObjectHelper::createQObject(true, metaObject);
            ISystem* system = qobject_cast<ISystem*>(object);
            if(!system)
            {
                qt_assert(QString("类型 %1 没有继承自 ISystem").arg(metaObject.className()).toStdString().c_str(), __FILE__, __LINE__);
            }

            //设置 objectName
            if(system->objectName().trimmed().isEmpty())
            {
                //全部小写 如果有 system 做后缀，则去掉 system 后缀
                QString objectName = QString(metaObject.className()).trimmed().toLower();
                if(objectName.length() > 6)
                {
                    if(objectName.right(6) == QStringLiteral("system"))
                    {
                        objectName = objectName.left(objectName.length() - 6);
                    }
                }
                system->setObjectName(objectName);
            }

            //注入 Model
            injectControllerModel(system);

            SystemGateway::instance()->insertSystem(system->objectName(), system);
            qDebug()<< "Add Web Controller :" << system->objectName();
        }
        return false;
    });
}

void WebControllerInit::injectControllerModel(QObject* object, bool showAssert)
{
    if(!object)
    {
        return ;
    }

    QString retValue;
    if(QMetaObjectHelper::getClassInfoValue(*object->metaObject(), QStringLiteral(KEY_MODEL_NAMES), retValue))
    {
        QStringList names = retValue.split(';', QString::SplitBehavior::SkipEmptyParts);
        for(auto name : names)
        {
            QMetaProperty property;
            if(QMetaObjectHelper::getMetaProperty(*object->metaObject(), name, property))
            {
                auto model = ModelCollection::instance()->getModel(property.typeName());
                if(model)
                {
                    if(!object->setProperty(name.toStdString().c_str(), QVariant::fromValue(model)))
                    {
                        QString msg = QString("给类型 %1 注入属性 %2 失败！").arg(object->metaObject()->className()).arg(name);
                        if(showAssert)
                        {
                            qt_assert(msg.toStdString().c_str(), __FILE__, __LINE__);
                        }
                        else
                        {
                            qDebug()<<msg;
                        }
                    }
                }
                else
                {
                    QString msg = QString("没有找到类型为 %1 的 Model ,可能是没有注册！").arg(property.typeName());
                    if(showAssert)
                    {
                        qt_assert(msg.toStdString().c_str(), __FILE__, __LINE__);
                    }
                    else
                    {
                        qDebug()<<msg;
                    }
                }
            }
            else
            {
                QString msg = QString("类型 %1 没有找到属性 %2 ！").arg(object->metaObject()->className()).arg(name);
                if(showAssert)
                {
                    qt_assert(msg.toStdString().c_str(), __FILE__, __LINE__);
                }
                else
                {
                    qDebug()<<msg;
                }
            }
        }
    }
}

#endif
