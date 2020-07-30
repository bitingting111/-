#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTimer>
#include "tcpsocket.h"

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
    void on_connectButton_clicked();

    void sendEcgWava();
    void displayMessage(QString content);

private:
    Ui::MainWidget *ui;
    TcpSocket* socket;
    QTimer* timer;
};

#endif // MAINWIDGET_H
