#include "mainwidget.h"
#include "serialport.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
   SerialPort* com = new SerialPort(this);

}

MainWidget::~MainWidget()
{
}

