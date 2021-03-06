//BB Notifier Server - server for receiving call notifications from phones
//Copyright (C) 2014  Green Snake Software

//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "mainwnd.h"
#include "ui_mainwnd.h"
#include "qevent.h"
#include "SettingsDialog.h"
#include "qsound.h"
#include "qvariant.h"
#include "qdatetime.h"
#include "HistoryItemDelegate.h"
#include "NotificationItemDelegate.h"
#include "qlayout.h"

QIcon CMainWnd::windowIcons[4];

CMainWnd::CMainWnd(QWidget *parent) :
    QDialog(parent),
    listener(this),
    ui(new Ui::CMainWnd)
#ifndef USE_MP3
    ,sound(":/Sounds/Call.wav")
    ,soundNotif(":/Sounds/Notif.wav")
#endif
{
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);

    PrepareTrayIcon();

    hasNewNotificationCame=false;

    //-F- Starting TCP server
    listener.RestartListening();

    //-F- Handling signals from Settings Dialog
    connect(CSettingsDialog::GetInstance(),SIGNAL(OnOK()),this,SLOT(OnSettingsChanged()));

    //-F- Set Up Item Delegate to the history list
    QAbstractItemDelegate* delegate = ui->listHistory->itemDelegate();
    ui->listHistory->setItemDelegate(new CHistoryItemDelegate());
    delete delegate;

    //-F- Set Up Item Delegate to the notification list
    delegate = ui->listNotifications->itemDelegate();
    ui->listNotifications->setItemDelegate(new CNotificationItemDelegate());
    delete delegate;

    //-F- Loading Icons
    windowIcons[0].addFile(":/Images/RedIco.ico");
    windowIcons[1].addFile(":/Images/GreenIco.ico");
    windowIcons[2].addFile(":/Images/NotifRed.ico");
    windowIcons[3].addFile(":/Images/NotifGreen.ico");


#ifdef USE_MP3
    player.setMedia(QUrl::fromLocalFile("Call.mp3"));
#endif

    RefreshTrayIconStatus();

    ui->linkLabel->URL = "http://gss.freeiz.com/index.php/en/products/bb-notifier";

    trayTimer.setSingleShot(true);
    connect(&trayTimer,SIGNAL(timeout()),this,SLOT(OnTimer()));

//    ui->listNotifications->addItem(CNotificationItem("NOTIF","Name","ServiceName",QDateTime::currentDateTime(), "Some Long Text for notification wrapping test...").Serialize());
//    ui->listNotifications->addItem(CNotificationItem("NOTIF","Name2","ServiceName",QDateTime::currentDateTime(), "Some Long Text for notification wrapping test... 111 222 333 44 55 6  767 546 45 674567 4567 dg5 6df 53e6h drg h dt  ue th56u 5e6u 56u 356u").Serialize());

}

CMainWnd::~CMainWnd()
{
    delete ui->listHistory->itemDelegate();
    delete ui->listNotifications->itemDelegate();
    delete ui;
}

void CMainWnd::IconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        break;
    case QSystemTrayIcon::DoubleClick:
        ShowAndActivate();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:break;
    }
}

void CMainWnd::ShowMessage(const QString& name,const QString& msg)
{
    trayIcon->showMessage(name, msg, QSystemTrayIcon::Information, 3000);
    trayTimer.start(7000);
}

void CMainWnd::MessageClicked()
{
}

void CMainWnd::PrepareTrayIcon()
{
    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(MessageClicked()));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                 this, SLOT(IconActivated(QSystemTrayIcon::ActivationReason)));

    //-F- Creating context menu
    trayIconMenu = new QMenu(this);

    showAction = new QAction(QIcon(":/Images/Conversation.png"), tr("&Show"), this);
    connect(showAction, SIGNAL(triggered()), this, SLOT(ShowAndActivate()));

    settingsAction = new QAction(QIcon(":/Images/Gear.png"),tr("S&ettings && Diagnostics"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(ShowSettings()));

    quitAction = new QAction(QIcon(":/Images/Close.png"),tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu->addAction(showAction);
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);

    trayIcon->show();
}

void CMainWnd::closeEvent(QCloseEvent *event)
{
    QSettings settingsStorage("GSS","BBNotifier");
    settingsStorage.setValue("mainWindow",this->saveGeometry());
    hide();
    event->ignore();
}

void CMainWnd::resizeEvent(QResizeEvent* ev)
{
    QSize rect = ev->size();
    ui->tabLists->resize(ev->size().width()+2,ev->size().height()-40);
    ui->listHistory->resize(ev->size().width()-3,ev->size().height() - ui->tabLists->tabBar()->height()-43);
    ui->listNotifications->resize(ev->size().width()-3,ev->size().height() - ui->tabLists->tabBar()->height()-43);
    ui->btnClearHistory->move(ev->size().width()-ui->btnClearHistory->size().width()-5,
                              ev->size().height()-ui->btnClearHistory->size().height()-5);
    ui->linkLabel->move(ui->linkLabel->pos().x(),
                        ev->size().height()-ui->linkLabel->size().height()-10);
    ui->listNotifications->setSpacing(0);
}

void CMainWnd::ShowSettings()
{
    if(!CSettingsDialog::GetInstance()->isVisible())
    {
        CSettingsDialog::GetInstance()->show();
    }

    CSettingsDialog::GetInstance()->activateWindow();
}

void CMainWnd::OnSettingsChanged()
{
    listener.RestartListening();
}

void CMainWnd::OnNewCall(const CNotificationItem& data)
{
    switch(data.Type)
    {
    case CNotificationItem::RINGING_CALL:
        if(CSettingsDialog::GetInstance()->IsCallPopupEnabled())
        {
            ShowMessage("Phone call",data.Name+"\n"+data.Phone);
        }
        AddNewCall(data);

        //-F- Play sound
#ifdef USE_MP3
        player.stop();
        player.play();
#else
        if(sound.isFinished())
        {
            sound.play();
        }
#endif
        break;

    case CNotificationItem::OFFHOOK_CALL:
        MarkCallAccepted(data);
        break;

    case CNotificationItem::NOTIFICATION:
        hasNewNotificationCame=true;
        if(soundNotif.isFinished())
        {
            soundNotif.play();
        }
        if(CSettingsDialog::GetInstance()->IsNotificationPopupEnabled())
        {
            ShowMessage(data.Phone,data.GetUnescapedData());
        }
        ui->listNotifications->addItem(data.Serialize());
        while(ui->listNotifications->count()>1 && ui->listNotifications->count() > CSettingsDialog::GetInstance()->GetMaxItemsCount())
        {
            delete ui->listNotifications->item(0);
        }
        ui->listNotifications->scrollToBottom();
        break;
    }

    RefreshTrayIconStatus();
}

void CMainWnd::on_btnClearHistory_clicked()
{
    if(ui->tabLists->tabBar()->currentIndex()==0)
    {
        ui->listNotifications->clear();
    }
    else
    {
        ui->listHistory->clear();
    }
    RefreshTrayIconStatus();
}

void CMainWnd::AddNewCall(const CNotificationItem& data)
{
    //-F- Find existing item with same phone and delete it first
    for(int i=0;i<ui->listHistory->count();i++)
    {
        QString str = ui->listHistory->item(i)->text();
        CNotificationItem listItem(str);
        if(listItem.Phone==data.Phone)
        {
            delete ui->listHistory->item(i);
            break;
        }
    }

    //-F- Inserting new item
    ui->listHistory->addItem(data.Serialize());

    while(ui->listHistory->count()>1 &&ui->listHistory->count()>CSettingsDialog::GetInstance()->GetMaxItemsCount())
    {
        delete ui->listHistory->item(0);
    }

    ui->listHistory->scrollToBottom();
}

void CMainWnd::MarkCallAccepted(const CNotificationItem& data)
{
    for(int i=0;i<ui->listHistory->count();i++)
    {
        QString str = ui->listHistory->item(i)->text();
        CNotificationItem listItem(str);
        if(listItem.Phone==data.Phone)
        {
            ui->listHistory->item(i)->setData(Qt::DisplayRole,data.Serialize());
            break;
        }
    }
}

void CMainWnd::ShowAndActivate()
{
    this->showNormal();
    this->activateWindow();
}

void CMainWnd::showEvent(QShowEvent* ev)
{
    QSettings settingsStorage("GSS","BBNotifier");
    restoreGeometry(settingsStorage.value("mainWindow",QByteArray()).toByteArray());
}


void CMainWnd::RefreshTrayIconStatus()
{
    bool isGreen=true;
    for(int i=0;i<ui->listHistory->count();i++)
    {
        QString str = ui->listHistory->item(i)->text();
        CNotificationItem listItem(str);
        if(listItem.Type==CNotificationItem::RINGING_CALL)
        {
            isGreen=false;
            break;
        }
    }

    int iconNum  = (hasNewNotificationCame ? 2:0)+(isGreen ? 1:0);
    setWindowIcon(windowIcons[iconNum]);
    trayIcon->setIcon(windowIcons[iconNum]);
}

void CMainWnd::OnTimer()
{
    trayIcon->hide();
    trayIcon->show();
}

void CMainWnd::changeEvent (QEvent *event)
{
    if(isActiveWindow())
    {
        hasNewNotificationCame=false;
        RefreshTrayIconStatus();
    }
}
