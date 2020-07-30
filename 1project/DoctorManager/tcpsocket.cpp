#include "tcpsocket.h"
#include <QDebug>
#include <QHostAddress>

TcpSocket::TcpSocket(QObject *parent) : QTcpSocket(parent)
{
    // 建立信号槽连接
    connect(this, &TcpSocket::readyRead,
            this, &TcpSocket::receiveMessage);

    // 初始化
    length = 0;
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

// 接收数据
void TcpSocket::receiveMessage()
{    
    if (length == 0)
    {
        // 开始接收新的数据包
        buffer = new char[1024];  // 申请
        int nread = this->read(buffer, 1024);
        length += nread;
    }
    else
    {
        int nread = this->read(buffer+length, 1024-length);
        length += nread;
    }

    if (1024 == length)
    {
        // 数据包读取结束
        // 发送信号
        emit dealMessage(buffer);

        // 初始化，准备接收下一个数据包
        length = 0;

        if(this->bytesAvailable() > 0)
        {
            // 继续解析下一个包
            this->receiveMessage();
        }
    }

}
