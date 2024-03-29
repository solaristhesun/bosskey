/*
 * This file is part of bosskey.
 *
 * Copyright (c) 2020 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>

#include "singleapplication.h"
#include "uglobalhotkeys.h"
#include "ui/bosskeydialog.h"
#if defined(Q_OS_WIN)
#include "platforms/win32/windowsplatform.h"
#elif defined(Q_OS_LINUX)
#include "platforms/linux/linuxplatform.h"
#endif

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/appicon/leader.svg"));
    app.setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("SCHELER");
    QCoreApplication::setOrganizationDomain("scheler.com");
    QCoreApplication::setApplicationName("bosskey");

    QApplication::setDesktopSettingsAware(false);
    QApplication::setEffectEnabled(Qt::UI_FadeMenu, false);
    QApplication::setEffectEnabled(Qt::UI_AnimateMenu, false);

    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

    QTranslator qtTranslator;
    if (qtTranslator.load(QString(":/translations/qtbase_%1.qm").arg(defaultLocale)))
    {
        qApp->installTranslator(&qtTranslator);
    }

#if defined(Q_OS_WIN)
    WindowsPlatform platform;
#elif defined(Q_OS_LINUX)
    LinuxPlatform platform;
#endif
    UGlobalHotkeys hotkeyManager;
    BossKeyDialog dialog(platform, hotkeyManager);

    return app.exec();
}

// EOF <stefan@scheler.com>
