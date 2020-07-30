#include "serialport.h"
#include <QDebug>


SerialPort::SerialPort(QObject *parent) : QSerialPort(parent)
{
    // 建立连接
    connect(this, &SerialPort::readyRead,
            this, &SerialPort::readMessage);

    // 设置串口名称（后面可以扩展）
    this->setPortName("COM1");

    // 尝试打开串口
    if (!this->open(QIODevice::ReadWrite))
    {
        qDebug() << "open fail";
        return ;
    }

    // 打开成功，参数配置
    // 波特率：115200
    this->setBaudRate(QSerialPort::Baud115200);
    // 数据位
    this->setDataBits(QSerialPort::Data8);
    // 流控制=无
    this->setFlowControl(QSerialPort::NoFlowControl);
    // 校验位
    this->setParity(QSerialPort::NoParity);
    // 停止位
    this->setStopBits(QSerialPort::OneStop);
}

// 发送数据
void SerialPort::sendMessage(QByteArray ba)
{
    this->write(ba.data(), ba.size());
}


// 接收数据
void SerialPort::readMessage()
{
    QByteArray data;

    // 读取串口数据
    data = this->read(1);
    rxDataHandle(data[0]);
    if (status == 4)
    {
        // 发信号通知画波形
    }
    if (this->bytesAvailable()>0)
    {
        readMessage();
    }
}

// 解析
void SerialPort::rxDataHandle(char data)
{
    // 状态机
    // 状态0： 空闲，接收到包类型ID 0x08后进入状态1
    // 状态1： 已经接收到数据包类型ID，接收数据包头，进入下一个状态。
    // 状态2： 数据接收状态， 用数据头中相应位填补接收数据的高位，并且缓存数据，
    //        接收满特定长度的数据后，进入下一个状态
    // 状态3： 数据校验，接收校验字节，并且进行校验
    // 状态4： 结束

    char dataTmp;
    data = data&0xff;
    int datamask = data&0x80;
    if (datamask == 0x00)
    {
        // 最高位为0，该数据是包类型ID
        this->status = 0;
    }
    switch (this->status)
    {
    case 0:
        if (data == 0x08) // ECG
        {
            this->status = 1;       // 状态切换
            this->pkgType = data;   // 保存包类型
            this->pkgDataHead = 0;  // 清空，为接收头部字节做准备
            this->dataLength = 7;   // 数据长度
        }
        else
        {
            qDebug() << "unknow pkgType";
        }
        break;
    case 1:
        this->pkgDataHead = data;
        this->pkgDataCnt = 0;
        this->status = 2;
        break;
    case 2:   // 接收数据
        dataTmp = data & 0x7f;  // 保留数据的低7位

        // 第一个数据的最高位，在数据头的最低位中
        dataTmp = dataTmp | (((this->pkgDataHead >> this->pkgDataCnt) & 0x1) << 7);
        this->pkgData[pkgDataCnt] = dataTmp; // 保存解析后的数据
        this->pkgDataBuf[pkgDataCnt] = data;  // 保存解析前的数据
        pkgDataCnt += 1;

        // 根据协议我们知道数据数据长度为7
        if (pkgDataCnt >= 7)
        {
            status = 3;
        }
        break;
    case 3:
        // 校验

        // 得到ecg数据
        ecg1 = this->pkgData[0];
        ecg1 = (ecg1 << 8);
        ecg1 = ecg1 | this->pkgData[1];

        ecg2 = this->pkgData[2];
        ecg2 = (ecg2 << 8) | this->pkgData[3];

        ecg3 = this->pkgData[4];
        ecg3 = (ecg3 << 8) | this->pkgData[5];
        // 测试
        qDebug() << ecg1;
        qDebug() << ecg2;
        qDebug() << ecg3;

        this->status = 4;
        break;
    default:
        break;
    }
}
