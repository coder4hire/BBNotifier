#ifndef LISTENERSOCKET_H
#define LISTENERSOCKET_H

#include <qtcpserver.h>

class CServerSocket;
class CMainWnd;

class CListenerSocket : public QTcpServer
{
    Q_OBJECT
public:
    explicit CListenerSocket(QObject *parent = 0);
    void RestartListening();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

    std::vector<CServerSocket*> serverSockets;
    CMainWnd* GetMainWnd();

signals:
    
public slots:

protected slots:
    void OnIncomingMessage(QString msg);
};

#endif // LISTENERSOCKET_H
