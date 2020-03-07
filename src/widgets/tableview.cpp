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

#include <QPainter>
#include <QDebug>

#include "tableview.h"

TableView::TableView(QWidget *parent)
    : QTableView(parent)
{
    // empty
}

void TableView::dropEvent(QDropEvent *event)
{
    QTableView::dropEvent(event);
    QTableView::resizeColumnToContents(0);
}


void TableView::setEmptyText(QString text)
{
    emptyText_ = text;
}

QString TableView::emptyText() const
{
    return emptyText_;
}

void TableView::paintEvent(QPaintEvent *e)
{
    QTableView::paintEvent(e);

    if (model() && model()->rowCount(rootIndex()) == 0) {
        drawEmptyText();
    }
}

void TableView::drawEmptyText()
{
    QPainter p(QTableView::viewport());
    p.setPen(QColor("#666666"));
    QFont font = p.font();
    font.setPixelSize(13);
    p.setFont(font);
    p.drawText(QTableView::viewport()->rect(), Qt::AlignCenter, emptyText_);
}

// EOF <stefan@scheler.com>
