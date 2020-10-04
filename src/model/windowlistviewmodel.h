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

#ifndef WINDOWLISTVIEWMODEL_H
#define WINDOWLISTVIEWMODEL_H

#include <QAbstractListModel>

#include "model/window.h"

class WindowListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    WindowListViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    QStringList mimeTypes() const override;

    void clear();
    virtual void addWindow(Window w);
    void removeItem(const QModelIndex& index);
    void toggleIgnoreTitle(const QModelIndex& index);
    void setWindowList(QList<Window> windowList);
    QList<Window> getWindowList() const;
    void saveToSettings(QString key);
    void loadFromSettings(QString key);
    Window getWindow(const QModelIndex& index);

protected:
    QList<Window> windowList_;
};

#endif // WINDOWLISTVIEWMODEL_H
