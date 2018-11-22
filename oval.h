#ifndef OVAL_H
#define OVAL_H
#include "shape.h"
#include<QPoint>
#include<QPen>
#include<QPixmap>

class Oval : public Shape
{
private:
    QPoint center;
    int a;
    int b;
    bool in_x;
    void setPixel(int x,int y,QPen& pen, QPixmap& Pix);
public:
    Oval();
    Oval(QPoint point1, QPoint point2);
    Oval(QPoint center,int double_a, int double_b, bool in_x);
    void draw(QPen& pen, QPixmap& Pix);
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void addRotateDegrees(int degrees);
    void zoom(double ratio);
    void moveAuxilaryPoint(int index,int delta_x, int delta_y);
    int inAuxilaryPoint(int x, int y);
    void showAuxilaryPoints(QPixmap& Pix);
    void horizontalFilp();
    void verticalFilp();
};

#endif // OVAL_H
