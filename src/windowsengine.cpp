#include <QDebug>

#include "windowsengine.h"

WindowsEngine::WindowsEngine()
{

}

void WindowsEngine::showWindows()
{
    qDebug() << "show" << hiddenWindows_.length();
    for (auto window : hiddenWindows_) {
        qDebug() << window;
        ShowWindow(window, SW_SHOW);
    }
    hiddenWindows_.clear();
}

void WindowsEngine::hideWindows(const QStringList patterns)
{
    patterns_ = patterns;

    ::EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        if (!IsWindowVisible(hWnd)) {
            return TRUE;
        }

        WindowsEngine* engine = reinterpret_cast<WindowsEngine*>(lParam);
        const DWORD TITLE_SIZE = 1024;
        TCHAR windowTitle[TITLE_SIZE];

        GetWindowText(hWnd, windowTitle, TITLE_SIZE);
        int length = ::GetWindowTextLength(hWnd);

        std::wstring temp(&windowTitle[0]);
        QString title = QString::fromStdWString(temp);

        for (auto pattern: engine->patterns_) {
            if (title.contains(pattern)) {
                qDebug() << "hiding" << title << hWnd;
                ShowWindow(hWnd, SW_HIDE);

                engine->hiddenWindows_.append(hWnd);
                qDebug() << engine->hiddenWindows_.length();
            }
        }

        return TRUE;
    }, reinterpret_cast<LPARAM>(this));
}

QStringList WindowsEngine::getWindowList()
{
    windowList_.clear();

    ::EnumWindows([](HWND hWnd, LPARAM lParam) -> BOOL {
        if (!IsWindowVisible(hWnd)) {
            return TRUE;
        }

        WindowsEngine* engine = reinterpret_cast<WindowsEngine*>(lParam);
        const DWORD TITLE_SIZE = 1024;
        TCHAR windowTitle[TITLE_SIZE];

        GetWindowText(hWnd, windowTitle, TITLE_SIZE);
        int length = ::GetWindowTextLength(hWnd);

        std::wstring temp(&windowTitle[0]);
        QString title = QString::fromStdWString(temp);

        engine->windowList_.append(title);

        return TRUE;
    }, reinterpret_cast<LPARAM>(this));

    return windowList_;
}
