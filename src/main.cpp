#include <QApplication>

#include "singleapplication.h"
#include "bosskeydialog.h"
#include "uglobalhotkeys.h"
#include "windowsengine.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);
    WindowsEngine engine;
    UGlobalHotkeys hotkeyManager;
    BossKeyDialog dialog(engine, hotkeyManager);
    return app.exec();
}

// EOF <stefan@scheler.com>
