#ifndef BUSINESS_H
#define BUSINESS_H

#include <QObject>

class Business : public QObject
{
    Q_OBJECT
public:
    explicit Business(QObject *parent = nullptr);

    void deviceLogin(QString devSerial);
};

#endif // BUSINESS_H
