#include "line.h"
#include <QPainter>
#include <math.h>
#include <QDebug>
#include <cmath>
#include <assert.h>
#define MAX 10000.0
Line::Line()
{
    attribute = LINE;
    rotateDegrees = 0;
}

Line::Line(QPoint startPoint, QPoint endPoint) {
    attribute = LINE;
    rotateDegrees = 0;
    this->startPoint = startPoint;
    this->endPoint = endPoint;
    this->center.setX((startPoint.x()+endPoint.x())/2);
    this->center.setY((startPoint.y()+endPoint.y())/2);
    auxilaryPoints.push_back(startPoint);
    auxilaryPoints.push_back(endPoint);
}
/*中点画线算法实现*/
void Line::draw(QPen &pen, QPixmap &Pix){
    //qDebug()<<"My draw";
    qDebug() <<"draw line:"<<startPoint<<" "<<endPoint;
    //变量声明与初始化
    int x0 = 0,y0 = 0,x1 = 0,y1 = 0;
    int a = 0,b = 0,c = 0,d = 0;
    int delta0 = 0,delta1 = 0;
    double delta_X = startPoint.x() - endPoint.x();
    double delta_Y = startPoint.y() - endPoint.y();
    double m = 0;
    if(delta_X == 0){
        //斜率不存在时，特殊处理
        //将这种情况归属到斜率大于1的中去
        m = MAX;
    }
    else {
        //计算斜率
        m = delta_Y/delta_X;
    }

    /*斜率的绝对值不大于1，x为增量*/
    if(abs(m) <= 1) {
        if(startPoint.x() < endPoint.x()) {
            x0 = startPoint.x();
            y0 = startPoint.y();
            x1 = endPoint.x();
            y1 = endPoint.y();
        }
        else{
            x1 = startPoint.x();
            y1 = startPoint.y();
            x0 = endPoint.x();
            y0 = endPoint.y();
        }
        //计算a,b,c
        a = y0 - y1;
        b = x1 - x0;
        c = x0 * y1 - x1 * y0;

        if(m > 0) {
            //0<m<=1的情况
            d = 2 * a + b;
            delta0 = 2 * a;
            delta1 = 2 * (a + b);

            int x = x0;
            int y = y0;
            //painter.drawPoint(x0,y0);
            setPixel(x,y,pen,Pix);
            while(x < x1) {
                if(d >=0){
                    x ++;
                    d+=delta0;
                }
                else {
                    x++;
                    y++;
                    d+=delta1;
                }
                //painter.drawPoint(x,y);
                setPixel(x,y,pen,Pix);
            }

        }
        //-1<=m<0
        else {
            d = 2 * a - b;
            delta0 = 2 * a;
            delta1 = 2 * (a - b);
            int x = x0;
            int y = y0;
            //painter.drawPoint(x0,y0);
            setPixel(x,y,pen,Pix);
            while(x < x1) {
                if(d <= 0) {
                    x++;
                    d += delta0;
                }
                else{
                    x++;
                    y--;
                    d += delta1;
                }
                //painter.drawPoint(x,y);
                setPixel(x,y,pen,Pix);
            }
        }

    }
    else{
        if(startPoint.y() < endPoint.y()) {
            x0 = startPoint.x();
            y0 = startPoint.y();
            x1 = endPoint.x();
            y1 = endPoint.y();
        }
        else{
            x1 = startPoint.x();
            y1 = startPoint.y();
            x0 = endPoint.x();
            y0 = endPoint.y();
        }
        a = y0 - y1;
        b = x1 - x0;
        c = x0 * y1 - x1 * y0;
        //m>1
        if(m > 0) {
            d = 2 * b + a;
            delta0 = 2 * b;
            delta1 = 2 * (a + b);
            int x = x0;
            int y = y0;
            //painter.drawPoint(x0,y0);
            setPixel(x,y,pen,Pix);

            while(y < y1) {
                if(d <= 0) {
                    y++;
                    d+=delta0;
                }
                else {
                    x++;
                    y++;
                    d+=delta1;
                }
                //painter.drawPoint(x,y);
                setPixel(x,y,pen,Pix);
            }
        }
        //m<-1
        else {
            d = 2 * b -a;
            delta0 = 2 * b;
            delta1 = 2 * (b - a);
            int x = x0;
            int y = y0;
            //painter.drawPoint(x0,y0);
            setPixel(x,y,pen,Pix);

            while(y < y1) {
                if(d >= 0) {
                    y ++;
                    d+=delta0;
                }
                else {
                    x--;
                    y++;
                    d+=delta1;
                }
                //painter.drawPoint(x,y);
                setPixel(x,y,pen,Pix);
            }
        }
    }
}

bool Line::inDraggingArea(int x, int y) {
    int xmin,xmax,ymin,ymax;
    xmin = startPoint.x();
    xmax = endPoint.x();
    ymin = startPoint.y();
    ymax = endPoint.y();

    if((x-xmin)*(x-xmax)<=0 && (y-ymin)*(y-ymax)<=0) {
        return true;
    }
    return false;
}

void Line::move(int delta_x, int delta_y) {
    startPoint.setX(startPoint.x()+delta_x);
    startPoint.setY(startPoint.y()+delta_y);
    endPoint.setX(endPoint.x()+delta_x);
    endPoint.setY(endPoint.y()+delta_y);
    center.setX((startPoint.x()+endPoint.x())/2);
    center.setY((startPoint.y()+endPoint.y())/2);

    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldy = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(oldX+delta_x);
        auxilaryPoints[i].setY(oldy+delta_y);
    }
}

void Line::setCenter(QPoint center) {
    this->center = center;
}

void Line::setPixel(int x, int y, QPen &pen, QPixmap &Pix) {
    //设置画笔颜色，粗细等属性。
    QPainter painter(&Pix);
    painter.setPen(pen);
    painter.drawPoint(x,y);
}

void Line::addRotateDegrees(int degrees) {
    rotateDegrees=(rotateDegrees+degrees)%360;
    int x1 = startPoint.x(),y1 = startPoint.y();
    int x2 = endPoint.x(),y2 = endPoint.y();
    double rad = ((double)degrees*3.14159265358) / 180.0;
    double COS = cos(rad);
    double SIN = sin(rad);
    qDebug() <<SIN <<" "<<COS;

    int xr = center.x();
    int yr = center.y();
    startPoint.setX(xr+COS*(x1-xr) - SIN*(y1-yr));
    startPoint.setY(yr+SIN*(x1-xr) + COS*(y1-yr));
    endPoint.setX(xr+COS*(x2-xr) - SIN*(y2-yr));
    endPoint.setY(yr+SIN*(x2-xr) + COS*(y2-yr));
    center.setX((startPoint.x()+endPoint.x())/2);
    center.setY((startPoint.y()+endPoint.y())/2);

    auxilaryPoints[0] = startPoint;
    auxilaryPoints[1] = endPoint;
}

void Line::zoom(double ratio) {
    int x1 = startPoint.x(),x2 = endPoint.x();
    int y1 = startPoint.y(),y2 = endPoint.y();
    startPoint.setX(center.x() + (x1-center.x())*ratio);
    startPoint.setY(center.y() + (y1-center.y())*ratio);
    endPoint.setX(center.x() + (x2-center.x())*ratio);
    endPoint.setY(center.y() + (y2-center.y())*ratio);
    center.setX((startPoint.x()+endPoint.x())/2);
    center.setY((startPoint.y()+endPoint.y())/2);
    auxilaryPoints[0] = startPoint;
    auxilaryPoints[1] = endPoint;
}

void Line::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    if(index==0) {
        startPoint.setX(startPoint.x()+delta_x);
        startPoint.setY(startPoint.y()+delta_y);
        auxilaryPoints[0] = startPoint;
    }
    else if(index ==1) {
        endPoint.setX(endPoint.x() + delta_x);
        endPoint.setY(endPoint.y() + delta_y);
        auxilaryPoints[1] = endPoint;
    }
}

void Line::horizontalFilp() {
    int x1 = startPoint.x();
    int x2 = endPoint.x();
    int xr = center.x();
    x1 = xr+(xr-x1);
    x2 = xr+(xr-x2);
    xr = (x1+x2)/2;
    startPoint.setX(x1);
    endPoint.setX(x2);
    center.setX(xr);
    auxilaryPoints[0] = startPoint;
    auxilaryPoints[1] = endPoint;
}

void Line::verticalFilp() {
    int y1 = startPoint.y();
    int y2 = endPoint.y();
    int yr = center.y();
    y1 = 2*yr-y1;
    y2 = 2*yr-y2;
    yr = (y1+y2)/2;
    startPoint.setY(y1);
    endPoint.setY(y2);
    center.setY(yr);
    auxilaryPoints[0] = startPoint;
    auxilaryPoints[1] = endPoint;
}
