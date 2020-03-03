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

#ifndef WINDOWSENGINE_H
#define WINDOWSENGINE_H

#include <QList>

#include <Windows.h>

#include "platforminterface.h"
#include "window.h"

class WindowListViewModel;

class WindowsEngine: public PlatformInterface
{
public:
    WindowsEngine();

    void hideWindows(QList<Window> patternList);
    void showWindows();
    QList<Window> getWindowList();
    bool isHidden() const;
    quint32 getUserIdleTime() const;

private:
    QString getWindowTitle(HWND hWindow) const;
    QString getProcessImageName(HWND hWindow) const;

private:
    QList<HWND> hiddenWindows_;
    QList<Window> windowList_;
    QList<Window> patternList_;
};

#endif // WINDOWSENGINE_H
