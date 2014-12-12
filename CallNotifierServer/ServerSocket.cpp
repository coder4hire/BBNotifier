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


#include "ServerSocket.h"
#include "stdio.h"
#include "AESWrapper.h"
#include "SettingsDialog.h"
#include "qdatetime.h"

#define MESSAGE_HEADER 4

QReadWriteLock CServerSocket::lock;
QString CServerSocket::LastMessageStatus;

CServerSocket::CServerSocket(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(OnReadyRead()));
    connect(this,SIGNAL(disconnected()),this,SLOT(OnDisconnected()));
}

void CServerSocket::OnReadyRead()
{
    int payloadLength=32;
    bool isCorrectMessage=false;

    //-F- Getting packet length
    if(bytesAvailable()>4)
    {
        QTcpSocket::peek((char*)&payloadLength,4);

        if(payloadLength>4096 || payloadLength<1)
        {
            //-F- Too big or small packet - that can't be true
            close();
            return;
        }

        if(bytesAvailable()>=payloadLength+MESSAGE_HEADER)
        {
            char* pBuf = new char[payloadLength+MESSAGE_HEADER+1];

            if(QTcpSocket::read(pBuf,payloadLength+4)==payloadLength+MESSAGE_HEADER)
            {
                 //-F- Got real message
                QString decryptedMessage = CAESWrapper::Decrypt(pBuf+MESSAGE_HEADER+CryptoPP::AES::BLOCKSIZE,payloadLength-CryptoPP::AES::BLOCKSIZE,CSettingsDialog::GetInstance()->GetAesKey(),((unsigned char*)pBuf)+MESSAGE_HEADER);

                if(decryptedMessage.mid(0,8)=="C@LLER#\n")
                {
                    isCorrectMessage=true;
                    emit MessageReady(decryptedMessage.mid(8));
                }
            }
            delete[] pBuf;
            //-F- After receiving packet close the connection
            close();
        }

        QString statusMsg = ("Packet is received at ")+QDateTime::currentDateTime().toString("hh:mm:ss")+
                (isCorrectMessage ? ", it is valid." : ", but it contains invalid information.");
        SetLastMessageStatus(statusMsg);
    }
}

void CServerSocket::OnDisconnected()
{
    this->deleteLater();
}

void CServerSocket::SetLastMessageStatus(QString str)
{
    lock.lockForWrite();
    LastMessageStatus = str;
    lock.unlock();
}

QString CServerSocket::GetLastMessageStatus()
{
    QString str;
    lock.lockForRead();
    str=LastMessageStatus;
    lock.unlock();
    return str;
}
