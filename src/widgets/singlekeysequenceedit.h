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

#ifndef SINGLEKEYSEQUENCEEDIT_H
#define SINGLEKEYSEQUENCEEDIT_H

#include <QKeySequenceEdit>

class QLineEdit;

class SingleKeySequenceEdit : public QKeySequenceEdit
{
    Q_OBJECT
public:
    explicit SingleKeySequenceEdit(QWidget *parent = nullptr);
    ~SingleKeySequenceEdit();

    void setInvalidSequence(const bool bInvalid);

    bool setProperty(const char *name, const QVariant &value);

    void refresh();

    QLineEdit* lineEdit() const;

    void lineEditEdited(const QString& text);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLineEdit* lineEdit_;
};

#endif // SINGLEKEYSEQUENCEEDIT_H
