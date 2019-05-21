#include "SingleApplication.h"

#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>
#include <QSettings>
#include <QDir>
#include <QDebug>
#ifdef IS_QML
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#elif IS_QWIDGET
#include <QWidget>
#include <qt_windows.h>
#endif

#include "ReflectionFramework/ClassCollection.h"
#include "ReflectionFramework/ClassInfo.h"
#include "ReflectionFramework/QMetaObjectHelper.h"

#ifdef QT_NO_DEBUG
#include "Utils/QDebugMessageToFile.h"
static QDebugMessageToFile debugMessageToFile;
#ifdef __cplusplus
extern "C"{
#endif
void gLogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    debugMessageToFile.logMsgOutput(type,context,msg);
}
#ifdef __cplusplus
}
#endif
#endif
#include "Utils/ModelCollection.h"

//初始化LocalServer的名字
const QString SingleApplication::m_serverName = "HymsonToolServer@!@#%$%^^&*()";

SingleApplication::SingleApplication(int &argc, char **argv)
#ifdef IS_QML
    : QGuiApplication(argc, argv)
    #elif IS_QWIDGET
    : QApplication(argc, argv)
    #else
    : QCoreApplication(argc, argv)
    #endif
    , _isRunning(false)
    , m_localServer(nullptr)
{

    for(int i = 0; i < argc; ++i)
    {
#if QT_NO_DEBUG
        if(QStringLiteral("autoStart") == argv[i])
        {
            setAutoStart(true);
            continue ;
        }
        else if(QStringLiteral("disableAutoStart") == argv[i])
        {
            setAutoStart(false);
            continue ;
        }
#endif
    }

#if QT_NO_DEBUG
    qDebug()<<"参数 autoStart 打开开机自启, disableAutoStart 关闭开机自启";
#endif

    if(initLocalConnection())
    {
        connect(this, &SingleApplication::aboutToQuit, [&]()
        {
            ModelCollection::instance()->saveSettings();
        });
    }
}

bool SingleApplication::isRunning()
{
    return _isRunning;
}

#ifdef IS_QML
int SingleApplication::run(const QString &qmlUrl)
#else
int SingleApplication::run()
#endif
{
    if(!isRunning())
    {
#ifdef QT_NO_DEBUG
        qInstallMessageHandler(gLogMsgOutput);
#endif
#ifdef IS_QML
        QQmlApplicationEngine engine;
        initReflection(&engine);
        const QUrl url(qmlUrl);
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         this, [&](QObject *obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
            else
            {
                m_qmlAppEngine = &engine;
                //找到主窗口
                auto rootObjects = m_qmlAppEngine->rootObjects();
                for(auto object : rootObjects)
                {
                    auto window = qobject_cast<QQuickWindow*>(object);
                    if(window)
                    {
                        m_mainWindow = window;
                        break;
                    }
                }
            }
        }, Qt::QueuedConnection);
        engine.load(url);
        return exec();
#elif IS_QWIDGET
        m_mainWindow = initMainWindow();
        initReflection();
        m_mainWindow->show();
        return exec();
#else
        initReflection();
        return exec();
#endif
    }
    return -1;
}

void SingleApplication::newLocalConnection()
{
    QLocalSocket *socket = m_localServer->nextPendingConnection();
    if (!socket)
        return;
    socket->waitForReadyRead(1000);

    QTextStream stream(socket);
    //在此处处理消息
    if(stream.readAll().trimmed() == "show")
    {
        if(m_mainWindow)
        {
#ifndef IS_WEBSERVER
#ifdef IS_QML
            if (QWindow::Visibility::Minimized == m_mainWindow->visibility())
#else
            if(m_mainWindow->isMinimized())
#endif
            {
                m_mainWindow->showNormal();
            }
            //设置窗口置顶
            ::SetWindowPos(HWND(m_mainWindow->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            ::SetWindowPos(HWND(m_mainWindow->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

            m_mainWindow->show();
#ifndef IS_QML
            m_mainWindow->activateWindow();
#endif
#endif
        }
    }

    socket->flush();
    socket->close();
    socket->deleteLater();
}

void SingleApplication::initReflection(
        #ifdef IS_QML
        QQmlApplicationEngine *qmlAppEngine
        #endif
        )
{
    QObject parent;
    auto metaObjectMap = ClassCollection::getMetaObjectMap();
    for(auto metaObject : metaObjectMap)
    {
        if(QMetaObjectHelper::hasClaaInfoKey(metaObject,QStringLiteral(KEY_REF_INIT)))
        {
            //Q_REF_INIT 的构造函数
            //第一个QObject* parent
            //如果有使用QML,则有第二个QQmlApplicationEngine*
            auto refInit = metaObject.newInstance(Q_ARG(QObject*,&parent)
                                      #ifdef IS_QML
                                                  , Q_ARG(QQmlApplicationEngine*,qmlAppEngine)
                                      #endif
                                                  );
            if(!refInit)
            {
                qt_assert(QString("类型 %1 含有 %2 标记，但是不能够通过反射构建对象!可能是参数不对或者没有加Q_INVOKABLE").arg(metaObject.className(),KEY_REF_INIT).toStdString().c_str()
                          ,__FILE__, __LINE__);
            }
        }
    }
}

#ifdef IS_QWIDGET
QWidget *SingleApplication::initMainWindow()
{
    auto metaObjectMap = ClassCollection::getMetaObjectMap();
    for(auto metaObject : metaObjectMap)
    {
        if(QMetaObjectHelper::hasClaaInfoKey(metaObject, KEY_MAIN_WINDOW))
        {
            auto mainWindow = QMetaObjectHelper::createQObject(true, metaObject);
            auto retMainWindow = qobject_cast<QWidget*>(mainWindow);
            if(retMainWindow)
            {
                return  retMainWindow;
            }
            else
            {
                qt_assert(QString("%1 无法转换为QWidget!").arg(metaObject.className()).toStdString()
                          .c_str(),__FILE__,__LINE__);
            }
        }
    }
    qt_assert("没有找到有标记MainWindow的类",__FILE__,__LINE__);
}
#endif

bool SingleApplication::initLocalConnection()
{
    _isRunning = false;
    QLocalSocket socket;
    socket.connectToServer(m_serverName);
    if(socket.waitForConnected(500))
    {
        _isRunning = true;
        // 其他处理，如：将启动参数发送到服务端
        QTextStream stream(&socket);
        //TODO 在此处给实例发送消息   stream<<xxx;
        stream<<"show";

        stream.flush();
        socket.waitForBytesWritten();
        socket.flush();
        socket.close();
        return false;
    }

    //连接不上服务器，就创建一个
    newLocalServer();
    return true;
}

void SingleApplication::newLocalServer()
{
    m_localServer = new QLocalServer(this);
    connect(m_localServer, SIGNAL(newConnection()), this, SLOT(newLocalConnection()));

    //监听
    if(!m_localServer->listen(m_serverName))
    {
        // 此时监听失败，可能是程序崩溃时,残留进程服务导致的,移除之
        if(m_localServer->serverError() == QAbstractSocket::AddressInUseError)
        {
            qCritical()<<"listen error = QAbstractSocket::AddressInUseError";
            qDebug()<<"relisten...";
            QLocalServer::removeServer(m_serverName); // <-- 重点
            m_localServer->listen(m_serverName); // 再次监听
        }
    }
}


void SingleApplication::setAutoStart(bool isAutoStart)
{
    qDebug()<<__FUNCTION__<<":"<<isAutoStart;

#if QT_NO_DEBUG
#ifdef Q_OS_WIN32
    QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",QSettings::NativeFormat);
    QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    QString appName = QCoreApplication::applicationName();

    if(isAutoStart)
    {
        reg.setValue(appName,appPath);
    }
    else
    {
        reg.remove(appName);
    }
#endif

#ifdef Q_OS_LINUX
    //TODO 写.desktop文件, 到/etc/xdg/autostart目录下
#endif

#ifdef Q_OS_MACOS
    if (isAutoStart){
        LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
        CFURLRef url = (CFURLRef)[NSURL fileURLWithPath:QStringToNSString(appPath)];
        LSSharedFileListItemRef item = LSSharedFileListInsertItemURL(loginItems, kLSSharedFileListItemLast, NULL, NULL, url, NULL, NULL);
        CFRelease(item);
        CFRelease(loginItems);
    }else{
        UInt32 seedValue;
        CFURLRef thePath;
        LSSharedFileListRef loginItems = LSSharedFileListCreate(NULL, kLSSharedFileListSessionLoginItems, NULL);
        CFArrayRef loginItemsArray = LSSharedFileListCopySnapshot(loginItems, &seedValue);
        for (id item in (NSArray *)loginItemsArray) {
            LSSharedFileListItemRef itemRef = (LSSharedFileListItemRef)item;
            if (LSSharedFileListItemResolve(itemRef, 0, (CFURLRef*) &thePath, NULL) == noErr){
                if ([[(NSURL *)thePath path] hasPrefix:QStringToNSString(appPath)]){
                    LSSharedFileListItemRemove(loginItems, itemRef);
                }
                CFRelease(thePath);
            }
        }
        CFRelease(loginItemsArray);
        CFRelease(loginItems);
    }
#endif
#endif
}
