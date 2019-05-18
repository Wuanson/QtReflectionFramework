#include "WebControllerInit.h"

#ifdef IS_WEBSERVER

#include <QMetaObject>
#include <functional>
#include <QDebug>

#include "ReflectionFramework/Reflection.h"
#include "ReflectionFramework/ClassCollection.h"
#include "ReflectionFramework/QMetaObjectHelper.h"
#include "Utils/SystemGateway.h"
#include "Utils/Interface/ISystem.h"

REG_QCLASS(WebControllerInit)

WebControllerInit::WebControllerInit(QObject *parent)
    : QObject(parent)
{
    initControllers();
}

#include <QMetaMethod>
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
            SystemGateway::instance()->insertSystem(system->objectName(), system);
            qDebug()<< "Add Web Controller :" << system->objectName();
        }
        return false;
    });
}

#endif
