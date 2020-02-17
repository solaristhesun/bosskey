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

#include <QSetIterator>

#include "singleapplication.h"
#include "bosskeydialog.h"
#include "uglobalhotkeys.h"
#include "windowsengine.h"

int main(int argc, char *argv[])
{
    QSet<unsigned long long> set;
    set.insert(0);
    set.insert(1);
    QSetIterator<unsigned long long> it(set);
    while (it.hasNext()) {
            set.remove(it.next());
        }

    SingleApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/assets/appicon/leader.svg"));
    app.setQuitOnLastWindowClosed(false);

    QCoreApplication::setOrganizationName("SCHELER");
    QCoreApplication::setOrganizationDomain("scheler.com");
    QCoreApplication::setApplicationName("bosskey");

    WindowsEngine engine;
    UGlobalHotkeys hotkeyManager;
    BossKeyDialog dialog(engine, hotkeyManager);

    return app.exec();
}

// EOF <stefan@scheler.com>
