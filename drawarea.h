#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QPoint>
#include <QPaintEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QRubberBand>
#include <QVector>
#include "shape.h"
#include "CutArea.h"

namespace Ui {
class DrawArea;
}

class DrawArea : public QWidget
{
    Q_OBJECT

public:
    explicit DrawArea(QWidget *parent = 0);
    void loadImage(QString path); //加载图片
    bool saveImage();             //保存图片

    void draw_Line(bool flag);                          //开启\关闭直线鼠标绘制模式
    void draw_Line(QPoint startPoint, QPoint endPoint); //重载：通过参数绘制

    void draw_Circle(bool flag);                        //开启\关闭圆鼠标绘制模式
    void draw_Circle(int radius, QPoint center);        //重载：通过参数绘制

    void draw_Rectangle(bool flag);                     //开启\关闭矩形鼠标绘制模式
    void draw_Rectangle(QPoint point1,QPoint point2);   //重载：通过参数绘制

    void draw_Oval(bool flag);                          //开启\关闭椭圆鼠标绘制模式
    void draw_Oval(QPoint center,int a,int b,bool in_x);//重载：通过参数绘制

    void draw_Polygon(bool flag);                       //开启\关闭多边形鼠标绘制模式
    void draw_Curve(bool flag);                         //开启\关闭矩形的鼠标绘制模式

    void use_Rubber(bool flag);                         //开启\关闭橡皮使用模式
    void use_Brush(bool flag);                          //开启\关闭画刷使用模式
    void use_Filling(bool flag);                        //让画板开启或退出填充空白区域功能
    void cut_Area(bool flag);                           //让画板开启或退出裁剪区域

    void setInkColor();            //设置墨水颜色
    void setPenAttribute(int penWidth, int edgeStyle, int rubberSize);//设置画笔属性
    void getPenAttribute(int& penWidth, int& edgeStyle, int& rubberSize);//给出画笔属性

    void cancleOperation(); //撤销操作
    void restoreOperation();//恢复操作
    ~DrawArea();

protected:
/*各类事件的覆写*/
    void paintEvent(QPaintEvent* );
    void mousePressEvent(QMouseEvent* );
    void mouseMoveEvent(QMouseEvent* );
    void mouseReleaseEvent(QMouseEvent* );
    void resizeEvent(QResizeEvent* );
    void contextMenuEvent(QContextMenuEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    Ui::DrawArea *ui;
    QVector<QPixmap>PixQueue; //用来保存进行绘制操作之前的Pix，便于进行恢复和撤销操作
    int Pix_index;             //指向当前画面在队列中的位置
    QPixmap Pix;      //相当于“帧缓冲器"
    QColor inkColor;   //墨水颜色
    QPen pen;      //画笔
    int penWidth;
    int edgeStyle;
    int rubberSize;
    QPoint pressPoint;  //鼠标按下坐标
    QPoint lastPoint;   //鼠标按下时，持续移动的坐标
    QPoint endPoint;    //鼠标按下时，移动结束的坐标
    QPoint releasePoint; //鼠标松起的坐标
    QPoint currentPos;   //鼠标当前位置的坐标
    QRubberBand* rubberBand;
    Shape* currentShape; //指示刚刚绘制好的图元
    CutArea* cutArea;

    int delta_x;          //保存鼠标移动时的偏移量
    int delta_y;

    /*指示当前功能的变量*/
    bool isUsingRubber;
    bool isUsingBrush;
    bool isFilling;
    bool isCuttingArea;
    bool isCuttingLine;
    bool isDrawingLine;
    bool isDrawingCircle;
    bool isDrawingRectangle;
    bool isDrawingOval;
    bool isDrawPolygon;
    bool isDrawCurve;
    bool isReleased;
    bool isDraggingShape;
    bool isDraggingArea;
    bool isEditing;

    int auxilaryPointIndex;            //鼠标当前在哪个辅助点上，若不在任何辅助点上，-1

    void rotateCurrentShape(int degrees);           //旋转指定度数
    void rotateCurrentShape90();                    //旋转90度
    void rotateCurrentShape180();                   //旋转180度
    void rotateCurrentShape270();                   //旋转270度
    void rotateCurrentShapeInput();                 //通过对话框输入获取旋转度数
    void zoomCurrentShape();                        //图形缩放
    void zoomCutArea();                             //裁剪区域缩放
    void horizontalFilp();                          //水平翻转
    void verticalFilp();                            //垂直翻转
    void initPen();                                 //初始画笔的属性
    void addToPixQueue();                           //保存这次操作的结果到队列中
    void sendPos();                                 //发送信号postionChange
    void changeHappen();                            //发送信号unsavedChange
    void hideAuxilaryPoints();                      //隐藏辅助点
    void fixCutArea();                              //固定裁剪区域
    void deleteCutArea();                           //删除所选区域
    void cut_Line();                                //开启直线裁剪，只有当前图元为直线才有可能用到
    void TODO();
signals:
    void postionChange(const QPoint& pos); //信号：鼠标移动，坐标改变
    void unsavedChange(const bool& flag);  //信号：产生了未保存的改动
    void quitFunctionSignal();             //让paintProject退出所有功能
};

#endif // DRAWAREA_H
