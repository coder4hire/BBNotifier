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

#include "NotificationItemDelegate.h"
#include "qpainter.h"
#include "NotificationItem.h"
#include "mainwnd.h"
#include "qlistwidget.h"
#include "qscrollbar.h"

CNotificationItemDelegate::CNotificationItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void CNotificationItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    CNotificationItem data(index.data().value<QString>());

    //-F- Preparing fonts, etc
    QFont smallFont = painter->font();
    QFont bigFont = painter->font();
    int smallHeight = QFontMetrics(painter->font()).height();
    int bigHeight = smallHeight*1.1;
    bigFont.setPixelSize(bigHeight);
    smallFont.setItalic(true);


    //-F- Drawing Item
    painter->setFont(bigFont);
    const QRect textRect = painter->boundingRect(option.rect.left()+5,option.rect.top(),option.rect.width()-10,100,Qt::TextWordWrap,data.Data);
    painter->drawText(textRect,Qt::TextWordWrap,data.Data);

    painter->setFont(smallFont);
    painter->drawText(QRect(option.rect.left(),option.rect.top()+textRect.height(),option.rect.width()-8,smallHeight),Qt::AlignRight,data.Phone);

    //-F- Painting Icon
    //    if(data.Type==CNotificationItem::RINGING_CALL || data.Type==CNotificationItem::OFFHOOK_CALL)
    //    {
    //        QImage& img = statusImages[(int)data.Type-(int)CNotificationItem::RINGING_CALL];
    //        painter->drawImage(30,option.rect.top()+bigHeight+2*smallHeight-img.height(),img);
    //    }

    painter->restore();

}

QSize CNotificationItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    CNotificationItem data(index.data().value<QString>());

    QFont bigFont = option.font;
    int smallHeight = QFontMetrics(option.font).height();
    int bigHeight = smallHeight*1.1;
    bigFont.setPixelSize(bigHeight);

    QRect rect= option.widget->rect();
    QScrollBar* bar=((QListWidget*)option.widget)->verticalScrollBar();
    int scrollWidth = bar->isVisible() ? bar->width() : 0;

    int height = QFontMetrics(bigFont).boundingRect(5,0,
                                                    rect.width()-10-scrollWidth,100,
                                                    Qt::TextWordWrap,data.Data).height()+smallHeight+5;
    //-F- Drawing Item
    return QSize(0,height);
}
