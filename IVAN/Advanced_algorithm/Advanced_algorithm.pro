#-------------------------------------------------
#
# Project created by QtCreator 2017-12-27T22:44:54
#
#-------------------------------------------------

QT       -= gui

TARGET = Advanced_algorithm
TEMPLATE = lib

DEFINES += ADVANCED_ALGORITHM_LIBRARY

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
    ReID.cpp

HEADERS += \
        Advanced_algorithm.h \
    Advanced_algorithm_global.h \
    ReID.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/release/ -lBasic_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/debug/ -lBasic_algorithm
else:unix: LIBS += -L$$OUT_PWD/../Basic_algorithm/ -lBasic_algorithm

INCLUDEPATH += $$PWD/../Basic_algorithm
DEPENDPATH += $$PWD/../Basic_algorithm

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core
