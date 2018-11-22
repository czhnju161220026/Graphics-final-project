#include "circle.h"
#include <QDebug>
#include <QPainter>
#include <cmath>

Circle::Circle()
{
    attribute = CIRCLE;
    rotateDegrees = 0;
}

//自定义构造函数，接受圆心和半径
Circle::Circle(int radius, QPoint center) {
    attribute = CIRCLE;
    rotateDegrees = 0;
    this->center = center;
    this->radius = radius;
    auxilaryPoints.push_back(QPoint(center.x()-radius,center.y()-radius));
    auxilaryPoints.push_back(QPoint(center.x()+radius,center.y()-radius));
    auxilaryPoints.push_back(QPoint(center.x()+radius,center.y()+radius));
    auxilaryPoints.push_back(QPoint(center.x()-radius,center.y()+radius));
}

//自定义构造函数，根据鼠标按下和松起的两个点的坐标，计算圆心半径
Circle::Circle(QPoint point1, QPoint point2) {
    attribute = CIRCLE;
    int delta_x = point1.x() - point2.x();
    int delta_y = point1.y() - point2.y();
    this->radius = (abs(delta_x)<abs(delta_y))?(abs(delta_x)/2):(abs(delta_y)/2);
    if(delta_x < 0) {
        this->center.setX(radius + point1.x());
    }
    else {
        this->center.setX(-radius + point1.x());
    }

    if(delta_y < 0) {
        this->center.setY(radius + point1.y());
    }
    else {
        this->center.setY(-radius + point1.y());
    }

    auxilaryPoints.push_back(QPoint(center.x()-radius,center.y()-radius));
    auxilaryPoints.push_back(QPoint(center.x()+radius,center.y()-radius));
    auxilaryPoints.push_back(QPoint(center.x()+radius,center.y()+radius));
    auxilaryPoints.push_back(QPoint(center.x()-radius,center.y()+radius));
}
//圆的中点生成算法
void Circle::draw(QPen &pen, QPixmap &Pix) {
    //qDebug() << center.x()<<","<<center.y()<<" radius="<<radius;
    //开始生成
    //起始位置（0，r）
    int x = 0;
    int y = radius;
    //指示参数 e
    int e = 1 - radius;
    //不同情况下，e的增量
    int delta1 = 5 - 2 * radius;
    int delta2 = 3;

    while(x<=y) {
        setPixel(x,y,pen,Pix);
        if(e > 0) { //中点在圆外，选择低点进行绘制
            x++;
            y--;
            e+=delta1;
            delta1+=4;
            delta2+=2;
        }
        else {    //中点在圆内，选择高点进行绘制
            x++;
            e+=delta2;
            delta1+=2;
            delta2+=2;
        }
    }
}

bool Circle::inDraggingArea(int x, int y) {
    int delta_x = abs(x-center.x());
    int delta_y = abs(y-center.y());
    int delta = (delta_x>delta_y)?delta_x:delta_y;
    if(delta<=radius) {
        return true;
    }
    else {
        return false;
    }
}

void Circle::move(int delta_x, int delta_y) {
    int old_x = center.x();
    int old_y = center.y();
    center.setX(old_x+delta_x);
    center.setY(old_y+delta_y);
    qDebug() << "move :"<<delta_x<<","<<delta_y;

    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldy = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(oldX+delta_x);
        auxilaryPoints[i].setY(oldy+delta_y);
    }
}

void Circle::setPixel(int x, int y,QPen &pen, QPixmap &Pix) {
    //设置画笔颜色，粗细等属性。
    QPainter painter(&Pix);
    painter.setPen(pen);
    painter.drawPoint(x+center.x(),y+center.y());
    painter.drawPoint(y+center.x(),x+center.y());
    painter.drawPoint(x+center.x(),-y+center.y());
    painter.drawPoint(y+center.x(),-x+center.y());
    painter.drawPoint(-x+center.x(),y+center.y());
    painter.drawPoint(-y+center.x(),x+center.y());
    painter.drawPoint(-x+center.x(),-y+center.y());
    painter.drawPoint(-y+center.x(),-x+center.y());
}

void Circle::zoom(double ratio) {
    radius*=ratio;
    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldY = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(center.x() + ratio*(oldX - center.x()));
        auxilaryPoints[i].setY(center.y() + ratio*(oldY - center.y()));
    }
}

void Circle::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    bool changeInX = (abs(delta_x)<abs(delta_y));
    if(index == 0) {
        if(changeInX) {
            auxilaryPoints[0].setX(auxilaryPoints[0].x() + delta_x);
            auxilaryPoints[0].setY(auxilaryPoints[0].y() + delta_x);
        }
        else {
            auxilaryPoints[0].setY(auxilaryPoints[0].y() + delta_y);
            auxilaryPoints[0].setX(auxilaryPoints[0].x() + delta_y);
        }
        auxilaryPoints[1].setX(auxilaryPoints[2].x());
        auxilaryPoints[1].setY(auxilaryPoints[0].y());
        auxilaryPoints[3].setX(auxilaryPoints[0].x());
        auxilaryPoints[3].setY(auxilaryPoints[2].y());
        center.setX((auxilaryPoints[0].x()+auxilaryPoints[2].x())/2);
        center.setY((auxilaryPoints[0].y()+auxilaryPoints[2].y())/2);
        radius = 0.5*abs(auxilaryPoints[2].x()-auxilaryPoints[0].x());
    }
    if(index == 1) {
        if(changeInX) {
            auxilaryPoints[1].setX(auxilaryPoints[1].x() + delta_x);
            auxilaryPoints[1].setY(auxilaryPoints[1].y() - delta_x);
        }
        else {
            auxilaryPoints[1].setY(auxilaryPoints[1].y() + delta_y);
            auxilaryPoints[1].setX(auxilaryPoints[1].x() - delta_y);
        }
        auxilaryPoints[0].setX(auxilaryPoints[3].x());
        auxilaryPoints[0].setY(auxilaryPoints[1].y());
        auxilaryPoints[2].setX(auxilaryPoints[1].x());
        auxilaryPoints[2].setY(auxilaryPoints[3].y());
        center.setX((auxilaryPoints[0].x()+auxilaryPoints[2].x())/2);
        center.setY((auxilaryPoints[0].y()+auxilaryPoints[2].y())/2);
        radius = 0.5*abs(auxilaryPoints[2].x()-auxilaryPoints[0].x());
    }
    if(index == 2) {
        if(changeInX) {
            auxilaryPoints[2].setX(auxilaryPoints[2].x() + delta_x);
            auxilaryPoints[2].setY(auxilaryPoints[2].y() + delta_x);
        }
        else {
            auxilaryPoints[2].setY(auxilaryPoints[2].y() + delta_y);
            auxilaryPoints[2].setX(auxilaryPoints[2].x() + delta_y);
        }
        auxilaryPoints[1].setX(auxilaryPoints[2].x());
        auxilaryPoints[1].setY(auxilaryPoints[0].y());
        auxilaryPoints[3].setX(auxilaryPoints[0].x());
        auxilaryPoints[3].setY(auxilaryPoints[2].y());
        center.setX((auxilaryPoints[0].x()+auxilaryPoints[2].x())/2);
        center.setY((auxilaryPoints[0].y()+auxilaryPoints[2].y())/2);
        radius = 0.5*abs(auxilaryPoints[2].x()-auxilaryPoints[0].x());
    }
    if(index == 3) {
        if(changeInX) {
            auxilaryPoints[3].setX(auxilaryPoints[3].x() + delta_x);
            auxilaryPoints[3].setY(auxilaryPoints[3].y() - delta_x);
        }
        else {
            auxilaryPoints[3].setY(auxilaryPoints[3].y() + delta_y);
            auxilaryPoints[3].setX(auxilaryPoints[3].x() - delta_y);
        }
        auxilaryPoints[0].setX(auxilaryPoints[3].x());
        auxilaryPoints[0].setY(auxilaryPoints[1].y());
        auxilaryPoints[2].setX(auxilaryPoints[1].x());
        auxilaryPoints[2].setY(auxilaryPoints[3].y());
        center.setX((auxilaryPoints[0].x()+auxilaryPoints[2].x())/2);
        center.setY((auxilaryPoints[0].y()+auxilaryPoints[2].y())/2);
        radius = 0.5*abs(auxilaryPoints[2].x()-auxilaryPoints[0].x());
    }
}
