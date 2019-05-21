#include "QJsonHelper.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMetaObject>
#include <QObject>
#include <QMetaProperty>
#include <QJsonValue>
#include <QDebug>

#include "ReflectionFramework/Reflection.h"
#include "ReflectionFramework/QMetaObjectHelper.h"

#define REG_QLIST_META_TYPE(TYPE) \
    qRegisterMetaType<QList<TYPE>>("QList<"#TYPE">"); \
    qRegisterMetaType<QList<QList<TYPE>>>("QList<QList<<"#TYPE">>");

STATIC_INIT_START(QJsonHelper)
/*qRegisterMetaType<QList<QVariant>>("QList<QVariant>");
qRegisterMetaType<QList<QList<QVariant>>>("QList<QList<QVariant>>");
qRegisterMetaType<QList<double>>("QList<double>");
qRegisterMetaType<QList<QList<double>>>("QList<QList<double>>");
qRegisterMetaType<QList<float>>("QList<float>");
qRegisterMetaType<QList<QList<float>>>("QList<QList<float>>");*/
REG_QLIST_META_TYPE(QVariant);
REG_QLIST_META_TYPE(double);
REG_QLIST_META_TYPE(float);
REG_QLIST_META_TYPE(int);
REG_QLIST_META_TYPE(QString);
REG_QLIST_META_TYPE(bool);
STATIC_INIT_END(QJsonHelper)

#define INJECT_ARRAY_BODY(OBJECT,PROPERTY,LIST,JSONVALUE) \
    buildQList(LIST, JSONVALUE); \
    auto ____propertyType____ = QMetaType::type(PROPERTY.typeName()); \
    QVariant ____var____(____propertyType____, &LIST); \
    property.write(OBJECT, ____var____);


QJsonHelper::QJsonHelper()
{

}

void QJsonHelper::fromJson(QObject *object, QJsonObject &jsonObject)
{
    if(!object)
    {
        return ;
    }

    for(int i = 0; i < object->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty property = object->metaObject()->property(i);

        if(jsonObject.contains(property.name()))
        {
            //注入数组
            if(isArrayType(property.typeName()))
            {
                QJsonValue value = jsonObject.take(property.name());
                QString type;
                int count = getArrayNestedCount(property.typeName());
                if(1 == count)
                {
                    QList<QVariant> list;
                    INJECT_ARRAY_BODY(object, property, list, value);
                }
                else if(2 == count)
                {
                    QList<QList<QVariant>> list;
                    INJECT_ARRAY_BODY(object, property, list, value);
                }
            }
            else
            {
                auto propertyType = QMetaType::type(property.typeName());
                QMetaType type(propertyType);

                //如果是 QObject ,那么继续注入
                static QObject obj;
                if(type.metaObject() && type.metaObject()->inherits(obj.metaObject()))
                {
                    QObject* newObject = QMetaObjectHelper::createQObject(true, *type.metaObject());
                    QJsonObject value = jsonObject.take(property.name()).toObject();
                    fromJson(newObject, value);
                }
                else
                {
                    QVariant value = jsonObject.take(property.name()).toVariant();
                    auto propertyType = QMetaType::type(property.typeName());
                    if(value.canConvert(propertyType))
                    {
                        value.convert(propertyType);
                    }
                    property.write(object, value);
                }
            }
        }
    }
}

QString QJsonHelper::toJson(QObject* object)
{
    QJsonDocument doc(toJsonObject(object));
    return doc.toJson();
}

QJsonObject QJsonHelper::toJsonObject(QObject* object)
{
    if(!object)
    {
        return QJsonObject();
    }

    static QObject obj;

    QJsonObject jsonObject;
    for(int i = 0; i < object->metaObject()->propertyCount(); ++i)
    {
        QMetaProperty property = object->metaObject()->property(i);
        QVariant var = property.read(object);
        QMetaType type(QMetaType::type(property.typeName()));

        if(isArrayType(property.typeName()))
        {
            int count = getArrayNestedCount(property.typeName());
            QJsonArray array;
            if(1 == count)
            {
                QList<QVariant> list = var.toList();
                for(auto data : list)
                {
                    array.push_back(QJsonValue::fromVariant(data));
                }
            }
            else if(2 == count)
            {
                QList<QVariant> list = var.value<QList<QVariant>>();
                for(auto data : list)
                {
                    QList<QVariant> _list = data.value<QList<QVariant>>();
                    QJsonArray _array;
                    for(auto _data : _list)
                    {
                        _array.push_back(QJsonValue::fromVariant(_data));
                    }
                    array.push_back(_array);
                }
            }
            jsonObject.insert(property.name(), array);
        }
        else if(type.metaObject() && type.metaObject()->inherits(obj.metaObject()))
        {
            jsonObject.insert(property.name(), toJsonObject(var.value<QObject*>()));
        }
        else
        {
            jsonObject.insert(property.name(), QJsonValue::fromVariant(var));
        }
    }
    return jsonObject;
}

void QJsonHelper::buildQList(QList<QVariant>& list, QJsonValue& values)
{
    if(values.isArray())
    {
        for(auto value : values.toArray())
        {
            if(value.isArray())
            {
                QList<QVariant> _list;
                QJsonValue _values = value.toArray();
                buildQList(_list, _values);
                list.push_back(QVariant::fromValue(_list));
            }
            else
            {
                list.push_back(value.toVariant());
            }
        }
    }
}

void QJsonHelper::buildQList(QList<QList<QVariant>>& lists, QJsonValue& values)
{
    if(values.isArray())
    {
        for(auto value : values.toArray())
        {
            if(value.isArray())
            {
                QList<QVariant> _list;
                QJsonValue _values = value.toArray();
                buildQList(_list, _values);
                lists.push_back(_list);
            }
        }
    }
}

int QJsonHelper::getArrayNestedCount(const QString &type)
{
    QString typeCopy = type;
    typeCopy.remove(QRegExp("\\s"));
    int ret = 0;
    while(true)
    {
        if(typeCopy.left(5) == QStringLiteral("QList"))
        {
            ret++;
            typeCopy = typeCopy.mid(6, typeCopy.length() - 7);
        }else
        {
            return ret;
        }
    }
}

bool QJsonHelper::isArrayType(const QString &type)
{
    return type.left(5) == QStringLiteral("QList");
}
