#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <qtcpsocket.h>
#include <qreadwritelock.h>

class CServerSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CServerSocket(QObject *parent = 0);
    static QString GetLastMessageStatus();
    
signals:
   void MessageReady(QString msg);
    
public slots:
    
protected slots:
    void OnReadyRead();
    void OnDisconnected();

protected:
    static void SetLastMessageStatus(QString str);
    static QReadWriteLock lock;
    static QString LastMessageStatus;
};

#endif // SERVERSOCKET_H
