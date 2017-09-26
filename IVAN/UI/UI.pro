#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T15:54:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../control_logic/release/ -lcontrol_logic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../control_logic/debug/ -lcontrol_logic
else:unix: LIBS += -L$$OUT_PWD/../control_logic/ -lcontrol_logic

INCLUDEPATH += $$PWD/../control_logic
DEPENDPATH += $$PWD/../control_logic

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core


INCLUDEPATH += D:\opencv\build\install\include
INCLUDEPATH += D:\opencv\build\install\include\opencv
INCLUDEPATH += D:\opencv\build\install\include\opencv2

LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_core320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_core320d.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_highgui320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_highgui320d.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_imgproc320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_imgproc320d.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_tracking320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_video320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_videoio320.lib
LIBS +=D:\opencv\build\install\x64\vc14\lib\opencv_videostab320.lib
