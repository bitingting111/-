#include "server.h"

Server::Server(QObject *parent) : QObject(parent)
{
    pTcpServer = new TcpServer(this);

    // 建立新连接接入的信号槽连接
    connect(pTcpServer, &QTcpServer::newConnection,
            this, &Server::acceptNewConnection);
}

// 获取服务器
Server *Server::getServerInstance()
{
    static Server ser;
    return &ser;
}

void Server::start(quint16 port)
{
    // 启动监听
    if (!pTcpServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "监听失败:" << pTcpServer->errorString();
    }
    else
    {
        qDebug() << "监听成功！";
    }

}

void Server::stop()
{
    // 清空连接通信socket
    clients.clear();

    // 关闭服务器
    pTcpServer->close();

}

void Server::addClient(TcpSocket *c)
{
    clients.append(c);
}

QList<TcpSocket *> Server::getClients()
{
    return clients;
}

// 接入新连接
void Server::acceptNewConnection()
{
//    // 获取新接入的socket
//    QTcpSocket* socket = pTcpServer->nextPendingConnection();
//    TcpSocket* tcpSocket = new TcpSocket(this);
//    tcpSocket->setSocketDescriptor(socket->socketDescriptor());
//    this->addClient(tcpSocket);
}

