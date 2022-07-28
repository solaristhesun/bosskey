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

#include <QSettings>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QDirIterator>

#include "ui/bosskeydialog.h"
#include "platforms/platforminterface.h"
#include "uglobalhotkeys.h"
#include "misc/globals.h"
#include "ui_bosskeydialog.h"

BossKeyDialog::BossKeyDialog(PlatformInterface& engine, UGlobalHotkeys& hotkeyManager)
    : QDialog(nullptr, Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint)
    , ui_(new Ui::BossKeyDialog)
    , platform_(engine)
    , hotkeyManager_(hotkeyManager)
    , trayIconMenu_(this)
    , windowsMenu_(this)

{
    ui_->setupUi(this);

    connect(ui_->patternTableView->verticalHeader(), &QHeaderView::sectionCountChanged,
        [=](int, int newCount) { ui_->clearButton->setEnabled(newCount > 0); });
    connect(ui_->bringToFrontTableView->verticalHeader(), &QHeaderView::sectionCountChanged,
        [=](int, int newCount) { ui_->clearBringToFrontButton->setEnabled(newCount > 0); });

    QDialog::setWindowTitle(Globals::ApplicationFullName);

    setupHotkeys();
    createTrayIcon();
    refreshTrayMenu();

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&windowList_);
    ui_->windowsTableView->setModel(proxyModel);
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

    connect(ui_->patternTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [=](const QItemSelection & selected, const QItemSelection &)
            {
                ui_->removeButton->setEnabled(!selected.empty());
            });
    connect(ui_->windowsTableView->selectionModel(), &QItemSelectionModel::selectionChanged,
        [=](const QItemSelection & selected, const QItemSelection &)
            {
                ui_->addButton->setEnabled(!selected.empty());
                ui_->bringToTopButton->setEnabled(!selected.empty());
            });

    applyFocusLineHack(ui_->windowsTableView);
    applyFocusLineHack(ui_->patternTableView);
    applyFocusLineHack(ui_->bringToFrontTableView);

    setupLocalization();
    refreshTrayToolTip();
    refreshVisibleWindowList();
    retranslateUserInterface();
}

BossKeyDialog::~BossKeyDialog()
{
    delete ui_;
}

void BossKeyDialog::setupHotkeys()
{
    QSettings settings;

    try {
        hotkeyManager_.registerHotkey(settings.value("hotkey_hide", "Ctrl+F12").toString(), KeyCode_HideWindows);
        hotkeyManager_.registerHotkey(settings.value("hotkey_show", "Ctrl+F11").toString(), KeyCode_ShowWindows);
    }
    catch (UException& e) {
        qDebug() << "exception: " << e.what();
    }

    QList<QKeySequence> sequenceHide;
    sequenceHide.push_back(QKeySequence(settings.value("hotkey_hide", "Ctrl+F12").toString()));

    QList<QKeySequence> sequenceShow;
    sequenceShow.push_back(QKeySequence(settings.value("hotkey_show", "Ctrl+F11").toString()));

    ui_->actionHideWindows->setShortcuts(sequenceHide);
    ui_->actionShowWindows->setShortcuts(sequenceShow);

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

void BossKeyDialog::tryRegisterHotkeys()
{
    qDebug() << "tryRegisterHotkeys";
    auto hideSquence = ui_->keySequenceEditHide->keySequence();

    if (!hideSquence.isEmpty())
    {
        if (hotkeyManager_.registerHotkey(hideSquence.toString(QKeySequence::PortableText), KeyCode_HideWindows))
        {
            ui_->keySequenceEditHide->setStatus(KeySequenceWidget::Status_Valid);
        }
        else
        {
            ui_->keySequenceEditHide->setStatus(KeySequenceWidget::Status_Invalid);
        }
    }
    else
    {
        ui_->keySequenceEditHide->setStatus(KeySequenceWidget::Status_Unset);
    }

    auto showSquence = ui_->keySequenceEditShow->keySequence();

    if (!showSquence.isEmpty())
    {
        if (hotkeyManager_.registerHotkey(showSquence.toString(QKeySequence::PortableText), KeyCode_ShowWindows))
        {

            ui_->keySequenceEditShow->setStatus(KeySequenceWidget::Status_Valid);
        }
        else
        {
            ui_->keySequenceEditShow->setStatus(KeySequenceWidget::Status_Invalid);
        }
    }
    else
    {
        ui_->keySequenceEditShow->setStatus(KeySequenceWidget::Status_Unset);
    }

    hotkeyManager_.unregisterAllHotkeys();
}

void BossKeyDialog::refreshVisibleWindowList()
{
    ui_->windowsTableView->clearSelection();
    windowList_.setWindowList(platform_.getWindowList());
    ui_->windowsTableView->resizeColumnsToContents();
}

void BossKeyDialog::showWindows()
{
    platform_.showWindows();
    refreshTrayToolTip();
    refreshWindowsMenu();
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

        refreshTrayToolTip();
        refreshWindowsMenu();
    }
}

void BossKeyDialog::showEvent(QShowEvent *event)
{
    refreshVisibleWindowList();
    hotkeyManager_.unregisterAllHotkeys();
    tryRegisterHotkeys();
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
    QDialog::changeEvent(event);

    if (event != nullptr) {
        switch(event->type()) {
        case QEvent::LanguageChange:
            retranslateUserInterface();
            break;
        default:
            break;
        }
    }
}

void BossKeyDialog::retranslateUserInterface()
{
    qDebug() << "retranslate";
    ui_->retranslateUi(this);
    ui_->donationLabel->setText(QString("<html><head/><body><p><a href=\"https://scheler.com/bosskey/donate\"><span style=\" font-weight:600; text-decoration: underline; color:white;\">%1</span></a></p></body></html>").arg(tr("If this software is useful to you, please consider making a donation.")));
    refreshWindowsMenu();
}

void BossKeyDialog::saveHotkeys()
{
    QSettings settings;

    settings.setValue("hotkey_hide", ui_->keySequenceEditHide->keySequence().toString());
    settings.setValue("hotkey_show", ui_->keySequenceEditShow->keySequence().toString());
    settings.setValue("hide_icon", ui_->hideSystrayIconCheckBox->isChecked());
    settings.setValue("auto_hide", ui_->autoHideCheckBox->isChecked());
    settings.setValue("auto_hide_interval", ui_->autoHideIntervalEdit->text());
    settings.setValue("hide_on_click", ui_->hideOnClickCheckBox->isChecked());
}

void BossKeyDialog::refreshTrayMenu()
{
    trayIconMenu_.clear();
    trayIconMenu_.addAction(ui_->actionHideWindows);
    trayIconMenu_.addAction(ui_->actionShowWindows);
    trayIconMenu_.addSeparator();
    trayIconMenu_.addMenu(&windowsMenu_);
    trayIconMenu_.addSeparator();
    trayIconMenu_.addAction(ui_->actionShowDialog);
    trayIconMenu_.addAction(ui_->actionShowAbout);
    trayIconMenu_.addSeparator();
    trayIconMenu_.addAction(ui_->actionExit);

    refreshWindowsMenu();
}

void BossKeyDialog::refreshWindowsMenu()
{
    auto windowList = platform_.getHiddenWindowList();

    windowsMenu_.setTitle(tr("Hidden windows"));
    windowsMenu_.clear();

    for (auto window: windowList) {
        windowsMenu_.addAction(window.title, [=] () { showWindow(window); });
    }

    windowsMenu_.setEnabled(windowList.count() > 0);
}

void BossKeyDialog::showWindow(HiddenWindow window)
{
    platform_.showWindow(window);
    refreshWindowsMenu();
}

void BossKeyDialog::createTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/appicon/leader.svg"));
    trayIcon->setContextMenu(&trayIconMenu_);
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
        "<p>Icons made by <a href=\"https://www.flaticon.com/authors/freepik\" title=\"Freepik\">Freepik</a> from <a href=\"https://www.flaticon.com/\" title=\"Flaticon\">www.flaticon.com</a></p>"
        "<p>Some icons made by <a href=\"http://www.famfamfam.com/lab/icons/silk/\">famfamfam</a>. Licensed under a <a href=\"http://creativecommons.org/licenses/by/3.0/\">Creative Commons Attribution 3.0 License</a>.</p>"
        "<p>Some icons made by <a href=\"https://p.yusukekamiyamane.com/\">Yusuke Kamiyamane</a>. Licensed under a <a href=\"http://creativecommons.org/licenses/by/3.0/\">Creative Commons Attribution 3.0 License</a>.</p>")
                             .arg(Globals::ApplicationFullName, QString::number(Globals::ApplicationRevision), tr("All rights reserved."), Globals::ApplicationWebsite, tr("Visit bosskey website"));

    QMessageBox::about(this, tr("About bosskey"), contents);
}

void BossKeyDialog::quitApplication()
{
    platform_.showWindows();
    QApplication::quit();
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

void BossKeyDialog::languageChanged(int index)
{
    QString locale = ui_->languageComboBox->itemData(index).toString();

    if (locale != language_) {
        qDebug() << "languageChanged" << locale;
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
    if(translator.load(":/i18n/" + filename))
        qApp->installTranslator(&translator);
}

void BossKeyDialog::loadUserInterfaceSettings()
{
    QSettings settings;

    ui_->keySequenceEditHide->setKeySequence(QKeySequence(settings.value("hotkey_hide", "Ctrl+F12").toString()));
    ui_->keySequenceEditShow->setKeySequence(QKeySequence(settings.value("hotkey_show", "Ctrl+F11").toString()));
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

    QDirIterator it(":/i18n", QDirIterator::Subdirectories);
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

void BossKeyDialog::refreshTrayToolTip()
{
    trayIcon->setToolTip(QString(tr("%1 hidden windows")).arg(platform_.hiddenWindowsCount()));
}

void BossKeyDialog::addWindow()
{
    auto index = ui_->windowsTableView->currentIndex();
    auto window = windowList_.getWindow(index);
    patternList_.addWindow(window);
}

void BossKeyDialog::bringWindowToFront()
{
    auto index = ui_->windowsTableView->currentIndex();
    auto window = windowList_.getWindow(index);
    bringToFrontList_.addWindow(window);
}

// EOF <stefan@scheler.com>
