#ifndef IMESSAGE_RECEVIER_H
#define IMESSAGE_RECEVIER_H

#include <QString>
#include <QVariant>

class IMessageReceiver
{
public:
    /**
     * @brief MessageReceiver
     * @param message
     * @param data
     * @return 返回true则中断
     */
    virtual bool MessageReceiver(const QString& message, const QVariant& data) = 0;
};

#endif // IMESSAGE_RECEVIER_H
