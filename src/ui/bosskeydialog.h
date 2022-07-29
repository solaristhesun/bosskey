/*
 * This file is part of bosskey.
 *
 * Copyright (c) 2020 Stefan Scheler.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOSSKEYDIALOG_H
#define BOSSKEYDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAbstractItemDelegate>
#include <QTimer>
#include <QItemSelection>
#include <QTranslator>
#include <QSettings>

#include "model/windowlistviewmodel.h"
#include "model/singlewindowlistviewmodel.h"
#include "model/hiddenwindow.h"
#include "misc/taskexecutor.h"

namespace Ui {
class BossKeyDialog;
}

class PlatformInterface;
class UGlobalHotkeys;

class BossKeyDialog : public QDialog
{
    Q_OBJECT

public:
    BossKeyDialog(PlatformInterface& engine, UGlobalHotkeys& hotkeyManager);
    ~BossKeyDialog();

    void closeEvent(QCloseEvent *e) override;

    enum HotKey {
        KeyCode_HideWindows = 0,
        KeyCode_ShowWindows,
    };

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent* event) override;

public slots:
    void systemTracActivated(QSystemTrayIcon::ActivationReason reason);
    void showAboutDialog();
    void quitApplication();
    void onTimeout();
    void enableDisableAutoHideIntervalEdit(bool bEnabled);
    void refreshVisibleWindowList();
    void languageChanged(int index);
    void showWindows();
    void hideWindows();
    void addWindow();
    void bringWindowToFront();
    void tryRegisterHotkeys();

private:
    void loadSettings();
    void setupLocalization();
    void setupHotkeys();
    void createTrayIcon();
    void saveSettings();
    void loadLanguage(const QString& language);
    void switchTranslator(QTranslator& translator, const QString& filename);
    void applyFocusLineHack(QWidget* widget);
    void refreshTrayToolTip();
    void refreshTrayMenu();
    void refreshWindowsMenu();
    void showWindow(HiddenWindow window);
    void retranslateUserInterface();

private:
    QTranslator translator_;
    Ui::BossKeyDialog *ui_;
    PlatformInterface& platform_;
    UGlobalHotkeys& hotkeyManager_;
    QSystemTrayIcon *trayIcon;
    QMenu trayIconMenu_;
    QMenu windowsMenu_;
    QStringList patterns_;
    QTimer timer_;
    WindowListViewModel windowList_;
    WindowListViewModel patternList_;
    SingleWindowListViewModel bringToFrontList_;
    QString language_;
    TaskExecutor taskExecutor_;
    QSettings settings_;
};

#endif // BOSSKEYDIALOG_H
