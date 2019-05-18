#ifndef ISYSTEM_H
#define ISYSTEM_H

#ifdef IS_WEBSERVER

#include <QObject>

#include "NetCore/EPermission.h"

class ISystem : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE ISystem(const EPermission& permission, QObject* parent)
        : QObject(parent)
        , m_permission(permission)
    {

    }
public:
    Q_PROPERTY(EPermission permission READ permission WRITE setPermission)

    EPermission permission() const
    {
        return m_permission;
    }

public slots:
    void setPermission(EPermission permission)
    {
        m_permission = permission;
    }

private:
    EPermission m_permission;
};

#else

class ISystem
{
public:
    ISystem(bool isEnabled = true) :isEnabled(isEnabled){}
    virtual ~ISystem(){}
public:
    
    virtual bool getIsEnabled() const
    {
        return isEnabled;
    }

    virtual void setIsEnabled(const bool &value)
    {
        isEnabled = value;
    }


public:
    /*
     调用顺序：
         1、loadSettings
         2、initSystem
         3、saveSetings
         4、exitSystem
    */

    /**
     * @brief loadConfig 加载配置
     * @param settings
     */
    virtual void loadSettings(class QSettings* settings) = 0;


    /**
     * @brief initSystem 初始化系统
     */
    virtual void initSystem() = 0;


    /**
     * @brief saveConfig 保存
     * @param settings
     */
    virtual void saveSetings(class QSettings* settings) = 0;

    /**
     * @brief exitSystem 退出系统
     */
    virtual void exitSystem() = 0;
    
protected:
    bool isEnabled;//是否启用该系统
};

#endif

#endif // ISYSTEM_H
