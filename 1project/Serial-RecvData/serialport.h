#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    // 发送数据
    void sendMessage(QByteArray ba);
public slots:
    // 接收数据
    void readMessage();

signals:
    void sigDealMessage();


private:
    void rxDataHandle(char data);

private:
    unsigned char pkgType;     // 包类型ID
    unsigned char pkgDataHead;     // 头部
    unsigned char pkgDataBuf[10];  // 数据包最大为10个字节，解析前
    unsigned char pkgData[10];     // 解析后
    unsigned int pkgDataCnt;       // 接收的数据个数
    unsigned int ecg1, ecg2, ecg3;
    int status;           // 状态
    int dataLength;       // 数据长度

};

#endif // SERIALPORT_H
