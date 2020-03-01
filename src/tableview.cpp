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

#include "tableview.h"

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{
    // empty
}

void TableView::dropEvent(QDropEvent *event)
{
    QTableView::dropEvent(event);
    QTableView::resizeColumnsToContents();
}

// EOF <stefan@scheler.com>