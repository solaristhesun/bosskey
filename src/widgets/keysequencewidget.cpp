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

#include <QStyle>
#include <QLineEdit>

#include "keysequencewidget.h"
#include "ui_keysequencewidget.h"

KeySequenceWidget::KeySequenceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeySequenceWidget)
{
    ui->setupUi(this);
    ui->hintLabel->hide();

    connect(ui->keySequenceEdit, &SingleKeySequenceEdit::editingFinished, this, &KeySequenceWidget::editingFinished);

}

KeySequenceWidget::~KeySequenceWidget()
{
    delete ui;
}

void KeySequenceWidget::setStatus(const SequenceStatus status)
{
    switch (status)
    {
    case Status_Invalid:
        ui->keySequenceEdit->setProperty("invalid", true);
        ui->keySequenceEdit->setProperty("valid", false);
        ui->hintLabel->show();
        break;
    case Status_Valid:
        ui->keySequenceEdit->setProperty("invalid", false);
        ui->keySequenceEdit->setProperty("valid", true);
        ui->hintLabel->hide();
        break;
    case Status_Unset:
        ui->keySequenceEdit->setProperty("invalid", false);
        ui->keySequenceEdit->setProperty("valid", false);
        ui->hintLabel->hide();
        break;
    default:
        break;
    }

    ui->keySequenceEdit->refresh();
}

void KeySequenceWidget::setInvalid(const bool bInvalid)
{
    ui->keySequenceEdit->lineEdit()->setProperty("invalid", bInvalid);
}

QKeySequence KeySequenceWidget::keySequence() const
{
    return ui->keySequenceEdit->keySequence();
}

void KeySequenceWidget::setKeySequence(const QKeySequence &keySequence)
{
    ui->keySequenceEdit->setKeySequence(keySequence);
}

// EOF <stefan@scheler.com>
