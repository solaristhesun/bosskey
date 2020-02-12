#ifndef BOSSKEYDIALOG_H
#define BOSSKEYDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>

namespace Ui {
class BossKeyDialog;
}

class EngineInterface;
class UGlobalHotkeys;

class BossKeyDialog : public QDialog
{
    Q_OBJECT

public:
    BossKeyDialog(EngineInterface& engine, UGlobalHotkeys& hotkeyManager);
    ~BossKeyDialog();

private:
    void setupHotkeys();
    void createTrayIcon();

private:
    Ui::BossKeyDialog *ui_;
    EngineInterface& engine_;
    UGlobalHotkeys& hotkeyManager_;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // BOSSKEYDIALOG_H
