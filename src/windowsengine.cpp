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

#include "windowsengine.h"

WindowsEngine::WindowsEngine()
{
    // empty
}

void WindowsEngine::showWindows()
{
    qDebug() << "show" << hiddenWindows_.length();
    for (auto window : hiddenWindows_) {
        qDebug() << window;
        ShowWindow(window, SW_SHOW);
    }
    hiddenWindows_.clear();
}

void WindowsEngine::hideWindows(const QStringList patterns)
{
    patterns_ = patterns;

    ::EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        if (!IsWindowVisible(hWnd)) {
            return TRUE;
        }

        WindowsEngine* engine = reinterpret_cast<WindowsEngine*>(lParam);
        const DWORD TITLE_SIZE = 1024;
        TCHAR windowTitle[TITLE_SIZE];

        GetWindowText(hWnd, windowTitle, TITLE_SIZE);
        std::wstring temp(&windowTitle[0]);
        QString title = QString::fromStdWString(temp);

        for (auto pattern: engine->patterns_) {
            if (title.contains(pattern)) {
                qDebug() << "hiding" << title << hWnd;
                ShowWindow(hWnd, SW_HIDE);

                engine->hiddenWindows_.append(hWnd);
                qDebug() << engine->hiddenWindows_.length();
            }
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(this));
}

QStringList WindowsEngine::getWindowList()
{
    windowList_.clear();

    ::EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        if (!IsWindowVisible(hWnd)) {
            return TRUE;
        }

        WindowsEngine* engine = reinterpret_cast<WindowsEngine*>(lParam);
        const DWORD TITLE_SIZE = 1024;
        TCHAR windowTitle[TITLE_SIZE];

        GetWindowText(hWnd, windowTitle, TITLE_SIZE);

        TCHAR buffer[MAX_PATH] = {0};
        DWORD dwProcessId = 0;
        GetWindowThreadProcessId(hWnd, &dwProcessId);

        HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, dwProcessId);
        DWORD ret = GetModuleFileName((HMODULE)hProc, buffer, MAX_PATH);
        DWORD error = GetLastError();

        DWORD value = MAX_PATH;
        wchar_t buffer2[MAX_PATH];
        QueryFullProcessImageName(hProc, 0, buffer2, &value);
        CloseHandle(hProc);



        std::wstring temp(&windowTitle[0]);
        QString title = QString::fromStdWString(temp);
        qDebug() << title << hProc << dwProcessId << QString::fromStdWString(std::wstring(&buffer2[0])) << ret << error;

        if (!title.isEmpty()) {
            engine->windowList_.append(title);
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(this));

    return windowList_;
}

bool WindowsEngine::isHidden() const
{
    return !hiddenWindows_.isEmpty();
}

// EOF <stefan@scheler.com>
