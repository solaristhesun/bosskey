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

#ifndef WINDOWPATTERN_H
#define WINDOWPATTERN_H

#include <QString>

class WindowPattern
{
public:
    WindowPattern();

    QString title;
    QString processImage;
    bool ignoreTitle;

    QString getText() const;

    QString fileName() const;

    bool operator== (WindowPattern rhs);
};

QDataStream &operator<<(QDataStream &ds, const WindowPattern&p);
QDataStream &operator>>(QDataStream &ds, WindowPattern &p);

#endif // WINDOWPATTERN_H
