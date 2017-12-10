#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
signals:
    void send(bool b);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void onprintscreen(bool b);
    void change();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
