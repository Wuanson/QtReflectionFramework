#include "SystemGateway.h"

#include <QObject>
#include <QMetaMethod>
#include <QSet>
#include <QJsonObject>
#include <QDebug>
#include <QMetaType>

#include "ReflectionFramework/QMetaObjectHelper.h"
#include "Utils/Interface/ISystem.h"
#include "NetCore/SocketToken.h"
#include "NetCore/SocketError.h"
#include "NetCore/NodeSocketModel.h"
#include "Utils/QJsonHelper.h"

SystemGateway::SystemGateway()
{

}

SystemGateway::~SystemGateway()
{

}

bool SystemGateway::invokeMethod(SocketToken *token, const QString &systemName, const QString &methodName, QJsonObject &params)
{
    QMetaMethod method;
    if(findMethod(systemName, methodName, method))
    {
        //权限验证
        ISystem* system = m_systems[systemName];
        if(system->permission() > token->permission())
        {
            NodeSocketModel model;
            model.setSystemName(systemName);
            model.setMethodName(methodName);
            model.setErrorCode(ERR_NO_PERMISSION);
            model.writeToSocket(token);
        }
        else
        {
            QVariant vars[10];//必须得放到外面,要不然销毁后就是乱的数据
            QGenericArgument args[10];
            auto names = method.parameterNames();
            auto types = method.parameterTypes();
            if(names.length() != types.length())
            {
                qt_assert("函数的参数类型的个数和参数名称的个数不匹配!", __FILE__, __LINE__);
            }
            for(int i = 0; i < names.size(); ++i)
            {
                //SocketToken* 不是客户端传来的
                if(types[i] == QString("SocketToken*"))
                {
                    args[i] = Q_ARG(SocketToken*, token);
                    continue;
                }

                //将客户端传入的参数转为 QVariant 然后将 QVariant 转为 QGenericArgument
                int typeId = QMetaType::type(types[i]);
                QMetaType type(typeId);
                if(type.isValid())
                {
                    if(QMetaObjectHelper::isNormalType(type))
                    {
                        QString typeName = names[i];
                        if(params.contains(typeName))
                        {
                            vars[i] = params.take(typeName).toVariant();
                            if(vars[i].canConvert(typeId))
                            {
                                vars[i].convert(typeId);
                            }
                            args[i] = QGenericArgument(vars[i].typeName(), vars[i].data());
                        }
                    }
                    else
                    {
                        QObject* data = QMetaObjectHelper::createQObject(true, *type.metaObject());

                        QString typeName = names[i];
                        auto jsonObject = params.take(typeName).toObject();
                        QJsonHelper::fromJson(data, jsonObject);
                        vars[i] = QVariant::fromValue(data);
                    }
                    args[i] = QGenericArgument(vars[i].typeName(), vars[i].data());
                }
            }

            //开始调用
            if(QMetaType::Void == method.returnType())
            {
                if(!method.invoke(system, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]))
                {
                    qCritical()<<system<< "调用函数"<< method.name()<< "失败,参数不匹配";
                }
            }
            else
            {
                if(QMetaObjectHelper::isNormalType(method.returnType()))
                {
                    QVariant retValue(method.returnType(), nullptr);
                    QGenericReturnArgument returnArg(retValue.typeName(), &retValue);
                    if(method.invoke(system, Qt::ConnectionType::DirectConnection, returnArg, args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8], args[9]))
                    {
                        if(retValue.isValid())
                        {
                            NodeSocketModel retModel;
                            retModel.setSystemName(systemName);
                            retModel.setMethodName(methodName);
                            if(QMetaObjectHelper::isNormalType(QMetaType::type(retValue.typeName())))
                            {
                                retModel.insertData("data", retValue.toString());
                            }
                            else
                            {
                                QObject* object = retValue.value<QObject*>();
                                object = dynamic_cast<QObject*>(object);
                                if(object)
                                {
                                    retModel.insertData("data", QJsonHelper::toJson(object));
                                }
                                else
                                {
                                    retModel.insertData("data", "");
                                    retModel.setErrorCode(ERR_RET_ERR);
                                }
                            }
                            retModel.writeToSocket(token);
                        }
                    }
                    else
                    {
                        qCritical()<<system<< "调用函数"<< method.name()<< "失败,参数不匹配,该函数的返回值类型是:"<< QMetaType::typeName(method.returnType());
                    }
                }
                else
                {
                    qCritical()<<system<< "调用函数"<< method.name()<< "失败,返回值只能为基本数据类型,该函数的返回值类型是:"<< QMetaType::typeName(method.returnType());
                }
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

void SystemGateway::insertSystem(const QString &key, ISystem *system)
{
    paserSystemMethods(system);
    m_systems.insert(key, system);
}

void SystemGateway::removeSystem(const QString &key)
{
    m_systems.remove(key);
}

ISystem *SystemGateway::takeSystem(const QString &key)
{
    return m_systems.take(key);
}

void SystemGateway::paserSystemMethods(QObject *system)
{
    if(m_systems.contains(system->objectName()))
    {
        return ;
    }

    //过滤元对象的函数
    static QSet<QString> filters =
    {
        "destroyed"
        , "objectNameChanged"
        , "deleteLater"
        , "_q_reregisterTimers"
        ,
    };

    QMap<QString, QMetaMethod> methodMap;
    for(int i = 0; i < system->metaObject()->methodCount(); ++i)
    {
        QMetaMethod method = system->metaObject()->method(i);
        if(filters.contains(method.name()))
        {
            continue;
        }
        else
        {
            methodMap.insert(method.name(), method);
        }
    }

    m_methodMap[system->objectName()] = methodMap;
}

bool SystemGateway::findMethod(const QString &systemName, const QString &methodName, QMetaMethod &metaMethod)
{
    if(!m_systems.contains(systemName))
    {
        qWarning()<< "没有找到 System :"<< systemName <<" 可能是没有使用REG_QCLASS注册!或者没有加上Q_SYSTEM标记";
    }
    else
    {
        QObject* system = m_systems[systemName];
        if(!system)
        {
            qWarning()<<"找到 System :"<< systemName << "为空";
        }
        else
        {
            if(!m_methodMap.contains(methodName) && !m_methodMap[systemName].contains(methodName))
            {
                qCritical()<< "System: "<< system->objectName()<< " 没有找到"<< methodName<< " 函数";
            }
            else
            {
                metaMethod = m_methodMap[systemName][methodName];
                return true;
            }
        }
    }
    return false;
}

