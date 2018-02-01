#-------------------------------------------------
#
# Project created by QtCreator 2017-12-26T21:38:13
#
#-------------------------------------------------

QT       -= gui

TARGET = Basic_algorithm
TEMPLATE = lib

DEFINES += BASIC_ALGORITHM_LIBRARY

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
    Detection.cpp \
    DetectionRCNN.cpp \
    DetectionFrameDiff.cpp \
    Match.cpp

HEADERS += \
    Basic_algorithm.h \
    Detection.h \
    DetectionRCNN.h \
    DetectionFrameDiff.h \
    Match.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


INCLUDEPATH += /home/jipeng/anaconda3/include/python3.6m
INCLUDEPATH += /home/jipeng/anaconda3/lib/python3.6/site-packages/numpy/core/include/numpy
LIBS += -lpython3.6m

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core


DISTFILES += \
    Detection.config \
    DetectionFrameDiff.config



