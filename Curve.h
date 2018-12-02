#ifndef CURVE_H
#define CURVE_H
#include "shape.h"
#include<QVector>
#include<QPoint>
#include<QPen>
#include<QPixmap>

class Curve: public Shape
{
private:
    QVector<QPoint> controlPoints;
    QPoint upperLeftConner;
    QPoint bottomRightConner;
    QPoint center;
    bool finished;
    double t;            //张力参数
    void initConner();
public:
    Curve();
    Curve(QVector<QPoint> points);
    void addNextPoint(QPoint point,QPen& pen, QPixmap& Pix);
    void finish(QPen& pen,QPixmap& Pix);
    bool isFinished();
    void draw(QPen& pen, QPixmap& Pix);//绘制函数
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void addRotateDegrees(int degrees);
    void zoom(double ratio);
    void moveAuxilaryPoint(int index, int delta_x,int delta_y);
    void verticalFilp();
    void horizontalFilp();
};

#endif // CURVE_H
