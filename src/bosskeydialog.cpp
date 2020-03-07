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
#include <QDirIterator>

#include "bosskeydialog.h"
#include "platforminterface.h"
#include "uglobalhotkeys.h"
#include "misc/globals.h"
#include "ui_bosskeydialog.h"

BossKeyDialog::BossKeyDialog(PlatformInterface& engine, UGlobalHotkeys& hotkeyManager)
    : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
    , ui_(new Ui::BossKeyDialog)
    , platform_(engine)
    , hotkeyManager_(hotkeyManager)

{
    ui_->setupUi(this);
    QDialog::setWindowTitle(Globals::ApplicationFullName);

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

    loadUserInterfaceSettings();

    QSettings settings;

    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimeout()));

    if (settings.value("auto_hide", false).toBool()) {
        timer_.start(1000);
    }

    connect(ui_->patternTableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &BossKeyDialog::patternViewSelectionChanged);

    applyFocusLineHack(ui_->windowsTableView);
    applyFocusLineHack(ui_->patternTableView);
    applyFocusLineHack(ui_->bringToFrontTableView);

    setupLocalization();
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

void BossKeyDialog::changeEvent(QEvent *event)
{
    if (event != nullptr) {
        switch(event->type()) {
        case QEvent::LanguageChange:
            qDebug() << "retranslate";
            ui_->retranslateUi(this);
            ui_->donationLabel->setText(QString("<html><head/><body><p><a href=\"https://scheler.com/bosskey/donate\"><span style=\" font-weight:600; text-decoration: underline; color:white;\">%1</span></a></p></body></html>").arg(tr("If this software is useful to you, please consider making a donation.")));
            break;
        default:
            break;
        }
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
    settings.setValue("hide_on_click", ui_->hideOnClickCheckBox->isChecked());
}

void BossKeyDialog::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(ui_->actionShowDialog);
    trayIconMenu->addAction(ui_->actionShowAbout);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui_->actionExit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/appicon/leader.svg"));
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
        QSettings settings;

        if (settings.value("hide_on_click", true).toBool())
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
        Window w = patternList_.getWindow(index);
        ui_->actionToggleIgnoreTitle->setChecked(w.ignoreTitle);
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
}

void BossKeyDialog::languageChanged(int index)
{
    QString locale = ui_->languageComboBox->itemData(index).toString();

    if (locale != language_) {
        qDebug() << "language changed" << index;
        loadLanguage(locale);
        QSettings settings;
        settings.setValue("language", locale);
        language_ = locale;
    }
}

void BossKeyDialog::loadLanguage(const QString& language)
{
    switchTranslator(translator_, QString("bosskey_%1.qm").arg(language));
}

void BossKeyDialog::switchTranslator(QTranslator& translator, const QString& filename)
{
    // remove the old translator
    qApp->removeTranslator(&translator);

    // load the new translator
    if(translator.load(":/translations/" + filename))
        qApp->installTranslator(&translator);
}

void BossKeyDialog::loadUserInterfaceSettings()
{
    QSettings settings;

    ui_->keySequenceEdit->setKeySequence(QKeySequence(settings.value("hotkey_hide", "Ctrl+F12").toString()));
    ui_->keySequenceEdit_2->setKeySequence(QKeySequence(settings.value("hotkey_show", "Ctrl+F11").toString()));
    ui_->hideSystrayIconCheckBox->setChecked(settings.value("hide_icon", false).toBool());
    ui_->autoHideCheckBox->setChecked(settings.value("auto_hide", false).toBool());
    ui_->autoHideIntervalEdit->setText(QString::number(settings.value("auto_hide_interval", 5).toInt()));
    ui_->autoHideIntervalEdit->setEnabled(ui_->autoHideCheckBox->isChecked());
    ui_->hideOnClickCheckBox->setChecked(settings.value("hide_on_click", true).toBool());
}

void BossKeyDialog::setupLocalization()
{
    QSettings settings;

    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

    QDirIterator it(":/translations", QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString locale = it.next();
        locale.truncate(locale.lastIndexOf('.')); // "TranslationExample_de"
        locale.remove(0, locale.indexOf('_') + 1); // "de"
        QString lang = QLocale::languageToString(QLocale(locale).language());
        QIcon icon(QString(":/flags/%1.png").arg(locale));
        ui_->languageComboBox->addItem(icon, lang, locale);
        qDebug() << "adding" << lang << locale;
    }

    int index = ui_->languageComboBox->findData(settings.value("language", defaultLocale));

    if (index != -1) {
        ui_->languageComboBox->setCurrentIndex(index);
    }
    else {
        index = ui_->languageComboBox->findData("en");
        if (index != -1) {
            ui_->languageComboBox->setCurrentIndex(index);
        }
    }

    languageChanged(ui_->languageComboBox->currentIndex());
}

// EOF <stefan@scheler.com>
