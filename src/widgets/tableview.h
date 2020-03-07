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

#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class TableView : public QTableView
{
    Q_OBJECT
    Q_PROPERTY(QString emptyText READ emptyText WRITE setEmptyText)

public:
    TableView(QWidget *parent = nullptr);

    void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *e) override;

    void setEmptyText(QString text);

    QString emptyText() const;

private:
    void drawEmptyText();

private:
    QString emptyText_;
};

#endif // TABLEVIEW_H
