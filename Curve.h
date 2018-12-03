#ifndef CURVE_H
#define CURVE_H
#include "shape.h"
#include<QVector>
#include<QPoint>
#include<QPen>
#include<QPixmap>

/*
* 使用Cardinal样条法生成插值函数
* 张量参数t=0.5
* 又称为Catmull曲线
*/

class Curve: public Shape
{
private:
    QVector<QPoint> controlPoints;
    QPoint upperLeftConner;
    QPoint bottomRightConner;
    QPoint center;
    bool finished;
    float t;            //张力参数
    float Ax,Bx,Cx,Dx;  //函数系数
    float Ay,By,Cy,Dy;  //函数系数
    float Matrix[4][4];//Cardinal 矩阵
    void initMatrix();
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
    float Fx(float u);
    float Fy(float u);
    void updateFunction(int index);

};

#endif // CURVE_H
