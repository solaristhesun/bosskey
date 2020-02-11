#ifndef WINDOWSENGINE_H
#define WINDOWSENGINE_H

#include <QList>

#include <Windows.h>

class WindowsEngine
{
public:
    WindowsEngine();

    void hideWindows(QStringList patterns);
    void showWindows();
    QStringList getWindowList();

private:
    QList<HWND> hiddenWindows_;
    QStringList patterns_;
    QStringList windowList_;
};

#endif // WINDOWSENGINE_H
