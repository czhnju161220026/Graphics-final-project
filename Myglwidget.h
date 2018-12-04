#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <string>

struct Point3D {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    Point3D(GLfloat x, GLfloat y, GLfloat z):x(x),y(y),z(z){}
    Point3D():x(0.0f),y(0.0f),z(0.0f){}
};


class MyGLWidget : public QGLWidget
{
    Q_OBJECT
    GLfloat angle_x,angle_y;
    QPoint lastPoint,currentPoint;
    QVector<Point3D> points;
    QVector<QVector<int>> faces;
public:
    MyGLWidget(QWidget* parent = 0);
    void loadOffFile(QString path);
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};

#endif // MYGLWIDGET_H
