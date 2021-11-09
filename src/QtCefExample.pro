QT       += core gui

include(QtCef/QtCef.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS +=

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: {
    #Including SSL for Windows. The corresponding Qt-module must be installed!!!

    contains(QT_ARCH, i386) {
        #For Windows x32
        INCLUDEPATH += $$(QTDIR)/../../Tools/OpenSSL/Win_x86/include
    } else {
        #For Windows x64
        INCLUDEPATH += $$(QTDIR)/../../Tools/OpenSSL/Win_x64/include
    }

    #Assembling release version files

    CONFIG(debug, debug|release) {
        #debug
    } else {
        #release
        contains(QT_ARCH, i386) {
            #For Windows x32
            DESTDIR = $$_PRO_FILE_PWD_/../release_win32
        } else {
            #For Windows x64
            DESTDIR = $$_PRO_FILE_PWD_/../release_win64
        }

        QMAKE_POST_LINK += $$(QTDIR)/bin/windeployqt --release --qmldir $$(QTDIR)/qml $$DESTDIR $$escape_expand(\\n\\t)
    }
}
