#include "oval.h"
#include <QDebug>
#include <QPainter>
#include <cmath>
Oval::Oval(){
    attribute = OVAL;
    a = 0;
    b = 0;
    in_x = true;
    center = QPoint(0,0);
    rotateDegrees = 0;
}

Oval::Oval(QPoint point1, QPoint point2) {
    attribute = OVAL;
    int delta_x = abs(point1.x()-point2.x());
    int delta_y = abs(point1.y()-point2.y());
    center = QPoint((point1.x()+point2.x())/2,(point1.y()+point2.y())/2);
    rotateDegrees = 0;
    if(delta_x>=delta_y) {
        a = delta_x/2;
        b = delta_y/2;
        in_x = true;
    }
    else {
        a = delta_y/2;
        b = delta_x/2;
        in_x = false;
    }

    delta_x =a;
    delta_y =b;
    if(!in_x) {
        delta_x = b;
        delta_y = a;
    }
    int xr = center.x();
    int yr = center.y();
    auxilaryPoints.push_back(QPoint(xr-delta_x,yr-delta_y));
    auxilaryPoints.push_back(QPoint(xr+delta_x,yr-delta_y));
    auxilaryPoints.push_back(QPoint(xr+delta_x,yr+delta_y));
    auxilaryPoints.push_back(QPoint(xr-delta_x,yr+delta_y));


}

Oval::Oval(QPoint center, int double_a, int double_b, bool in_x) {
    attribute = OVAL;
    this->center = center;
    this->a = double_a/2;
    this->b = double_b/2;
    this->in_x = in_x;
    rotateDegrees = 0;

    int delta_x =a;
    int delta_y =b;
    if(!in_x) {
        delta_x = b;
        delta_y = a;
    }
    int xr = center.x();
    int yr = center.y();
    auxilaryPoints.push_back(QPoint(xr-delta_x,yr-delta_y));
    auxilaryPoints.push_back(QPoint(xr+delta_x,yr-delta_y));
    auxilaryPoints.push_back(QPoint(xr+delta_x,yr+delta_y));
    auxilaryPoints.push_back(QPoint(xr-delta_x,yr+delta_y));
}

void Oval::draw(QPen &pen, QPixmap &Pix) {
    qDebug()<<"draw Oval:"<<"("<<center.x()<<","<<center.y()<<"), a = "<<a<<" b = "<<b<<" in_X: "<<in_x;
    if(a==0&&b==0) {
        return;
    }
    int x = 0;
    int y = b;
    /*
    *计算区域1的判定参数p
    *p在不同情况下的增量初值
    *为了保证计算过程中只有整数出现，
    * 所以将参数p和增量都乘了4
    */
    int p_area1 = 4*b*b + a*a - 4*a*a*b;
    int delta1 = 8*a*a + 12*b*b - 8*a*a*b;
    int delta2 = 12*b*b;

    //area1
    setPixel(x,y,pen,Pix);
    while(b*b*x <= a*a*y) {
        if(p_area1 >= 0) {
            x++;
            y--;
            p_area1 += delta1;
            delta1 = delta1 + 8*b*b + 8*a*a;
            delta2 = delta2 + 8*b*b;
        }
        else {
            x++;
            p_area1 += delta2;
            delta1 += 8*b*b;
            delta2 += 8*b*b;
        }
        setPixel(x,y,pen,Pix);
    }

    //area2
    /*计算区域2的判定参数p
    *p在不同情况下的增量初值
    * 为了保证计算过程中只有整数出现，
    * 所以将参数p和增量都乘了4
    */

    int p_area2 = 4*b*b*x*x+4*b*b*x+b*b+4*a*a*y*y-8*a*a*y+4*a*a-4*a*a*b*b;
    delta1 = 8*b*b*x-8*a*a*y+8*b*b+12*a*a;
    delta2 = -8*a*a*y+12*a*a;
    while(y>=0) {
        if(p_area2<=0) {
            x++;
            y--;
            p_area2+=delta1;
            delta1=delta1+8*b*b+8*a*a;
            delta2=delta2+8*a*a;
        }
        else {
            y--;
            p_area2+=delta2;
            delta1+=8*a*a;
            delta2+=8*a*a;
        }
        setPixel(x,y,pen,Pix);
    }

    //test_draw(Pix,pen,center.x(),center.y(),a,b);
}

void Oval::setPixel(int x, int y, QPen &pen, QPixmap &Pix) {
    //根据椭圆方向，进行旋转
    if(!in_x) {
        int temp =x;
        x = y;
        y = temp;
    }
    QPainter painter(&Pix);
    painter.setPen(pen);
    //对称绘制

    double rad = ((double)rotateDegrees * 3.14159265358) / 180;
    double COS = cos(rad);
    double SIN = sin(rad);
    int xr = center.x();
    int yr = center.y();
    int oldX = x;
    int oldY = y;

    x = oldX * COS - oldY * SIN;
    y = oldX * SIN + oldY * COS;
    painter.drawPoint(x+center.x(),y+center.y());
    painter.drawPoint(-x+center.x(),-y+center.y());

    oldX = -oldX;
    x = oldX * COS - oldY * SIN;
    y = oldX * SIN + oldY * COS;
    painter.drawPoint(x+center.x(),y+center.y());
    painter.drawPoint(-x+center.x(),-y+center.y());

    /*painter.drawPoint(-x+center.x(),y+center.y());
    painter.drawPoint(x+center.x(),-y+center.y());
    painter.drawPoint(-x+center.x(),-y+center.y());*/
}

bool Oval::inDraggingArea(int x, int y) {
    if(in_x) {
        if(abs(x-center.x())<=a&&abs(y-center.y())<=b) {
            return true;
        }
    }
    else {
        if(abs(y-center.y())<=a&&abs(x-center.x())<=b) {
            return true;
        }
    }
    return false;
}

void Oval::move(int delta_x, int delta_y) {
    center.setX(center.x()+delta_x);
    center.setY(center.y()+delta_y);
    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldy = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(oldX+delta_x);
        auxilaryPoints[i].setY(oldy+delta_y);
    }
}

void Oval::addRotateDegrees(int degrees) {
    rotateDegrees = (rotateDegrees+degrees)%360;
    double rad = ((double)rotateDegrees * 3.14159265358) / 180;
    double COS = cos(rad);
    double SIN = sin(rad);
    int xr = center.x();
    int yr = center.y();
    if(rotateDegrees%90==0) {
        int x1,x2,x3,x4;
        int y1,y2,y3,y4;
        x1 = auxilaryPoints[0].x();x2 = auxilaryPoints[1].x();x3 = auxilaryPoints[2].x();x4 = auxilaryPoints[3].x();
        y1 = auxilaryPoints[0].y();y2 = auxilaryPoints[1].y();y3 = auxilaryPoints[2].y();y4 = auxilaryPoints[3].y();
        auxilaryPoints[0].setX(xr+(y1-yr));auxilaryPoints[0].setY(yr+(x1-xr));
        auxilaryPoints[1].setX(xr+(y2-yr));auxilaryPoints[1].setY(yr+(x2-xr));
        auxilaryPoints[2].setX(xr+(y3-yr));auxilaryPoints[2].setY(yr+(x3-xr));
        auxilaryPoints[3].setX(xr+(y4-yr));auxilaryPoints[3].setY(yr+(x4-xr));
    }
}

void Oval::zoom(double ratio) {
    a*=ratio;
    b*=ratio;
    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldY = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(center.x() + ratio*(double)(oldX - center.x()));
        auxilaryPoints[i].setY(center.y() + ratio*(double)(oldY - center.y()));
    }
}

void Oval::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    if(index==0||index==2) {
        auxilaryPoints[index].setX(auxilaryPoints[index].x()+delta_x);
        auxilaryPoints[index].setY(auxilaryPoints[index].y()+delta_y);
        auxilaryPoints[1].setX(auxilaryPoints[2].x());
        auxilaryPoints[1].setY(auxilaryPoints[0].y());
        auxilaryPoints[3].setX(auxilaryPoints[0].x());
        auxilaryPoints[3].setY(auxilaryPoints[2].y());
    }
    else {
        auxilaryPoints[index].setX(auxilaryPoints[index].x()+delta_x);
        auxilaryPoints[index].setY(auxilaryPoints[index].y()+delta_y);
        auxilaryPoints[0].setX(auxilaryPoints[3].x());
        auxilaryPoints[0].setY(auxilaryPoints[1].y());
        auxilaryPoints[2].setX(auxilaryPoints[1].x());
        auxilaryPoints[2].setY(auxilaryPoints[3].y());
    }
    center.setX((auxilaryPoints[0].x()+auxilaryPoints[2].x())/2);
    center.setY((auxilaryPoints[0].y()+auxilaryPoints[2].y())/2);
    int deltaX = abs(auxilaryPoints[0].x()-auxilaryPoints[1].x());
    int deltaY = abs(auxilaryPoints[0].y()-auxilaryPoints[3].y());
    if(deltaX>deltaY) {
        a = deltaX/2;
        b = deltaY/2;
    }
    else {
        a = deltaY/2;
        b = deltaX/2;
    }

    if(deltaX>deltaY&&in_x) {
        rotateDegrees = 0;
    }
    else if(deltaX>deltaY&&!in_x) {
        rotateDegrees = 90;
    }
    else if(deltaX<=deltaY&&in_x) {
        rotateDegrees = 90;
    }
    else {
        rotateDegrees = 0;
    }
}

void Oval::showAuxilaryPoints(QPixmap &Pix) {
    if(rotateDegrees%90==0) {
        Shape::showAuxilaryPoints(Pix);
    }
}

int Oval::inAuxilaryPoint(int x, int y) {
    if(rotateDegrees%90==0) {
        return Shape::inAuxilaryPoint(x,y);
    }
    else {
        return -1;
    }
}

void Oval::horizontalFilp() {
    if(rotateDegrees%90==0) {
        //do nothing
        return;
    }
    if(rotateDegrees<=90) {
        addRotateDegrees(180-2*rotateDegrees);
    }
    else if(rotateDegrees<=180) {
        addRotateDegrees(360-2*rotateDegrees);
    }
    else if(rotateDegrees<=270) {
        addRotateDegrees(540-2*rotateDegrees);
    }
    else if(rotateDegrees<=360) {
        addRotateDegrees(720-2*rotateDegrees);
    }
}

void Oval::verticalFilp() {
    if(rotateDegrees%90==0) {
        //do nothing
        return;
    }
    if(rotateDegrees<=90) {
        addRotateDegrees(720-2*rotateDegrees);
    }
    else if(rotateDegrees<=180) {
        addRotateDegrees(720-2*rotateDegrees);
    }
    else if(rotateDegrees<=270) {
        addRotateDegrees(180+2*rotateDegrees);
    }
    else if(rotateDegrees<=360) {
        addRotateDegrees(720-2*rotateDegrees);
    }
}
