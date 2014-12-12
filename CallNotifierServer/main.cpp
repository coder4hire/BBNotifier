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
#include <QApplication>
#include <qsharedmemory.h>
#include <qmessagebox.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory mem("CallNotifierSem");
    if (!mem.create(1024))
    {
        QMessageBox::warning(NULL,"BB Notifier","Another instance of this application is already running. Exiting...");
        return -1;
    }

    CMainWnd w;

    //-F- Start hidden
    //w.show();

    //mem.deleteLater();
    
    return a.exec();
}
