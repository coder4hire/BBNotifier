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

#include "HistoryItemDelegate.h"
#include "qpainter.h"
#include "NotificationItem.h"
#include "mainwnd.h"

QImage CHistoryItemDelegate::statusImages[2];

CHistoryItemDelegate::CHistoryItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    if(statusImages[0].size().width()==0)
    {
        statusImages[0] = QImage(":/Images/MissedCall.png");
        statusImages[1] = QImage(":/Images/OffhookCall.png");
    }
}

void CHistoryItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    CNotificationItem data(index.data().value<QString>());

    //-F- Preparing fonts, etc
    QFont bigFont = painter->font();
    int smallHeight = QFontMetrics(painter->font()).height();
    int bigHeight = smallHeight*1.5;
    bigFont.setPixelSize(bigHeight);
    bigFont.setBold(true);

    //-F- Drawing Item
    painter->drawText(option.rect.left()+65,option.rect.top()+bigHeight+smallHeight,data.Phone);
    painter->drawText(option.rect.left()+65,option.rect.top()+bigHeight+smallHeight*2,
                                  data.DateTime.toString("hh:mm:ss"));

    painter->setFont(bigFont);
    painter->drawText(option.rect.left()+5,option.rect.top()+bigHeight-2,data.Name);

    //-F- Painting Icon
    if(data.Type==CNotificationItem::RINGING_CALL || data.Type==CNotificationItem::OFFHOOK_CALL)
    {
        QImage& img = statusImages[(int)data.Type-(int)CNotificationItem::RINGING_CALL];
        painter->drawImage(30,option.rect.top()+bigHeight+2*smallHeight-img.height(),img);
    }

    painter->restore();

}

QSize CHistoryItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0,option.fontMetrics.height()*4.2);
}
