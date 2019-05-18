#include "AModel.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QSettings>
#include <QDebug>

#include "Utils/MessageCenter.h"

AModel::AModel(QObject *parent)
    : QObject (parent)
{
    MessageCenter::instance()->registerReceiver("saveSettings", this);
    MessageCenter::instance()->registerReceiver("loadSettings", this);
}

AModel::~AModel()
{
    MessageCenter::instance()->unRegisterReceiver(this);
}

void AModel::loadSettings(QSettings *settings)
{
    if(!settings)
    {
        return ;
    }
qDebug()<<metaObject()->className()<<__FUNCTION__;
    QString className = metaObject()->className();
    className += "/";
    for(int i = 0; i < metaObject()->propertyCount(); i++)
    {
        QString key = className + (metaObject()->property(i).name());
        if(settings->contains(key))
        {
            setProperty(metaObject()->property(i).name(),settings->value(key));
        }
    }

    settings->sync();
}

void AModel::saveSettings(QSettings *settings)
{
    if(!settings)
    {
        return ;
    }
qDebug()<<metaObject()->className()<<__FUNCTION__;
    QString className = metaObject()->className();
    className += "/";
    for(int i = 0; i < metaObject()->propertyCount(); i++)
    {
        QString propertyName = metaObject()->property(i).name();
        if(canSave(propertyName))
        {
            settings->setValue(className + (metaObject()->property(i).name()),this->property(metaObject()->property(i).name()));
        }
    }

    settings->sync();
}

bool AModel::MessageReceiver(const QString &message, const QVariant &data)
{
    qDebug()<<metaObject()->className()<<__FUNCTION__;
    if("saveSettings" == message)
    {
        saveSettings(data.value<QSettings*>());
    }else if("loadSettings" == message)
    {
        loadSettings(data.value<QSettings*>());
    }

    return false;
}
