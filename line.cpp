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
   // QPainter painter(&Pix);
   // painter.setPen(pen);
   // painter.drawLine(startPoint,endPoint);
    double deltaX = startPoint.x()-endPoint.x();
    double deltaY = startPoint.y()-endPoint.y();
    double k;
    if(deltaX==0) {
        k = MAX;
    }
    else {
        k = deltaY/deltaX;
    }

    if(abs(k)<1) {
        if(deltaX*deltaY>0) {  //case 1 : 0<k<1
            int x0,x1,y0,y1;
            if(startPoint.x()<endPoint.x()) {
                x0 = startPoint.x();
                x1 = endPoint.x();
                y0 = startPoint.y();
                y1 = endPoint.y();
            }
            else {
                x0 = endPoint.x();
                y0 = endPoint.y();
                x1 = startPoint.x();
                y1 = startPoint.y();
            }
            int a = y0-y1,b=x1-x0,c=x0*y1-x1*y0;
            int x=x0,y=y0;
            setPixel(x,y,pen,Pix);
            while(x<=x1) {
                if(2*a*x+2*a+2*b*y+b+2*c<0) {
                    x++;
                    y++;
                }
                else {
                    x++;
                }
                setPixel(x,y,pen,Pix);
            }
        }
        else {   //case 2: -1<k<0
            int x0,x1,y0,y1;
            if(startPoint.x()<endPoint.x()) {
                x0 = startPoint.x();
                x1 = endPoint.x();
                y0 = startPoint.y();
                y1 = endPoint.y();
            }
            else {
                x0 = endPoint.x();
                y0 = endPoint.y();
                x1 = startPoint.x();
                y1 = startPoint.y();
            }
            int a = y0-y1,b=x1-x0,c=x0*y1-x1*y0;
            int x=x0,y=y0;
            setPixel(x,y,pen,Pix);
            while(x<=x1) {
                if(2*a*x+2*a+2*b*y-b+2*c<0) {
                    x++;
                }
                else {
                    x++;
                    y--;
                }
                setPixel(x,y,pen,Pix);
            }
        }
    }
    else {
        if(deltaX*deltaY>0) {
            int x0,x1,y0,y1;
            if(startPoint.y()<endPoint.y()) {
                y0 = startPoint.y();
                x0 = startPoint.x();
                y1 = endPoint.y();
                x1 = endPoint.x();
            }
            else {
                y0 = endPoint.y();
                x0 = endPoint.x();
                y1 = startPoint.y();
                x1 = startPoint.x();
            }
            int a = y0-y1,b=x1-x0,c=x0*y1-x1*y0;
            int x=x0,y=y0;
            setPixel(x,y,pen,Pix);
            while(y<=y1) {
                if(2*b*y+2*b+2*a*x+a+2*c<0) {
                    y++;
                }
                else {
                    x++;
                    y++;
                }
                setPixel(x,y,pen,Pix);
            }
        }
        else {
            int x0,x1,y0,y1;
            if(startPoint.y()<endPoint.y()) {
                y0 = startPoint.y();
                x0 = startPoint.x();
                y1 = endPoint.y();
                x1 = endPoint.x();
            }
            else {
                y0 = endPoint.y();
                x0 = endPoint.x();
                y1 = startPoint.y();
                x1 = startPoint.x();
            }
            int a = y0-y1,b=x1-x0,c=x0*y1-x1*y0;
            int x=x0,y=y0;
            setPixel(x,y,pen,Pix);
            while(y<=y1) {
                if(2*b*y+2*b+2*a*x-a+2*c<0) {
                    y++;
                    x--;
                }
                else {
                    y++;
                }
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
    /*如果点到直线的距离小于30，就认为在拖动区域*/
    int a = ymin-ymax,b=xmax-xmin,c=xmin*ymax-xmax*ymin;
    double distance = (double)abs(a*x+b*y+c)/pow((double)(a*a+b*b),0.5);
    if(distance<=30) {
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
//裁剪函数，如果返回false，直接删除该直线
//如果返回true，根据裁剪结果进行重绘
//使用Cohen-sutherland裁剪算法
bool Line::cut(QPoint point1, QPoint point2) {
    qDebug() <<"cut by"<<point1<<point2;
    int xmin,xmax,ymin,ymax;
    if(point1.x()>point2.x()) {
        xmin = point2.x();
        xmax = point1.x();
    }
    else {
        xmin = point1.x();
        xmax = point2.x();
    }
    if(point1.y()>point2.y()) {
        ymin = point2.y();
        ymax = point1.y();
    }
    else {
        ymin = point1.y();
        ymax = point2.y();
    }

    int areaCode1=0,areaCode2=0;
    int x1 = startPoint.x(),y1=startPoint.y();
    int x2 = endPoint.x(),y2=endPoint.y();
    int a = y2-y1,b=x1-x2,c=x2*y1-x1*y2;
    areaCode1 = setAreaCode(x1,y1,xmin,xmax,ymin,ymax);
    areaCode2 = setAreaCode(x2,y2,xmin,xmax,ymin,ymax);
    //return false;
    qDebug()<<"code1:"<<areaCode1;
    qDebug()<<"code2:"<<areaCode2;
    if((areaCode1&areaCode2)!=0) {
        //完全不在区域内
        qDebug()<<"不在区域内";
        return false;
    }
    if(areaCode1==0&&areaCode2==0) {
        //完全在区域内
        return true;
    }
    //否则进行求交运算,直到裁剪出的线段完全落在区域内部，或判断出其不在区域内
    //依次比较四条边界
    //左边界
    int flag1 = areaCode1&1;
    int flag2 = areaCode2&1;
    qDebug() << "falg1="<<flag1<<",flag2="<<flag2;
    bool isDone = false;
    bool res = false;
    if(flag1==1) { //起始点在左边界外侧
        x1 = xmin;
        y1 = (-c-a*x1)/b;
        areaCode1 = setAreaCode(x1,y1,xmin,xmax,ymin,ymax);
        qDebug()<<"left Cutting:"<<areaCode1;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    if(flag2==1) {
        x2 = xmin;
        y2 = (-c-a*x2)/b;
        areaCode2 = setAreaCode(x2,y2,xmin,xmax,ymin,ymax);
        qDebug()<<"left Cutting:"<<areaCode2;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    /*右边界*/
    flag1=(areaCode1&0x2)/2;
    flag2=(areaCode2&0x2)/2;
    qDebug() << "falg1="<<flag1<<",flag2="<<flag2;
    if(flag1==1) {
        x1 = xmax;
        y1 = (-c-a*x1)/b;
        areaCode1 = setAreaCode(x1,y1,xmin,xmax,ymin,ymax);
        qDebug()<<"right Cutting:"<<areaCode1;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    if(flag2==1) {
        x2 = xmax;
        y2 = (-c-a*x2)/b;
        areaCode2 = setAreaCode(x2,y2,xmin,xmax,ymin,ymax);
        qDebug()<<"right Cutting:"<<areaCode2;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    //上边界
    flag1 = (areaCode1&0x4)/4;
    flag2 = (areaCode2&0x4)/4;
    qDebug() << "falg1="<<flag1<<",flag2="<<flag2;
    if(flag1==1) {
        y1 = ymin;
        x1 = (-c-b*y1)/a;
        areaCode1 = setAreaCode(x1,y1,xmin,xmax,ymin,ymax);
        qDebug()<<"upper Cutting:"<<areaCode1;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    if(flag2==1) {
        y2 = ymin;
        x2 = (-c-b*y2)/a;
        areaCode2 = setAreaCode(x2,y2,xmin,xmax,ymin,ymax);
        qDebug()<<"upper Cutting:"<<areaCode2;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }

    //下边界
    flag1 = (areaCode1&0x8)/8;
    flag2 = (areaCode2&0x8)/8;
    qDebug() << "falg1="<<flag1<<",flag2="<<flag2;
    if(flag1==1) {
        y1 = ymax;
        x1 = (-c-b*y1)/a;
        areaCode1 = setAreaCode(x1,y1,xmin,xmax,ymin,ymax);
        qDebug()<<"bottom Cutting:"<<areaCode1;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }
    if(flag2==1){
        y2 = ymax;
        x2 = (-c-b*y2)/a;
        areaCode2 = setAreaCode(x2,y2,xmin,xmax,ymin,ymax);
        qDebug()<<"bottom Cutting:"<<areaCode2;
        res=judgeAreaCode(areaCode1,areaCode2,isDone);
        if(isDone) {
            startPoint.setX(x1);startPoint.setY(y1);
            endPoint.setX(x2);endPoint.setY(y2);
            auxilaryPoints[0]=startPoint;auxilaryPoints[1]=endPoint;
            return res;
        }
    }

    return false;
}

int Line::setAreaCode(int x, int y, int xmin, int xmax, int ymin, int ymax) {
    int areaCode = 0;
    if(x<xmin) {
        areaCode|=1;
    }
    else if(x>xmax) {
        areaCode|=2;
    }

    if(y<ymin) {
        areaCode|=4;
    }
    else if(y>ymax) {
        areaCode|=8;
    }

    return areaCode;
}

bool Line::judgeAreaCode(int areaCode1, int areaCode2,bool& success) {
    if(areaCode1==0&&areaCode2==0) {
        success = true;
        return true;
    }
    if((areaCode1&areaCode2)!=0) {
        //完全不在区域内
        success = true;
        return false;
    }
    return true;
}
