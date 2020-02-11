#include "bosskeydialog.h"
#include "windowsengine.h"
#include "uglobalhotkeys.h"
#include "ui_bosskeydialog.h"

BossKeyDialog::BossKeyDialog(WindowsEngine& engine, UGlobalHotkeys& hotkeyManager)
    : QDialog(nullptr)
    , ui_(new Ui::BossKeyDialog)
    , engine_(engine)
    , hotkeyManager_(hotkeyManager)
{
    ui_->setupUi(this);

    setupHotkeys();
    createTrayIcon();
}

BossKeyDialog::~BossKeyDialog()
{
    delete ui_;
}

void BossKeyDialog::setupHotkeys()
{
    hotkeyManager_.registerHotkey("Ctrl+F12", 1);
    hotkeyManager_.registerHotkey("Ctrl+F11", 2);

    QObject::connect(&hotkeyManager_, &UGlobalHotkeys::activated, [=](size_t id)
    {
        if ( id == 2) {
            engine_.showWindows();
        }
        else {
            engine_.hideWindows(QStringList() << "EditPad Pro 7");
        }
    });
}

void BossKeyDialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(ui_->actionShowDialog);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui_->actionExit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/assets/leader.svg"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

// EOF <stefan@scheler.com>
