#include <QFileInfo>

#include "window.h"

QString Window::fileName() const
{
    QFileInfo info(processImage);
    return info.fileName();
}
