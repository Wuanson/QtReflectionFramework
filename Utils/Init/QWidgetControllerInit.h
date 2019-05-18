#ifndef QWIDGETCONTROLLERINIT_H
#define QWIDGETCONTROLLERINIT_H

#ifdef IS_QWIDGET

#include <QObject>

#include "ReflectionFramework/ClassInfo.h"

class QWidgetControllerInit : public QObject
{
    Q_OBJECT
    Q_SYSTEM

public:
    Q_INVOKABLE explicit QWidgetControllerInit(QObject *parent = nullptr);

signals:

public slots:
};

#endif

#endif // QWIDGETCONTROLLERINIT_H
