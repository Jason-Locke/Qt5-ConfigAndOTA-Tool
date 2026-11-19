#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("终端产品升级小助手 V1.0.3");//修改上位机左上角名称
    w.setFixedSize(921,930);//固定主窗口大小
    w.show();
    return a.exec();
}
