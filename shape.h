#ifndef SHAPE_H
#define SHAPE_H

#include <QPixmap>
#include <QPen>
#include <QPoint>
#include <QVector>

enum{SHAPE=0,LINE,CIRCLE,RECTANGLE,OVAL,POLYGON};
class Shape
{
protected:
    int attribute;
    int rotateDegrees;
    QVector<QPoint> auxilaryPoints; //辅助点
public:
    Shape();
    virtual void draw(QPen& pen, QPixmap& Pix);
    virtual bool inDraggingArea(int x, int y);          //鼠标是否在可拖动区域内
    virtual void move(int delta_x, int delta_y);       //平移图形函数
    virtual void addRotateDegrees(int degrees);        //旋转函数
    virtual void zoom(double ratio);                    //缩放函数
    virtual int getAttribute();                          //获取属性
    virtual void showAuxilaryPoints(QPixmap& Pix);      //展示辅助点
    virtual int inAuxilaryPoint(int x, int y);          //返回鼠标当前在哪个辅助点上，若不在任何辅助点上，则返回-1
    virtual void moveAuxilaryPoint(int index,int delta_x, int delta_y); //移动辅助点
    virtual void horizontalFilp();
    virtual void verticalFilp();
    /*
    *这几个虚函数在多边形类中被重写，用来表示
    * (1)多边形是否绘制完成，
    * (2)为多边形添加下一个点，
    * (3)多边形完成绘制这三个功能
    */
    virtual bool isFinished();
    virtual void addNextPoint(QPoint point, QPen& pen,QPixmap& Pix);
    virtual void finish(QPen& pen,QPixmap& Pix);

    //虚函数，直线裁剪
    virtual bool cut(QPoint point1,QPoint point2);
};

#endif // SHAPE_H
