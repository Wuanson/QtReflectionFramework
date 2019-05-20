#ifndef QMLCONTROLLERINIT_H
#define QMLCONTROLLERINIT_H

#ifdef IS_QML

#include <QObject>

#include "ReflectionFramework/ClassInfo.h"

class QmlControllerInit : public QObject
{
    Q_OBJECT
    Q_REF_INIT

public:
    Q_INVOKABLE explicit QmlControllerInit(QObject *parent = nullptr);

signals:

public slots:
};

#endif

#endif // QMLCONTROLLERINIT_H
