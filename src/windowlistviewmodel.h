#ifndef WINDOWLISTVIEWMODEL_H
#define WINDOWLISTVIEWMODEL_H

#include <QAbstractListModel>

#include "window.h"

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
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action,
                                             int row, int column,
                                             const QModelIndex &parent) const override;

    void clear();
    void addWindow(Window w);
    void setWindowList(QList<Window> windowList);

private:
    QList<Window> windowList_;
};

#endif // WINDOWLISTVIEWMODEL_H
