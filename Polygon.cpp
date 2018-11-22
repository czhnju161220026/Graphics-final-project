#include "Polygon.h"
#include "line.h"
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QPainter>

Polygon::Polygon() {
    qDebug() <<"New Polygon";
    attribute = POLYGON;
    finished = false;
    rotateDegrees = 0;
}

Polygon::Polygon(QVector<QPoint> points) {
    for(int i = 0;i<points.size();i++) {
        this->points.push_back(points[i]);
        auxilaryPoints.push_back(points[i]);
    }
    attribute = POLYGON;
    finished = false;
    rotateDegrees = 0;
}

void Polygon::addNextPoint(QPoint point,QPen& pen, QPixmap& Pix) {
    qDebug() <<"add point "<<point;
    this->points.push_back(point);
    auxilaryPoints.push_back(point);
    if(this->points.size() > 1) {
        qDebug()<<"draw"<<points[points.size()-2]<<","<<points[points.size()-1];
        Line* newLine = new Line(points[points.size()-2],points[points.size()-1]);
        newLine->draw(pen,Pix);
        QPainter painter(&Pix);
        painter.setPen(pen);
        painter.drawLine(points[points.size()-2],points[points.size()-1]);
    }
    else {
        QPainter painter(&Pix);
        painter.setPen(pen);
        painter.drawPoint(point.x(),point.y());
    }
}

void Polygon::finish(QPen& pen, QPixmap& Pix) {
    qDebug() << "finish drawing polygon";
    finished = true;
    Line* newLine = new Line(points[points.size()-1],points[0]);
    newLine->draw(pen, Pix);
    //find the center
    //TODO:
    initConner();
}

bool Polygon::isFinished() {
    return finished;
}

void Polygon::draw(QPen &pen, QPixmap &Pix) {
    qDebug() <<"Draw poly";
    QPainter painter(&Pix);
    painter.setPen(pen);
    for(int i = 0;i<points.size()-1;i++) {
        /*Line * newLine = new Line(points[i], points[i+1]);
        newLine->draw(pen,Pix);*/
        painter.drawLine(points[i],points[i+1]);
    }
    painter.drawLine(points[points.size()-1],points[0]);
    /*Line* newLine = new Line(points[points.size()-1],points[0]);
    newLine->draw(pen,Pix);*/
}

bool Polygon::inDraggingArea(int x, int y) {
    int xmin = upperLeftConner.x();
    int xmax = bottomRightConner.x();
    int ymin = upperLeftConner.y();
    int ymax = bottomRightConner.y();
    if((x-xmin)*(x-xmax)<=0 && (y-ymin)*(y-ymax)<=0) {
        return true;
    }
    return false;

}

void Polygon::addRotateDegrees(int degrees) {
    rotateDegrees=(rotateDegrees+degrees)%360;
    double rad = ((double)degrees*3.14159265358) / 180.0;
    double COS = cos(rad);
    double SIN = sin(rad);
    int xr = center.x();
    int yr = center.y();
    for(int i = 0;i<points.size();i++) {
        int oldX = points[i].x();
        int oldY = points[i].y();
        points[i].setX(xr + (oldX-xr)*COS - (oldY - yr)*SIN);
        points[i].setY(yr + (oldX-xr)*SIN + (oldY - yr)*COS);
    }
    for(int i = 0;i<points.size();i++) {
        auxilaryPoints[i] = points[i];
    }
    initConner();
}

void Polygon::move(int delta_x, int delta_y) {
    qDebug()<<"Move "<<delta_x<<","<<delta_y;
    for(int i = 0;i<points.size();i++) {
        points[i].setX(points[i].x()+delta_x);
        points[i].setY(points[i].y()+delta_y);
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

void Polygon::zoom(double ratio) {
    int xr = center.x();
    int yr = center.y();
    for(int i = 0;i<points.size();i++) {
        int oldX = points[i].x();
        int oldY = points[i].y();
        points[i].setX(xr + ratio*(oldX-xr));
        points[i].setY(yr + ratio*(oldY-yr));
    }
    for(int i = 0;i<points.size();i++) {
        auxilaryPoints[i] = points[i];
    }
    initConner();
}

void Polygon::moveAuxilaryPoint(int index, int delta_x, int delta_y) {
    auxilaryPoints[index].setX(delta_x+auxilaryPoints[index].x());
    auxilaryPoints[index].setY(delta_y+auxilaryPoints[index].y());

    points[index].setX(delta_x+points[index].x());
    points[index].setY(delta_y+points[index].y());

    initConner();
}

void Polygon::initConner() {
    int minY,maxY,minX,maxX;
    minY=points[0].y();
    maxY=points[0].y();
    minX=points[0].x();
    maxX=points[0].x();

    for(int i = 0;i<points.size();i++) {
        if(points[i].x()<minX) {
            minX = points[i].x();
        }
        if(points[i].x()>maxX) {
            maxX = points[i].x();
        }
        if(points[i].y()<minY) {
            minY = points[i].y();
        }
        if(points[i].y()>maxY) {
            maxY = points[i].y();
        }
    }

    upperLeftConner.setX(minX);
    upperLeftConner.setY(minY);
    bottomRightConner.setX(maxX);
    bottomRightConner.setY(maxY);
    center.setX((minX+maxX)/2);
    center.setY((minY+maxY)/2);
}

void Polygon::horizontalFilp() {
    for(int i=0;i<points.size();i++) {
        points[i].setX(2*center.x()-points[i].x());
        auxilaryPoints[i] = points[i];
    }
    initConner();
}

void Polygon::verticalFilp() {
    for(int i =0;i<points.size();i++) {
        points[i].setY(2*center.y()-points[i].y());
        auxilaryPoints[i] = points[i];
    }
    initConner();
}
