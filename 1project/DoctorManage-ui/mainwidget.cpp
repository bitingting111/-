#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QSqlRelation>
#include <QSqlRelationalDelegate>
#include <QSqlError>
#include "patientmodel.h"
#include <QLineEdit>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    ui->patientButton->setMinimumHeight(30);
    ui->deviceButton->setMinimumHeight(30);
    ui->bindButton->setMinimumHeight(30);
    ui->realtimeButton->setMinimumHeight(30);
    ui->historyButton->setMinimumHeight(30);
    ui->ecgwid->resize(600,150);
    ui->ecgwid->setStyleSheet("background-color:black");



    // 1.加载数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    // 2.设置数据库名
    db.setDatabaseName("D:/360Downloads/monitor-cloud.db");
    // 3.打开数据库
    if (!db.open())
    {
        qDebug() << "打开失败";
    }

    // 4.创建模型对象
//    model = new QSqlQueryModel(this);
    patientComboBoxModel = new QSqlQueryModel(this);
    deviceComboBoxModel = new QSqlQueryModel(this);
    mode = new QSqlQueryModel(ui->deviceView); //建立一个Model
    lineEdit2= new QLineEdit(ui->lineEdit_2);

//    deviceView = new QTableView(this);












}

MainWidget::~MainWidget()
{
    delete ui;
}


void MainWidget::on_patientButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    model = new PatientModel(this, db);

    // 5.创建视图：ui->tableView

    // 6.关联模型与表格
    model->setQuery("select * from patient;", db);

    // 7.关联模型与视图
    ui->tableView->setModel(model);

    // 8.修改视图的显示方式
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID号"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("姓名"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("性别"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("年龄"));

}

void MainWidget::on_deviceButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->deviceView->setColumnWidth(2,150);




    mode->setQuery(QString("select * from device"),this->db) ;
    mode->setHeaderData(0,Qt::Horizontal,QObject::tr("dev_id"));
    mode->setHeaderData(1,Qt::Horizontal,QObject::tr("dev_serial"));
   // deviceView->setColumnWidth(2, 90);

   // deviceView->setColumnWidth(2,100);


    //mode->setQuery("select * from device;", db);
    ui->deviceView->setModel(mode);







}

void MainWidget::on_bindButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);

    ui->patientComboBox->setModel(patientComboBoxModel);
    ui->deviceCombox->setModel(deviceComboBoxModel);

    patientComboBoxModel->setQuery("select patientId from patient where patientId not in (select patientId from patient_device);", db);
    deviceComboBoxModel->setQuery("select dev_serial from device where dev_id not in (select devId from patient_device);", db);
    // 绑定列表显示
    patientDeviceModel = new QSqlRelationalTableModel(this,db);
    patientDeviceModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientDeviceModel->setTable("patient_device");  // 设置模型操作的表。
    patientDeviceModel->setRelation(1, QSqlRelation("patient", "patientId", "name"));
    patientDeviceModel->setRelation(2, QSqlRelation("device", "dev_id", "dev_serial"));
    patientDeviceModel->select();
    ui->patient_device_tableView->setModel(patientDeviceModel);
    // 设置代理
    ui->patient_device_tableView->setItemDelegate(new QSqlRelationalDelegate(ui->patient_device_tableView));
}


void MainWidget::on_realtimeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    //setWindowFlag(Qt::FramelessWindowHint);
    if ( iffir==true)
    {
        titleLabel = new QLabel("ECG");
        titleLabel->setStyleSheet("color:white");

        ecgWidget = new EcgWaveWidget;

        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(titleLabel);
        layout->addWidget(ecgWidget);
        layout->setStretch(0, 1);
        layout->setStretch(1, 5);
        ui->ecgwid->setLayout(layout);
        iffir=false;

    }







}

void MainWidget::on_historyButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);

}


// 绑定
void MainWidget::on_bindOpButton_clicked()
{
    // 病人id
    int patientId = ui->patientComboBox->currentText().toInt();


    // 设备id
    int devId;
    QString serial = ui->deviceCombox->currentText();
    QSqlQuery query(db);
    query.prepare("select dev_id from device where dev_serial = :serial;");
    query.bindValue(":serial", QVariant(serial));
    if (!query.exec())
    {
        qDebug() << "绑定失败！";
        QMessageBox::warning(this, "提示", "绑定失败", QMessageBox::Ok);
        return;
    }
    if (query.next())
    {
        devId = query.value(0).toInt();
    }

    // 绑定
    query.prepare("insert into patient_device values(null, :patientId, :devId);");
    query.bindValue(":patientId", patientId);
    query.bindValue(":devId", devId);
    if (!query.exec())
    {
        qDebug() << "绑定失败";
        return ;
    }
    qDebug() << "绑定成功";

    // 刷新绑定列表模型
    patientDeviceModel->select();
    // 刷新病人comboBox模型 // 刷新设备comboBox模型
    patientComboBoxModel->setQuery("select patientId from patient where patientId not in (select patientId from patient_device);", db);
    deviceComboBoxModel->setQuery("select dev_serial from device where dev_id not in (select devId from patient_device);", db);



}

void MainWidget::on_pushButton_4_clicked()
{
    patientDeviceModel->submitAll();
}

void MainWidget::on_pushButton_2_clicked()
{
    QSqlQuery query(db);
    if (!query.exec("insert into patient values(null, null, null, null);"))
    {
        qDebug() << "insert fail" << query.lastError().text();
        return ;
    }

    model->setQuery("select * from patient;", db);
}

void MainWidget::on_pushButton_3_clicked()
{
    QString content = ui->lineEdit->text();
    QString sql;
    if (content.isEmpty())
    {
        sql = "select * from patient;";
    }
    else
    {
        sql = QString("select * from patient where name='%1';").arg(content);
    }

    model->setQuery(sql, db);
}

void MainWidget::on_pushButton_clicked()
{
    QSqlQuery query(db);
    int curRow=ui->tableView->currentIndex().row();
    //int id=model->data(model->index(curRow,2)).toInt();
    QModelIndex index=model->index(curRow,0,QModelIndex());
    int id= index.data().toInt();
    qDebug() << id;
    query.prepare("delete from patient where patientId=:id;");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug() << "delete fail" << query.lastError().text();
        return ;
    }

    model->setQuery("select * from patient;", db);


}

void MainWidget::on_pushButton_5_clicked()
{
    QSqlQuery query(db);
    int curRow=ui->patient_device_tableView->currentIndex().row();
    //int id=model->data(model->index(curRow,2)).toInt();
    QModelIndex index=patientDeviceModel->index(curRow,0,QModelIndex());
    int id= index.data().toInt();
    qDebug() << id;
    query.prepare("delete from patient_device where id=:id;");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug() << "delete fail" << query.lastError().text();
        return ;
    }

    patientDeviceModel->select();


}

void MainWidget::on_pushButton_6_clicked()
{
    //bs = new Business(this);
    //bs->deviceLogin("dev-01");

    QString str=lineEdit2->text();


    QSqlQuery query(db);
    query.prepare("insert into device values(null, :devserial,  datetime('now','localtime'));");
    query.bindValue(":devserial",str);
    if (!query.exec())
    {
        qDebug() << "insert fail" << query.lastError().text();
        return ;
    }

    mode->setQuery("select * from device;", db);

}


void MainWidget::on_pushButton_7_clicked()
{
    QSqlQuery query(db);
    int curRow=ui->deviceView->currentIndex().row();
    //int id=model->data(model->index(curRow,2)).toInt();
    QModelIndex index=mode->index(curRow,0,QModelIndex());
    int id= index.data().toInt();
    qDebug() << id;
    query.prepare("delete from device where dev_id=:id;");
    query.bindValue(":id", id);
    if (!query.exec())
    {
        qDebug() << "delete fail" << query.lastError().text();
        return ;
    }
    mode->setQuery("select * from device;", db);

}
