# QtReflectionFramework
一个基于Qt的反射功能的框架
这套框架有什么用？
框架包含自动给创建QWidget和QML还有一个简单的RPC。
这套框架利用反射来解耦,当你要使用某个类的时候不需要去引用头文件,可以直接获取到它的QMetaObject，通过QMetaObject就能构造出它的实例。
类似于Java Spring的IOC功能和属性注入的功能。

使用方法：
下载代码,放到项目根目录,修改项目的pro文件,添加两行:
```
include (ReflectionFramework/ReflectionFramework.pri)
include (Utils/Utils.pri)
```
根据项目的类型添加一个宏定义:
```
DEFINES += IS_QML //QML项目添加此宏
DEFINES += IS_QWIDGET //QWidget项目添加此宏
```

改造main函数，整个main.c只需要包含如下5行代码(如果不需要程序只能运行一个实例,需要手动修改):
```

#include "Utils/SingleApplication.h"

int main(int argc, char *argv[])
{
    return SingleApplication(argc, argv).run();
}

```

首先要注册QObject对象,在每个类的cpp文件中加入如下代码:
```
#include "ReflectionFramework/Reflection.h"
REG_QCLASS(your class name)
```
将your class name替换成你的类名,这个类就会被注册到ClassCollection类里面,后续可以在ClassCollection类里面找到它
例:
````
//.h
#include <QObject>
class MyClass : QObject
{
  Q_OBJECT
  Q_INVOKABLE MyClass(Qobject* parent);
}

//.cpp
#include "MyClass.h"

#include "ReflectionFramework/Reflection.h"
REG_QCLASS(MyClass)

MyClass::MyClass(Qobject* parent)
  : QObject(parent)
{
}
````

STATIC_INIT_START和STATIC_INIT_END宏
这两个宏可以用于初始化,这两个宏之间的代码块会在静态数据初始化之前被初始化，所以最好不要在这个代码块里访问静态数据,因为执行该代码块时静态数据还没有分配。

例:
````
#include <QList>
class Test{
public:
  static QList<int> testList; 
};
STATIC_INIT_START(MyClass)
testList.push_back(1);//此时程序会中断,因为程序还没有为静态数据分配空间。
STATIC_INIT_END(MyClass)
````

框架有四个重要的东西，分别是System(也可以说是Controller)和Model和View还有Init
System代表控制层
Model代表模型层
View代表显示层
Init是用于初始化的
对应的System和View都持有Model对象,System和View通过Model对象的信号来通讯以达到解耦

使用Init:
只要在QObject的子类的Q_OBJECT宏的下面加上Q_REF_INIT宏，就能在程序开始的时候被构造,只需要在构造函数写对应的初始化逻辑就行。

使用System:
首先引用头文件
```
#include "Utils/Interface/ISystem.h"
#include "ReflectionFramework/ClassInfo.h"
```

然后用自己的类继承自QObject和ISystem,在Q_OBJECT下面添加Q_SYSTEM宏和Q_MODEL_NAME宏
Q_SYSTEM宏用来标记一个System，Q_MODEL_NAME用来标记Model的名称,方便注入Model。
例:
```
#include <QObject>

#include "Utils/Interface/ISystem.h"
#include "ReflectionFramework/ClassInfo.h"
class MySystem : public QObject, public ISystem
{
    Q_OBJECT
    Q_SYSTEM
    Q_MODEL_NAME(id)
public:
    Q_INVOKABLE explicit WaveformSystem(QObject *parent = nullptr);
    
    Q_PROPERTY(int id READ id WRITE setId)
};
```

