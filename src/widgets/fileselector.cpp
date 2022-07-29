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
#include <QFileDialog>
#include <QStandardPaths>

#include "widgets/fileselector.h"
#include "ui_fileselector.h"

FileSelector::FileSelector(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::FileSelector)
{
    ui_->setupUi(this);
}

FileSelector::~FileSelector()
{
    delete ui_;
}

void FileSelector::openFileDialog()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first());
    dialog.setNameFilter(tr("Executables (*.exe *.bat *.cmd)"));
    dialog.setViewMode(QFileDialog::Detail);

    QString filename;
    if (dialog.exec() == QDialog::Accepted) {
        filename = dialog.selectedUrls().value(0).toLocalFile();
    }

    ui_->filenameEdit->setText(filename);
}

void FileSelector::setLabel(const QString& text)
{
    ui_->label->setText(text);
}

QString FileSelector::label() const
{
    return ui_->label->text();
}

void FileSelector::setFilename(const QString& filename)
{
    ui_->filenameEdit->setText(filename);
}

QString FileSelector::filename() const
{
    return ui_->filenameEdit->text();
}

// EOF <stefan@scheler.com>
