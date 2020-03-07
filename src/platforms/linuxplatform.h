#ifndef LINUXPLATFORM_H
#define LINUXPLATFORM_H

#include "platforminterface.h"

class LinuxPlatform : public PlatformInterface
{
public:
    LinuxPlatform();

    void hideWindows(QList<Window> patternList);
    void showWindows();
    QList<Window> getWindowList();
    bool isHidden() const;
    quint32 getUserIdleTime() const;
    void bringToFront(Window window);
};

#endif // LINUXPLATFORM_H
