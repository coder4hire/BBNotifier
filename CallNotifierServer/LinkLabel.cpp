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

#include "LinkLabel.h"
#include "qdesktopservices.h"
#include "qstyle.h"

#define BLINK_START 10

QLinkLabel::QLinkLabel(QWidget *parent) :
    QLabel(parent)
{
    blinkingCounter=-1;
    connect(&timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer.start(5000);

    QPalette palette;
    palette.setColor(QPalette::WindowText,QColor(0,0,255));

    this->setPalette(palette);
    this->update();
}

void QLinkLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton)
    {
        QDesktopServices::openUrl(QUrl(URL));
    }
}

void QLinkLabel::OnTimer()
{
    if(isVisible())
    {
        if(blinkingCounter==-1)
        {
            if(rand()<RAND_MAX/2)
            {
                blinkingCounter=BLINK_START;
                timer.setInterval(100);
            }

        }
        else
        {
            blinkingCounter--;

            if(blinkingCounter>=0)
            {
                QPalette palette;
                palette.setColor(QPalette::WindowText, QColor(255*blinkingCounter/BLINK_START,255*blinkingCounter/BLINK_START,255));

                this->setPalette(palette);
                this->update();
            }

            if(blinkingCounter==0)
            {
                blinkingCounter=-1;
                timer.setInterval(5000);
            }
        }
    }
}
