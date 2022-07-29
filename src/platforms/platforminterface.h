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

#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include <QList>
#include <QPoint>

#include "model/windowpattern.h"
#include "model/hiddenwindow.h"

class PlatformInterface
{
public:
    virtual ~PlatformInterface() {}

    virtual void hideWindows(QList<WindowPattern> patternList) =0;
    virtual void showWindows() =0;
    virtual void showWindow(HiddenWindow window) =0;
    virtual QList<WindowPattern> getWindowList() =0;
    virtual bool isHidden() const =0;
    virtual quint32 getUserIdleTime() const =0;
    virtual void bringToFront(WindowPattern window) =0;
    virtual int hiddenWindowsCount() const =0;
    virtual QList<HiddenWindow> getHiddenWindowList() const =0;
    virtual QPoint getCursorPos() const =0;
};

#endif // ENGINEINTERFACE_H
