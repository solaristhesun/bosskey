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

#include <QCoreApplication>
#include <QDataStream>
#include <QFileInfo>

#include "windowpattern.h"

WindowPattern::WindowPattern()
    : ignoreTitle(false)
{
    // empty
}

QString WindowPattern::fileName() const
{
    QFileInfo info(processImage);
    return info.fileName();
}

QString WindowPattern::getText() const
{
    if (ignoreTitle)
        return QCoreApplication::translate("Window", "(any title)");
    else
        return title;
}

bool WindowPattern::operator==(WindowPattern rhs)
{
    return rhs.processImage == this->processImage && (rhs.title == this->title || this->ignoreTitle);
}

QDataStream& operator<<(QDataStream& ds, const WindowPattern& w)
{
    ds << w.title << w.processImage << w.ignoreTitle;
    return ds;
}

QDataStream& operator>>(QDataStream& ds, WindowPattern& w)
{
    ds >> w.title;
    ds >> w.processImage;
    ds >> w.ignoreTitle;
    return ds;
}

// EOF <stefan@scheler.com>
