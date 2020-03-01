#include <QList>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QDebug>

#include "windowlistviewmodel.h"

WindowListViewModel::WindowListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void WindowListViewModel::addWindow(Window w)
{
    beginInsertRows(QModelIndex(), windowList_.length(), windowList_.length()+1);
    windowList_.push_back(w);
    endInsertRows();
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
            return windowList_.at(index.row()).title;
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
