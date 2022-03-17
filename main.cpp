#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //程序仅可以运行一个实例
    QSharedMemory shared("学生违纪信息管理");
    if (shared.attach())
    {
            return  EXIT_SUCCESS;
    }
    shared.create(1);

    MainWindow w;
    w.show();
    return a.exec();
}
