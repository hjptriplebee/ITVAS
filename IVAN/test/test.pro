TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /home/jipeng/anaconda3/include/python3.6m

LIBS += /usr/local/lib/libopencv_highgui.so
LIBS += /usr/local/lib/libopencv_core.so
LIBS += /usr/local/lib/libopencv_imgproc.so
LIBS += /usr/local/lib/libopencv_videoio.so
LIBS += /usr/local/lib/libopencv_imgcodecs.so
LIBS += -lpython3.6m

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/release/ -lBasic_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/debug/ -lBasic_algorithm
else:unix: LIBS += -L$$OUT_PWD/../Basic_algorithm/ -lBasic_algorithm

INCLUDEPATH += $$PWD/../Basic_algorithm
DEPENDPATH += $$PWD/../Basic_algorithm
