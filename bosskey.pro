QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/bosskeydialog.cpp \
    src/globals.cpp \
    src/main.cpp \
    src/softwareversion.cpp \
    src/windowsengine.cpp

HEADERS += \
    src/bosskeydialog.h \
    src/engineinterface.h \
    src/globals.h \
    src/softwareversion.h \
    src/windowsengine.h

FORMS += \
    src/bosskeydialog.ui

INCLUDEPATH += src/ libs/UGlobalHotkey/

include(libs/UGlobalHotkey/uglobalhotkey.pri)
include(libs/SingleApplication/singleapplication.pri)

DEFINES += QAPPLICATION_CLASS=QApplication

REVISION = $$system(git rev-list --count HEAD)

isEmpty(REVISION){
    REVISION = 0
}

DEFINES += REVISION=$${REVISION}

QTPLUGIN += qsvg

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bosskey.qrc

win32:RC_FILE += bosskey.rc
win32:CONFIG += embed_manifest_exe
