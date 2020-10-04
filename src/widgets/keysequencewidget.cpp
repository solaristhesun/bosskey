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
    ui_(new Ui::KeySequenceWidget)
{
    ui_->setupUi(this);
    ui_->hintLabel->hide();

    connect(ui_->keySequenceEdit, &SingleKeySequenceEdit::editingFinished, this, &KeySequenceWidget::editingFinished);
}

KeySequenceWidget::~KeySequenceWidget()
{
    delete ui_;
}

void KeySequenceWidget::setStatus(const SequenceStatus status)
{
    switch (status)
    {
    case Status_Invalid:
        ui_->keySequenceEdit->setProperty("invalid", true);
        ui_->keySequenceEdit->setProperty("valid", false);
        ui_->hintLabel->show();
        break;
    case Status_Valid:
        ui_->keySequenceEdit->setProperty("invalid", false);
        ui_->keySequenceEdit->setProperty("valid", true);
        ui_->hintLabel->hide();
        break;
    case Status_Unset:
        ui_->keySequenceEdit->setProperty("invalid", false);
        ui_->keySequenceEdit->setProperty("valid", false);
        ui_->hintLabel->hide();
        break;
    default:
        break;
    }

    ui_->keySequenceEdit->refresh();
}

void KeySequenceWidget::setInvalid(const bool bInvalid)
{
    ui_->keySequenceEdit->lineEdit()->setProperty("invalid", bInvalid);
}

QKeySequence KeySequenceWidget::keySequence() const
{
    return ui_->keySequenceEdit->keySequence();
}

void KeySequenceWidget::setKeySequence(const QKeySequence &keySequence)
{
    ui_->keySequenceEdit->setKeySequence(keySequence);
}

bool KeySequenceWidget::event(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        ui_->retranslateUi(this);
        break;
    default:
        break;
    }

    return QObject::event(event);
}

// EOF <stefan@scheler.com>
