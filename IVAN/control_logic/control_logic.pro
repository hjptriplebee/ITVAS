#-------------------------------------------------
#
# Project created by QtCreator 2017-09-20T16:02:28
#
#-------------------------------------------------

QT       -= gui

TARGET = control_logic
TEMPLATE = lib

DEFINES += CONTROL_LOGIC_LIBRARY

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
        control_logic.cpp

HEADERS += \
        control_logic.h \
        control_logic_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../basic_algorithm/release/ -lbasic_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../basic_algorithm/debug/ -lbasic_algorithm
else:unix: LIBS += -L$$OUT_PWD/../basic_algorithm/ -lbasic_algorithm

INCLUDEPATH += $$PWD/../basic_algorithm
DEPENDPATH += $$PWD/../basic_algorithm

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../advanced_algorithm/release/ -ladvanced_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../advanced_algorithm/debug/ -ladvanced_algorithm
else:unix: LIBS += -L$$OUT_PWD/../advanced_algorithm/ -ladvanced_algorithm

INCLUDEPATH += $$PWD/../advanced_algorithm
DEPENDPATH += $$PWD/../advanced_algorithm
