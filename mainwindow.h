#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLFunctions>
namespace Ui {
class MainWindow;
}

class MainWindow :public QGLWidget,protected QOpenGLFunctions //public QOpenGLWindow,
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0, bool fs = false );
    ~MainWindow();

    void initializeGL();
    void resizeGL(int width,int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
    bool fullscreen;
};

#endif // MAINWINDOW_H
