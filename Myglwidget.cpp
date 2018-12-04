#include "Myglwidget.h"
#include <GL/glu.h>
#include <QDebug>
#include <QMouseEvent>
#include <fstream>
#include <string>
#include <assert.h>
using namespace std;


MyGLWidget::MyGLWidget(QWidget *parent):QGLWidget(parent) {
    angle_x = 0.0f;
    angle_y = 0.0f;
    lastPoint = QPoint(0,0);
    currentPoint = QPoint(0,0);
    distance = -4.0;
}

//初始化GL窗口
void MyGLWidget::initializeGL() {
    glClearColor(0,0,0,0);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

}

//缩放GL窗口
void MyGLWidget::resizeGL(int w, int h) {
    glViewport(0,0,(GLint)w,(GLint)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
//加载OFF文件
void MyGLWidget::loadOffFile(QString path) {
    string filepath = path.toLocal8Bit().constData();
    ifstream fin(filepath,ios::in);
    string fileHeader;
    getline(fin,fileHeader);
    //qDebug() << fileHeader;
    assert(fileHeader=="OFF");
    int numOfPoints,numOfFaces,numOfLines;
    fin>>numOfPoints>>numOfFaces>>numOfLines;
    qDebug() << "numofpoints: "<<numOfPoints;
    qDebug() << "numoffaces: "<<numOfFaces;

    float x,y,z;
    for(int i = 0;i<numOfPoints;i++) {
        fin>>x>>y>>z;
        Point3D point((GLfloat)x,(GLfloat)y,(GLfloat)z);
        this->points.push_back(point);
    }

    for(int i = 0;i<points.size();i++) {
        //qDebug()<<i <<": "<< points[i].x<<" "<<points[i].y<<" "<<points[i].z;
    }

    for(int i = 0;i<numOfFaces;i++) {
        int faceSize;
        fin>>faceSize;
        QVector<int> face;
        int index;
        for(int j = 0;j<faceSize;j++) {
            fin>>index;
            face.push_back(index);
        }
        this->faces.push_back(face);
    }

    for(int i = 0; i < faces.size();i++ ) {
        //qDebug() <<i<<": "<<faces[i][0] << " "<<faces[i][1]<<" "<<faces[i][2];
    }
}

//重写鼠标按下，移动，滚轮事件，为模型的旋转缩放服务
void MyGLWidget::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        QPoint pressPoint = event->pos();
        lastPoint = pressPoint;
    }
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons()&Qt::LeftButton) {
        currentPoint = event->pos();
        updateGL();//重绘
    }
}

void MyGLWidget::wheelEvent(QWheelEvent *event) {
    if(event->delta()>0) {
        distance/=1.05;
    }
    else {
        distance*=1.05;
    }
    updateGL();
}

//绘制函数
void MyGLWidget::paintGL()                              //从这里开始进行所以的绘制
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存
    glLoadIdentity();                                   //重置当前的模型观察矩阵

    glTranslatef(0.0, 0.0f, (GLfloat)distance);                   //坐标系移入屏幕4.0单位
    //glRotatef(angle,0.5f,1.0f,0.0f);
    int delta_x = currentPoint.x() - lastPoint.x();
    int delta_y = currentPoint.y() - lastPoint.y();
    lastPoint = currentPoint;
    angle_x+=0.1f*delta_y;
    angle_y+=0.1f*delta_x;
    glRotatef(angle_x,1.0f,0.0f,0.0f); //旋转
    glRotatef(angle_y,0.0f,1.0f,0.0f); //旋转

    //glColor3f(0.6f,0.6f,0.6f);
    //glColor3f(0.7f,0.7f,0.7f); 用以增强效果的两种颜色

    //绘制每个面
    for(int i = 0;i < faces.size();i++) {
        if(i%2==0) {
            glColor3f(0.7f,0.7f,0.7f);
        }
        else {
            glColor3f(0.6f,0.6f,0.6f);
        }

        glBegin(GL_POLYGON);
        for(int j = 0;j< faces[i].size();j++) {
            Point3D point = points[faces[i][j]];
            glVertex3f(point.x,point.y,point.z);
        }
        glEnd();
    }

}

