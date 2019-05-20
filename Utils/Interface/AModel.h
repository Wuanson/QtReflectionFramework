#ifndef AMODEL_H
#define AMODEL_H

#include <QObject>
#include <QSet>
#include <QVariant>

class AModel : public QObject
{
    Q_OBJECT
public:
    AModel(QObject* parent);
    virtual ~AModel() override;

public:
    /**
     * @brief loadFromFile 从文件读取配置
     */
    virtual void loadSettings(class QSettings *settings);

    /**
     * @brief saveToFile 写配置到文件
     */
    virtual void saveSettings(class QSettings *settings);

public:
    /**
     * @brief canSave
     * @param propertyName
     * @return 返回true表示该属性可以被保存
     */
    virtual bool canSave(const QString& propertyName) = 0;
};
#endif // AMODEL_H
