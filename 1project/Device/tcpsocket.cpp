#include "tcpsocket.h"
#include <QDebug>
#include <QHostAddress>

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    // 建立信号槽连接
    connect(this, &TcpSocket::readyRead,
            this, &TcpSocket::receiveMessage);
}

// 向服务器发起连接
bool TcpSocket::connectToServer(QString strIp, quint16 port)
{
    bool ret = true;

    // 中断当前socket的一切连接，并且复位socket
    this->abort();

    this->connectToHost(QHostAddress(strIp), port);

    ret = this->waitForConnected();

    return ret;

}


// 发送数据
void TcpSocket::sendMessage(char* buffer)
{
    int nwrite = this->write(buffer, 1024);
    if (nwrite != 1024)
    {
        qDebug() << "发送失败！";
    }
    else
    {
        qDebug() << "发送成功！";
    }

}


// 接收数据
void TcpSocket::receiveMessage()
{
    QByteArray ba;
    ba.resize(this->bytesAvailable());

    int nread = this->read(ba.data(), ba.size());

    if (-1 == nread)
    {
        qDebug() << "receive fail!";
    }
    else
    {
        qDebug() << "receive sucess:" << nread;
        emit dealMessage(QString::fromUtf8(ba));
    }

}
