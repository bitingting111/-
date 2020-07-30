#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <business.h>
#include <QTableView>
#include <QLabel>
#include <ecgwavewidget.h>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


private slots:
    void on_patientButton_clicked();

    void on_deviceButton_clicked();

    void on_realtimeButton_clicked();

    void on_historyButton_clicked();

    void on_bindButton_clicked();

    void on_bindOpButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWidget *ui;
    QSqlDatabase db;
    QSqlQueryModel* model;
    Business *bs;
    QSqlQueryModel *patientComboBoxModel;
    QSqlQueryModel *deviceComboBoxModel;
    QSqlRelationalTableModel* patientDeviceModel;
    QTableView* deviceView;
    QSqlQueryModel *mode;
    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QLabel* titleLabel;
    EcgWaveWidget* ecgWidget;
        bool iffir=true;

};

#endif // MAINWIDGET_H
