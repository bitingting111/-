#ifndef HEADER_H
#define HEADER_H
#include <QString>
#include <QDateTime>

typedef struct tagDev
{
    int devId;
    QString devSerial;
    QDateTime refresh;
} Device_t;

#endif // HEADER_H
