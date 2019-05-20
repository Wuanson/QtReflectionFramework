#ifndef IENTITY_H
#define IENTITY_H

#include <QJsonObject>

class IEntity
{
public:
    virtual void fromJson(QJsonObject& json) = 0;
    virtual class QString toJson() = 0;
    virtual QJsonObject toJsonObject() = 0;
};

#endif // IENTITY_H
