# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$PWD/Init/QWidgetControllerInit.cpp \
    $$PWD/Init/QmlControllerInit.cpp \
    $$PWD/Init/WebControllerInit.cpp \
    $$PWD/MessageCenter.cpp \
    $$PWD/Interface/AModel.cpp \
    $$PWD/QDebugMessageToFile.cpp \
    $$PWD/SingleApplication.cpp \
    $$PWD/SystemGateway.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

HEADERS += \
    $$PWD/Init/QWidgetControllerInit.h \
    $$PWD/Init/QmlControllerInit.h \
    $$PWD/Init/WebControllerInit.h \
    $$PWD/Interface/AModel.h \
    $$PWD/Interface/ISystem.h \
    $$PWD/Interface/IView.h \
    $$PWD/IMessageReceiver.h \
    $$PWD/MessageCenter.h \
    $$PWD/QDebugMessageToFile.h \
    $$PWD/SingleApplication.h \
    $$PWD/Singleton.h \
    $$PWD/SystemGateway.h
