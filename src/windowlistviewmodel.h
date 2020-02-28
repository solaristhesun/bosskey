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

    void addWindow(Window w);
    void setWindowList(QList<Window> windowList);

private:
    QList<Window> windowList_;
};

#endif // WINDOWLISTVIEWMODEL_H
