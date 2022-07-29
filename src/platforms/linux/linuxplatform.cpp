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

#include "platforms/linux/linuxplatform.h"

LinuxPlatform::LinuxPlatform()
{
    // empty
}

void LinuxPlatform::hideWindows(QList<WindowPattern> patternList)
{
    // empty
}

void LinuxPlatform::showWindows()
{
    // empty
}

void LinuxPlatform::showWindow(HiddenWindow window)
{
    // empty
}

QList<WindowPattern> LinuxPlatform::getWindowList()
{
    return QList<WindowPattern>();
}

bool LinuxPlatform::isHidden() const
{
    return false;
}

quint32 LinuxPlatform::getUserIdleTime() const
{
    return 1000000;
}

void LinuxPlatform::bringToFront(WindowPattern window)
{
    // empty
}

int LinuxPlatform::hiddenWindowsCount() const
{
    return 0;
}

QList<HiddenWindow> LinuxPlatform::getHiddenWindowList() const
{
    return QList<HiddenWindow>();
}

QPoint LinuxPlatform::getCursorPos() const
{
    return QPoint();
}

// EOF <stefan@scheler.com>
