#include <QApplication>

#include "bosskeydialog.h"
#include "uglobalhotkeys.h"
#include "windowsengine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WindowsEngine engine;
    UGlobalHotkeys hotkeyManager;
    BossKeyDialog dialog(engine, hotkeyManager);
    return a.exec();
}

// EOF <stefan@scheler.com>
