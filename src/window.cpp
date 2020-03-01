#include <QFileInfo>

#include "window.h"

QString Window::fileName() const
{
    QFileInfo info(processImage);
    return info.fileName();
}

QDataStream &operator<<(QDataStream &ds, const Window&w)
{
    ds << w.title << w.processImage;
    return ds;
}

QDataStream &operator >> (QDataStream &ds, Window &w)
{
    ds >> w.title;
    ds >> w.processImage;
    return ds;
}
