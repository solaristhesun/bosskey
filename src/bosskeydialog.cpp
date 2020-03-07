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
#include <QSortFilterProxyModel>

#include "bosskeydialog.h"
#include "platforminterface.h"
#include "uglobalhotkeys.h"
#include "misc/globals.h"
#include "ui_bosskeydialog.h"

BossKeyDialog::BossKeyDialog(PlatformInterface& engine, UGlobalHotkeys& hotkeyManager)
    : QDialog(nullptr)
    , ui_(new Ui::BossKeyDialog)
    , platform_(engine)
    , hotkeyManager_(hotkeyManager)

{
    ui_->setupUi(this);

    setupHotkeys();
    createTrayIcon();

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&windowList_);
    ui_->windowsTableView->setModel(proxyModel);
    windowList_.setWindowList(platform_.getWindowList());
    ui_->patternTableView->setModel(&patternList_);

    ui_->bringToFrontTableView->setModel(&bringToFrontList_);

    patternList_.loadFromSettings("hide");
    bringToFrontList_.loadFromSettings("bringToFront");

    if (patternList_.rowCount() == 0) {
        QDialog::show();
    }

    ui_->autoHideIntervalEdit->setValidator(new QIntValidator(0, 7200, this));

    QSettings settings;

    ui_->keySequenceEdit->setKeySequence(QKeySequence(settings.value("hotkey_hide", "Ctrl+F12").toString()));
    ui_->keySequenceEdit_2->setKeySequence(QKeySequence(settings.value("hotkey_show", "Ctrl+F11").toString()));
    ui_->hideSystrayIconCheckBox->setChecked(settings.value("hide_icon", false).toBool());
    ui_->autoHideCheckBox->setChecked(settings.value("auto_hide", false).toBool());
    ui_->autoHideIntervalEdit->setText(QString::number(settings.value("auto_hide_interval", 5).toInt()));
    ui_->autoHideIntervalEdit->setEnabled(ui_->autoHideCheckBox->isChecked());

    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));

    if (settings.value("auto_hide", false).toBool()) {
        timer_.start(1000);
    }

    connect(ui_->patternTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &BossKeyDialog::patternViewSelectionChanged);

    applyFocusLineHack(ui_->windowsTableView);
    applyFocusLineHack(ui_->patternTableView);
    applyFocusLineHack(ui_->bringToFrontTableView);
}

BossKeyDialog::~BossKeyDialog()
{
    delete ui_;
}

void BossKeyDialog::setupHotkeys()
{
    QSettings settings;

    hotkeyManager_.registerHotkey(settings.value("hotkey_hide", "Ctrl+F12").toString(), KeyCode_HideWindows);
    hotkeyManager_.registerHotkey(settings.value("hotkey_show", "Ctrl+F11").toString(), KeyCode_ShowWindows);

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

void BossKeyDialog::refreshVisibleWindowList()
{
    windowList_.setWindowList(platform_.getWindowList());
    ui_->windowsTableView->resizeColumnsToContents();
}

void BossKeyDialog::showWindows()
{
    platform_.showWindows();
    trayIcon->show();
}

void BossKeyDialog::hideWindows()
{
    if (!platform_.isHidden()) {
        platform_.hideWindows(patternList_.getWindowList());

        auto bringToFrontList = bringToFrontList_.getWindowList();

        if (!bringToFrontList.isEmpty()) {
            platform_.bringToFront(bringToFrontList.front());
        }

        if (ui_->hideSystrayIconCheckBox->isChecked()) {
            trayIcon->hide();
        }
    }
}

void BossKeyDialog::showEvent(QShowEvent *event)
{
    hotkeyManager_.unregisterAllHotkeys();
    ui_->tabWidget->setCurrentIndex(0);
    timer_.stop();

    QDialog::showEvent(event);
}

void BossKeyDialog::hideEvent(QHideEvent *event)
{
    QDialog::hideEvent(event);

    saveHotkeys();
    setupHotkeys();
    patternList_.saveToSettings("hide");
    bringToFrontList_.saveToSettings("bringToFront");

    QSettings settings;
    if (settings.value("auto_hide", false).toBool()) {
        timer_.start(1000);
    }
}

void BossKeyDialog::saveHotkeys()
{
    QSettings settings;
    settings.setValue("hotkey_hide", ui_->keySequenceEdit->keySequence());
    settings.setValue("hotkey_show", ui_->keySequenceEdit_2->keySequence());
    settings.setValue("hide_icon", ui_->hideSystrayIconCheckBox->isChecked());
    settings.setValue("auto_hide", ui_->autoHideCheckBox->isChecked());
    settings.setValue("auto_hide_interval", ui_->autoHideIntervalEdit->text());
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
    if (reason == QSystemTrayIcon::QSystemTrayIcon::Trigger)
    {
        if (platform_.isHidden())
        {
            showWindows();
        }
        else
        {
            hideWindows();
        }
    }

}

void BossKeyDialog::addButtonClicked()
{
    /*
    auto item = ui_->listWidget->currentItem();
    QListWidgetItem * newItem = new QListWidgetItem(item->text());
    newItem->setFlags (item->flags () | Qt::ItemIsEditable);
    ui_->listWidget_2->addItem(newItem);
    savePatterns();*/
}

void BossKeyDialog::deleteButtonClicked()
{
    /*auto item = ui_->listWidget_2->currentItem();
    ui_->listWidget_2->takeItem(ui_->listWidget_2->row(item));*/
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
    platform_.showWindows();
    QApplication::quit();
}

void BossKeyDialog::clearPatterns()
{
    ui_->patternTableView->clearSelection();
    patternList_.clear();
}

void BossKeyDialog::clearBringToFrontList()
{
    ui_->bringToFrontTableView->clearSelection();
    bringToFrontList_.clear();
}

void BossKeyDialog::onTimeout()
{
    QSettings settings;

    if (settings.value("auto_hide", false).toBool() && !QDialog::isVisible()) {
        uint autoHideTime = settings.value("auto_hide_interval", 5).toUInt();
        if (platform_.getUserIdleTime() >= autoHideTime) {
            hideWindows();
        }
    }
}

void BossKeyDialog::enableDisableAutoHideIntervalEdit(bool bEnabled)
{
    ui_->autoHideIntervalEdit->setEnabled(bEnabled);
}

void BossKeyDialog::applyFocusLineHack(QWidget *widget)
{
    widget->setStyleSheet(QString("QTableView::item::focus { outline: 0; background-color:%1; } QTableView { outline: 0; }").arg(this->palette().color(QPalette::Highlight).name()));
}

void BossKeyDialog::showContextMenu(const QPoint & pos)
{
    QMenu contextMenu;

    auto index = ui_->patternTableView->indexAt(pos);

    if (index.isValid()) {
        contextMenu.addAction(ui_->actionToggleIgnoreTitle);
        contextMenu.addAction(ui_->actionRemovePattern);
        contextMenu.addSeparator();
    }

    contextMenu.addAction(ui_->actionClearPatterns);

    contextMenu.exec(ui_->patternTableView->viewport()->mapToGlobal(pos));
}

void BossKeyDialog::removePattern()
{
    auto index = ui_->patternTableView->currentIndex();
    patternList_.removeItem(index);
}

void BossKeyDialog::toggleIgnoreTitle()
{
    auto index = ui_->patternTableView->currentIndex();
    patternList_.toggleIgnoreTitle(index);
}

void BossKeyDialog::patternViewSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
    ui_->removeButton->setEnabled(!selected.empty());
    ui_->toggleIgnoreButton->setEnabled(!selected.empty());
}

// EOF <stefan@scheler.com>
