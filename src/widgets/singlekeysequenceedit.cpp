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
#include <QStyle>

#include "singlekeysequenceedit.h"

SingleKeySequenceEdit::SingleKeySequenceEdit(QWidget* parent)
    : QKeySequenceEdit(parent)
{
    lineEdit_ = this->findChild<QLineEdit*>("qt_keysequenceedit_lineedit");
    lineEdit_->setClearButtonEnabled(true);

    QObject::connect(lineEdit_, &QLineEdit::textEdited, this, &SingleKeySequenceEdit::lineEditEdited);
}

SingleKeySequenceEdit::~SingleKeySequenceEdit()
{
    // empty
}

void SingleKeySequenceEdit::keyPressEvent(QKeyEvent* pEvent)
{
    QKeySequenceEdit::keyPressEvent(pEvent);

    setKeySequence(keySequence()[0]);
    emit editingFinished();
}

void SingleKeySequenceEdit::setInvalidSequence(const bool bInvalid)
{
    if (bInvalid) {
        lineEdit_->setStyleSheet("background-color: #ff8080");
    } else {
        lineEdit_->setStyleSheet("");
    }
}

QLineEdit* SingleKeySequenceEdit::lineEdit() const
{
    return lineEdit_;
}

bool SingleKeySequenceEdit::setProperty(const char* name, const QVariant& value)
{
    lineEdit_->setProperty(name, value);

    return QKeySequenceEdit::setProperty(name, value);
}

void SingleKeySequenceEdit::refresh()
{
    lineEdit_->style()->unpolish(lineEdit_);
    lineEdit_->style()->polish(lineEdit_);
}

void SingleKeySequenceEdit::lineEditEdited(const QString& text)
{
    if (text.isEmpty()) {
        setKeySequence(QKeySequence());
        emit editingFinished();
    }
}

// EOF <stefan@scheler.com>
