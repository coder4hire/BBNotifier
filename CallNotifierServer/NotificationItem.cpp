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

#include "NotificationItem.h"
#include "qstringlist.h"

const QString CNotificationItem::typeNames[]={"EMPTY","RINGING","OFFHOOK"};

CNotificationItem::CNotificationItem()
{
    Type=EMPTY;
}

CNotificationItem::CNotificationItem(const QString& typeStr,const QString& phone,const QString& name,const QDateTime& dateTime)
{
    Phone = phone;
    Name = name;
    DateTime = dateTime;
    SetType(typeStr);
}

CNotificationItem::CNotificationItem(const QString& serializedString)
{
    QStringList dataItems = serializedString.split("\n");
    int len = dataItems.length();
    if(len>0)
    {
        SetType(dataItems[0]);
    }

    Phone = len>1 ? dataItems[1] : "Unknown Phone";
    Name = len>2 ? dataItems[2] : "Unknown Contact";
    DateTime = len>3 ? QDateTime::fromString(dataItems[3]) : QDateTime();
}

QString CNotificationItem::Serialize() const
{
    return typeNames[Type]+"\n"+Phone+"\n"+Name+"\n"+DateTime.toString();
}

void CNotificationItem::SetType(const QString& strType)
{
    for(int i=0;i<_countof(typeNames);i++)
    {
        if(strType==typeNames[i])
        {
            Type = (EType)i;
            return;
        }
    }

    Type = EMPTY;
}
