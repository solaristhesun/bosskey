/*
 * This file is part of bosskey.
 *
 * Copyright (c) 2022 Stefan Scheler.
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

#include <QCoreApplication>
#include <QDir>

#include "misc/globals.h"
#include "platforms/win32/autostart.h"

Autostart::Autostart()
    : registry_("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat)
{
    // empty
}

void Autostart::setEnabled(bool bEnabled)
{
    QString regKey = Globals::ApplicationName;

    if (bEnabled) {
        QString appPath = QCoreApplication::applicationFilePath();
        registry_.setValue(regKey, QDir::toNativeSeparators(appPath));
    } else {
        registry_.remove(regKey);
    }
}

bool Autostart::isEnabled() const
{
    return !registry_.value(Globals::ApplicationName).toString().isEmpty();
}

// EOF <stefan@scheler.com>
