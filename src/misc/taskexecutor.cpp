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

#include <QDebug>
#include <QFileInfo>
#include <QProcess>

#include "misc/taskexecutor.h"

TaskExecutor::TaskExecutor()
{
    // empty
}

void TaskExecutor::execute(const QString& filename)
{
    QFileInfo fileInfo(filename);

    if (fileInfo.exists()) {
        QProcess* process = new QProcess();

        if (fileInfo.suffix() == "bat" || fileInfo.suffix() == "cmd") {
            process->setProgram("cmd.exe");
            process->setArguments({ "/C", filename });
        } else {
            process->setProgram(filename);
        }

        QObject::connect(process, SIGNAL(finished(int)), this, SLOT(onProcessFinished(int)));
        QObject::connect(process, SIGNAL(finished(int)), process, SLOT(deleteLater()));

        qDebug() << "starting" << filename;

        process->start();
    }
}

void TaskExecutor::onProcessFinished(int exitCode)
{
    const auto process = qobject_cast<QProcess*>(QObject::sender());

    qDebug() << "process" << process->program() << process->arguments() << "exited with error code" << exitCode;
}

// EOF <stefan@scheler.com>
