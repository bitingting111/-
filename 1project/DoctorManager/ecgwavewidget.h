#ifndef ECGWAVEWIDGET_H
#define ECGWAVEWIDGET_H

#include <QWidget>
#include <QPixmap>

class EcgWaveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EcgWaveWidget(QWidget *parent = nullptr);

public:
    void refreshPixmap();

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    QPixmap *pix_;

};

#endif // ECGWAVEWIDGET_H
