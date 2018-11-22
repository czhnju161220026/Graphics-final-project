#ifndef PAINTPROJECT_H
#define PAINTPROJECT_H
#include <QDialog>
#include <QVBoxLayout>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMainWindow>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include "drawarea.h"

/*
* Todo: 1.catch closEvent
*       2.add paintEvent
*
*/

namespace Ui {
class PaintProject;
}

class PaintProject : public QDialog
{
    Q_OBJECT

public:
    explicit PaintProject(QWidget *parent = 0);
    void loadImage(QString path);               //加载图片，作为画布
    void saveImage();                           //保存图片
    /*相关功能函数*/
    void draw_Line();                            //绘制直线功能
    void draw_Circle();                          //绘制圆功能
    void draw_Rec();                             //绘制矩形功能
    void draw_Oval();                            //绘制椭圆功能
    void draw_Polygon();                         //绘制多边形
    void use_Rubber();                           //使用橡皮擦
    void use_Brush();                            //使用画刷
    void use_Filling();                          //使用填充空白区域功能
    void cut_Area();                             //裁剪区域
    void setInkColor();                          //设置墨水颜色
    void setAttribute();                         //设置画刷属性
    void showInputDialog();                      //展示参数输入对话框
    void cancleOperation();                      //撤销
    void restoreOperation();                     //恢复
    void displayPosInStatusBar(QPoint pos);      //实时显示鼠标位置
    ~PaintProject();

private:
    Ui::PaintProject *ui;
    QToolBar* myToolbar;
    QMainWindow* mainWindow; //内嵌窗口
    DrawArea* drawArea;        //画板
    QAction *drawLine, *drawCurve, *drawCircle, *drawOval, *drawRec,*drawPolygon,*rubber,*brush,*fillBlank,
    *getColor,*setPenAttribute,*cutArea,*edit,*cancle,*restore,*save, *question; //Action变量
    void TODO();
    void closeEvent(QCloseEvent* event);  //重写的关闭事件，提示用户保存
    bool dirty;                            //指示是否发生过修改
    /*指示使用何种功能*/
    bool isUsingRubber;
    bool isUsingBrush;
    bool isFilling;
    bool isDrawingLine;
    bool isDrawingCircle;
    bool isDrawingRectangle;
    bool isDrawingOval;
    bool isDrawingPolygon;
    bool isCuttingArea;
    void setDirty(bool flag);           //修改dirty变量

    void quitAllFunctions();          //退出所有功能

};

#endif // PAINTPROJECT_H
