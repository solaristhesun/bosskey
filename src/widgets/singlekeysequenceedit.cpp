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

#include <QLineEdit>

#include "singlekeysequenceedit.h"

SingleKeySequenceEdit::SingleKeySequenceEdit(QWidget *parent)
    : QKeySequenceEdit(parent)
{
    QLineEdit *widget = this->findChild<QLineEdit*>("qt_keysequenceedit_lineedit");
    widget->setClearButtonEnabled(true);
}

SingleKeySequenceEdit::~SingleKeySequenceEdit()
{
    // empty
}


void SingleKeySequenceEdit::keyPressEvent(QKeyEvent *pEvent)
{
    QKeySequenceEdit::keyPressEvent(pEvent);

    setKeySequence(keySequence()[0]);
}

// EOF <stefan@scheler.com>
