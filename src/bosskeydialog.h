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

#include "windowlistviewmodel.h"
#include "singlewindowlistviewmodel.h"

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
    void changeEvent(QEvent* event) override;

public slots:
    void systemTracActivated(QSystemTrayIcon::ActivationReason reason);
    void showAboutDialog();
    void quitApplication();
    void onTimeout();
    void enableDisableAutoHideIntervalEdit(bool bEnabled);
    void refreshVisibleWindowList();
    void clearPatterns();
    void clearBringToFrontList();
    void removePattern();
    void toggleIgnoreTitle();
    void showContextMenu(const QPoint & point);
    void patternViewSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void languageChanged(int index);

private:
    void loadUserInterfaceSettings();
    void setupLocalization();
    void setupHotkeys();
    void createTrayIcon();
    void saveHotkeys();
    void showWindows();
    void hideWindows();
    void loadLanguage(const QString& language);
    void switchTranslator(QTranslator& translator, const QString& filename);
    void applyFocusLineHack(QWidget* widget);

private:
    QTranslator translator_;
    Ui::BossKeyDialog *ui_;
    PlatformInterface& platform_;
    UGlobalHotkeys& hotkeyManager_;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QStringList patterns_;
    QTimer timer_;
    WindowListViewModel windowList_;
    WindowListViewModel patternList_;
    SingleWindowListViewModel bringToFrontList_;
    QString language_;
};

#endif // BOSSKEYDIALOG_H
