#include "ecgwavewidget.h"
#include <QTimer>
#include <QPainter>
#include <QColor>
extern unsigned short ecgData[];

EcgWaveWidget::EcgWaveWidget(QWidget *parent) : QWidget(parent)
{
    this->resize(600,150);
    pix_ = new QPixmap(this->width(), this->height());
    pix_->fill(Qt::black);

    // 创建和启动定时器
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,  &EcgWaveWidget::refreshPixmap);
    timer->start(10);

}

// 更新图片上的数据
void EcgWaveWidget::refreshPixmap()
{
    static int index = 0;          // x
    static int ecgDataIndex = 0;   // y：取数组数据的索引

    // 1.将波形画到pixmap上
    // 1.1 定义painter
    QPainter painter(pix_);
    painter.setPen(QPen(QColor(0,255,0), 2));

    // 1.2擦除当前列
    painter.save();
    painter.setPen(Qt::black);
    painter.drawLine(index, 0, index,  pix_->height());
    painter.drawLine(index+1, 0, index+1,  pix_->height());
    painter.drawLine(index+2, 0, index+2,  pix_->height());
    painter.drawLine(index+3, 0, index+3,  pix_->height());
    painter.drawLine(index+4, 0, index+4,  pix_->height());
    painter.drawLine(index+5, 0, index+5,  pix_->height());
    painter.drawLine(index+6, 0, index+6,  pix_->height());
    painter.restore();

    // 1.3更新绘图参数，水平方向循环绘图
    index = index + 2;
    if (index > pix_->width())
    {
        index = 2;
    }

    QPoint lineStart;
    QPoint lineEnd;
    // 设置起始、结束的x坐标
    lineStart.setX(index-2);
    lineEnd.setX(index);

    // 设置起始点、结束点y坐标
    // 起始点的y值：
    lineStart.setY(round((double)pix_->height()/2 - (((double)ecgData[ecgDataIndex] - 2048)/600 * pix_->height()/2)));

    // 更新参数循环取数
    ecgDataIndex += 10;
    if (ecgDataIndex >= 500)
    {
        ecgDataIndex = 0;
    }

    // 设置结束点的y值
    lineEnd.setY(round((double)pix_->height()/2 - (((double)ecgData[ecgDataIndex] - 2048)/600 * pix_->height()/2)));

    // 1.4 绘制直线
    painter.drawLine(lineStart, lineEnd);

    // 2. 更新
    this->update();

}

// 重绘事件
void EcgWaveWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, pix_->width(), pix_->height(), *pix_);
}
