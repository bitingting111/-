#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>

class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    void sendMessage(char buff[], int len);
public slots:
    void readMessage();
signals:
    void sigDealMessage();


};

#endif // SERIALPORT_H
