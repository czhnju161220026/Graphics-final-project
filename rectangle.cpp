#include "rectangle.h"
#include "line.h"
#include <cmath>
#include <QVector>
rectangle::rectangle() {
    point1 = QPoint(0,0);
    point2 = QPoint(0,0);
    attribute = RECTANGLE;
    rotateDegrees = 0;
}

rectangle::rectangle(QPoint point1, QPoint point2) {
    this->point1 = point1;
    this->point2 = point2;
    attribute = RECTANGLE;
    point3 = QPoint(point1.x(),point2.y());
    point4 = QPoint(point2.x(),point1.y());
    center.setX((point1.x()+point2.x())/2);
    center.setY((point1.y()+point2.y())/2);

    QPoint temp = this->point2;
    this->point2 = point4;
    this->point4 = point3;
    this->point3 = temp;

    auxilaryPoints.push_back(this->point1);
    auxilaryPoints.push_back(this->point2);
    auxilaryPoints.push_back(this->point3);
    auxilaryPoints.push_back(this->point4);

    rotateDegrees = 0;
}

void rectangle::draw(QPen &pen, QPixmap &Pix) {
    line1 = Line(point1,point2);
    line2 = Line(point2,point3);
    line3 = Line(point3,point4);
    line4 = Line(point4,point1);

    line1.draw(pen,Pix);
    line2.draw(pen,Pix);
    line3.draw(pen,Pix);
    line4.draw(pen,Pix);
}

bool rectangle::inDraggingArea(int x, int y) {
    if((x-auxilaryPoints[0].x())*(x-auxilaryPoints[2].x())<=0 && (y-auxilaryPoints[0].y())*(y-auxilaryPoints[2].y())<=0) {
        return true;
    }
    else {
        return false;
    }
}

void rectangle::move(int delta_x, int delta_y) {
    point1.setX(point1.x()+delta_x);
    point1.setY(point1.y()+delta_y);
    point2.setX(point2.x()+delta_x);
    point2.setY(point2.y()+delta_y);
    point3.setX(point3.x()+delta_x);
    point3.setY(point3.y()+delta_y);
    point4.setX(point4.x()+delta_x);
    point4.setY(point4.y()+delta_y);

    for(int i = 0;i<auxilaryPoints.size();i++) {
        int oldX = auxilaryPoints[i].x();
        int oldy = auxilaryPoints[i].y();
        auxilaryPoints[i].setX(oldX+delta_x);
        auxilaryPoints[i].setY(oldy+delta_y);
    }

}

void rectangle::addRotateDegrees(int degrees) {
    rotateDegrees=(rotateDegrees+degrees)%360;
    int xr = (point1.x()+point2.x())/2;
    int yr = (point1.y()+point2.y())/2;

    double rad = ((double)degrees*3.14159265358) / 180.0;
    double COS = cos(rad);
    double SIN = sin(rad);

    int x1 = point1.x(),x2=point2.x(),x3=point3.x(),x4=point4.x();
    int y1 = point1.y(),y2=point2.y(),y3=point3.y(),y4=point4.y();

    point1.setX(xr + (x1-xr)*COS - (y1-yr)*SIN);
    point1.setY(yr + (x1-xr)*SIN + (y1-yr)*COS);
    point2.setX(xr + (x2-xr)*COS - (y2-yr)*SIN);
    point2.setY(yr + (x2-xr)*SIN + (y2-yr)*COS);
    point3.setX(xr + (x3-xr)*COS - (y3-yr)*SIN);
    point3.setY(yr + (x3-xr)*SIN + (y3-yr)*COS);
    point4.setX(xr + (x4-xr)*COS - (y4-yr)*SIN);
    point4.setY(yr + (x4-xr)*SIN + (y4-yr)*COS);

    center.setX(xr);
    center.setY(yr);

    QVector<int>x;QVector<int>y;
    x.push_back(point1.x());x.push_back(point2.x());x.push_back(point3.x());x.push_back(point4.x());
    y.push_back(point1.y());y.push_back(point2.y());y.push_back(point3.y());y.push_back(point4.y());
    int xmin,xmax,ymin,ymax;
    xmin=x[0];xmax=x[0];ymin=y[0],ymax=y[0];
    for(int i=1;i<4;i++) {
        if(x[i]>xmax) {
            xmax=x[i];
        }
        if(x[i]<xmin) {
            xmin=x[i];
        }
        if(y[i]>ymax) {
            ymax = y[i];
        }
        if(y[i]<ymin) {
            ymin =y[i];
        }
    }

    auxilaryPoints[0].setX(xmin);auxilaryPoints[0].setY(ymin);
    auxilaryPoints[1].setX(xmax);auxilaryPoints[1].setY(ymin);
    auxilaryPoints[2].setX(xmax);auxilaryPoints[2].setY(ymax);
    auxilaryPoints[3].setX(xmin);auxilaryPoints[3].setY(ymax);
}

void rectangle::zoom(double ratio) {
    int x1 = point1.x(),y1 = point1.y();
    int x2 = point2.x(),y2 = point2.y();
    int x3 = point3.x(),y3 = point3.y();
    int x4 = point4.x(),y4 = point4.y();

    point1.setX(center.x() + (x1 - center.x())*ratio); point1.setY(center.y() + (y1 - center.y())*ratio);
    point2.setX(center.x() + (x2 - center.x())*ratio); point2.setY(center.y() + (y2 - center.y())*ratio);
    point3.setX(center.x() + (x3 - center.x())*ratio); point3.setY(center.y() + (y3 - center.y())*ratio);
    point4.setX(center.x() + (x4 - center.x())*ratio); point4.setY(center.y() + (y4 - center.y())*ratio);

    x1 = auxilaryPoints[0].x(),y1 = auxilaryPoints[0].y();
    x2 = auxilaryPoints[1].x(),y2 = auxilaryPoints[1].y();
    x3 = auxilaryPoints[2].x(),y3 = auxilaryPoints[2].y();
    x4 = auxilaryPoints[3].x(),y4 = auxilaryPoints[3].y();

    auxilaryPoints[0].setX(center.x()+(x1-center.x())*ratio);
    auxilaryPoints[1].setX(center.x()+(x2-center.x())*ratio);
    auxilaryPoints[2].setX(center.x()+(x3-center.x())*ratio);
    auxilaryPoints[3].setX(center.x()+(x4-center.x())*ratio);

    auxilaryPoints[0].setY(center.y()+(y1-center.y())*ratio);
    auxilaryPoints[1].setY(center.y()+(y2-center.y())*ratio);
    auxilaryPoints[2].setY(center.y()+(y3-center.y())*ratio);
    auxilaryPoints[3].setY(center.y()+(y4-center.y())*ratio);
}

void rectangle::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    if(rotateDegrees%90==0) {
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
        point1 = auxilaryPoints[0];point2=auxilaryPoints[1];
        point3 = auxilaryPoints[2];point4=auxilaryPoints[3];
    }
}

void rectangle::showAuxilaryPoints(QPixmap &Pix) {
    if(rotateDegrees%90==0) {
        Shape::showAuxilaryPoints(Pix);
    }
}

int rectangle::inAuxilaryPoint(int x, int y) {
    if(rotateDegrees%90==0) {
        return Shape::inAuxilaryPoint(x,y);
    }
    else {
        return -1;
    }
}
void rectangle::horizontalFilp() {
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

void rectangle::verticalFilp() {
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

