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
    //指示曲线种类
    //0为cardinal，1为贝塞尔
    static int type;
    QVector<QPoint> controlPoints;
    QPoint upperLeftConner;
    QPoint bottomRightConner;
    QPoint center;
    bool finished;
    /*Cardinal样条的变量*/
    float t;            //张力参数
    float Ax,Bx,Cx,Dx;  //函数系数
    float Ay,By,Cy,Dy;  //函数系数
    float Matrix[4][4];//Cardinal 矩阵
    /*贝塞尔曲线的变量*/
    QVector<int> Cn;
    void initMatrix();
    void initConner();
public:
    Curve();
    Curve(QVector<QPoint> points);
    static void setType(int curveType);
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
    void showAuxilaryPoints(QPixmap& pix);
    /*Cardinal样条相关函数*/
    float Fx(float u);
    float Fy(float u);
    void updateFunction(int index);
    /*贝塞尔相关函数*/
    void updateCn();
    QPoint getPoint(float u);
};

#endif // CURVE_H
