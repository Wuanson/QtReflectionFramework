#ifndef QJSONHELPER_H
#define QJSONHELPER_H

class QJsonObject;
class QJsonValue;
class QObject;
template <typename T>
class QList;
class QVariant;
class QString;
class QMetaProperty;

class QJsonHelper
{
public:
    QJsonHelper();

public:
    static void fromJson(QObject* object, QJsonObject& jsonObject);
    static QString toJson(QObject* object);
    static QJsonObject toJsonObject(QObject* object);
protected:
    static void buildQList(QList<QVariant>& list, QJsonValue& values);
    static void buildQList(QList<QList<QVariant>>& lists, QJsonValue& values);
    static int getArrayNestedCount(const QString& type);
    static bool isArrayType(const QString& type);
};

#endif // QJSONHELPER_H
