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

#include <QDebug>

#include "platforms/win32/windowsplatform.h"
#include "platforms/win32/windowshelper.h"

WindowsPlatform::WindowsPlatform()
{
    // empty
}

void WindowsPlatform::showWindows()
{
    for (auto window : hiddenWindows_) {
        showWindow(window);
    }
    trayIcons_.showIcons();
}

void WindowsPlatform::hideWindows(QList<WindowPattern> patternList)
{
    patternList_ = patternList;
    hForegroundWindow_ = ::GetForegroundWindow();

    ::EnumWindows([](HWND hWindow, LPARAM lParam) -> BOOL {
        if (::IsWindowVisible(hWindow)) {
            WindowsPlatform* engine = reinterpret_cast<WindowsPlatform*>(lParam);

            QString title = engine->getWindowTitle(hWindow);
            QString imageName = engine->getProcessImageName(hWindow);

            for (auto window: engine->patternList_) {
                if (window.processImage == imageName) {
                    if (window.ignoreTitle || window.title == title) {
                        qDebug() << "hiding" << title << hWindow << imageName;
                        //::ShowWindow(hWindow, SW_HIDE);
                        ::SetWindowPos(hWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);

                        HiddenWindow hiddenWindow;
                        hiddenWindow.hWindow = hWindow,
                        hiddenWindow.title = title,
                        hiddenWindow.bForeground = (engine->hForegroundWindow_ == hWindow);


                        engine->hiddenWindows_.append(hiddenWindow);
                    }
                }
            }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(this));

    trayIcons_.hideIcons(patternList_);
}

QList<WindowPattern> WindowsPlatform::getWindowList()
{
    windowList_.clear();

    ::EnumWindows([](HWND hWindow, LPARAM lParam) -> BOOL {
        if (IsWindowVisible(hWindow)) {
            WindowsPlatform* engine = reinterpret_cast<WindowsPlatform*>(lParam);

            WindowPattern pattern;
            pattern.processImage = engine->getProcessImageName(hWindow);
            pattern.title = engine->getWindowTitle(hWindow);

            if (pattern.processImage.contains("WindowsInternal.ComposableShell.Experiences.TextInput.InputApp.exe"))
                return TRUE;

            if (pattern.processImage.contains("bosskey.exe"))
                return TRUE;

            if (!pattern.title.isEmpty()) {
                engine->windowList_.append(pattern);
            }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(this));

    return windowList_;
}

bool WindowsPlatform::isHidden() const
{
    return !hiddenWindows_.isEmpty();
}

quint32 WindowsPlatform::getUserIdleTime() const
{
    LASTINPUTINFO lastInputInfo = { sizeof(lastInputInfo), 0 };
    ::GetLastInputInfo(&lastInputInfo);

    DWORD dwTickCount = ::GetTickCount();

    return (dwTickCount - lastInputInfo.dwTime) / 1000;
}

QString WindowsPlatform::getWindowTitle(HWND hWindow) const
{
    int textLength = ::GetWindowTextLength(hWindow);

    if (textLength > 0) {
        std::vector<wchar_t> windowTitle(textLength + 1);
        int bytesCopied = ::GetWindowText(hWindow, &windowTitle[0], textLength + 1);

        if (bytesCopied == textLength) {
            return QString::fromStdWString(std::wstring(&windowTitle[0]));
        }
    }

    return QString();
}

QString WindowsPlatform::getProcessImageName(HWND hWindow) const
{
    DWORD dwProcessId = 0;
    ::GetWindowThreadProcessId(hWindow, &dwProcessId);

    return WindowsHelper::getImageNameFromPid(dwProcessId);
}

void WindowsPlatform::bringToFront(WindowPattern window)
{
    window_ = window;

    ::EnumWindows([](HWND hWindow, LPARAM lParam) -> BOOL {
        if (::IsWindowVisible(hWindow)) {
            WindowsPlatform* engine = reinterpret_cast<WindowsPlatform*>(lParam);

            QString title = engine->getWindowTitle(hWindow);
            QString imageName = engine->getProcessImageName(hWindow);

            if (engine->window_.processImage == imageName) {
                if (engine->window_.ignoreTitle || engine->window_.title == title) {
                    qDebug() << "bringing to front" << title << hWindow;
                    ::ShowWindow(hWindow, SW_MAXIMIZE);
                    ::SetForegroundWindow(hWindow);
                }
            }
        }
        return TRUE;
    }, reinterpret_cast<LPARAM>(this));
}

int WindowsPlatform::hiddenWindowsCount() const
{
    return hiddenWindows_.count();
}

QList<HiddenWindow> WindowsPlatform::getHiddenWindowList() const
{
    return hiddenWindows_;
}

void WindowsPlatform::showWindow(HiddenWindow window)
{
    qDebug() << "showing" << getWindowTitle(window.hWindow);

    //::ShowWindow(window.hWindow, SW_SHOW);
    ::SetWindowPos(window.hWindow, NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);

    if (window.bForeground) {
        ::SetForegroundWindow(window.hWindow);
    }

    hiddenWindows_.removeOne(window);
}

QPoint WindowsPlatform::getCursorPos() const
{
    POINT point;

    if (::GetCursorPos(&point) != 0) {
        return QPoint(point.x, point.y);
    }

    return QPoint();
}

// EOF <stefan@scheler.com>
