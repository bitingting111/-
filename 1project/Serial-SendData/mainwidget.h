#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "serialport.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void sendEcgPkg();

private:

    int ecgDataIndex;  // 索引
    SerialPort *com_;
};
#endif // MAINWIDGET_H
