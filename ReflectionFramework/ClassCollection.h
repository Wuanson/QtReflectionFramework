#ifndef CLASSCOLLECTION_H
#define CLASSCOLLECTION_H

#include <QMetaObject>

namespace std {
    template<typename _Signature>
    class function;
}

/**
 * @brief The ClassCollection class  QMetaObject收集器,收集所有注册的Qobject的QMetaObject
 */
class ClassCollection final
{
private:
    ClassCollection();

public:
    ClassCollection(const ClassCollection& other) = delete;
    ClassCollection(const ClassCollection&& other) = delete;

public:
    /**
     * @brief RegisterClass 注册一个QMetaObject
     * @param metaObject
     * @param isOverride    是否覆盖原有的
     */
    static void registerClass(const QMetaObject& metaObject, const bool& isOverride = false);

    /**
     * @brief GetClass
     * @param className
     * @param showAssert
     * @return  如果找到则返回对应的 QMetaObject，否则返回 QObject,如果 showAssert 为true则程序会中断
     */

    /**
     * @brief getClass 通过获取一个 QMetaObject
     * @param className QObject 类名
     * @param retMetaObject 返回查找到的 MetaObject
     * @param showAssert 找不到时是否出现断言
     * @return 找到返回true
     */
    static bool getClass(const QString& className, QMetaObject& retMetaObject, bool showAssert = false);

    /**
     * @brief getClassByFlag 通过 ClassInfo 的 FLAG 来查找类
     * @param key
     * @param retMetaObject
     * @param showAssert
     * @return
     */
    static bool getClassByFlag(const QString& key, QMetaObject& retMetaObject, bool showAssert = false);


    static const QMap<QString, QMetaObject> getMetaObjectMap();

    /**
     * @brief iterateMetaObjectMap 迭代所有已注册的类,回调中返回true可以终止迭代
     * @param callBack
     */
    static void iterateMetaObjectMap(std::function<bool(const QMetaObject& metaObject)> callBack);
public:
    /**
     * @brief initMetaObjectMap  初始化MetaObjectMap
     */
    static void initMetaObjectMap();

private:
    static class QMap<QString,QMetaObject>* _metaObjectMap;
};

#endif // CLASSCOLLECTION_H
