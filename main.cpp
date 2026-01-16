#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序信息
    a.setApplicationName("社区医疗管理系统");
    a.setOrganizationName("Qt课程设计");
    a.setApplicationVersion("1.0.0");

    // 设置Fusion样式（可选）
    // a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();

    return a.exec();
}
