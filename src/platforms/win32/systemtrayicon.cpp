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

#include <QDebug>

#include "platforms/win32/systemtrayicon.h"

SystemTrayIcon::SystemTrayIcon(QString imageName, TRAYDATA trayData)
    : imageName_(imageName)
    , notifyIconData_ { 0 }
{
    notifyIconData_.cbSize = sizeof(notifyIconData_);
    notifyIconData_.hWnd = trayData.hwnd;
    notifyIconData_.uID = trayData.uID;
    notifyIconData_.uFlags = NIF_ICON;
    notifyIconData_.hIcon = trayData.hIcon;
    notifyIconData_.uFlags |= NIF_STATE;
    notifyIconData_.dwStateMask = NIS_HIDDEN;
}

void SystemTrayIcon::show()
{
    qDebug() << "showing trayicon for" << imageName_;

    notifyIconData_.dwState = 0;

    ::Shell_NotifyIcon(NIM_MODIFY, &notifyIconData_);
}
void SystemTrayIcon::hide()
{
    qDebug() << "hiding trayicon for" << imageName_;

    notifyIconData_.dwState = NIS_HIDDEN;

    ::Shell_NotifyIcon(NIM_MODIFY, &notifyIconData_);
}

// EOF <stefan@scheler.com>
