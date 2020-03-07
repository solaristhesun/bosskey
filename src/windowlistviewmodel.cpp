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

#include <QList>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QMimeData>
#include <QSettings>
#include <QDebug>

#include "windowlistviewmodel.h"

WindowListViewModel::WindowListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // empty
}

void WindowListViewModel::addWindow(Window w)
{
    if (!windowList_.contains(w)) {
        beginInsertRows(QModelIndex(), windowList_.length(), windowList_.length()+1);
        windowList_.push_back(w);
        endInsertRows();
    }
}

void WindowListViewModel::setWindowList(QList<Window> windowList)
{
    beginResetModel();
    windowList_ = windowList;
    std::sort(windowList_.begin(), windowList_.end(), [](const Window&a, const Window&b) -> bool {
        return a.fileName() < b.fileName();
    });
    endResetModel();
}

QList<Window> WindowListViewModel::getWindowList() const
{
    return windowList_;
}

void WindowListViewModel::clear()
{
    beginResetModel();
    windowList_.clear();
    endResetModel();
}

int WindowListViewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return windowList_.length();
}

int WindowListViewModel::columnCount(const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant WindowListViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Process Name");

            case 1:
                return tr("Window Title");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant WindowListViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= windowList_.size())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if (index.column() == 1) {
            return windowList_.at(index.row()).getText();
        }
        else  {
            QFileInfo info(windowList_.at(index.row()).processImage);
            return info.fileName();
        }
    }

    if (role == Qt::DecorationRole) {
        if (index.column() == 0) {
            QFileInfo info(windowList_.at(index.row()).processImage);
            QFileIconProvider iconProvider;
            return iconProvider.icon(info);
        }
    }

    return QVariant();
}

Qt::ItemFlags WindowListViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractListModel::flags(index);
}

Qt::DropActions WindowListViewModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction ;
}

QMimeData *WindowListViewModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            Window w = windowList_.at(index.row());
            w.ignoreTitle = false;
            stream << w;
        }
    }

    mimeData->setData("application/vnd.text.list", encodedData);
    return mimeData;
}

QStringList WindowListViewModel::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

bool WindowListViewModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    qDebug() << "drop";

    if(!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    Window w;

    QByteArray encodedData = data->data("application/vnd.text.list");
    QDataStream stream(&encodedData, QIODevice::ReadOnly);

    stream >> w;

    addWindow(w);
    return true;
}

void WindowListViewModel::removeItem(const QModelIndex& index)
{
    beginRemoveRows(index.parent(), index.row(), index.row());
    windowList_.removeAt(index.row());
    endRemoveRows();
}

void WindowListViewModel::toggleIgnoreTitle(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    Window& w = windowList_[index.row()];
    w.ignoreTitle = !w.ignoreTitle;

    emit dataChanged(index, index);
}

void WindowListViewModel::saveToSettings(QString key)
{
    QSettings settings;

    settings.beginWriteArray(key);
    settings.remove("");

    for (int i = 0; i < windowList_.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("ProcessImage",  windowList_.at(i).processImage);
        settings.setValue("WindowTitle", windowList_.at(i).title);
        settings.setValue("IgnoreTitle", windowList_.at(i).ignoreTitle);
    }
    settings.endArray();
}

void WindowListViewModel::loadFromSettings(QString key)
{
    QSettings settings;
    int size = settings.beginReadArray(key);

    beginResetModel();
    windowList_.clear();
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        Window w;
        w.processImage = settings.value("ProcessImage").toString();
        w.title = settings.value("WindowTitle").toString();
        w.ignoreTitle = settings.value("IgnoreTitle").toBool();
        windowList_.push_back(w);
    }
    endResetModel();

    settings.endArray();
}

Window WindowListViewModel::getWindow(const QModelIndex& index)
{
    return windowList_.at(index.row());
}

// EOF <stefan@scheler.com>
