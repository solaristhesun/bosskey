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

    void closeEvent(QCloseEvent *e);

    enum HotKey {
        KeyCode_HideWindows = 0,
        KeyCode_ShowWindows,
    };

    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

public slots:
    void systemTracActivated(QSystemTrayIcon::ActivationReason reason);
    void addButtonClicked();
    void deleteButtonClicked();
    void showAboutDialog();
    void quitApplication();
    void patternEditDone(QWidget *editor, QAbstractItemDelegate::EndEditHint hint);
    void onTimeout();

private:
    void setupHotkeys();
    void createTrayIcon();
    void savePatterns();
    void saveHotkeys();
    void showWindows();
    void hideWindows();

private:
    Ui::BossKeyDialog *ui_;
    PlatformInterface& engine_;
    UGlobalHotkeys& hotkeyManager_;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QStringList patterns_;
    QTimer timer_;
};

#endif // BOSSKEYDIALOG_H
