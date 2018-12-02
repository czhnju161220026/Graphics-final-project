#include "paintproject.h"
#include "ui_paintproject.h"
#include "penAttribute.h"
#include "inputdialog.h"
#include "inputline.h"
#include "inputcircle.h"
#include "inputrectangle.h"
#include "inputoval.h"
#include "shape.h"
#include <QChar>


PaintProject::PaintProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaintProject)
{
    ui->setupUi(this);
    setWindowTitle(tr("New Project"));

    setWindowFlags(Qt::Window);
    mainWindow = new QMainWindow();

    //add statusbar
    mainWindow->statusBar();
    //add toolbar
    myToolbar = new QToolBar();
    myToolbar->setMovable(false);
    myToolbar->setIconSize(QSize(39,39));
    myToolbar->setFixedHeight(40);
    mainWindow->addToolBar(Qt::TopToolBarArea,myToolbar);
    //set layout
    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(mainWindow);
    setLayout(layout);
    //add paint area
    drawArea = new DrawArea();
    drawArea->setGeometry(0,0,1530,800);
    mainWindow->setCentralWidget(drawArea);
    //paintArea->setStyleSheet("background-color:white");
    //add the painter

    /*
    *为QAction添加相应的触发时行为
    */
    drawLine = new QAction(QIcon(":/images/line"),tr("绘制直线..."),myToolbar);
    drawLine->setStatusTip(tr("绘制直线..."));
    drawLine->setToolTip(tr("绘制直线..."));
    drawLine->setCheckable(true);
    connect(drawLine,&QAction::triggered,this,&PaintProject::draw_Line);
    myToolbar->addAction(drawLine);

    drawCurve = new QAction(QIcon(":/images/curve"),tr("绘制曲线..."),myToolbar);
    drawCurve->setStatusTip(tr("绘制曲线..."));
    drawCurve->setToolTip(tr("绘制曲线..."));
    drawCurve->setCheckable(true);
    connect(drawCurve,&QAction::triggered,this,&PaintProject::draw_Curve);
    myToolbar->addAction(drawCurve);

    drawCircle = new QAction(QIcon(":/images/circle"),tr("绘制圆..."),myToolbar);
    drawCircle->setStatusTip(tr("绘制圆..."));
    drawCircle->setToolTip(tr("绘制圆..."));
    drawCircle->setCheckable(true);
    connect(drawCircle,&QAction::triggered,this,&PaintProject::draw_Circle);
    myToolbar->addAction(drawCircle);

    drawOval = new QAction(QIcon(":/images/oval"),tr("绘制椭圆..."),myToolbar);
    drawOval->setStatusTip(tr("绘制椭圆..."));
    drawOval->setToolTip(tr("绘制椭圆..."));
    drawOval->setCheckable(true);
    connect(drawOval,&QAction::triggered,this,&PaintProject::draw_Oval);
    myToolbar->addAction(drawOval);

    drawRec = new QAction(QIcon(":/images/rec"),tr("绘制矩形..."),myToolbar);
    drawRec->setStatusTip(tr("绘制矩形..."));
    drawRec->setToolTip(tr("绘制矩形..."));
    drawRec->setCheckable(true);
    connect(drawRec,&QAction::triggered,this,&PaintProject::draw_Rec);
    myToolbar->addAction(drawRec);

    drawPolygon = new QAction(QIcon(":/images/polygon"),tr("绘制多边形"),myToolbar);
    drawPolygon->setStatusTip(tr("绘制多边形"));
    drawPolygon->setToolTip(tr("绘制多边形"));
    drawPolygon->setCheckable(true);
    connect(drawPolygon,&QAction::triggered,this,&PaintProject::draw_Polygon);
    myToolbar->addAction(drawPolygon);


    rubber = new QAction(QIcon(":/images/rubber"),tr("橡皮擦..."),myToolbar);
    rubber->setStatusTip(tr("橡皮擦..."));
    rubber->setToolTip(tr("橡皮擦..."));
    rubber->setCheckable(true);
    connect(rubber,&QAction::triggered,this,&PaintProject::use_Rubber);
    myToolbar->addAction(rubber);

    brush = new QAction(QIcon(":/images/brush"),tr("Brush"),myToolbar);
    brush->setStatusTip(tr("使用画刷"));
    brush->setToolTip("使用画刷");
    brush->setCheckable(true);
    connect(brush,&QAction::triggered,this,&PaintProject::use_Brush);
    myToolbar->addAction(brush);

    fillBlank = new QAction(QIcon(":/images/filling"),tr("FillBlank"),myToolbar);
    fillBlank->setStatusTip(tr("使用填充空白区域功能"));
    fillBlank->setToolTip(tr("使用填充空白区域功能"));
    fillBlank->setCheckable(true);
    connect(fillBlank,&QAction::triggered,this,&PaintProject::use_Filling);
    myToolbar->addAction(fillBlank);

    getColor = new QAction(QIcon(":/images/color"),tr("设置墨水颜色..."),myToolbar);
    getColor->setStatusTip("设置墨水颜色...");
    getColor->setToolTip("设置墨水颜色...");
    connect(getColor,&QAction::triggered,this,&PaintProject::setInkColor);
    myToolbar->addAction(getColor);

    setPenAttribute = new QAction(QIcon(":/images/attribute"),tr("设置画笔的粗细,形式,橡皮大小等属性。"),myToolbar);
    setPenAttribute->setStatusTip("设置画笔的粗细，形式,橡皮大小等属性。");
    setPenAttribute->setToolTip("设置画笔的粗细，形式,橡皮大小等属性。");
    connect(setPenAttribute,&QAction::triggered,this,&PaintProject::setAttribute);
    myToolbar->addAction(setPenAttribute);

    cutArea = new QAction(QIcon(":/images/cut"),tr("裁剪区域..."),myToolbar);
    cutArea->setStatusTip(tr("裁剪区域..."));
    cutArea->setToolTip(tr("裁剪区域..."));
    connect(cutArea,&QAction::triggered,this,&PaintProject::cut_Area);
    cutArea->setCheckable(true);
    myToolbar->addAction(cutArea);

    edit = new QAction(QIcon(":/images/edit"),tr("编辑功能"),myToolbar);
    edit->setStatusTip("通过参数绘制图形");
    edit->setToolTip(tr("通过参数绘制图形"));
    connect(edit,&QAction::triggered,this,&PaintProject::showInputDialog);
    myToolbar->addAction(edit);

    cancle = new QAction(QIcon(":/images/cancle"),tr("撤销"),myToolbar);
    cancle->setStatusTip(tr("撤销上一步操作...(Ctrl+Z)"));
    cancle->setToolTip(tr("撤销上一步操作..."));
    cancle->setShortcut(QKeySequence::Undo);
    connect(cancle,&QAction::triggered,this,&PaintProject::cancleOperation);
    myToolbar->addAction(cancle);

    restore = new QAction(QIcon(":/images/restore"),tr("恢复"),myToolbar);
    restore->setStatusTip(tr("恢复上一次撤销...(Ctrl+Shift+Z)"));
    restore->setToolTip(tr("恢复上一次撤销"));
    restore->setShortcut(QKeySequence::Redo);
    connect(restore,&QAction::triggered,this,&PaintProject::restoreOperation);
    myToolbar->addAction(restore);

    save = new QAction(QIcon(":/images/save"),tr("save image..."),myToolbar);
    save->setStatusTip(tr("save image...(Ctrl+S)"));
    save->setToolTip(tr("save image..."));
    save->setShortcut(QKeySequence::Save);
    connect(save,&QAction::triggered,this,&PaintProject::saveImage);
    myToolbar->addAction(save);

    question = new QAction(QIcon(":/images/question"),tr("Help"),myToolbar);
    question->setStatusTip(tr("How to use..."));
    question->setToolTip(tr("How to use..."));
    connect(question,&QAction::triggered,this,&PaintProject::TODO);
    myToolbar->addAction(question);
    //初始化成员变量
    dirty = false;
    isUsingRubber = false;
    isUsingBrush = false;
    isFilling = false;
    isDrawingLine = false;
    isDrawingCircle = false;
    isDrawingRectangle = false;
    isDrawingOval = false;
    isDrawingPolygon = false;
    isDrawingCurve = false;
    isCuttingArea = false;

    connect(drawArea,&DrawArea::postionChange,this,&PaintProject::displayPosInStatusBar);
    connect(drawArea,&DrawArea::unsavedChange,this,&PaintProject::setDirty);
    connect(drawArea,&DrawArea::quitFunctionSignal,this,&PaintProject::quitAllFunctions);
}

PaintProject::~PaintProject()
{
    delete ui;
}
//TODO
void PaintProject::TODO() {
    QMessageBox::information(this,tr("Information"),tr("该功能待补充！"));
}

//重写的关闭事件，提示用户进行保存
void PaintProject::closeEvent(QCloseEvent *event) {
    if(!dirty) {
        event->accept();
        return;
    }
    switch (QMessageBox::information(this,tr("Information"),tr("您确定要关闭该项目吗？关闭会丢弃所有未保存的改动。"),tr("&YES"),tr("&NO"),0,1)) {
    case 0:
        event->accept();
        break;
    default:
        event->ignore();
        break;
    }
}
//调用画板，加载图片
void PaintProject::loadImage(QString path) {
    drawArea->loadImage(path);
}
//保存图片
void PaintProject::saveImage() {
    if(drawArea->saveImage()) {
        setDirty(false);
    }
}
//设置是否有改动
void PaintProject::setDirty(bool flag) {
    if((!flag)&&dirty) {
        QString title =this->windowTitle();
        QString newTitle="";
        for(int i = 2;i < title.length();i++) {
           newTitle.append(title.at(i));
        }
        this->setWindowTitle(newTitle);
        dirty = false;
    }
    else if(flag&&(!dirty)) {
        QString title =this->windowTitle();
        this->setWindowTitle("* "+title);
        dirty = true;
    }
}
//设置墨水颜色
void PaintProject::setInkColor() {
    drawArea->setInkColor();
}
//设置画刷属性
void PaintProject::setAttribute() {
    int penWidth=2,edgeStyle=0,rubberSize =8;
    drawArea->getPenAttribute(penWidth,edgeStyle,rubberSize);
    penAttribute p(this,penWidth,edgeStyle,rubberSize);
    p.exec();
    p.getPenAttribute(penWidth,edgeStyle,rubberSize);
   // qDebug()<<penWidth<<" "<<edgeStyle;
    drawArea->setPenAttribute(penWidth,edgeStyle,rubberSize);
}

//展示输入对话框
void PaintProject::showInputDialog() {
    quitAllFunctions();
    InputDialog select(this);
    select.exec();
    int shape = select.getShapeCategory();//获取图形种类
    switch (shape) {
        case LINE:{
            qDebug()<<"input Line";
            inputLine input(this);  //展示直线参数输入对话框
            input.setRange(drawArea->width(),drawArea->height());
            input.exec();
            QPoint startPoint,endPoint;
            if(input.getPoint(startPoint,endPoint)){ //如果Accept，绘制
                qDebug()<<"Input accepted";
                drawArea->draw_Line(startPoint,endPoint);
                setDirty(true);
            }
            break;
        }

        case CIRCLE:{
            qDebug()<<"Input Circle";
            InputCircle input(this); //展示圆参数输入对话框
            input.setRange(drawArea->width(),drawArea->height());
            input.exec();
            QPoint center;
            int radius;
            if(input.getCircle(radius,center)) { //如果Accept，绘制
                qDebug()<<"Input accepted";
                drawArea->draw_Circle(radius,center);
                setDirty(true);
            }
            break;
        }

        case RECTANGLE:{
            qDebug() <<"input a rectangle";
            InputRectangle input(this);
            input.setRange(drawArea->width(),drawArea->height());
            input.exec();
            QPoint point1,point2;
            if(input.getRectangle(point1,point2)) {
                qDebug()<<"Input accepted";
                drawArea->draw_Rectangle(point1,point2);
                setDirty(true);
            }
            break;
        }

        case OVAL: {
            qDebug()<<"input oval";
            InputOval input(this);
            input.setRange(drawArea->width(),drawArea->height());
            input.exec();
            QPoint center;
            int double_a, double_b;
            bool in_x;
            if(input.getOval(center,double_a,double_b,in_x)) {
                qDebug() <<"input accepted";
                drawArea->draw_Oval(center,double_a,double_b,in_x);
                setDirty(true);
            }

            break;
        }
    }
}

//让画板开启或退出直线绘制模式
void PaintProject::draw_Line() {
    //drawArea->draw_Line();
    if(!isDrawingLine) {
        quitAllFunctions();
        drawArea->draw_Line(true);
        isDrawingLine = true;
    }
    else {
        drawArea->draw_Line(false);
        isDrawingLine = false;
    }
    //setDirty(true);
}

//让画板开启或退出圆绘制模式
void PaintProject::draw_Circle() {
    if(!isDrawingCircle) {
        quitAllFunctions();
        drawArea->draw_Circle(true);
        isDrawingCircle = true;
    }
    else {
        drawArea->draw_Circle(false);
        isDrawingCircle = false;
    }

    //setDirty(true);
}
//绘制矩形模式
void PaintProject::draw_Rec() {
    //TODO();
    if(!isDrawingRectangle) {
        quitAllFunctions();
        drawArea->draw_Rectangle(true);
        isDrawingRectangle = true;
    }
    else {
        drawArea->draw_Rectangle(false);
        isDrawingRectangle = false;
    }

    //setDirty(true);
}
//绘制椭圆模式
void PaintProject::draw_Oval() {
    if(!isDrawingOval) {
        quitAllFunctions();
        drawArea->draw_Oval(true);
        isDrawingOval = true;
    }
    else {
        drawArea->draw_Oval(false);
        isDrawingOval = false;
    }
}

//绘制多边形模式
void PaintProject::draw_Polygon() {
    if(!isDrawingPolygon) {
        quitAllFunctions();
        drawArea->draw_Polygon(true);
        isDrawingPolygon = true;
    }
    else {
        drawArea->draw_Polygon(false);
        isDrawingPolygon = false;
    }
}

//绘制曲线模式
void PaintProject::draw_Curve() {
    if(!isDrawingCurve) {
        quitAllFunctions();
        drawArea->draw_Curve(true);
        isDrawingCurve = true;
    }
    else {
        drawArea->draw_Curve(false);
        isDrawingCurve = false;
    }
}

//让画板开启或退出橡皮模式
void PaintProject::use_Rubber() {
    if(!isUsingRubber) {
        quitAllFunctions();
        drawArea->use_Rubber(true);
        isUsingRubber = true;
    }
    else {
        drawArea->use_Rubber(false);
        isUsingRubber = false;
    }
    //setDirty(true);
}
//让画板开启或退出画刷模式
void PaintProject::use_Brush() {
    if(!isUsingBrush) {
        quitAllFunctions();
        drawArea->use_Brush(true);
        isUsingBrush = true;
    }
    else {
        drawArea->use_Brush(false) ;
        isUsingBrush = false;
    }
    //setDirty(true);
}

//让画板开启或退出填充空白区域功能
void PaintProject::use_Filling() {
    if(!isFilling) {
        quitAllFunctions();
        drawArea->use_Filling(true);
        isFilling = true;
    }
    else {
        drawArea->use_Filling(false);
        isFilling = false;
    }
}
//让画板开启或退出裁剪区域
void PaintProject::cut_Area() {
    if(!isCuttingArea) {
        quitAllFunctions();
        drawArea->cut_Area(true);
        isCuttingArea = true;
    }
    else {
        drawArea->cut_Area(false);
        isCuttingArea = false;
    }
}

//退出所有功能
void PaintProject::quitAllFunctions() {
    if(isUsingBrush) brush->trigger();
    if(isUsingRubber) rubber->trigger();
    if(isFilling) fillBlank->trigger();
    if(isDrawingLine) drawLine->trigger();
    if(isDrawingCircle) drawCircle->trigger();
    if(isDrawingRectangle) drawRec->trigger();
    if(isDrawingOval) drawOval->trigger();
    if(isDrawingPolygon) drawPolygon->trigger();
    if(isCuttingArea) cutArea->trigger();
    if(isDrawingCurve) drawCurve->trigger();
}
//展示鼠标位置
void PaintProject::displayPosInStatusBar(QPoint pos) {
    QString res="";
    res=res+QString::number(pos.x())+","+QString::number(pos.y());
    mainWindow->statusBar()->showMessage(res);
}
//撤销
void PaintProject::cancleOperation() {
    quitAllFunctions();
    drawArea->cancleOperation();
}

//恢复
void PaintProject::restoreOperation() {
    quitAllFunctions();
    drawArea->restoreOperation();
}
