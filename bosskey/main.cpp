#include "mainwindow.h"

#include "uglobalhotkeys.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow *w = new MainWindow;
    UGlobalHotkeys *hotkeyManager = new UGlobalHotkeys();
    hotkeyManager->registerHotkey("Ctrl+F12", 1);
    hotkeyManager->registerHotkey("Ctrl+F11", 2);
    QObject::connect(hotkeyManager, &UGlobalHotkeys::activated, [=](size_t id)
    {
        if ( id == 2) {
            w->showWindows();
        }
        else {
            w->hideWindows();
        }
    });


    return a.exec();
}
