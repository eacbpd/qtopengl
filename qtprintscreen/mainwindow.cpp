#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QScreen>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::onprintscreen);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::change);
    connect(this,&MainWindow::send,this,&MainWindow::onprintscreen);

    //ui->listWidget->setVisible(false);
    //ui->listWidget->addItem("asdfasdf");
}

MainWindow::~MainWindow()
{
    emit send(false);
}

void MainWindow::onprintscreen(bool b)
{
    QScreen *screen =QApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(QApplication::desktop()->winId());
    //QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());

    ui->frame->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    ui->frame->setPalette(palette);
    ui->frame->show();
    pixmap.save("/home/e/桌面/haha.png","PNG");
}

void MainWindow::change()
{
    //ui->frame->setVisible(!(ui->frame->isVisible()));
    //ui->listWidget->setVisible(!(ui->listWidget->isVisible()));
}
