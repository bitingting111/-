#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

    // 连接到服务器
    bool connectToServer(QString strIp, quint16 port);

    // 发送数据
    void sendMessage(char *buffer);

public slots:
    // 接收数据
    void receiveMessage();

signals:
    void dealMessage(QString content);


};

#endif // TCPSOCKET_H
