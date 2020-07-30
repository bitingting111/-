#include "mainserverwidget.h"
#include "ui_mainserverwidget.h"
#include "server.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

MainServerWidget::MainServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainServerWidget)
{
    ui->setupUi(this);
    ecgWidget = new EcgWaveWidget(this);
    ecgWidget->move(50,200);
    ecgWidget->resize(600,200);

}

MainServerWidget::~MainServerWidget()
{
    delete ui;
}

// 自动连接
void MainServerWidget::on_startButton_clicked()
{
    // 获取服务器对象
    Server* ser = Server::getServerInstance();
    quint16 port = ui->portLineEdit->text().toUInt();
    ser->start(port);

    // 修改界面
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
}

void MainServerWidget::on_stopButton_clicked()
{
    // 获取服务器对象
    Server* ser = Server::getServerInstance();
    ser->stop();

    // 修改界面
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

}

unsigned short ecgData[500];
void MainServerWidget::displayMessage(char *buffer)
{
    char serial[24] = {0};
    memcpy(serial, buffer, 24);
    memcpy(ecgData, buffer+24, 1000);
    delete[] buffer;
    qDebug() << serial;

    // 0. 判断数据库是否已经添加

    // 1.加载数据库驱动
    QSqlDatabase db;
    if(QSqlDatabase::contains("wave_connection"))
    {
        db = QSqlDatabase::database("wave_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "wave_connection");
    }


    // 2. 设置数据库名
    db.setDatabaseName("D:/360Downloads/monitor-cloud.db");

    //3.打开数据库
    if(!db.open())
    {
        return;
    }

    //4.准备数据
    QByteArray ba;
    unsigned char ch;
    for(int i=0;i<500;i++)
    {
        ch = (ecgData[i] >> 8) & 0xff;
        ba.append(ch);
        ch = ecgData[i] & 0xff;
        ba.append(ch);
    }

    //5.执行插入语句
    QSqlQuery query(db);
    QString sql = "insert into waveData values(null,:serial,:waveData,datetime('now','localtime'));";
    query.prepare(sql);
    query.bindValue(":waveData",QVariant(ba));
    query.bindValue(":serial",QVariant(serial));
    if(query.exec())
    {
        qDebug() << "插入成功";
    }
    else
    {
        qDebug() << "插入失败";
    }

}
