QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 lrelease embed_translations

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
    src/model/windowpattern.cpp \
    src/ui/bosskeydialog.cpp \
    src/widgets/fileselector.cpp \
    src/widgets/keysequencewidget.cpp \
    src/main.cpp \
    src/widgets/singlekeysequenceedit.cpp \
    src/widgets/tableview.cpp \
    src/misc/globals.cpp \
    src/misc/softwareversion.cpp \
    src/model/singlewindowlistviewmodel.cpp \
    src/model/windowlistviewmodel.cpp

win32 {
SOURCES += \
    src/platforms/win32/windowsplatform.cpp \
    src/platforms/win32/systemtray.cpp \
    src/platforms/win32/systemtrayicon.cpp \
    src/platforms/win32/windowshelper.cpp \
}

linux-g++* {
SOURCES += \
    src/platforms/linux/linuxplatform.cpp
}

HEADERS += \
    src/model/windowpattern.h \
    src/ui/bosskeydialog.h \
    src/platforms/platforminterface.h \
    src/widgets/fileselector.h \
    src/widgets/singlekeysequenceedit.h \
    src/widgets/tableview.h \
    src/widgets/keysequencewidget.h \
    src/misc/softwareversion.h \
    src/misc/globals.h \
    src/model/hiddenwindow.h \
    src/model/windowlistviewmodel.h \
    src/model/singlewindowlistviewmodel.h

win32 {
HEADERS += \
    src/platforms/win32/windowsplatform.h \
    src/platforms/win32/process.h \
    src/platforms/win32/systemtray.h \
    src/platforms/win32/systemtrayicon.h \
    src/platforms/win32/windowshelper.h \
}

linux-g++* {
HEADERS += \
    src/platforms/linux/linuxplatform.h
}

FORMS += \
    src/ui/bosskeydialog.ui \
    src/widgets/fileselector.ui \
    src/widgets/keysequencewidget.ui \
    src/widgets/tableview.ui

INCLUDEPATH += src/ libs/UGlobalHotkey/

include(libs/UGlobalHotkey/uglobalhotkey.pri)
include(libs/SingleApplication/singleapplication.pri)

DEFINES += QAPPLICATION_CLASS=QApplication

REVISION = $$system(git rev-list --count HEAD)

isEmpty(REVISION){
    REVISION = 0
}

BOSSKEY_VERSION=2022.08a

DEFINES += REVISION=$${REVISION}
DEFINES += BOSSKEY_VERSION=$${BOSSKEY_VERSION}

QTPLUGIN += qsvg

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets/bosskey.qrc

win32:RC_FILE += bosskey.rc
win32:CONFIG += embed_manifest_exe

win32 {
QMAKE_CXXFLAGS += /FI"qdebug.h"
}

TRANSLATIONS += \
    assets/translations/bosskey_en.ts \
    assets/translations/bosskey_de.ts
