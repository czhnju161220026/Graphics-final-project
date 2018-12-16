#include "Curve.h"
#include "line.h"
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QPainter>

/*
* Cardinal样条法生成插值函数
* 张量参数t=0.5
* 又称为Catmull曲线
*
* 贝塞尔样条曲线
*/

int Curve::type = 0;

Curve::Curve() {
    attribute = CURVE;
    finished = false;
    rotateDegrees = 0;
    t = 0.5;
    initMatrix();
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
     initMatrix();
     initConner();
}

void Curve::setType(int curveType) {
    Curve::type = curveType;
}

void Curve::addNextPoint(QPoint point, QPen &pen, QPixmap &Pix) {
    qDebug() <<"add point "<<point;
    this->controlPoints.push_back(point);
    auxilaryPoints.push_back(point);
    if(type == 1) {
        updateCn();
    }
}

void Curve::finish(QPen& pen,QPixmap& Pix) {
    finished = true;
    initConner();
}

bool Curve::isFinished() {
    return finished;
}

void Curve::draw(QPen &pen, QPixmap &Pix) {
    qDebug() <<"Draw curve:"<<type;
    QPainter painter(&Pix);
    painter.setPen(pen);
    //只有一个点，直接绘制
    if(controlPoints.size()==1) {
        painter.drawPoint(controlPoints[0]);
        return;
    }
    //进行Cardinal曲线绘制
    if(type == 0) {
        for(int i = 0;i < controlPoints.size()-1;i++) {
            //计算第i段的插值函数
            updateFunction(i);
            int delta_x = controlPoints[i].x() - controlPoints[i+1].y();
            int delta_y = controlPoints[i].y() - controlPoints[i+1].y();
            double length = sqrt(delta_x * delta_x + delta_y * delta_y);
            float delta_u = 0.5 / length;         //计算U自增的步长
            QVector<QPoint> points;
            float u = 0.0;
            while(u<1.0) {
                QPoint point(Fx(u),Fy(u));
                points.push_back(point);
                u+=delta_u;
            }
            points.push_back(QPoint(Fx(1),Fy(1)));
            for(int j = 0;j < points.size()-1; j++) {
                painter.drawLine(points[j],points[j+1]);
            }
        }
    }
    else if(type == 1) {
        float u = 0;
        QVector<QPoint> points;
        while(u <= 1) {
            points.push_back(getPoint(u));
            u += 0.0005;
        }
        for(int i = 0;i < points.size()-1;i++) {
            painter.drawLine(points[i],points[i+1]);
        }
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

float Curve::Fx(float u) {
    return Dx + u*(Cx+u*(Bx+u*Ax));
}

float Curve::Fy(float u) {
    return Dy + u*(Cy+u*(By+u*Ay));
}
void Curve::updateFunction(int index) {
    QPoint P0 = controlPoints[index];
    QPoint P1 = controlPoints[index+1];
    QPoint prev,next;
    if(index==0) {
        prev = P0;
    }
    else {
        prev = controlPoints[index-1];
    }

    if(index == controlPoints.size()-2) {
        next = P1;
    }
    else {
        next = controlPoints[index+2];
    }

    //更新函数参数
    Ax = Matrix[0][0]*prev.x() + Matrix[0][1]*P0.x() + Matrix[0][2]*P1.x() + Matrix[0][3]*next.x();
    Bx = Matrix[1][0]*prev.x() + Matrix[1][1]*P0.x() + Matrix[1][2]*P1.x() + Matrix[1][3]*next.x();
    Cx = Matrix[2][0]*prev.x() + Matrix[2][1]*P0.x() + Matrix[2][2]*P1.x() + Matrix[2][3]*next.x();
    Dx = Matrix[3][0]*prev.x() + Matrix[3][1]*P0.x() + Matrix[3][2]*P1.x() + Matrix[3][3]*next.x();

    Ay = Matrix[0][0]*prev.y() + Matrix[0][1]*P0.y() + Matrix[0][2]*P1.y() + Matrix[0][3]*next.y();
    By = Matrix[1][0]*prev.y() + Matrix[1][1]*P0.y() + Matrix[1][2]*P1.y() + Matrix[1][3]*next.y();
    Cy = Matrix[2][0]*prev.y() + Matrix[2][1]*P0.y() + Matrix[2][2]*P1.y() + Matrix[2][3]*next.y();
    Dy = Matrix[3][0]*prev.y() + Matrix[3][1]*P0.y() + Matrix[3][2]*P1.y() + Matrix[3][3]*next.y();

    qDebug() << "Fx:"<<Ax<<"u^3 + "<<Bx<<"u^2 + "<<Cx<<"u + "<<Dx;
    qDebug() << "Fy:"<<Ay<<"u^3 + "<<By<<"u^2 + "<<Cy<<"u + "<<Dy;
}

//初始化Cardinal矩阵
void Curve::initMatrix() {
    Matrix[0][0] = -t;Matrix[0][1] = 2 - t; Matrix[0][2] = t - 2; Matrix[0][3] = t;
    Matrix[1][0] =2*t;Matrix[1][1] = t - 3; Matrix[1][2] = 3-2*t; Matrix[1][3] = -t;
    Matrix[2][0] = -t;Matrix[2][1] = 0    ; Matrix[2][2] = t    ; Matrix[2][3] = 0;
    Matrix[3][0] = 0 ;Matrix[3][1] = 1    ; Matrix[3][2] = 0    ; Matrix[3][3] = 0;
}

//更新组合数数值
void Curve::updateCn() {
    int n = controlPoints.size()-1;
    Cn.erase(Cn.begin(),Cn.end());
    for(int i = 0;i <= n;i++) {
        int res = 1;
        for(int j = i+1;j <= n;j++) {
            res *= j;
        }
        for(int j = 1;j <= n-i;j++) {
            res /= j;
        }
        Cn.push_back(res);
    }
}
//根据U值计算贝塞尔曲线的点
QPoint Curve::getPoint(float u) {
    float x = 0,y=0;
    int n = Cn.size()-1;
    for(int i = 0;i <= n;i++) {
        float Bezn_i = Cn[i] * pow(u,i) * pow(1-u,n-i);
        x += Bezn_i * controlPoints[i].x();
        y += Bezn_i * controlPoints[i].y();
    }
    return  QPoint((int)x,(int)y);
}

void Curve::showAuxilaryPoints(QPixmap& pix) {
    Shape::showAuxilaryPoints(pix);
    if(type == 1) {
        QPen pen;
        pen.setStyle(Qt::DashLine);
        QPainter paint(&pix);
        paint.setPen(pen);
        for(int i = 0;i < auxilaryPoints.size()-1;i++) {
            paint.drawLine(auxilaryPoints[i],auxilaryPoints[i+1]);
        }
    }
}
