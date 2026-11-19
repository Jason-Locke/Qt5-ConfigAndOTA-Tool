#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("快速配置小助手 V2.1.1.6");//修改上位机左上角名称
    w.setFixedSize(921,935);//固定主窗口大小
    w.show();
    return a.exec();
}
