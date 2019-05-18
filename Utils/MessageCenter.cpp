#include "MessageCenter.h"

#include <QDebug>

#include "IMessageReceiver.h"

MessageCenter::MessageCenter()
{

}

MessageCenter::~MessageCenter()
{
    for(auto receivers : _messageMap)
    {
        if(receivers)
        {

            delete receivers;
            receivers = nullptr;
        }
    }
}

void MessageCenter::triggerMessage(const QString &message, const QVariant &data)
{
    if(_messageMap.contains(message) && _messageMap[message])
    {
        QList<IMessageReceiver*> recivers;

        recivers = *_messageMap[message];

        for(auto* reciver : recivers)
        {
            if(reciver)
            {
                bool isBreak(false);

                isBreak = reciver->MessageReceiver(message, data);

                if(isBreak)
                    break;
            }
        }
    }
}

void MessageCenter::registerReceiver(const QString &message, IMessageReceiver *receiver)
{
    if(_messageMap.contains(message))
    {
        _messageMap[message]->push_back(receiver);
    }
    else
    {
        _messageMap[message] = new QList<IMessageReceiver*>();
        registerReceiver(message,receiver);
    }
}

void MessageCenter::unRegisterReceiver(IMessageReceiver *receiver)
{
    for(auto irecviers : _messageMap)
    {
        QMutableListIterator<IMessageReceiver *> it(*irecviers);
        while(it.hasNext())
        {
            if(it.next() == receiver)
            {
                it.remove();
            }
        }
    }
}
