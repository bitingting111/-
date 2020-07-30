#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <Header.h>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    static Database* getDatabaseInstance();

    // 根据序列号查询
    bool queryDeviceBySerial(QString serial);
    // 插入设备
    bool insertDevice(QString serial);

    // 更新在线时间
    bool updateDeviceRefresh(QString serial);



private:
    QSqlDatabase db;
};

#endif // DATABASE_H
