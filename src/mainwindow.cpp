#include <QIcon>
#include <QDebug>
#include <QAction>
#include <Windows.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

QStringList windowList;
QStringList hideList = { "Android Emulator - Stefans_Pixel:5554", "Philipp Burghardt", "Buddy-Liste", "Emulator", "Visual Studio Code"};

QList<HWND> hiddenWindows;


BOOL CALLBACK showFenster(HWND hwnd, LPARAM substring){

    const DWORD TITLE_SIZE = 1024;
    TCHAR windowTitle[TITLE_SIZE];

    GetWindowText(hwnd, windowTitle, TITLE_SIZE);
    int length = ::GetWindowTextLength(hwnd);

    std::wstring temp(&windowTitle[0]);
    QString title = QString::fromStdWString(temp);


    if (hideList.contains(title)) {

        ShowWindow(hwnd, SW_SHOW);
    }


    return TRUE;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
qDebug()<<"bam";
    createTrayIcon();
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::quit() {
    trayIcon->hide();
    QMainWindow::close();
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    QAction *newAct = new QAction(tr("&New"), this);
        connect(newAct, &QAction::triggered, this, &MainWindow::close);
    trayIconMenu->addAction(newAct);

    /*trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(maximizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
*/
    trayIcon = new QSystemTrayIcon(this);

    trayIcon->setIcon(QIcon(":/icons/assets/leader.svg"));
    trayIcon->setContextMenu(trayIconMenu);
}
