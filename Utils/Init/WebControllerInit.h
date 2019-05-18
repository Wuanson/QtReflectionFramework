#ifndef WEBCONTROLLERINIT_H
#define WEBCONTROLLERINIT_H

#ifdef IS_WEBSERVER

#include <QObject>

#include "ReflectionFramework/ClassInfo.h"

class WebControllerInit : public QObject
{
    Q_OBJECT
    Q_REF_INIT
public:
    Q_INVOKABLE explicit WebControllerInit(QObject *parent = nullptr);

protected:
    void initControllers();
};

#endif
#endif // WEBCONTROLLERINIT_H
