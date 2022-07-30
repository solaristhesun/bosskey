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

#include <QMenu>
#include <QPainter>

#include "model/windowlistviewmodel.h"
#include "ui_tableview.h"
#include "widgets/tableview.h"

TableView::TableView(QWidget* parent)
    : QTableView(parent)
    , ui_(new Ui::TableView)
{
    ui_->setupUi(this);
}

TableView::~TableView()
{
    delete ui_;
}

void TableView::dropEvent(QDropEvent* event)
{
    QTableView::dropEvent(event);
    QTableView::resizeColumnToContents(0);
}

void TableView::setEmptyText(QString text)
{
    emptyText_ = text;
    emit emptyTextChanged();
}

QString TableView::emptyText() const
{
    return emptyText_;
}

void TableView::paintEvent(QPaintEvent* e)
{
    QTableView::paintEvent(e);

    if (model() && model()->rowCount(rootIndex()) == 0) {
        drawEmptyText();
    }
}

void TableView::drawEmptyText()
{
    QPainter p(QTableView::viewport());
    p.setPen(QColor(0x666666));
    QFont font = p.font();
    font.setPixelSize(13);
    p.setFont(font);
    p.drawText(QTableView::viewport()->rect(), Qt::AlignCenter, emptyText_);
}

void TableView::showContextMenu(const QPoint& pos)
{
    QMenu contextMenu;

    auto index = QTableView::indexAt(pos);

    if (index.isValid()) {
        WindowPattern pattern = model()->getWindow(index);
        ui_->actionToggleIgnoreTitle->setChecked(pattern.ignoreTitle);
        contextMenu.addAction(ui_->actionToggleIgnoreTitle);
        contextMenu.addAction(ui_->actionRemoveItem);
        contextMenu.addSeparator();
    }

    contextMenu.addAction(ui_->actionClearAll);

    contextMenu.exec(QTableView::viewport()->mapToGlobal(pos));
}

WindowListViewModel* TableView::model() const
{
    return static_cast<WindowListViewModel*>(QTableView::model());
}

void TableView::clear()
{
    QTableView::clearSelection();
    model()->clear();
}

void TableView::removeCurrentItem()
{
    auto index = QTableView::currentIndex();
    model()->removeItem(index);
}

void TableView::toggleIgnoreTitle()
{
    auto index = QTableView::currentIndex();
    model()->toggleIgnoreTitle(index);
}

void TableView::changeEvent(QEvent* event)
{
    QTableView::changeEvent(event);

    if (event != nullptr) {
        switch (event->type()) {
        case QEvent::LanguageChange:
            ui_->retranslateUi(this);
            break;
        default:
            break;
        }
    }
}

// EOF <stefan@scheler.com>
