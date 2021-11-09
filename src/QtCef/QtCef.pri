QT       +=

CONFIG += c++11

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/cef/include

HEADERS += \
    $$PWD/qcefwidget.h \
    $$PWD/qtcef.h \
    $$PWD/qtcefapp.h

SOURCES += \
    $$PWD/qcefwidget.cpp \
    $$PWD/qtcef.cpp \
    $$PWD/qtcefapp.cpp

# CEF
win32: LIBS += -L$$PWD/cef/Release/ -llibcef

INCLUDEPATH += $$PWD/cef
DEPENDPATH += $$PWD/cef

win32: LIBS += -L$$PWD/cef/libcef_dll_wrapper/Release/ -llibcef_dll_wrapper

QMAKE_CXXFLAGS_RELEASE += /MT
QMAKE_CXXFLAGS_DEBUG += /MT#/MTd

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/cef/libcef_dll_wrapper/Release/libcef_dll_wrapper.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/cef/libcef_dll_wrapper/Release/libcef_dll_wrapper.a
