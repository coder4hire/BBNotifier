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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include "AESWrapper.h"
#include "qmessagebox.h"
#include "qdesktopservices.h"
#include "qnetworkinterface.h"
#include "qdir.h"
#include "qlist.h"
#include "qhostaddress.h"
#include "qhostinfo.h"
#include "qnetworkinterface.h"

#include "ServerSocket.h"


CSettingsDialog* CSettingsDialog::pInst=NULL;

CSettingsDialog::CSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSettingsDialog),
    settingsStorage("GSS","BBNotifier")
{
    this->setAttribute(Qt::WA_QuitOnClose,false);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    shouldShowCallPopup=settingsStorage.value("showCallPopup",true).toBool();
    shouldShowNotificationPopup=settingsStorage.value("showNotifPopup",true).toBool();
    maxItemsCount=settingsStorage.value("maxItemsCount",20).toInt();

    TCPPort = settingsStorage.value("port",8083).toInt();
    QStringList baKey=settingsStorage.value("key").toStringList();
    for(int i=0;i<baKey.size();i++)
    {
        aesKey.Data[i]=(unsigned char)baKey[i].toInt();
    }

    connect(&timer,SIGNAL(timeout()),this,SLOT(OnTimer()));

    ui->setupUi(this);
    setFixedSize(width(),height());

    ui->linkLabel->URL = "https://play.google.com/store/apps/details?id=com.gss.myapplication";
}

void CSettingsDialog::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev);
    ui->edtPort->setValue(TCPPort);
    ui->edtPass->setText(aesKey.ToString());

    //-F- Run at startup settings
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QStringList list = settings.allKeys();
    bool isRunningAtStartup = settings.value("BBNotifier").isValid();
    ui->chkStartup->setChecked(isRunningAtStartup);

    ui->chkShowCallPopups->setChecked(shouldShowCallPopup);
    ui->chkShowNotificationPopup->setChecked(shouldShowNotificationPopup);
    ui->edtListLength->setValue(maxItemsCount);

    //-F_ Start broadcasting
    timer.start(5000);
}

void CSettingsDialog::done(int r)
{
    timer.stop();
    QDialog::done(r);
}

CSettingsDialog::~CSettingsDialog()
{
    pInst=NULL;
    delete ui;
}

void CSettingsDialog::on_btnGen_clicked()
{
    if(QMessageBox::question(this,"Generating new key","Are you sure about generating new key ?\nPrevious key will be lost and could not be restored.")==QMessageBox::Yes)
    {
        aesKey=CAESWrapper::GenKey();
        ui->edtPass->setText(aesKey.ToString());
    }
}

void CSettingsDialog::on_buttonBox_accepted()
{
    TCPPort=ui->edtPort->value();
    shouldShowCallPopup = ui->chkShowCallPopups->isChecked();
    shouldShowNotificationPopup = ui->chkShowNotificationPopup->isChecked();
    maxItemsCount = ui->edtListLength->value();

    settingsStorage.setValue("port",TCPPort);
    settingsStorage.setValue("key",aesKey.ToQVList());
    settingsStorage.setValue("showCallPopup",shouldShowCallPopup);
    settingsStorage.setValue("showNotifPopup",shouldShowNotificationPopup);
    settingsStorage.setValue("maxItemsCount",maxItemsCount);

    //-F- Put this program to startup section of registry
    QSettings settings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if(ui->chkStartup->isChecked())
    {
        QString path = QString("\"")+QDir::toNativeSeparators(QApplication::applicationFilePath())+"\"";
        settings.setValue("BBNotifier",path);
    }
    else
    {
        settings.remove("BBNotifier");
    }

    OnOK();
}

void CSettingsDialog::OnTimer()
{
    QString str;
    str.sprintf("BBNotifierWelcome:%05d:",ui->edtPort->value());
    str+=QHostInfo::localHostName();

    //-F- Sending broadcasts to each interface separately (to avoid WIndows bugs).
    // And another one to "Total broadcast", just in case :)
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    for(QList<QNetworkInterface>::Iterator it = list.begin();it!=list.end();it++)
    {
        QList<QNetworkAddressEntry> addrList = it->addressEntries();
        for(QList<QNetworkAddressEntry>::Iterator itAddr = addrList.begin();itAddr!=addrList.end();itAddr++)
        udpSocket.writeDatagram(str.toLatin1(),itAddr->broadcast(),0xBBBB);
    }
    udpSocket.writeDatagram(str.toLatin1(),QHostAddress(QHostAddress::Broadcast),0xBBBB);

    //-F- Show last packet status
    ui->edtDiagnostics->setPlainText(CServerSocket::GetLastMessageStatus());
}
