#ifndef FILLING_H
#define FILLING_H
#include <QPoint>
#include <QColor>
#include <QPixmap>
#include <QImage>
#include <QStack>
#include <QDebug>
#include <QQueue>
#include <assert.h>

void fillTheBlank(QPoint seed, QColor color,QPixmap &Pix) {
    qDebug() << "fill Area:" <<seed.x()<<","<<seed.y()<<" with color :"<<color.red()<<" "<<color.green()<<" "<<color.blue();
    //QColor重载了比较操作符，所以可以直接进行比较
    QImage image = Pix.toImage();
    qDebug()<<image.width()<<","<<image.height();
    QColor blank = image.pixelColor(seed);
    if(color == blank) {
        return;
    }
    int start_x = seed.x();
    for(int i = seed.x();i>=0;i--) {
        if(image.pixelColor(i,seed.y())==blank) {
            start_x = i;
        }
        else {
            break;
        }
    }
    seed.setX(start_x);
    qDebug()<<seed.x();
    QStack<QPoint> stack;
    stack.push(seed);
    while(!stack.empty()) {
        QPoint start = stack.back();
        stack.pop();
        int begin = start.x();
        int end = start.x();
        int y = start.y();
        assert(begin>=0 && y>=0);
        for(int i = begin;i < image.width();i++) {
            if(image.pixelColor(i,y) == blank) {
                image.setPixelColor(i,y,color);
                end = i;
            }
            else {
                break;
            }
        }

        int previous_y = y-1;
        int next_y = y+1;

        if(previous_y>=0) {
            int previous_x = 0;
            bool flag = false;
            for(int i = begin;i<=end;i++) {
                if(image.pixelColor(i,previous_y)==blank) {
                    if(!flag) {
                        previous_x = i;
                        for(;previous_x>=0;previous_x--) {
                            if(image.pixelColor(previous_x,previous_y)!=blank) {
                                break;
                            }
                        }
                        previous_x++;
                        assert(previous_x>=0);
                        stack.push(QPoint(previous_x,previous_y));
                        flag = true;
                    }
                }
                else {
                    flag = false;
                }

            }
        }

        if(next_y<image.height()) {
            int next_x = 0;
            bool flag = false;
            for(int i =begin; i <= end;i++) {
                if(image.pixelColor(i,next_y)==blank) {
                    if(!flag) {
                        next_x = i;
                        for(;next_x>=0;next_x--) {
                            if(image.pixelColor(next_x,next_y)!=blank) {

                                break;
                            }
                        }
                        next_x++;
                        assert(next_x>=0);
                        stack.push(QPoint(next_x,next_y));
                        flag = true;
                    }
                }
                else {
                    flag = false;
                }
            }
        }
    }
    Pix = QPixmap::fromImage(image);
    qDebug() << Pix.width()<<","<<Pix.height();
    return ;
}

#endif // FILLING_H
