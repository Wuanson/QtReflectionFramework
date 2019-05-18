#ifndef QDEBUGMESSAGETOFILE_H
#define QDEBUGMESSAGETOFILE_H

#ifdef QT_NO_DEBUG


#include <qlogging.h>
#include <QString>

/**
 * @brief The QDebugMessageToFile class
 * 
 * 将Qt 的qDebug等写到文件
 */
class QDebugMessageToFile
{
public:
    QDebugMessageToFile();
    ~QDebugMessageToFile();
    
public:
    void logMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    
    void saveLogFile();
private:
    QString log;
};

#endif

#endif // QDEBUGMESSAGETOFILE_H
