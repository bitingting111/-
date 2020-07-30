#include "business.h"
#include "database.h"
#include <QDebug>

Business::Business(QObject *parent) : QObject(parent)
{

}

// 设备登录
void Business::deviceLogin(QString devSerial)
{
    // 查询当前设备是否存在
    Database* db = Database::getDatabaseInstance();
    bool ret = db->queryDeviceBySerial(devSerial);

    // 数据库打开失败、查询失败可以做其他处理（todo）
    if (!ret)
    {
        // 不存在，往数据插入设备数据
        db->insertDevice(devSerial);
        qDebug() << "登录成功1";
    }
    else
    {
        // 存在，更新在线时间，登录成功
        db->updateDeviceRefresh(devSerial);
        qDebug() << "登录成功2";
    }

}

