#include "tcpserver.h"
#include <tcpsocket.h>
#include "server.h"
#include "mainserverwidget.h"
#include <QDebug>
extern MainServerWidget* pw;

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{

    // 创建socket
    TcpSocket* socket = new TcpSocket(this);

    // 设置文件描述符
    socket->setSocketDescriptor(socketDescriptor);

    Server* s = Server::getServerInstance();
    s->addClient(socket);

    // 建立连接
    connect(socket, &TcpSocket::dealMessage, pw, &MainServerWidget::displayMessage);

}
