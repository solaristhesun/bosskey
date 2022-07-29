#ifndef LINUXPLATFORM_H
#define LINUXPLATFORM_H

#include "platforms/platforminterface.h"

class LinuxPlatform : public PlatformInterface
{
public:
    LinuxPlatform();

    void hideWindows(QList<WindowPattern> patternList);
    void showWindows();
    QList<WindowPattern> getWindowList();
    bool isHidden() const;
    quint32 getUserIdleTime() const;
    void bringToFront(WindowPattern window);
};

#endif // LINUXPLATFORM_H
