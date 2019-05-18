#ifndef SYSTEMGATEWAY_H
#define SYSTEMGATEWAY_H

#include <QMap>
#include <QGenericArgument>
#include <QVariant>

#include "Singleton.h"

class SystemGateway : public Singleton<SystemGateway>
{
    friend Singleton<SystemGateway>;

private:
    SystemGateway();

public:
    virtual ~SystemGateway();

public:
    /**
     * @brief invokeMethod 调用一个方法
     * @param token
     * @param systemName 方法所在的 System 名称
     * @param methodName 方法的名称
     * @param params     参数
     * @return           如果方法存在返回 true
     */
    bool invokeMethod(class SocketToken *token, const QString& systemName, const QString& methodName, class QJsonObject& params);

    /**
     * @brief insertSystem 添加一个 System
     * @param key
     * @param system
     */
    void insertSystem(const QString& key, class ISystem *system);

    /**
     * @brief removeSystem 移除一个 System
     * @param key
     */
    void removeSystem(const QString& key);

    /**
     * @brief takeSystem 移除并返回一个 System
     * @param key
     * @return
     */
    class ISystem* takeSystem(const QString& key);

private:
    /**
     * @brief paserSystemMethods 解析 System 里面的所有方法并缓存起来
     * @param system
     */
    void paserSystemMethods(class QObject* system);

    /**
     * @brief findMethod 查找方法
     * @param systemName 方法所在的 System  名称
     * @param methodName 方法的名称
     * @param metaMethod 返回的方法
     * @return 找到返回 true
     */
    bool findMethod(const QString& systemName, const QString& methodName, QMetaMethod& metaMethod);

private:
    QMap<class QString, class ISystem*> m_systems;
    QMap<QString, QMap<QString, class QMetaMethod>> m_methodMap;
};

#endif // SYSTEMGATEWAY_H
