#ifndef WINDOW_H
#define WINDOW_H

#include <QString>

class Window
{
public:
    QString title;
    QString processImage;

    QString fileName() const;

};

QDataStream &operator<<(QDataStream &ds, const Window&w);
QDataStream &operator >> (QDataStream &ds, Window &w);

#endif // WINDOW_H
