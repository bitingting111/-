#include "mainserverwidget.h"
#include <QApplication>

MainServerWidget* pw;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainServerWidget w;
    pw = &w;
    w.show();

    return a.exec();
}
