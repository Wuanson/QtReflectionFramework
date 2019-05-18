#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include <QList>
#include <QMap>
#include <QVariant>

#include "Singleton.h"

class MessageCenter : public Singleton<MessageCenter>
{
    friend Singleton<MessageCenter>;
private:
    MessageCenter();
public:
    ~MessageCenter();

    /**
     * @brief triggerMessage 触发一个消息
     * @param message
     * @param data
     */
    void triggerMessage(const QString& message, const QVariant& data = QVariant());

    /**
     * @brief registerReceiver 注册一个消息监听
     * @param message
     * @param receiver
     */
    void registerReceiver(const QString& message, class IMessageReceiver *receiver);

    /**
     * @brief unRegisterReceiver
     * @param receiver
     */
    void unRegisterReceiver(class IMessageReceiver* receiver);
private:
    QMap<QString, QList<class IMessageReceiver*>*> _messageMap;
};

#endif // MESSAGECENTER_H
