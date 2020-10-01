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

#include "globals.h"

const QString Globals::ApplicationName = "BossKey";
const SoftwareVersion Globals::ApplicationVersion = SoftwareVersion("2020.10b");
const QString Globals::ApplicationFullName = Globals::ApplicationName + " " + Globals::ApplicationVersion.toString();
const QString Globals::ApplicationWebsite = "https://scheler.com/bosskey/";
const quint32 Globals::ApplicationRevision = REVISION;
bool Globals::bIsPremium = false;

// EOF <stefan@scheler.com>
