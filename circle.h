#ifndef CIRCLE_H
#define CIRCLE_H

#include"shape.h"
#include<QPoint>
#include<QPen>
#include<QPixmap>

//图形类：圆
class Circle : public Shape
{
private:
    QPoint center; //圆心坐标
    int radius;// 半径
    void setPixel(int x, int y,QPen &pen, QPixmap &Pix);
public:
    Circle();
    Circle(int radius, QPoint center);
    Circle(QPoint point1,QPoint point2);
    void draw(QPen& pen, QPixmap& Pix);//绘制函数
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void zoom(double ratio);
    //旋转和翻转对于圆来说是没有意义的

    void moveAuxilaryPoint(int index, int delta_x,int delta_y);

};
//Test
#endif // CIRCLE_H
