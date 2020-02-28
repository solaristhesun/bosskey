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

#endif // WINDOW_H
