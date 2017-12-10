#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, bool fs):QGLWidget(parent)
{
    fullscreen = fs;
    //保存窗口是否为全屏的状态。

    setGeometry( 500, 300, 400, 300 );
    //设置窗口的位置，即左上角为(0,0)点，大小为640*480。


    //setCaption( "NeHe's OpenGL Framework" );
    //设置窗口的标题为“NeHe's OpenGL Framework”。

    if ( fullscreen )
        showFullScreen();
    //如果fullscreen为真，那么就全屏显示这个窗口。
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeGL()
{
    initializeOpenGLFunctions();//

    glShadeModel( GL_SMOOTH );
    //这一行启用smooth shading(阴影平滑)。阴影平滑通过多边形精细的混合色彩，并对外部光进行平滑。我将在另一个教程中更详细的解释阴影平滑。

    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    //这一行设置清除屏幕时所用的颜色。如果您对色彩的工作原理不清楚的话，我快速解释一下。色彩值的范围从0.0到1.0。0.0代表最黑的情况，1.0就是最亮的情况。glClearColor后的第一个参数是红色,第二个是绿色，第三个是蓝色。最大值也是1.0，代表特定颜色分量的最亮情况。最后一个参数是Alpha值。当它用来清除屏幕的时候，我们不用关心第四个数字。现在让它为0.0。我会用另一个教程来解释这个参数。
    //通过混合三种原色（红、绿、蓝），您可以得到不同的色彩。希望您在学校里学过这些。因此，当您使用glClearColor(0.0, 0.0, 1.0, 0.0 )，您将用亮蓝色来清除屏幕。如果您用glClearColor(0.5, 0.0, 0.0, 0.0 )的话，您将使用中红色来清除屏幕。不是最亮(1.0)，也不是最暗 (0.0)。要得到白色背景，您应该将所有的颜色设成最亮(1.0)。要黑色背景的话，您该将所有的颜色设为最暗(0.0)。

    glClearDepth( 1.0 );
    //设置深度缓存。

    glEnable( GL_DEPTH_TEST );
    //启用深度测试。

    glDepthFunc( GL_LEQUAL );
    //所作深度测试的类型。
    //上面这三行必须做的是关于depth buffer（深度缓存）的。将深度缓存设想为屏幕后面的层。深度缓存不断的对物体进入屏幕内部有多深进行跟踪。我们本节的程序其实没有真正使用深度缓存，但几乎所有在屏幕上显示3D场景OpenGL程序都使用深度缓存。它的排序决定那个物体先画。这样您就不会将一个圆形后面的正方形画到圆形上来。深度缓存是OpenGL十分重要的部分。

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //真正精细的透视修正。这一行告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点。
}

void MainWindow::resizeGL(int width,int height)
{
    if ( height == 0 )
    {
        height = 1;
    }
    //防止height为0。

    glViewport( 0, 0, (GLint)width, (GLint)height );
    //重置当前的视口（Viewport）。

    glMatrixMode( GL_PROJECTION );
    //选择投影矩阵。

    glLoadIdentity();
    //重置投影矩阵。

    //gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
    //建立透视投影矩阵。

    glMatrixMode( GL_MODELVIEW );
    //选择模型观察矩阵。

    glLoadIdentity();
    //重置模型观察矩阵。
}
void MainWindow::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //清楚屏幕和深度缓存。

    glLoadIdentity();
    //重置当前的模型观察矩阵。
    //当您调用glLoadIdentity()之后，您实际上将当前点移到了屏幕中心，X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外。OpenGL屏幕中心的坐标值是X和Y轴上的0.0点。中心左面的坐标值是负值，右面是正值。移向屏幕顶端是正值，移向屏幕底端是负值。移入屏幕深处是负值，移出屏幕则是正值。

    glTranslatef( -1.5,  0.0, -6.0 );
    //glTranslatef(x, y, z)沿着 X, Y 和 Z 轴移动。根据前面的次序，下面的代码沿着X轴左移1.5个单位，Y轴不动(0.0)，最后移入屏幕6.0个单位。注意在glTranslatef(x, y, z)中当您移动的时候，您并不是相对屏幕中心移动，而是相对与当前所在的屏幕位置。
    //现在我们已经移到了屏幕的左半部分，并且将视图推入屏幕背后足够的距离以便我们可以看见全部的场景－创建三角形。

    glBegin( GL_TRIANGLES );
    //开始绘制三角形。
    //glBegin(GL_TRIANGLES)的意思是开始绘制三角形，glEnd() 告诉OpenGL三角形已经创建好了。通常您会需要画3个顶点，可以使用GL_TRIANGLES。在绝大多数的显卡上，绘制三角形是相当快速的。如果要画四个顶点，使用GL_QUADS的话会更方便。但据我所知，绝大多数的显卡都使用三角形来为对象着色。最后，如果您想要画更多的顶点时，可以使用GL_POLYGON。
    //本节的简单示例中，我们只画一个三角形。如果要画第二个三角形的话，可以在这三点之后，再加三行代码(3点)。所有六点代码都应包含在glBegin(GL_TRIANGLES)和glEnd()之间。在他们之间再不会有多余的点出现，也就是说，(GL_TRIANGLES)和glEnd()之间的点都是以三点为一个集合的。这同样适用于四边形。如果您知道实在绘制四边形的话，您必须在第一个四点之后，再加上四点为一个集合的点组。另一方面，多边形可以由任意个顶点，(GL_POLYGON)不在乎glBegin(GL_TRIANGLES)和glEnd()之间有多少行代码。

    glVertex3f(  0.0,  1.0,  0.0 );
    //上顶点。
    //glBegin之后的第一行设置了多边形的第一个顶点，glVertex 的第一个参数是X坐标，然后依次是Y坐标和Z坐标。第一个点是上顶点，然后是左下顶点和右下顶点。glEnd()告诉OpenGL没有其他点了。这样将显示一个填充的三角形。
    //CKer注：这里要注意的是存在两种不同的坐标变换方式，glTranslatef(x, y, z)中的x, y, z是相对与您当前所在点的位移，但glVertex(x,y,z)是相对于glTranslatef(x, y, z)移动后的新原点的位移。因而这里可以认为glTranslate移动的是坐标原点，glVertex中的点是相对最新的坐标原点的坐标值。

    glVertex3f( -1.0, -1.0,  0.0 );
    //左下顶点。

    glVertex3f(  1.0, -1.0,  0.0 );
    //右下顶点。

    glEnd();
    //三角形绘制结束。

    glTranslatef(  3.0,  0.0,  0.0 );
    //在屏幕的左半部分画完三角形后，我们要移到右半部分来画正方形。为此要再次使用glTranslate。这次右移，所以X坐标值为正值。因为前面左移了1.5个单位，这次要先向右移回屏幕中心(1.5个单位)，再向右移动1.5个单位。总共要向右移3.0个单位。

    glBegin( GL_QUADS );
    //开始绘制四边形。

    glVertex3f( -1.0,  1.0,  0.0 );
    //左上顶点。

    glVertex3f(  1.0,  1.0,  0.0 );
    //右上顶点。

    glVertex3f(  1.0, -1.0,  0.0 );
    //右下顶点。

    glVertex3f( -1.0, -1.0,  0.0 );
    //左下顶点。

    glEnd();
    //四边形绘制结束。
}
//这个函数中包括了所有的绘图代码。任何您所想在屏幕上显示的东东都将在此段代码中出现。以后的每个教程中我都会在例程的此处增加新的代码。
//如果您对OpenGL已经有所了解的话，您可以在 glLoadIdentity()调用之后，函数返回之前，试着添加一些OpenGL代码来创建基本的形。
//如果您是OpenGL新手，等着我的下个教程。目前我们所作的全部就是将屏幕清除成我们前面所决定的颜色，清除深度缓存并且重置场景。
//我们仍没有绘制任何东东。


void MainWindow::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
    case Qt::Key_F2:
        fullscreen = !fullscreen;
        if ( fullscreen )
        {
            showFullScreen();
        }
        else
        {
            showNormal();
            setGeometry( 0, 0, 640, 480 );
        }
        updateGL();
        break;
        //如果按下了F2键，那么屏幕是否全屏的状态就切换一次。然后再根据需要，显示所要的全屏窗口或者普通窗口。

    case Qt::Key_Escape:
        close();
    }
    //如果按下了Escape键，程序退出。
}


