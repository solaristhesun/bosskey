#include <QList>
#include <QFileInfo>
#include <QFileIconProvider>
#include <QIcon>
#include <QMimeData>
#include <QDebug>

#include "windowlistviewmodel.h"

WindowListViewModel::WindowListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // empty
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
    qDebug() << "mimeData";
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            Window w = windowList_.at(index.row());
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
    qDebug() << "drop" << parent << data->data("application/vnd.text.list");

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

bool WindowListViewModel::canDropMimeData(const QMimeData *data, Qt::DropAction action,
                     int row, int column,
                                          const QModelIndex &parent) const {
    bool bVal = QAbstractItemModel::canDropMimeData(data,action,row,column,parent);
    qDebug() << "candrop" << bVal;
    return bVal;
}

// EOF <stefan@scheler.com>
