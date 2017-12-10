#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    //w.resize(800,600);

    bool fs = false;
    //我们把这个布尔型变量的初始值设置为false。

    QApplication a(argc,argv);
    //每一个Qt应用程序都使用QApplication类。

//    switch( QMessageBox::information( 0,
//                                      "Start FullScreen?",
//                                      "Would You Like To Run In Fullscreen Mode?",
//                                      QMessageBox::Yes,
//                                      QMessageBox::No | QMessageBox::Default ) )
//    {
//    case QMessageBox::Yes:
//        fs = true;
//        break;
//    case QMessageBox::No:
//        fs = false;
//        break;
//    }
    //这里弹出一个消息对话框，让用户选择是否使用全屏模式。

    MainWindow w( 0, fs );
    //创建一个NeHeWidget对象。

    //a.setMainWidget( &w );
    //设置应用程序的主窗口部件为w。

    w.show();
    //显示w。

    return a.exec();
    //程序返回。
}
