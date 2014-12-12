#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <qtimer.h>
#include <qudpsocket.h>
#include "qsettings.h"
#include "AESWrapper.h"

namespace Ui {
class CSettingsDialog;
}

class CSettingsDialog : public QDialog
{
    Q_OBJECT

protected:
    explicit CSettingsDialog(QWidget *parent = 0);
    virtual void showEvent(QShowEvent* ev);
    virtual void closeEvent(QCloseEvent* ev);

    QTimer timer;
    QUdpSocket udpSocket;

public:
    ~CSettingsDialog();

    int GetPort(){return TCPPort;}
    AESKey GetAesKey(){return aesKey;}
    static CSettingsDialog* GetInstance(){return pInst? pInst : (pInst=new CSettingsDialog);}
    
private slots:
    void on_btnGen_clicked();
    void on_buttonBox_accepted();
    void OnTimer();

signals:
    void OnOK();

private:
    Ui::CSettingsDialog *ui;
    static CSettingsDialog *pInst;
    QSettings settingsStorage;

    int TCPPort;
    AESKey aesKey;
};

#endif // SETTINGSDIALOG_H
