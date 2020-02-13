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
#include <QMessageBox>

#include "bosskeydialog.h"
#include "engineinterface.h"
#include "uglobalhotkeys.h"
#include "globals.h"
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

    connect(ui_->listWidget_2->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)), this, SLOT(patternEditDone(QWidget*, QAbstractItemDelegate::EndEditHint)));

    auto list = engine_.getWindowList();
    for (auto title: engine_.getWindowList()) {
        qDebug() << title;
        ui_->listWidget->addItem(title);
    }

    QSettings settings;
    patterns_ = settings.value("patterns").toStringList();
    for (auto pattern: patterns_) {
        auto item = new QListWidgetItem(pattern);
        item->setFlags (item->flags () | Qt::ItemIsEditable);
        ui_->listWidget_2->addItem(item);
    }
    if (patterns_.empty()) {
        QDialog::show();
    }

}

BossKeyDialog::~BossKeyDialog()
{
    delete ui_;
}

void BossKeyDialog::setupHotkeys()
{
    hotkeyManager_.registerHotkey("Ctrl+F12", KeyCode_HideWindows);
    hotkeyManager_.registerHotkey("Ctrl+F11", KeyCode_ShowWindows);

    QObject::connect(&hotkeyManager_, &UGlobalHotkeys::activated, [=](size_t id)
    {
        if (id == KeyCode_ShowWindows) {
            engine_.showWindows();
        }
        else {
            engine_.hideWindows(patterns_);
        }
    });
}

void BossKeyDialog::savePatterns()
{
    QSettings settings;
    settings.setValue("patterns", patterns_);
}

void BossKeyDialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(ui_->actionShowDialog);
    trayIconMenu->addAction(ui_->actionShowAbout);
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
    patterns_.append(newItem->text());
    savePatterns();
}

void BossKeyDialog::deleteButtonClicked()
{
    auto item = ui_->listWidget_2->currentItem();
    ui_->listWidget_2->takeItem(ui_->listWidget_2->row(item));
    patterns_.removeAll(item->text());
    savePatterns();
}

void BossKeyDialog::showAboutDialog()
{
    const QString contents = QString(
        "<p><font color=#000080><font size=6><b>%1</b></font> <font size=4>(revision %2)</font></font></p>"
        "<p align=left>Copyright &copy; 2020 Stefan Scheler. %3</p>"
        "<p><a href=\"%4\">%5</a></p>"
        "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>")
                             .arg(Globals::ApplicationFullName, QString::number(Globals::ApplicationRevision), tr("All rights reserved."), Globals::ApplicationWebsite, tr("Visit bosskey website"));

    QMessageBox::about(this, tr("About bosskey"), contents);
}

void BossKeyDialog::quitApplication()
{
    QApplication::quit();
}

void BossKeyDialog::patternEditDone(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    savePatterns();
}

// EOF <stefan@scheler.com>
