#ifndef MODELCOLLECTION_H
#define MODELCOLLECTION_H

#include <QMap>

#include "Singleton.h"

class ModelCollection : public Singleton<ModelCollection>
{
    friend Singleton<ModelCollection>;
private:
    ModelCollection();

public:
    ~ModelCollection();

public:
    void insertModel(const QString& className, class AModel* model);
    void removeModel(const QString& className);
    class AModel* takeModel(const QString& className);
    class AModel* getModel(const QString& className);
    void loadSettings();
    void saveSettings();
private:
    QMap<QString, class AModel*> m_modelMap;
    class QSettings* m_settings;
};

#endif // MODELCOLLECTION_H
