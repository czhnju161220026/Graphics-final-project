#ifndef POLYGON_H
#define POLYGON_H
#include"shape.h"
#include<QVector>
#include<QPoint>
#include<QPen>
#include<QPixmap>

class Polygon : public Shape
{
private:
    QVector<QPoint> points;
    QPoint upperLeftConner;
    QPoint bottomRightConner;
    QPoint center;
    bool finished;
    void initConner();
public:
    Polygon();
    Polygon(QVector<QPoint> points);
    void addNextPoint(QPoint point,QPen& pen, QPixmap& Pix);
    void finish(QPen& pen, QPixmap& Pix);
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

#endif // POLYGON_H
