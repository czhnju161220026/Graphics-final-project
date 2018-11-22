#ifndef CUTAREA_H
#define CUTAREA_H
#include <QDebug>
#include <QPen>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QPoint>

class CutArea
{
private:
    QPoint upperLeftPoint;
    int width;
    int height;
    QImage* cutImage;
public:
    CutArea(QPoint startpoint,QPoint endpoint,QPixmap& Pix);
    QPoint getUpperLeftPoint();
    int getWidth();
    int getHeight();
    QImage getImage();
    bool inDraggingArea(int x, int y);
    void move(int delta_x, int delta_y);
    void fixImage(QPixmap& Pix);
    void zoom(double ratio);
    void horizontalFilp();
    void verticalFilp();
};

#endif // CUTAREA_H
