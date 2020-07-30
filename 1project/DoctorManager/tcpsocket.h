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

public slots:
    // 接收数据
    void receiveMessage();

signals:
    void dealMessage(char* buffer);

private:
    int length;  //成功接收的字节数4
    char* buffer;


};

#endif // TCPSOCKET_H
