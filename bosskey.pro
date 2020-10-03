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
    src/bosskeydialog.cpp \
    src/main.cpp \
    src/singlewindowlistviewmodel.cpp \
    src/widgets/singlekeysequenceedit.cpp \
    src/widgets/tableview.cpp \
    src/misc/globals.cpp \
    src/misc/softwareversion.cpp \
    src/model/window.cpp \
    src/windowlistviewmodel.cpp

win32 {
SOURCES += \
    src/platforms/windowsplatform.cpp
}

linux-g++* {
SOURCES += \
    src/platforms/linuxplatform.cpp
}

HEADERS += \
    src/bosskeydialog.h \
    src/model/hiddenwindow.h \
    src/platforminterface.h \
    src/singlewindowlistviewmodel.h \
    src/widgets/singlekeysequenceedit.h \
    src/widgets/tableview.h \
    src/misc/softwareversion.h \
    src/misc/globals.h \
    src/model/window.h \
    src/windowlistviewmodel.h

win32 {
HEADERS += \
    src/platforms/windowsplatform.h
}

linux-g++* {
HEADERS += \
    src/platforms/linuxplatform.h
}


FORMS += \
    src/bosskeydialog.ui \
    src/tableview.ui

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
    assets/bosskey.qrc

win32:RC_FILE += bosskey.rc
win32:CONFIG += embed_manifest_exe

TRANSLATIONS += \
    assets/translations/bosskey_en.ts \
    assets/translations/bosskey_de.ts
