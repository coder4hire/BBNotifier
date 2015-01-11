#ifndef MAINWND_H
#define MAINWND_H

#include <QDialog>
#include <QMenu>
#include "qsystemtrayicon.h"
#include "ListenerSocket.h"
#include "NotificationItem.h"
#include "qsound.h"
#include "qtimer.h"

//#define USE_MP3

#ifdef USE_MP3
    #include "qmediaplayer.h"
#endif

namespace Ui {
class CMainWnd;
}

class CMainWnd : public QDialog
{
    Q_OBJECT
    
public:
    explicit CMainWnd(QWidget *parent = 0);
    void ShowMessage(const QString& name,const QString& msg);
    void OnNewCall(const CNotificationItem& data);

    ~CMainWnd();

protected:
    void PrepareTrayIcon();
    virtual void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QResizeEvent* ev);

    void AddNewCall(const CNotificationItem& data);
    void MarkCallAccepted(const CNotificationItem& data);    
    void RefreshTrayIconStatus();

//    static QIcon statusIcons[2];
    static QIcon windowIcons[2];

#ifdef USE_MP3
    QMediaPlayer player;
#else
    QSound sound;
#endif
    QSound soundNotif;

private slots:
    void IconActivated(QSystemTrayIcon::ActivationReason reason);
    void MessageClicked();
    void ShowSettings();
    void OnSettingsChanged();
    void ShowAndActivate();

    void on_btnClearHistory_clicked();
    void OnTimer();

private:
    Ui::CMainWnd *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QAction* settingsAction;
    QAction* showAction;
    QAction* quitAction;

    QTimer trayTimer;

    CListenerSocket listener;

    virtual void showEvent(QShowEvent* ev);
};

#endif // MAINWND_H
