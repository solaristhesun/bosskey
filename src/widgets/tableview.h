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

namespace Ui {
class TableView;
}

class WindowListViewModel;

class TableView : public QTableView
{
    Q_OBJECT
    Q_PROPERTY(QString emptyText READ emptyText WRITE setEmptyText NOTIFY emptyTextChanged)

public:
    TableView(QWidget *parent = nullptr);
    ~TableView() override;

    void dropEvent(QDropEvent *event) override;

    void paintEvent(QPaintEvent *e) override;

    void changeEvent(QEvent *event) override;

    void setEmptyText(QString text);

    QString emptyText() const;

    void retranslateUserInterface();

signals:
    void emptyTextChanged();

public slots:
    void clear();
    void removeCurrentItem();
    void toggleIgnoreTitle();
    void showContextMenu(const QPoint & point);

private:
    void drawEmptyText();
    WindowListViewModel* model() const;

private:
    Ui::TableView *ui_;
    QString emptyText_;
};

#endif // TABLEVIEW_H
