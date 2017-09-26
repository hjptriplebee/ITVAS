TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../control_logic/release/ -lcontrol_logic
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../control_logic/debug/ -lcontrol_logic
else:unix: LIBS += -L$$OUT_PWD/../control_logic/ -lcontrol_logic

INCLUDEPATH += $$PWD/../control_logic
DEPENDPATH += $$PWD/../control_logic
