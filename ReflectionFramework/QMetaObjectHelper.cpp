#include "QMetaObjectHelper.h"

#include <QMetaClassInfo>
#include <QMetaProperty>
#include <QDebug>

#include "ClassInfo.h"

const static QMap<QString,QString> emptyMap;

QObject *QMetaObjectHelper::createQObject(bool showAssert, const QMetaObject &metaObject, QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3, QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7, QGenericArgument val8, QGenericArgument val9)
{
    auto retObject = metaObject.newInstance(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    if(showAssert && !retObject)
    {
        QString msg = QString("类型 %1 无法被实例化,可能是没有给构造函数添加 Q_INVOKABLE 标记或者参数传递不正确!").arg(metaObject.className());
        qt_assert(msg.toStdString().c_str(), __FILE__, __LINE__);
    }
    return retObject;
}

bool QMetaObjectHelper::isA(const QMetaObject &metaObject, const QString &className)
{
#if 0
    static const QString parentClass = QStringLiteral("ParentClass");
    //便利所有的classInfo，拿到ParentClass对应的value,来判断
    for(int i = 0; i < metaObject.classInfoCount();i++)
    {
        if(parentClass == metaObject.classInfo(i).name())
        {
            if(className == metaObject.classInfo(i).value())
                return true;
        }
    }
    return false;
#endif

    return hasClassInfo(metaObject,QStringLiteral("ParentClass"),className);
}

bool QMetaObjectHelper::getClassInfoValue(const QMetaObject &metaObject, const QString &key, QString &retValue)
{
    int classInfoCount = metaObject.classInfoCount();
    for(int i = 0; i < classInfoCount; ++i)
    {
        QMetaClassInfo classInfo = metaObject.classInfo(i);
        if(key == classInfo.name())
        {
            retValue = classInfo.value();
            return true;
        }
    }
    return false;
}


bool QMetaObjectHelper::getClassInfoValues(const QMetaObject &metaObject, const QStringList &keys, QMap<QString, QString> &retValues)
{
    bool isFind = false;
    int classInfoCount = metaObject.classInfoCount();
    for(int i = 0; i < classInfoCount; ++i)
    {
        QMetaClassInfo classInfo = metaObject.classInfo(i);
        for(auto key : keys)
        {
            if(key == classInfo.name())
            {
                retValues[key] = classInfo.value();
                isFind = true;
            }
        }

    }
    return isFind;
}

bool QMetaObjectHelper::hasClassInfo(const QMetaObject &metaObject, const QString &key, const QString &value)
{
    QString retValue;
    if(getClassInfoValue(metaObject,key,retValue))
    {
        return retValue == value;
    }
    return false;
}

bool QMetaObjectHelper::hasClaaInfoKey(const QMetaObject &metaObject, const QString &key)
{
    QString retValue;
    return getClassInfoValue(metaObject,key,retValue);
}

void QMetaObjectHelper::connectSlotsByNameVariant(QObject *sender, QObject *receiver, const QMap<QString, QString> &variant)
{
    if(!sender || !receiver)
    {
        return ;
    }

    //收集receiver的函数
    QMap<QString,QMetaMethod> methodMap;
    int methodCount = receiver->metaObject()->methodCount();
    for(int i = 0; i < methodCount; ++i)
    {
        QMetaMethod method = receiver->metaObject()->method(i);
        if(QMetaMethod::MethodType::Slot == method.methodType())
        {
            methodMap.insert(method.name(),method);
        }
    }

    //遍历sender的信号，全部绑定receiver的函数
    methodCount = sender->metaObject()->methodCount();
    for(int i = 0; i < methodCount; ++i)
    {
        QMetaMethod method = sender->metaObject()->method(i);
        if(QMetaMethod::MethodType::Signal == method.methodType())
        {
            QString methodName = method.name();
            if(variant.contains(methodName))
            {
                methodName = variant[methodName];
            }

            QString name = QString("on_%1_%2").arg(sender->objectName(),methodName);
            if(methodMap.contains(name))
            {
                QObject::connect(sender,method,receiver,methodMap[name],Qt::ConnectionType::UniqueConnection);
            }
        }
    }
}

void QMetaObjectHelper::connectSlotsByName(QObject *sender, QObject *receiver)
{
    if(!sender || !receiver)
    {
        return ;
    }

    connectSlotsByNameVariant(sender,receiver,emptyMap);
}

void QMetaObjectHelper::connectAllSlotsByName(QObject *sender, QObject *receiver)
{
    if(!sender || !receiver)
    {
        return ;
    }

    connectAllSlotsByNameVariant(sender, receiver, emptyMap);
}


void QMetaObjectHelper::connectAllSlotsByNameVariant(QObject *sender, QObject *receiver, const QMap<QString, QString> &variant)
{
    if(!sender || !receiver)
    {
        return ;
    }

    //连接所有子对象
    for(auto object : sender->children())
    {
        if(!object->objectName().isEmpty())
        {
            connectSlotsByNameVariant(object,receiver,variant);
        }
    }
    //连接自己
    connectSlotsByNameVariant(sender,receiver,variant);
}

void QMetaObjectHelper::iterateObjectProperty(const QMetaObject &metaObject, std::function<bool (const QMetaProperty &)> callBack)
{
    int propertyCount = metaObject.propertyCount();
    for(int i = 0; i < propertyCount; ++i)
    {
        if(callBack(metaObject.property(i)))
        {
            break;
        }
    }
}

bool QMetaObjectHelper::getMetaProperty(const QMetaObject &metaObject, const QString &propertyName, QMetaProperty &retProperty)
{
    for(int i = 0; i < metaObject.propertyCount(); ++i)
    {
        if(propertyName == metaObject.property(i).name())
        {
            retProperty = metaObject.property(i);
            return true;
        }
    }
    return false;
}

bool QMetaObjectHelper::isNormalType(const QMetaType &metaType)
{
    return  isNormalType(metaType.id());
}

bool QMetaObjectHelper::isNormalType(const QMetaObject &metaObject)
{
    return isNormalType(QMetaType::type(metaObject.className()));
}

bool QMetaObjectHelper::isNormalType(const int &typeId)
{
    return     (typeId >= QMetaType::Bool && typeId <= QMetaType::Double)
            || (typeId >= QMetaType::Long && typeId <= QMetaType::Float)
            || typeId == QMetaType::QString
            || typeId == QMetaType::SChar
            || typeId == QMetaType::QChar
            ;
}

void QMetaObjectHelper::connectAllSlotsByName(void **startAddreass, void **endAddress, QObject *receiver)
{
    while(endAddress >= startAddreass)
    {
        QObject* tempObjectPtr = static_cast<QObject*>(*startAddreass);
        tempObjectPtr = dynamic_cast<QObject*>(tempObjectPtr);
        if(tempObjectPtr)
        {
            connectSlotsByName(tempObjectPtr, receiver);
        }

        startAddreass += 1;//二级指针的步长为4，它永远都是指向一个指针，所以步长为4
    }
}
