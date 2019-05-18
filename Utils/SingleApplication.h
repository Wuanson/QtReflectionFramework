#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#ifdef IS_QML
#include <QGuiApplication>
#elif IS_QWIDGET
#include <QApplication>
#else
#include <QCoreApplication>
#endif

/**
 * @brief The SingleApplication class 单例QGuiApplication
 */
class SingleApplication :
#ifdef IS_QML
        public QGuiApplication
#elif  IS_QWIDGET
        public QApplication
#else
        public QCoreApplication
#endif
{
    Q_OBJECT

public:
    explicit SingleApplication(int &argc, char **argv);

    /**
     * @brief isRunning 检查是否已经有一个实例在运行
     * @return true - 有实例运行， false - 没有实例运行
     */
    bool isRunning();

    /**
     * @brief run 运行
     * @return
     */
#ifdef IS_QML
    int run(const QString& qmlUrl);
#else
    int run();
#endif

signals:

public slots:
    /**
     * @brief newLocalConnection 有新连接时触发
     * 通过socket通讯实现程序单实例运行，监听到新的连接时触发该函数
     */
    void newLocalConnection();

private:
    /**
     * @brief initReflection 初始化
     * @param qmlAppEngine
     */
    void initReflection(
#ifdef IS_QML
            class QQmlApplicationEngine* m_qmlAppEngine
#endif
            );

#ifdef IS_QWIDGET
    /**
     * @brief initMainWindow 构建MainWindow对象
     */
    QWidget* initMainWindow();
#endif

    /**
     * @brief initLocalConnection 初始化本地连接
     * 通过socket通讯实现程序单实例运行
     * 如果连接不上server，则创建，否则退出
     */
    bool initLocalConnection();

    /**
     * @brief newLocalServer 创建服务端(LocalServer)
     */
    void newLocalServer();

private:
    /**
     * @brief bRunning 是否已经有实例在运行
     */
    bool _isRunning;

    /**
     * @brief localServer 本地socket Server
     */
    class QLocalServer *m_localServer;

    /**
     * @brief serverName 服务名称
     */
    static const QString m_serverName;

#ifdef IS_QML
    /**
     * @brief mainWindow 主窗口对象
     */
    class QQuickWindow*
#else
    class QWidget*
#endif
            m_mainWindow;

    /**
     * @brief qmlAppEngine
     */
    class QQmlApplicationEngine* m_qmlAppEngine;
};

#endif // SINGLEAPPLICATION_H
