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

#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <Windows.h>

#include <QList>

#include "model/window.h"
#include "platforms/win32/systemtrayicon.h"

class SystemTray
{
public:
    SystemTray();

    void hideIcons(QList<Window> patternList);
    void showIcons();

private:
    HWND getTrayToolbarHandle() const;

private:
    QList<SystemTrayIcon> hiddenTrayIcons_;
};

#endif // SYSTEMTRAY_H
