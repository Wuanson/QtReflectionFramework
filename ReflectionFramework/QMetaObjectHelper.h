#ifndef QMETAOBJECTHELPER_H
#define QMETAOBJECTHELPER_H

#include <QMetaObject>
#include <QStringList>

namespace std {
    template<typename _Signature>
    class function;
}

class QMetaObjectHelper
{

public:
    /**
     * @brief createQObject 创建一个 QObject ,并根据 CLASSINFO 来初始化对象
     * @param showAssert 创建失败时 是否出现断言
     * @param metaObject
     * @param val0
     * @param val1
     * @param val2
     * @param val3
     * @param val4
     * @param val5
     * @param val6
     * @param val7
     * @param val8
     * @param val9
     * @return
     */
    static class QObject* createQObject(bool showAssert, const QMetaObject& metaObject,
                                        QGenericArgument val0 = QGenericArgument(nullptr),
                                        QGenericArgument val1 = QGenericArgument(),
                                        QGenericArgument val2 = QGenericArgument(),
                                        QGenericArgument val3 = QGenericArgument(),
                                        QGenericArgument val4 = QGenericArgument(),
                                        QGenericArgument val5 = QGenericArgument(),
                                        QGenericArgument val6 = QGenericArgument(),
                                        QGenericArgument val7 = QGenericArgument(),
                                        QGenericArgument val8 = QGenericArgument(),
                                        QGenericArgument val9 = QGenericArgument());
    /**
     * @brief isA 判断该类是否继承于某一个类(通过 classInfo 来判断,需要在类的 Q_OBJECT 下面添加对应的信息)
     * @param metaObject
     * @param className
     * @return
     */
    static bool isA(const QMetaObject& metaObject, const QString& className);

    /**
     * @brief getClassInfoValue 获取 ClassInfo 中 key 对应的 Value
     * @param metaObject
     * @param key
     * @param retValue
     * @return
     */
    static bool getClassInfoValue(const QMetaObject& metaObject, const QString& key, QString& retValue);

    /**
     * @brief getClassInfoValues
     * @param metaObject
     * @param keys
     * @param retValues
     * @return
     */
    static bool getClassInfoValues(const QMetaObject& metaObject, const QStringList& keys, QMap<QString,QString>& retValues);

    /**
     * @brief hasClassInfo
     * @param metaObject
     * @param key
     * @param value
     * @return
     */
    static bool hasClassInfo(const QMetaObject& metaObject, const QString& key, const QString& value);

    /**
     * @brief hasClaaInfoKey
     * @param metaObject
     * @param key
     * @return
     */
    static bool hasClaaInfoKey(const QMetaObject& metaObject, const QString& key);

    /**
     * @brief connectSlotsByNameVariant 将 sender 所有的信号与 receiver 的槽连接 带校正
     * @param sender
     * @param receiver
     * @param variant
     */
    static void connectSlotsByNameVariant(class QObject* sender, class QObject* receiver, const QMap<QString,QString>& variant);

    /**
     * @brief connectSlotsByName 将 sender 所有的信号与 receiver 的槽连接
     * @param sender
     * @param receiver
     */
    static void connectSlotsByName(class QObject* sender, class QObject* receiver);

    /**
     * @brief connectAllSlotsByName 将 sender 与 sender 的子对象的所有的信号与 receiver 的槽连接
     * @param sender
     * @param receiver
     */
    static void connectAllSlotsByName(class QObject* sender, class QObject* receiver);

    /**
     * @brief connectAllSlotsByNameVariant 将 sender 与 sender 的子对象的所有的信号与 receiver 的槽连接 带校正
     * @param sender
     * @param receiver
     * @param variant
     */
    static void connectAllSlotsByNameVariant(class QObject* sender, QObject *receiver, const QMap<QString,QString>& variant);


    static void iterateObjectProperty(const QMetaObject& metaObject, std::function<bool(const QMetaProperty& property)> callBack);

    static bool isNormalType(const class QMetaType& metaType);

    static bool isNormalType(const QMetaObject& metaObject);

    static bool isNormalType(const int& typeId);

#define CONNECT_ALL_SLOTS_BY_NAME(START_ADDR, END_ADDR, RECEIVER) \
        void** ___startAddr___ = reinterpret_cast<void **>(&START_ADDR); \
        void** ___endAddr___ = reinterpret_cast<void **>(&END_ADDR); \
        QMetaObjectHelper::connectAllSlotsByName(___startAddr___, ___endAddr___, RECEIVER);

    /**
     * @brief connectAllSlotsByName 起始地址和结束地址之间的所有 QObject 的信号与  receiver 的槽连接
     * @param startAddreass
     * @param endAddress
     */
    static void connectAllSlotsByName(void** startAddreass, void** endAddress, QObject *receiver);
};


#endif // QMETAOBJECTHELPER_H
