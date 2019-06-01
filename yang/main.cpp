#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(760,500);    //设置这个窗口部件的大小，防止它变大或者变小

    w.setWindowFlags(Qt::WindowCloseButtonHint);//窗口上方只有一个关闭按钮

    w.show();

    return a.exec();
}
