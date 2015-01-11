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

#include "ListenerSocket.h"
#include "ServerSocket.h"
#include "SettingsDialog.h"
#include "mainwnd.h"

CListenerSocket::CListenerSocket(QObject *parent) :
    QTcpServer(parent)
{
}

void CListenerSocket::incomingConnection(qintptr socketDescriptor)
{
    CServerSocket * pSock = new CServerSocket();
    pSock->setSocketDescriptor(socketDescriptor);
    connect(pSock,SIGNAL(MessageReady(QString)),this,SLOT(OnIncomingMessage(QString)));
    serverSockets.push_back(pSock);
    addPendingConnection(pSock);
    nextPendingConnection();
}

void CListenerSocket::RestartListening()
{
    if(isListening())
    {
        close();
    }

    listen(QHostAddress::Any,CSettingsDialog::GetInstance()->GetPort());
}

CMainWnd* CListenerSocket::GetMainWnd()
{
    return (CMainWnd*)parent();
}

void CListenerSocket::OnIncomingMessage(QString msg)
{
    CNotificationItem data(msg);
    //data.NormalizeIcon();
    if(data.Type!=CNotificationItem::EMPTY)
    {
        data.DateTime=QDateTime::currentDateTime();
        GetMainWnd()->OnNewCall(data);
    }
}
