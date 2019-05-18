#pragma once

#include <QMetaType>
#include "ClassCollection.h"

/**
  *@brief REG_QCLASS 注册一个QObject到元对象收集系统
  */
#define REG_QCLASS(CLASSNAME) \
static struct _____##CLASSNAME##_____ \
{ \
    _____##CLASSNAME##_____() \
    { \
        ClassCollection::registerClass(CLASSNAME::staticMetaObject); \
        qRegisterMetaType<CLASSNAME*>(#CLASSNAME"*"); \
        delete this; \
    } \
}P_____________##CLASSNAME##_____;

/**
  *@brief Q_REGISTER_METATYPE 注册一个普通类型到Qt的元对象系统
  */
#define Q_REGISTER_METATYPE(CLASSNAME) \
static struct _____REGISTER_METATYPE__##CLASSNAME##_____ \
{ \
    _____REGISTER_METATYPE__##CLASSNAME##_____() \
    { \
        qRegisterMetaType<CLASSNAME>(#CLASSNAME); \
        qRegisterMetaType<CLASSNAME*>(#CLASSNAME"*"); \
        delete this; \
    } \
}P___________REGISTER_METATYPE__##CLASSNAME##_____________;


#define STATIC_INIT_START(CLASSNAME) \
static struct _____STATIC_INIT__##CLASSNAME##_____ \
{ \
    _____STATIC_INIT__##CLASSNAME##_____() \
    { \

#define STATIC_INIT_END(CLASSNAME) delete this;}} \
P___________STATIC_INIT__##CLASSNAME##_____________;

