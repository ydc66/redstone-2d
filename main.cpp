#include "mainwindow.h"

#include <QApplication>

// 强制保留静态库中的元件注册逻辑（防止链接器裁剪静态初始化）
#include "components/registration/ComponentRegistrator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    registerComponents();   // 显式触发元件注册（函数引用防链接器裁剪静态初始化）


    MainWindow w;
    w.show();
    return QApplication::exec();
}
