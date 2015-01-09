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

const QString CNotificationItem::typeNames[]={"EMPTY","RINGING","OFFHOOK","NOTIF"};

CNotificationItem::CNotificationItem()
{
    Type=EMPTY;
}

CNotificationItem::CNotificationItem(const QString& typeStr, const QString& phone, const QString& name, const QDateTime& dateTime, const QString &data)
{
    Phone = phone;
    Name = name;
    DateTime = dateTime;
    Data = data;
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
    Data = len>3 ? dataItems[3] : "";
    QByteArray byteArray = QByteArray::fromBase64(dataItems[4].toUtf8());
    const uchar* p = (const uchar*)byteArray.constData();
    Image = len>4 && dataItems[4]!=""?
        QImage(p,24,24,QImage::Format_RGBA8888).copy() :
        QImage();
    DateTime = len>5 ? QDateTime::fromString(dataItems[5]) : QDateTime();
}

QString CNotificationItem::Serialize() const
{
    const char* bits = (const char*)Image.bits();
    QString enc = (QString)QByteArray((const char*)bits,24*24*4).toBase64();
    return typeNames[Type]+"\n"+Phone+"\n"+Name+"\n"+Data+"\n"+
            (Image.isNull() ? "" : enc) +"\n"+DateTime.toString();
}

void CNotificationItem::SetType(const QString& strType)
{
    for(int i=0;i<(int)_countof(typeNames);i++)
    {
        if(strType==typeNames[i])
        {
            Type = (EType)i;
            return;
        }
    }

    Type = EMPTY;
}

QString CNotificationItem::GetUnescapedData() const
{
    return QString(Data).replace("\\n","\n");
}


void CNotificationItem::NormalizeIcon()
{
    int minLight=255;
    int maxLight=0;
    double colorDiff=0;

    if(Image.isNull())
    {
        return;
    }

    QRgb pixels[24][24];

    //-F- Checking parameters of icon
    for (int x = 0; x < 24; x++)
    {
        for (int y = 0; y < 24; y++)
        {
            QRgb pixel = Image.pixel(x,y);
            colorDiff += abs(qRed(pixel)-qGreen(pixel))+abs(qRed(pixel)-qBlue(pixel));
            int alpha = qAlpha(pixel);
            pixel=qRgba(qRed(pixel)*alpha/255+(255-alpha),qGreen(pixel)*alpha/255+(255-alpha),qBlue(pixel)*alpha/255+(255-alpha),alpha);
            pixels[x][y]=pixel;

            if(minLight>qGreen(pixel) && qAlpha(pixel)>128)
            {
                minLight=qGreen(pixel);
            }
            if(maxLight<qGreen(pixel) && qAlpha(pixel)>128)
            {
                maxLight=qGreen(pixel);
            }
        }
    }

    int meanDiff = (maxLight + minLight)/2;
    double scale = 200./(maxLight - minLight);

    //-F- If icon is grayscale image, make it more contrast
    if(minLight<maxLight && colorDiff < 24 * 24 * 2 * 8 && scale>1)
    {
        for (int x = 0; x < 24; x++)
        {
            for (int y = 0; y < 24; y++)
            {
                QRgb pixel = pixels[x][y];
                double alphaCorrection = qAlpha(pixel)>16 ? 255.0/qAlpha(pixel) : 1;
                Image.setPixel(x,y,
                               qRgba(((qRed(pixel)-meanDiff)*scale+150)*alphaCorrection,
                               ((qGreen(pixel)-meanDiff)*scale+150)*alphaCorrection,
                               ((qBlue(pixel)-meanDiff)*scale+150)*alphaCorrection,
                               qAlpha(pixel)));
            }
        }
    }
}
