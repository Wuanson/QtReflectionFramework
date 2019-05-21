#include "ModelCollection.h"

#include <QDebug>
#include <QSettings>
#include <QTimer>

#include "Interface/AModel.h"

ModelCollection::ModelCollection()
 : m_settings(new QSettings("config.ini", QSettings::Format::IniFormat, nullptr))
{
    m_modelMap.clear();

    //5min 保存一次数据
    QTimer* timer = new QTimer();
    timer->start(50000);
    QObject::connect(timer, &QTimer::timeout, [&]()
    {
        this->saveSettings();
    });
}

ModelCollection::~ModelCollection()
{
    m_settings->sync();
    m_settings->deleteLater();
    m_settings = nullptr;
}

void ModelCollection::insertModel(const QString &className, AModel *model)
{
    m_modelMap.insert(className, model);
}

void ModelCollection::removeModel(const QString &className)
{
    m_modelMap.remove(className);
}

AModel* ModelCollection::takeModel(const QString &className)
{
    return m_modelMap.take(className);
}

AModel *ModelCollection::getModel(const QString &className)
{
    if(QStringLiteral("*") == className.right(1))
    {
        return m_modelMap.value(className.left(className.length() - 1), nullptr);
    }
    else
    {
        return m_modelMap.value(className, nullptr);
    }
}

void ModelCollection::loadSettings()
{
    if(!m_settings)
    {
        return ;
    }

    for(auto model : m_modelMap)
    {
        model->loadSettings(m_settings);
    }
}

void ModelCollection::saveSettings()
{
    if(!m_settings)
    {
        return ;
    }
    for(auto model : m_modelMap)
    {
        model->saveSettings(m_settings);
    }
    m_settings->sync();
}
