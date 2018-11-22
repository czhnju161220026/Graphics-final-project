#include "shape.h"
#include "QDebug"
#include <cmath>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QBrush>
Shape::Shape()
{
    attribute = SHAPE;

}

void Shape::draw(QPen& pen, QPixmap& Pix){}
bool Shape::inDraggingArea(int x, int y){}
void Shape::move(int delta_x, int delta_y) {}


void Shape::addRotateDegrees(int degrees) {
    ; //do nothing
}

void Shape::zoom(double ratio) {
    ; //do nothing
}

int Shape::getAttribute() {
    return attribute;
}

void Shape::addNextPoint(QPoint point, QPen &pen, QPixmap &Pix) {
    ;
}

void Shape::finish(QPen &pen, QPixmap &Pix) {
    ;
}

bool Shape::isFinished(){
    return false;
}

void Shape::showAuxilaryPoints(QPixmap& Pix) {
    QPainter painter(&Pix);
    QBrush* qbrush = new QBrush(QColor(32,74,135),Qt::SolidPattern);
    painter.setBrush(*qbrush);
    for(int i = 0;i<auxilaryPoints.size();i++) {
        painter.drawRect(auxilaryPoints[i].x()-4,auxilaryPoints[i].y()-4,8,8);
    }
}

int Shape::inAuxilaryPoint(int x, int y) {
    int index =-1;
    for(int i = 0;i<auxilaryPoints.size();i++) {
        if(abs(x-auxilaryPoints[i].x())<=3&&abs(y-auxilaryPoints[i].y())<=3) {
            index = i;
            qDebug() << "In auxilaryPoint:"<<i;
            break;
        }
    }
    return index;
}

void Shape::moveAuxilaryPoint(int index,int delta_x, int delta_y) {
    ;
}

void Shape::horizontalFilp() {
    ;
}

void Shape::verticalFilp() {
    ;
}
