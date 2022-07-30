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

#ifndef AUTOSTART_H
#define AUTOSTART_H

#include <QSettings>

class Autostart
{
public:
    Autostart();

    void setEnabled(bool bEnabled);
    bool isEnabled() const;

private:
    QSettings registry_;
};

#endif // AUTOSTART_H

// EOF <stefan@scheler.com>
