#include "CutArea.h"
#include <QBrush>
#include <cmath>

CutArea::CutArea(QPoint startpoint, QPoint endpoint, QPixmap &Pix) {
    //预留2像素宽的边界
    this->width = abs(startpoint.x()-endpoint.x())+4;
    this->height = abs(startpoint.y()-endpoint.y())+4;
    int xmin = (startpoint.x()< endpoint.x())?startpoint.x():endpoint.x()-2;
    int ymin = (startpoint.y()< endpoint.y())?startpoint.y():endpoint.y()-2;
    this->upperLeftPoint.setX(xmin);
    this->upperLeftPoint.setY(ymin);

    cutImage = new QImage(this->width,this->height,QImage::Format_RGB32);
    cutImage->fill(Qt::red);
    QImage rawImage = Pix.toImage();
    for(int i = 0;i<width-4;i++) {
        for(int j = 0;j<height-4;j++) {
            QColor color = rawImage.pixel(upperLeftPoint.x()+i,upperLeftPoint.y()+j);
            cutImage->setPixelColor(i+2,j+2,color);
        }
    }
    QPainter painter(&Pix);
    QColor White(Qt::white);
    QBrush brush(White);
    painter.fillRect(xmin,ymin,width,height,brush);
    qDebug() <<"Cut Area init success";

}

QPoint CutArea::getUpperLeftPoint() {
    return this->upperLeftPoint;
}

int  CutArea::getHeight() {
    return this->height;
}

int CutArea::getWidth() {
    return this->width;
}

QImage CutArea::getImage() {
    return *(this->cutImage);
}

bool CutArea::inDraggingArea(int x, int y) {
    return (x>upperLeftPoint.x()&&x<upperLeftPoint.x()+width&&y>upperLeftPoint.y()&&y<upperLeftPoint.y()+height);
}

void CutArea::move(int delta_x, int delta_y) {
    qDebug() <<"move" <<delta_x<<","<<delta_y;
    upperLeftPoint.setX(upperLeftPoint.x()+delta_x);
    upperLeftPoint.setY(upperLeftPoint.y()+delta_y);
}

void CutArea::fixImage(QPixmap &Pix) {
     QImage rawImage = Pix.toImage();
     for(int i = 0;i<width-4;i++) {
         for(int j = 0;j<height-4;j++) {
             QColor color = cutImage->pixel(i+2,j+2);
             rawImage.setPixelColor(upperLeftPoint.x()+i,upperLeftPoint.y()+j,color);
         }
     }
     Pix = QPixmap::fromImage(rawImage);
}

void CutArea::zoom(double ratio) {
    qDebug() <<"Ratio:"<<ratio;
    QPoint center;
    center.setX(upperLeftPoint.x()+width/2);
    center.setY(upperLeftPoint.y()+height/2);
    int old_width =this->width-4;
    int old_height = this->height-4;
    QImage* newImage = new QImage(old_width,old_height,QImage::Format_RGB32);
    for(int i = 0;i<width-4;i++) {
        for(int j = 0;j<height-4;j++) {
            QColor color = cutImage->pixel(i+2,j+2);
            newImage->setPixelColor(i,j,color);
        }
    }
    QImage afterScaleImage = newImage->scaled(old_width*ratio,old_height*ratio);
    this->width = afterScaleImage.width()+4;
    this->height = afterScaleImage.height()+4;
    delete newImage;
    delete cutImage;
    cutImage = new QImage(width,height,QImage::Format_RGB32);
    cutImage->fill(Qt::red);
    for(int i = 0;i<width-4;i++) {
        for(int j = 0;j<height-4;j++) {
            QColor color = afterScaleImage.pixel(i,j);
            cutImage->setPixelColor(i+2,j+2,color);
        }
    }
    upperLeftPoint.setX(center.x()-this->width/2);
    upperLeftPoint.setY(center.y()-this->height/2);
}

void CutArea::horizontalFilp() {
    for(int i = 0;i<height;i++) {
        for(int j = 0;j<width/2;j++) {
            QColor color = cutImage->pixelColor(j,i);
            cutImage->setPixelColor(j,i,cutImage->pixelColor(width-1-j,i));
            cutImage->setPixelColor(width-1-j,i,color);
        }
    }
}

void CutArea::verticalFilp() {
    for(int i = 0;i<width;i++) {
        for(int j =0;j<height/2;j++) {
            QColor color = cutImage->pixelColor(i,j);
            cutImage->setPixelColor(i,j,cutImage->pixelColor(i,height-1-j));
            cutImage->setPixelColor(i,height-1-j,color);
        }
    }
}
