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

#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>

class Process {
    HANDLE handle_ = NULL;

public:
    Process(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
        handle_ = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    }

    ~Process() {
        if (handle_ != NULL) {
            ::CloseHandle(handle_);
        }
    }

    inline bool isValid() const {
        return handle_ != NULL;
    }

    inline HANDLE handle() const {
        return handle_;
    }
};

#endif // PROCESS_H
