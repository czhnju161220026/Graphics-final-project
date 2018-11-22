#ifndef LINE_H
#define LINE_H

#include"shape.h"
#include<QPoint>
#include<QPen>
#include<QPixmap>
#include<QPainter>

//图形类：直线
class Line : public Shape
{
private:
    QPoint startPoint; //起始位置
    QPoint endPoint;   //结束位置
    QPoint center; //spin
     void setPixel(int x,int y,QPen& pen, QPixmap& Pix);
public:
    Line();
    Line(QPoint startPoint,QPoint endPoint);  
    void draw(QPen& pen, QPixmap& Pix);//绘制函数
    void setCenter(QPoint center);
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void addRotateDegrees(int degrees);
    void zoom(double ratio);
    void moveAuxilaryPoint(int index, int delta_x, int delta_y);
    void horizontalFilp();
    void verticalFilp();
};

#endif // LINE_H
