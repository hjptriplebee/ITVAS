#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T15:56:17
#
#-------------------------------------------------

QT       -= gui

TARGET = core
TEMPLATE = lib

DEFINES += CORE_LIBRARY

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
        core.cpp \
    Blob.cpp \
    core.cpp \
    Blob.cpp \
    computationalgeometry.cpp \
    core.cpp

HEADERS += \
        core.h \
        core_global.h \ 
    Blob.h \
    core.h \
    core_global.h \
    Blob.h \
    ComputationalGeometry.h \
    core.h \
    core_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


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
