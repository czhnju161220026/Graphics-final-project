#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"
#include "line.h"
#include<QPoint>
#include<QPen>
#include<QPixmap>

class rectangle :public Shape
{
private:
    QPoint point1;
    QPoint point2;
    QPoint point3;
    QPoint point4;
    QPoint center;
    Line line1,line2,line3,line4;
    int rotateDegrees;
public:
    rectangle();
    rectangle(QPoint point1,QPoint point2);
    void draw(QPen& pen, QPixmap& Pix);
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void addRotateDegrees(int degrees);
    void zoom(double ratio);
    void moveAuxilaryPoint(int index, int delta_x,int delta_y);
    int inAuxilaryPoint(int x, int y);
    void showAuxilaryPoints(QPixmap& Pix);
    void horizontalFilp();
    void verticalFilp();

};

#endif // RECTANGLE_H
