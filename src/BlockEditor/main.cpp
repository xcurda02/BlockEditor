#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setGeometry(100,100,800,500);
    w.show();

    qInfo() << "starting";
    return a.exec();
}
