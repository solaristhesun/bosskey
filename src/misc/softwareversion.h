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

#ifndef SOFTWAREVERSION_H
#define SOFTWAREVERSION_H

#include <QDebug>
#include <QLatin1Literal>
#include <QtGlobal>

class SoftwareVersion
{
public:
    SoftwareVersion(QString versionString);

    QString toString() const;

    quint16 versionMajor() const;
    quint16 versionMinor() const;
    char    versionPatch() const;
    bool    isValid() const;

    bool operator==(const SoftwareVersion& other);
    bool operator!=(const SoftwareVersion& other);
    bool operator>(const SoftwareVersion& other);
    bool operator<(const SoftwareVersion& other);
    bool operator<=(const SoftwareVersion& other);
    bool operator>=(const SoftwareVersion& other);

private:
    quint16 versionMajor_;
    quint8  versionMinor_;
    char    versionPatch_;
};

QDebug operator<<(QDebug dbg, const SoftwareVersion& version);

#endif // SOFTWAREVERSION_H
