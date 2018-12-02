#include "Curve.h"
#include "line.h"
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QPainter>

/*
* 使用Cardinal样条法生成插值函数
* 张量参数t=0.5
* 这种插值法又叫CatMull
*/

/*
 * TODO: 在draw中完成对给定控制点的曲线的函数计算与绘制
*/
Curve::Curve() {
    attribute = CURVE;
    finished = false;
    rotateDegrees = 0;
    t = 0.5;
    qDebug() << "New Curve";
}

Curve::Curve(QVector<QPoint> points) {
    for(int i = 0;i < points.size();i++) {
        controlPoints.push_back(points[i]);
        auxilaryPoints.push_back(points[i]);
    }
     attribute = CURVE;
     finished = true;
     rotateDegrees = 0;
     t = 0.5;
     initConner();
}

void Curve::addNextPoint(QPoint point, QPen &pen, QPixmap &Pix) {
    qDebug() <<"add point "<<point;
    this->controlPoints.push_back(point);
    auxilaryPoints.push_back(point);
}

void Curve::finish(QPen& pen,QPixmap& Pix) {
    finished = true;
    initConner();
}

bool Curve::isFinished() {
    return finished;
}

void Curve::draw(QPen &pen, QPixmap &Pix) {
    qDebug() <<"Draw curve";
    QPainter painter(&Pix);
    painter.setPen(pen);
    for(int i = 0;i<controlPoints.size()-1;i++) {
        /*Line * newLine = new Line(points[i], points[i+1]);
        newLine->draw(pen,Pix);*/
        painter.drawLine(controlPoints[i],controlPoints[i+1]);
    }
    if(controlPoints.size()==1) {
        painter.drawPoint(controlPoints[0]);
    }
}

void Curve::initConner() {
    int minY,maxY,minX,maxX;
    minY=controlPoints[0].y();
    maxY=controlPoints[0].y();
    minX=controlPoints[0].x();
    maxX=controlPoints[0].x();

    for(int i = 0;i<controlPoints.size();i++) {
        if(controlPoints[i].x()<minX) {
            minX = controlPoints[i].x();
        }
        if(controlPoints[i].x()>maxX) {
            maxX = controlPoints[i].x();
        }
        if(controlPoints[i].y()<minY) {
            minY = controlPoints[i].y();
        }
        if(controlPoints[i].y()>maxY) {
            maxY = controlPoints[i].y();
        }
    }

    upperLeftConner.setX(minX);
    upperLeftConner.setY(minY);
    bottomRightConner.setX(maxX);
    bottomRightConner.setY(maxY);
    center.setX((minX+maxX)/2);
    center.setY((minY+maxY)/2);
}

bool Curve::inDraggingArea(int x, int y) {
    int xmin = upperLeftConner.x();
    int xmax = bottomRightConner.x();
    int ymin = upperLeftConner.y();
    int ymax = bottomRightConner.y();
    if((x-xmin)*(x-xmax)<=0 && (y-ymin)*(y-ymax)<=0) {
        return true;
    }
    return false;
}

void Curve::zoom(double ratio) {
    int xr = center.x();
    int yr = center.y();
    for(int i = 0;i<controlPoints.size();i++) {
        int oldX = controlPoints[i].x();
        int oldY = controlPoints[i].y();
        controlPoints[i].setX(xr + ratio*(oldX-xr));
        controlPoints[i].setY(yr + ratio*(oldY-yr));
    }
    for(int i = 0;i<controlPoints.size();i++) {
        auxilaryPoints[i] = controlPoints[i];
    }
    initConner();
}

void Curve::move(int delta_x, int delta_y) {
    qDebug()<<"Move "<<delta_x<<","<<delta_y;
    for(int i = 0;i<controlPoints.size();i++) {
        controlPoints[i].setX(controlPoints[i].x()+delta_x);
        controlPoints[i].setY(controlPoints[i].y()+delta_y);
    }
    center.setX(center.x() + delta_x);
    center.setY(center.y() + delta_y);
    upperLeftConner.setX(upperLeftConner.x()+delta_x);
    upperLeftConner.setY(upperLeftConner.y()+delta_y);
    bottomRightConner.setX(bottomRightConner.x()+delta_x);
    bottomRightConner.setY(bottomRightConner.y()+delta_y);

    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldy = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(oldX+delta_x);
        auxilaryPoints[i].setY(oldy+delta_y);
    }

    initConner();
}

void Curve::addRotateDegrees(int degrees) {
    rotateDegrees=(rotateDegrees+degrees)%360;
    double rad = ((double)degrees*3.14159265358) / 180.0;
    double COS = cos(rad);
    double SIN = sin(rad);
    int xr = center.x();
    int yr = center.y();
    for(int i = 0;i<controlPoints.size();i++) {
        int oldX = controlPoints[i].x();
        int oldY = controlPoints[i].y();
        controlPoints[i].setX(xr + (oldX-xr)*COS - (oldY - yr)*SIN);
        controlPoints[i].setY(yr + (oldX-xr)*SIN + (oldY - yr)*COS);
    }
    for(int i = 0;i<controlPoints.size();i++) {
        auxilaryPoints[i] = controlPoints[i];
    }
    initConner();
}

void Curve::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    auxilaryPoints[index].setX(delta_x+auxilaryPoints[index].x());
    auxilaryPoints[index].setY(delta_y+auxilaryPoints[index].y());

    controlPoints[index].setX(delta_x+controlPoints[index].x());
    controlPoints[index].setY(delta_y+controlPoints[index].y());

    initConner();
}

void Curve::verticalFilp() {
    for(int i =0;i<controlPoints.size();i++) {
        controlPoints[i].setY(2*center.y()-controlPoints[i].y());
        auxilaryPoints[i] = controlPoints[i];
    }
    initConner();
}

void Curve::horizontalFilp() {
    for(int i=0;i<controlPoints.size();i++) {
        controlPoints[i].setX(2*center.x()-controlPoints[i].x());
        auxilaryPoints[i] = controlPoints[i];
    }
    initConner();
}
