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

    ui_->keySequenceEdit->setKeySequence(QKeySequence(settings.value("hotkey_hide").toString()));
    ui_->keySequenceEdit_2->setKeySequence(QKeySequence(settings.value("hotkey_show").toString()));
    ui_->hideSystrayIconCheckBox->setChecked(settings.value("hide_icon").toBool());
}

BossKeyDialog::~BossKeyDialog()
{
    delete ui_;
}

void BossKeyDialog::setupHotkeys()
{
    QSettings settings;

    hotkeyManager_.registerHotkey(settings.value("hotkey_hide").toString(), KeyCode_HideWindows);
    hotkeyManager_.registerHotkey(settings.value("hotkey_show").toString(), KeyCode_ShowWindows);

    QObject::connect(&hotkeyManager_, &UGlobalHotkeys::activated, [=](size_t id)
    {
        if (id == KeyCode_ShowWindows) {
            showWindows();
        }
        else {
            hideWindows();
        }
    });
}

void BossKeyDialog::showWindows()
{
    engine_.showWindows();
    trayIcon->show();
}

void BossKeyDialog::hideWindows()
{
    engine_.hideWindows(patterns_);

    if (ui_->hideSystrayIconCheckBox->isChecked()) {
        trayIcon->hide();
    }
}

void BossKeyDialog::showEvent(QShowEvent *event)
{
    hotkeyManager_.unregisterAllHotkeys();
    QDialog::showEvent(event);
    qDebug() << "show";
}

void BossKeyDialog::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);
    qDebug() << "hide";
    saveHotkeys();
    setupHotkeys();
}

void BossKeyDialog::saveHotkeys()
{
    QSettings settings;
    settings.setValue("hotkey_hide", ui_->keySequenceEdit->keySequence());
    settings.setValue("hotkey_show", ui_->keySequenceEdit_2->keySequence());
    settings.setValue("hide_icon", ui_->hideSystrayIconCheckBox->isChecked());
}

void BossKeyDialog::savePatterns()
{
    patterns_.clear();

    for(int i = 0; i < ui_->listWidget_2->count(); ++i)
    {
        QListWidgetItem* item = ui_->listWidget_2->item(i);
        patterns_ << item->text();
    }

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
    trayIcon->setIcon(QIcon(":/icons/assets/appicon/leader.svg"));
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
    savePatterns();
}

void BossKeyDialog::deleteButtonClicked()
{
    auto item = ui_->listWidget_2->currentItem();
    ui_->listWidget_2->takeItem(ui_->listWidget_2->row(item));
    savePatterns();
}

void BossKeyDialog::showAboutDialog()
{
    const QString contents = QString(
        "<p><font color=#000080><font size=6><b>%1</b></font> <font size=4>(revision %2)</font></font></p>"
        "<p align=left>Copyright &copy; 2020 Stefan Scheler. %3</p>"
        "<p><a href=\"%4\">%5</a></p>"
        "<p>The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.</p>"
        "<p>Icons made by <a href=\"https://www.flaticon.com/authors/freepik\" title=\"Freepik\">Freepik</a> from <a href=\"https://www.flaticon.com/\" title=\"Flaticon\">www.flaticon.com</a>"
        "<p>Some icons made by <a href=\"https://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a>. Licensed under a <a href=\"http://creativecommons.org/licenses/by/3.0/\">Creative Commons Attribution 3.0 License</a>.")
                             .arg(Globals::ApplicationFullName, QString::number(Globals::ApplicationRevision), tr("All rights reserved."), Globals::ApplicationWebsite, tr("Visit bosskey website"));

    QMessageBox::about(this, tr("About bosskey"), contents);
}

void BossKeyDialog::quitApplication()
{
    engine_.showWindows();
    QApplication::quit();
}

void BossKeyDialog::patternEditDone(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    savePatterns();
}

// EOF <stefan@scheler.com>
