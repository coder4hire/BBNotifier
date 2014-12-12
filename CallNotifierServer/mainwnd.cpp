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

//QIcon CMainWnd::statusIcons[2];
QIcon CMainWnd::windowIcons[2];

CMainWnd::CMainWnd(QWidget *parent) :
    QDialog(parent),
    listener(this),
    ui(new Ui::CMainWnd)
#ifndef USE_MP3
    ,sound(":/Sounds/Call.wav")
#endif
{
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    ui->setupUi(this);

    PrepareTrayIcon();

    //-F- Starting TCP server
    listener.RestartListening();

    //-F- Handling signals from Settings Dialog
    connect(CSettingsDialog::GetInstance(),SIGNAL(OnOK()),this,SLOT(OnSettingsChanged()));

    //-F- Set Up Item Delegate to the history list
    QAbstractItemDelegate* delegate = ui->listHistory->itemDelegate();
    ui->listHistory->setItemDelegate(new CHistoryItemDelegate());
    delete delegate;

    //-F- Loading Icons
//    statusIcons[0].addFile(":/Images/MissedCall.png");
//    statusIcons[1].addFile(":/Images/OffhookCall.png");
    windowIcons[0].addFile(":/Images/RedIco.ico");
    windowIcons[1].addFile(":/Images/GreenIco.ico");


#ifdef USE_MP3
    player.setMedia(QUrl::fromLocalFile("Call.mp3"));
#endif

    RefreshTrayIconStatus();

    ui->linkLabel->URL = "http://gss.freeiz.com/index.php/en/products/bb-notifier";
}

CMainWnd::~CMainWnd()
{
    delete ui->listHistory->itemDelegate();
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

void CMainWnd::ShowMessage(QString msg)
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information;
    trayIcon->showMessage("BB Notifier", msg, icon,3000);
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
    hide();
    event->ignore();
}

void CMainWnd::resizeEvent(QResizeEvent* ev)
{
    ui->listHistory->resize(ev->size().width(),ev->size().height()-40);
    ui->btnClearHistory->move(ev->size().width()-ui->btnClearHistory->size().width()-5,
                              ev->size().height()-ui->btnClearHistory->size().height()-5);
    ui->linkLabel->move(ui->linkLabel->pos().x(),
                        ev->size().height()-ui->linkLabel->size().height()-10);
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
    if(data.Type==CNotificationItem::RINGING_CALL)
    {
        ShowMessage(data.Name+"\n"+data.Phone);
        AddNewCall(data);
        RefreshTrayIconStatus();

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
    }
    else if(data.Type==CNotificationItem::OFFHOOK_CALL)
    {
        MarkCallAccepted(data);
        RefreshTrayIconStatus();
    }
}

void CMainWnd::on_btnClearHistory_clicked()
{
    ui->listHistory->clear();
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

    setWindowIcon(windowIcons[isGreen ? 1:0]);
    trayIcon->setIcon(windowIcons[isGreen ? 1:0]);
}
