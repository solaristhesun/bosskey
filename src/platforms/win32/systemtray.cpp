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

#include <windows.h>
#include <psapi.h>
#include <CommCtrl.h>

#include "platforms/win32/systemtray.h"
#include "platforms/win32/systemtrayicon.h"
#include "platforms/win32/process.h"
#include "platforms/win32/windowshelper.h"

SystemTray::SystemTray()
{
    // empty
}

void SystemTray::hideIcons(QList<WindowPattern> patternList)
{
    HWND hWnd = getTrayToolbarHandle();

    DWORD dwTrayPid;
    GetWindowThreadProcessId(hWnd, &dwTrayPid);

    int buttonCount = (int)::SendMessage(hWnd, TB_BUTTONCOUNT, 0, 0);

    Process process(PROCESS_ALL_ACCESS, FALSE, dwTrayPid);

    for(int i=0; i<buttonCount; i++)
    {
        LPVOID lpData = VirtualAllocEx(process.handle(), NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
        ::SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)lpData);

        TBBUTTON buttonData;
        ::ReadProcessMemory(process.handle(), lpData, (LPVOID)&buttonData, sizeof(TBBUTTON), NULL);

        TRAYDATA trayData;
        ::ReadProcessMemory(process.handle(), (LPCVOID)buttonData.dwData, (LPVOID)&trayData, sizeof(TRAYDATA), NULL);

        DWORD dwProcessId = 0;
        ::GetWindowThreadProcessId(trayData.hwnd, &dwProcessId);

        QString imageName = WindowsHelper::getImageNameFromPid(dwProcessId);

        for (auto pattern: qAsConst(patternList)) {
            if (pattern.processImage == imageName) {
                SystemTrayIcon trayIcon(imageName, trayData);
                trayIcon.hide();
                hiddenTrayIcons_.append(trayIcon);

                break;
            }
        }

        VirtualFreeEx(process.handle(),lpData,NULL,MEM_RELEASE);
    }
}

void SystemTray::showIcons()
{
    for (auto trayIcon: qAsConst(hiddenTrayIcons_)) {
        trayIcon.show();
    }
    hiddenTrayIcons_.clear();
}

HWND SystemTray::getTrayToolbarHandle() const
{
    HWND hWindow = ::FindWindow(L"Shell_TrayWnd", NULL);

    if (hWindow)
    {
        hWindow = ::FindWindowEx(hWindow, NULL, L"TrayNotifyWnd", NULL);

        if (hWindow)
        {
            hWindow = ::FindWindowEx(hWindow, NULL, L"SysPager", NULL);

            if (hWindow)
            {
                hWindow = ::FindWindowEx(hWindow, NULL, L"ToolbarWindow32", NULL);
            }
        }
    }

    return hWindow;
}

// EOF <stefan@scheler.com>
