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

#include <QDebug>
#include <QSettings>

#include "bosskeydialog.h"
#include "engineinterface.h"
#include "uglobalhotkeys.h"
#include "ui_bosskeydialog.h"

BossKeyDialog::BossKeyDialog(EngineInterface& engine, UGlobalHotkeys& hotkeyManager)
    : QDialog(nullptr)
    , ui_(new Ui::BossKeyDialog)
    , engine_(engine)
    , hotkeyManager_(hotkeyManager)

{
    ui_->setupUi(this);

    setupHotkeys();
    createTrayIcon();

    auto list = engine_.getWindowList();
    for (auto title: engine_.getWindowList()) {
        qDebug() << title;
        ui_->listWidget->addItem(title);
    }

    QSettings settings;
    //tringList list2;
    //list2 << "Visual Studio Code" << "Qt Creator" << "Cmder" << "Philipp Burghardt" << "Emulator";
    patterns_ = settings.value("patterns").toStringList();
    for (auto pattern: patterns_) {
        auto item = new QListWidgetItem(pattern);
        item->setFlags (item->flags () | Qt::ItemIsEditable);
        ui_->listWidget_2->addItem(item);
    }
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
            engine_.hideWindows(QStringList() << "Visual Studio Code" << "Qt Creator" << "Cmder" << "Philipp Burghardt" << "Emulator");
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

    connect(trayIcon, &QSystemTrayIcon::activated, this, &BossKeyDialog::systemTracActivated);
}

void BossKeyDialog::closeEvent(QCloseEvent *e)
{
    QDialog::hide();
    e->ignore();
}

void BossKeyDialog::systemTracActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::QSystemTrayIcon::DoubleClick)
    {
        if (engine_.isHidden())
        {
            engine_.showWindows();
        }
        else
        {
            engine_.hideWindows(QStringList() << "Visual Studio Code" << "Qt Creator" << "Cmder" << "Philipp Burghardt" << "Emulator");
        }
    }
}

void BossKeyDialog::addButtonClicked()
{
    auto item = ui_->listWidget->currentItem();
    QListWidgetItem * newItem = new QListWidgetItem(item->text());
    newItem->setFlags (item->flags () | Qt::ItemIsEditable);
    ui_->listWidget_2->addItem(newItem);
}

void BossKeyDialog::deleteButtonClicked()
{
    auto item = ui_->listWidget_2->currentItem();
    ui_->listWidget_2->takeItem(ui_->listWidget_2->row(item));
}

// EOF <stefan@scheler.com>
