#include "drawarea.h"
#include "ui_drawarea.h"
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QCursor>
#include <QStatusBar>
#include <QMenu>
#include <QInputDialog>
#include <QImage>
#include "line.h"
#include "circle.h"
#include "rectangle.h"
#include "oval.h"
#include "Polygon.h"
#include "Curve.h"
#include "filling.h"
#include <cmath>
#include "assert.h"

/*构造函数*/
DrawArea::DrawArea(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DrawArea)
{
    ui->setupUi(this);

    qDebug()<<"w="<<this->width();
    qDebug()<<"h="<<this->height();
    //设置画布的宽高
    Pix = QPixmap(1530,800);
    Pix.fill(Qt::white);    //填充为白色
    Pix.scaled(this->width(),this->height());
    Pix_index = 0;
    PixQueue.push_back(Pix); //储存最初的画布
    inkColor.setRgb(0,0,0); //设置墨水颜色，默认为黑色
    penWidth = 2;           //设置画笔宽度等属性
    edgeStyle =0;
    rubberSize=16;
    rubberBand = NULL;
    pressPoint.setX(0);     //初始化成员
    pressPoint.setY(0);
    lastPoint.setX(0);
    lastPoint.setY(0);
    endPoint.setX(0);
    endPoint.setY(0);
    releasePoint.setX(0);
    releasePoint.setY(0);
    isUsingRubber = false;
    isUsingBrush = false;
    isFilling = false;
    isCuttingArea = false;
    isCuttingLine = false;
    isDrawingLine = false;
    isDrawingCircle = false;
    isDrawingRectangle = false;
    isDrawingOval = false;
    isDrawPolygon = false;
    isDrawCurve = false;
    isReleased = false;
    isDraggingShape = false;
    isDraggingArea = false;
    isEditing = false;
    currentShape = NULL;
    cutArea = NULL;
    auxilaryPointIndex = -1;
    delta_x = 0;
    delta_y = 0;
    initPen();
    /*
    *打开鼠标跟踪，即使鼠标不按下也会触发mouseMoveEvent
    *为了能在状态栏实时显示鼠标位置
    */
    this->setMouseTracking(true); 

}

DrawArea::~DrawArea()
{
    delete ui;
}


/*装载图片*/
void DrawArea::loadImage(QString path) {
    QImage image(path);
    Pix = QPixmap(1530,800);
    Pix.fill(Qt::white);    //填充为白色
    Pix.scaled(this->width(),this->height());
    int width = this->width();
    int height = this->height();
    if(image.width()>width) {
        image = image.scaled(width,image.height()*width/image.width());
    }

    if(image.height()>height) {
        image = image.scaled(image.width()*height/image.height(),height);
    }
    QPainter painter(&Pix);
    painter.drawImage(0,0,image);
    update();
    PixQueue.pop_back();
    PixQueue.push_back(Pix); //储存最初的画布
}
/*保存图片*/
bool DrawArea::saveImage() {
    QFileDialog qfileDialog(this);
    qfileDialog.setDefaultSuffix("jpg");
    QString filePath = qfileDialog.getSaveFileName(
                this,
                tr("Save File"),
                "D:\\MyPainterProject.jpg",
                tr("JPG(*.jpg)") //默认以jpg保存
               );
    //异常处理
    if(!filePath.isEmpty()) {
        QFile file(filePath);
        if(!(filePath.contains(".jpg")|(filePath.contains(".png")|filePath.contains(".jpeg")))) {
            QMessageBox::warning(this,tr("Save file"),tr("保存失败，路径错误!请检查文件保存格式(.jpg,.jpeg,.png)"));
            return false;
        }
        //QTextStream out(&file);
        Pix.save(filePath);
        QMessageBox::information(this,tr("Information"),tr("保存成功！"));
        return true;
    }
    else {
        return false;
    }
}

/*重写画板的绘制事件*/
void DrawArea::paintEvent(QPaintEvent *) {
    initPen();
    //如果使用橡皮，将橡皮划过的区域涂白。
    if(isUsingRubber) {
        //qDebug()<<"Is Using Rubber";
        QColor color;
        QPen rubber;
        color.setRgb(255,255,255);
        rubber.setColor(color);
        rubber.setWidth(rubberSize);
        QPainter Rubber(&Pix);
        Rubber.setPen(rubber);
        Rubber.drawLine(lastPoint,endPoint);
        lastPoint=endPoint;
    }
    //如果使用画刷，将画刷划过的区域涂成相应颜色
    if(isUsingBrush) {
        QPainter Pen(&Pix);
        Pen.setPen(pen);
        Pen.drawLine(lastPoint,endPoint);
        lastPoint = endPoint;
    }
    //绘制直线
    if(isDrawingLine&&isReleased&&!isDraggingShape&&!isEditing) {
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        Line* newLine=new Line(pressPoint,releasePoint);
        newLine->draw(pen,Pix);//绘制
        newLine->showAuxilaryPoints(Pix);//绘制辅助点
        currentShape = newLine; //更新currentShape
        isReleased = false;    //鼠标已经松开
        //保存这次操作的结果到队列中
        addToPixQueue();
    }
    //绘制圆
    if(isDrawingCircle&&isReleased&&!isDraggingShape&&!isEditing) {
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        Circle* newCircle=new Circle(pressPoint,releasePoint);
        isReleased = false;      //鼠标已经松开
        newCircle->draw(pen,Pix);//绘制
        newCircle->showAuxilaryPoints(Pix);//绘制辅助点
        currentShape = newCircle;//更新currentShape
        //保存这次操作的结果到队列中
        addToPixQueue();
    }
    //绘制矩形
    if(isDrawingRectangle&&isReleased&&!isDraggingShape&&!isEditing) {
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        rectangle* newRectangle = new rectangle(pressPoint,releasePoint);
        isReleased = false;     //鼠标已经松开
        newRectangle->draw(pen,Pix);//绘制
        newRectangle->showAuxilaryPoints(Pix); //绘制辅助点
        currentShape = newRectangle; //更新currentShape
        //保存这次操作的结果到队列中
        addToPixQueue();
    }
    //绘制椭圆
    if(isDrawingOval&&isReleased&&!isDraggingShape&&!isEditing) {
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        Oval* newOval = new Oval(pressPoint,releasePoint);
        isReleased = false;     //鼠标已经松开
        newOval->draw(pen,Pix);//绘制
        newOval->showAuxilaryPoints(Pix); //绘制辅助点
        currentShape = newOval; //更新currentShape
        //保存这次操作的结果到队列中
        addToPixQueue();
    }
    //正在使用填充功能
    if(isFilling && isReleased) {
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        isReleased = false; //鼠标已经松开
        fillTheBlank(pressPoint,pen.color(),Pix); //fiil the blank,defined in filling.h
        //保存这次操作的结果到队列中
        currentShape = NULL;
        addToPixQueue();
    }
    //正在使用裁剪
    if(isCuttingArea && isReleased &&!isDraggingArea) {
        fixCutArea();
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        isReleased = false;
        currentShape = NULL;
        if(cutArea!=NULL) {
            delete cutArea;
        }
        cutArea = new CutArea(pressPoint,releasePoint,Pix);
        //TODO
        //show cutArea and edge
    }

    //裁剪直线功能
    if(isCuttingLine&&isReleased&&!isDraggingArea) {
        assert(currentShape!=NULL);
        assert(currentShape->getAttribute()==LINE);
        if(currentShape->cut(pressPoint,releasePoint)) {
            //repaint the line
            qDebug() <<"直线裁剪重绘";
            QPixmap oldPix = PixQueue[Pix_index-1];
            //在底图上绘制平移后的样子
            currentShape->draw(pen,oldPix);
            currentShape->showAuxilaryPoints(oldPix);
            //直接更新画板，并不需要保存
            PixQueue[Pix_index] = oldPix;
            Pix = oldPix;
        }
        else {
            //delete the line
            qDebug()<<"删除直线";
            cancleOperation();
            PixQueue.pop_back();
            delete currentShape;
            currentShape=NULL;
        }
    }

    //鼠标松起，如果正在拖动，退出拖动;如果正在编辑，退出编辑
    if(isReleased) {
        if(isDraggingShape) {
            isDraggingShape = false;
            qDebug() << "quit Dragging";
        }
        if(isEditing) {
            isEditing = false;
            auxilaryPointIndex = -1;
            qDebug() <<"quit Editing";
        }
        if(isDraggingArea) {
            isDraggingArea = false;
            qDebug() << "quit Dragging";
        }
        if(isCuttingLine) {
            isCuttingLine = false;
            setCursor(Qt::ArrowCursor);
            qDebug() << "quit Cutting Line";
        }
        isReleased = false; //鼠标已经松开
    }

    QPainter Painter(this);
    Painter.drawPixmap(0,0,this->width(),this->height(),Pix);
    if(isCuttingArea&&cutArea!=NULL) {
        QImage image = cutArea->getImage();
        Painter.drawImage(cutArea->getUpperLeftPoint().x()-2,cutArea->getUpperLeftPoint().y()-2,image);
    }
}

//捕捉鼠标按下的坐标
void DrawArea::mousePressEvent(QMouseEvent *event) {
    if(event->button()==Qt::LeftButton) {
        isReleased = false;  //鼠标尚未松开，不要绘制
        pressPoint = event->pos();
        lastPoint = event->pos();
        //是否进行选择辅助点或拖动
        if(currentShape!=NULL&&!isCuttingLine) {
            auxilaryPointIndex = currentShape->inAuxilaryPoint(pressPoint.x(),pressPoint.y());
            if(auxilaryPointIndex!=-1) {
                qDebug() <<"Using auxilartPointIndex: "<<auxilaryPointIndex;
                isEditing = true;
            }
            else if(currentShape->inDraggingArea(pressPoint.x(),pressPoint.y())) {
                qDebug() << "isDraggingShape";
                isDraggingShape = true;
            }
        }

        //是否要拖拽剪切好的区域
        if(cutArea!=NULL) {
            if(cutArea->inDraggingArea(pressPoint.x(),pressPoint.y())) {
                qDebug() <<"is Dragging Area";
                isDraggingArea = true;
            }
        }
        //初始化橡皮筋线
        if(!rubberBand) {
            rubberBand = new QRubberBand(QRubberBand::Rectangle,this);
        }
        rubberBand->setGeometry(QRect(pressPoint,QSize()));
        rubberBand->show();
       // qDebug()<<"Start:"<<pressPoint.rx()<<" "<<pressPoint.ry();

        //如果不在拖拽
        //如果在绘制多边形
        if(!isDraggingArea&&!isDraggingShape&&!isEditing&&(isDrawPolygon||isDrawCurve)) {
            initPen();
            ////如果是多边形第一个点
           if(isDrawPolygon) {
               if(currentShape==NULL||currentShape->getAttribute()!=POLYGON) {
                   if(currentShape!=NULL) {
                       hideAuxilaryPoints();
                   }
                   Polygon* polygon = new Polygon();
                   polygon->addNextPoint(pressPoint,pen,Pix);
                   currentShape = polygon;
               }
               else {
                   if(currentShape->isFinished()) {
                       hideAuxilaryPoints();
                       Polygon* polygon = new Polygon();
                       polygon->addNextPoint(pressPoint,pen,Pix);
                       currentShape = polygon;
                   }
                   ////如果不是多边形第一个点
                   else {
                      qDebug() << "old poly";
                      currentShape->addNextPoint(pressPoint,pen,Pix);

                   }
               }
               update();
           }
           //如果是曲线的第一个点
           else if(isDrawCurve) {
               if(currentShape==NULL||currentShape->getAttribute()!=CURVE) {
                   if(currentShape!=NULL) {
                       hideAuxilaryPoints();
                   }
                   addToPixQueue();
                   Curve* curve = new Curve();
                   curve->addNextPoint(pressPoint,pen,Pix);
                   currentShape = curve;
               }
               else {
                   if(currentShape->isFinished()) {
                       hideAuxilaryPoints();
                       addToPixQueue();
                       Curve* curve = new Curve();
                       curve->addNextPoint(pressPoint,pen,Pix);
                       currentShape = curve;
                   }
                   else {
                       currentShape->addNextPoint(pressPoint,pen,Pix);
                   }
               }
               QPixmap oldPix;
               oldPix = PixQueue[Pix_index-1];
               currentShape->draw(pen,oldPix);
               Pix = oldPix;
               PixQueue[Pix_index] = Pix;
               update();
           }
        }
    }
}
//跟踪鼠标移动
void DrawArea::mouseMoveEvent(QMouseEvent *event) {
    QPoint temp = currentPos;
    currentPos = event->pos();
    sendPos();
    if(currentShape!=NULL&&!isCuttingLine) {
        int index = currentShape->inAuxilaryPoint(event->pos().x(),event->pos().y());
        if(index!=-1) {
            setCursor(Qt::SizeAllCursor);
            setToolTip("可以通过拖动，编辑图形");
            setToolTipDuration(1500);
        }
        else if( currentShape->inDraggingArea(event->pos().x(),event->pos().y())) {
            setCursor(Qt::OpenHandCursor);
            setToolTip("左键按住可以拖动，右键可以进行变换操作");
            setToolTipDuration(1500);
        }
        else {
            setCursor(Qt::ArrowCursor);
            setToolTip("");
        }
    }

    if(cutArea!=NULL&&!isCuttingLine) {
        if(cutArea->inDraggingArea(event->pos().x(),event->pos().y())) {
            setCursor(Qt::OpenHandCursor);
            setToolTip("左键按住可以拖动，右键可以进行变换操作");
            setToolTipDuration(1500);
        }
        else {
            if(isCuttingArea) {
                QCursor cursor = QCursor(QPixmap(":/images/cut").scaled(32,32),-1,-1);
                setCursor(cursor);
            }
            else {
                setCursor(Qt::ArrowCursor);
                setToolTip("");
            }
        }
    }

    if(event->buttons()&Qt::LeftButton) {
        endPoint = event->pos();
        //如果没有在拖动
        if(!isDraggingShape&&!isEditing&&!isDraggingArea) {
            //如果使用橡皮或画刷，不需要橡皮筋线辅助
            //否则随着鼠标的移动，改变橡皮筋线
            if(isUsingBrush||isUsingRubber) {
                 update();
            }
            else {
                //如果正在使用绘制圆的功能，那么橡皮筋线的形状必须是正方形
                //否则可以是矩形
                if(!isDrawingCircle) {
                    rubberBand->setGeometry(QRect(pressPoint,event->pos()).normalized());
                }
                else {
                    int delta_x = event->pos().x()-pressPoint.x();
                    int delta_y = event->pos().y()-pressPoint.y();
                    if(abs(delta_x)>abs(delta_y)) {
                        rubberBand->setGeometry(QRect(pressPoint,QPoint(pressPoint.x()+abs(delta_y)*delta_x/abs(delta_x),pressPoint.y()+delta_y)).normalized());
                    }
                    else {
                        rubberBand->setGeometry(QRect(pressPoint,QPoint(pressPoint.x()+delta_x,pressPoint.y()+abs(delta_x)*delta_y/abs(delta_y))).normalized());
                    }
                }
            }
        }
        //如果在拖动图元
        else if(isDraggingShape){
            //this->cancleOperation();
            //平移选中的图元
            currentShape->move(currentPos.x()-temp.x(),currentPos.y()-temp.y());
            //得到绘制该图元之前的底图
            QPixmap oldPix = PixQueue[Pix_index-1];
            //在底图上绘制平移后的样子
            currentShape->draw(pen,oldPix);
            currentShape->showAuxilaryPoints(oldPix);
            //直接更新画板，并不需要保存
            PixQueue[Pix_index] = oldPix;
            Pix = oldPix;
            update();
        }
        //如果在拖拽裁剪区域
        else if(isDraggingArea) {
            cutArea->move(currentPos.x()-temp.x(),currentPos.y()-temp.y());
            update();
        }
        else if(isEditing) {
            delta_x+=currentPos.x()-temp.x();
            delta_y+=currentPos.y()-temp.y();
            if(currentShape->getAttribute()!=OVAL) {
                currentShape->moveAuxilaryPoint(auxilaryPointIndex,delta_x,delta_y);
                delta_x=0;
                delta_y=0;
            }
            else {
                if(delta_x%2==0&&delta_y%2==0) {
                    currentShape->moveAuxilaryPoint(auxilaryPointIndex,delta_x,delta_y);
                    delta_x=0;
                    delta_y=0;
                }
                else {
                    return;
                }
            }
            //得到绘制该图元之前的底图
            QPixmap oldPix = PixQueue[Pix_index-1];
            //在底图上绘制编辑后的样子
            currentShape->draw(pen,oldPix);
            currentShape->showAuxilaryPoints(oldPix);
            //直接更新画板，并不需要保存
            PixQueue[Pix_index] = oldPix;
            Pix = oldPix;
            update();
        }
    }
}
//捕捉鼠标松起
void DrawArea::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::LeftButton) {
        endPoint = event->pos();
        releasePoint = event->pos();
        rubberBand->hide();
        //鼠标松起
        isReleased = true;
        update();//更新画面
        //如果使用的时橡皮或画刷
        //当鼠标松起时，认为是一次操作
        //保存这次操作的结果
        if(isUsingBrush||isUsingRubber) {
            addToPixQueue();
        }
    }
}
//缩放事件，重设画布宽高
void DrawArea::resizeEvent(QResizeEvent *) {
    qDebug()<<"resize: "<<this->width()<<" "<<this->height();
    if(cutArea!=NULL) {
        cutArea->fixImage(Pix);
        delete cutArea;
        cutArea=NULL;
    }
    for(int i = 0;i<PixQueue.size();i++) {
        PixQueue[i] = PixQueue[i].scaled(this->width(),this->height());
    }
    Pix=Pix.scaled(this->width(),this->height());
    if(currentShape!=NULL) {
        if(isDrawPolygon&&!currentShape->isFinished()) {
            initPen();
            currentShape->finish(pen,Pix);
            currentShape->showAuxilaryPoints(Pix);
            update();
            addToPixQueue();
        }
        else if(isDrawCurve&&!currentShape->isFinished()) {
            currentShape->finish(pen,Pix);
            currentShape->showAuxilaryPoints(Pix);
            update();
            //addToPixQueue();
        }
        hideAuxilaryPoints();
    }
    currentShape = NULL;
    update();
}

//重写右键菜单事件
void DrawArea::contextMenuEvent(QContextMenuEvent *event) {
    if(currentShape!=NULL||cutArea!=NULL) {
        //右键表示多边形完成绘制
        if(isDrawPolygon&&currentShape!=NULL&&!currentShape->isFinished()) {
            initPen();
            currentShape->finish(pen,Pix);
            currentShape->showAuxilaryPoints(Pix);
            update();
            addToPixQueue();
        }
        else if(isDrawCurve&&currentShape!=NULL&&!currentShape->isFinished()) {
            currentShape->finish(pen,Pix);
            currentShape->showAuxilaryPoints(Pix);
            update();
            //addToPixQueue();
        }
        else if(currentShape!=NULL&&currentShape->inDraggingArea(event->pos().x(),event->pos().y())) {
            QMenu contex;
            //QAction* a = new QAction("Test",this);
            QAction* rotate_90_degree;
            QAction* rotate_180_degree;
            QAction* rotate_270_degree;
            QAction* rotate_specified_degree;
            QAction* zoom;
            QAction* horizontal_flip;
            QAction* vertical_flip;
            QAction* cutLine;

            rotate_90_degree = new QAction(tr("顺时针旋转90度..."),this);
            rotate_90_degree->setStatusTip("顺时针旋转90度...");
            connect(rotate_90_degree,&QAction::triggered,this,&DrawArea::rotateCurrentShape90);

            rotate_180_degree = new QAction(tr("顺时针旋转180度..."),this);
            rotate_180_degree->setStatusTip("顺时针旋转180度...");
            connect(rotate_180_degree,&QAction::triggered,this,&DrawArea::rotateCurrentShape180);

            rotate_270_degree = new QAction(tr("顺时针旋转270度..."),this);
            rotate_270_degree->setStatusTip("顺时针旋转270度...");
            connect(rotate_270_degree,&QAction::triggered,this,&DrawArea::rotateCurrentShape270);

            rotate_specified_degree = new QAction(tr("顺时针旋转指定度数..."),this);
            rotate_specified_degree->setStatusTip("顺时针旋转指定度数...");
            connect(rotate_specified_degree,&QAction::triggered,this,&DrawArea::rotateCurrentShapeInput);

            zoom = new QAction(tr("图形缩放..."),this);
            zoom ->setStatusTip("图形缩放...");
            connect(zoom,&QAction::triggered,this,&DrawArea::zoomCurrentShape);

            horizontal_flip = new QAction(tr("水平翻转..."),this);
            horizontal_flip->setStatusTip("水平翻转...");
            connect(horizontal_flip,&QAction::triggered,this,&DrawArea::horizontalFilp);

            vertical_flip = new QAction(tr("垂直翻转..."),this);
            vertical_flip->setStatusTip("垂直翻转...");
            connect(vertical_flip,&QAction::triggered,this,&DrawArea::verticalFilp);

            cutLine = new QAction("裁剪直线",this);
            cutLine->setStatusTip("使用鼠标裁剪直线图元...");
            connect(cutLine,&QAction::triggered,this,&DrawArea::cut_Line);


            contex.addAction(rotate_90_degree);
            contex.addAction(rotate_180_degree);
            contex.addAction(rotate_270_degree);
            contex.addAction(rotate_specified_degree);
            contex.addAction(zoom);
            contex.addAction(horizontal_flip);
            contex.addAction(vertical_flip);
            if(currentShape->getAttribute()==LINE) {
                contex.addAction(cutLine);
            }
            QPoint pos = QCursor::pos();
            pos.setX(pos.x()+1);
            pos.setY(pos.y()+1);
            contex.exec(pos);
        }

        else if(cutArea!=NULL && cutArea->inDraggingArea(event->pos().x(),event->pos().y())) {
            QMenu contex;
            QAction* zoom;
            QAction* horizontal_flip;
            QAction* vertical_flip;
            QAction* deleteArea;
            zoom = new QAction(tr("裁剪区域缩放..."),this);
            zoom ->setStatusTip("裁剪区域缩放...");
            connect(zoom,&QAction::triggered,this,&DrawArea::zoomCutArea);

            horizontal_flip = new QAction(tr("水平翻转..."),this);
            horizontal_flip->setStatusTip("水平翻转...");
            connect(horizontal_flip,&QAction::triggered,this,&DrawArea::horizontalFilp);

            vertical_flip = new QAction(tr("垂直翻转..."),this);
            vertical_flip->setStatusTip("垂直翻转...");
            connect(vertical_flip,&QAction::triggered,this,&DrawArea::verticalFilp);

            deleteArea = new QAction(tr("删除所选区域..."),this);
            deleteArea->setStatusTip("删除所选区域...");
            connect(deleteArea,&QAction::triggered,this,&DrawArea::deleteCutArea);


            contex.addAction(zoom);
            contex.addAction(horizontal_flip);
            contex.addAction(vertical_flip);
            contex.addAction(deleteArea);
            QPoint pos = QCursor::pos();
            pos.setX(pos.x()+1);
            pos.setY(pos.y()+1);
            contex.exec(pos);
        }
    }
}
//重写鼠标滚轮事件
void DrawArea::wheelEvent(QWheelEvent *event) {
    /*如果鼠标在当前图元的范围内*/
    if(currentShape!=NULL) {
        if(currentShape->inDraggingArea(event->pos().x(),event->pos().y())) {
            double ratio=1;
            /*根据滚轮滚动方向进行缩放
*/
            if(event->delta()>0) {
                qDebug() << "add 5% ratio";
                ratio = 1.05;
            }
            if(event->delta()<0) {
                qDebug() << "decrease 5% ratio";
                ratio = 0.95;
            }
            QPixmap oldPix = PixQueue[Pix_index-1];
            //在底图上
            currentShape->zoom(ratio);
            currentShape->draw(pen,oldPix);
            currentShape->showAuxilaryPoints(oldPix);
            //直接更新画板，并不需要保存
            PixQueue[Pix_index] = oldPix;
            Pix = oldPix;
            update();
        }
    }
    if(cutArea!=NULL) {
        if(cutArea->inDraggingArea(event->pos().x(),event->pos().y())) {
            double ratio=1;
            /*根据滚轮滚动方向进行缩放
*/
            if(event->delta()>0) {
                qDebug() << "add 5% ratio";
                ratio = 1.05;
            }
            if(event->delta()<0) {
                qDebug() << "decrease 5% ratio";
                ratio = 0.95;
            }
            cutArea->zoom(ratio);
            update();
        }
    }
}

//设置墨水颜色
void DrawArea::setInkColor() {
    QColorDialog askForColor;
    inkColor = askForColor.getColor();
    if(currentShape!=NULL) {
        hideAuxilaryPoints();
        currentShape = NULL;
    }
    qDebug() <<inkColor.red()<<" "<<inkColor.green()<<" "<<inkColor.blue();
}

//设置画笔
void DrawArea::initPen() {
    pen.setColor(inkColor);
    pen.setWidth(penWidth);
    //pen.setStyle(edgeStyle);
    switch (edgeStyle) {
    case 0:
        pen.setStyle(Qt::SolidLine);
        break;
    case 1:
        pen.setStyle(Qt::DashLine);
        break;
    case 2:
        pen.setStyle(Qt::DotLine);
        break;
    default:
        pen.setStyle(Qt::SolidLine);
        break;
    }
}
//设置画笔属性
void DrawArea::setPenAttribute(int penWidth, int edgeStyle,int rubberSize) {
    this->penWidth = penWidth;
    this->edgeStyle = edgeStyle;
    this->rubberSize = rubberSize;
    if(currentShape!=NULL) {
        hideAuxilaryPoints();
    }
    currentShape = NULL;
}

//给出画笔属性
void DrawArea::getPenAttribute(int &penWidth, int &edgeStyle, int& rubberSize) {
    penWidth = this->penWidth;
    edgeStyle = this->edgeStyle;
    rubberSize = this->rubberSize;
}
//开启直线绘制模式
void DrawArea::draw_Line(bool flag) {
    isDrawingLine=flag;
}
//重载：通过参数绘制直线
void DrawArea::draw_Line(QPoint startPoint, QPoint endPoint) {
    initPen();
    Line* newLine = new Line(startPoint,endPoint);
    newLine->draw(pen,Pix);
    newLine->showAuxilaryPoints(Pix);
    currentShape = newLine;
    addToPixQueue();
    update();
}

//开启圆绘制模式
void DrawArea::draw_Circle(bool flag) {
    isDrawingCircle=flag;
}
//重载：通过参数绘制圆
void DrawArea::draw_Circle(int radius, QPoint center) {
    initPen();
    Circle* newCircle = new Circle(radius,center);
    newCircle->draw(pen,Pix);
    newCircle->showAuxilaryPoints(Pix);
    currentShape = newCircle;
    addToPixQueue();
    update();
}
//开启矩形绘制模式
void DrawArea::draw_Rectangle(bool flag) {
    isDrawingRectangle = flag;
    if(flag) {
        qDebug() <<"draw rec";
    }
    else {
        qDebug() <<"quit draw res";
    }
}

//重载：通过参数绘制矩形
void DrawArea::draw_Rectangle(QPoint point1, QPoint point2) {
    initPen();
    rectangle* newRectangle = new rectangle(point1,point2);
    newRectangle->draw(pen,Pix);
    newRectangle->showAuxilaryPoints(Pix);
    currentShape = newRectangle;
    addToPixQueue();
    update();
}
//开启椭圆绘制模式
void DrawArea::draw_Oval(bool flag) {
    isDrawingOval = flag;
    if(flag) {
        qDebug() <<"draw Oval";
    }
    else {
        qDebug() <<"quit Oval";
    }
}
//重载：通过参数绘制椭圆
void DrawArea::draw_Oval(QPoint center, int a, int b, bool in_x) {
    initPen();
    Oval* newOval= new Oval(center,a,b,in_x);
    newOval->draw(pen,Pix);
    newOval->showAuxilaryPoints(Pix);
    currentShape = newOval;
    addToPixQueue();
    update();
}

//开启多边形绘制模式
void DrawArea::draw_Polygon(bool flag) {
    isDrawPolygon = flag;
    if(flag) {
        qDebug() <<"draw polygon";
    }
    else {
        qDebug() <<"quit Polygon";
    }
}

void DrawArea::draw_Curve(bool flag) {
    isDrawCurve = flag;
    if(flag) {
        qDebug() <<"draw Curve";
    }
    else {
        qDebug() <<"quit Curve";
    }
}

//橡皮使用模式
void DrawArea::use_Rubber(bool flag) {
    //qDebug()<<"Use rubber";
    if(flag){
        isUsingRubber = true;
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        currentShape = NULL;
        QCursor cursor = QCursor(QPixmap(":/images/rubber_cursor").scaled(32,32),1,1);
        setCursor(cursor);
        qDebug()<<"TODO: set cursor";
    }
    else {
        isUsingRubber = false;
        setCursor(Qt::ArrowCursor);
    }
}

//画刷使用模式
void DrawArea::use_Brush(bool flag) {
    if(flag){
        isUsingBrush = true;
        lastPoint = QPoint(0,0);
        endPoint = QPoint(0,0);
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        currentShape = NULL;
        QCursor cursor = QCursor(QPixmap(":/images/brush_cursor").scaled(32,32),1,-1);
        setCursor(cursor);
        qDebug()<<"TODO: set cursor";
    }
    else {
        isUsingBrush = false;
        setCursor(Qt::ArrowCursor);
    }
}

//填充空白区域功能
void DrawArea::use_Filling(bool flag) {
    if(flag) {
        isFilling = true;
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        currentShape = NULL;
        QCursor cursor = QCursor(QPixmap(":/images/filling").scaled(32,32),-1,-1);
        setCursor(cursor);
    }
    else {
        isFilling = false;
        setCursor(Qt::ArrowCursor);
    }
}

//裁剪区域功能
void DrawArea::cut_Area(bool flag) {
    if(flag) {
        isCuttingArea = true;
        if(currentShape!=NULL) {
            hideAuxilaryPoints();
        }
        currentShape = NULL;
        QCursor cursor = QCursor(QPixmap(":/images/cut").scaled(32,32),-1,-1);
        setCursor(cursor);
    }
    else {
        fixCutArea();
        isCuttingArea = false;
        setCursor(Qt::ArrowCursor);
    }
}

//保存这次操作的结果到队列中
void DrawArea::addToPixQueue() {
    //保存这次操作的结果到队列中
    if(PixQueue.size()-1!=Pix_index)
        PixQueue.erase(PixQueue.begin()+1+Pix_index,PixQueue.end());
    PixQueue.push_back(Pix);
    Pix_index++;

    //控制结果队列的长度不会超过20
    if(PixQueue.size()>=20) {
        PixQueue.erase(PixQueue.begin());
        Pix_index--;
    }
    changeHappen();//提示paintProject有未保存的改变发生了

}

//撤销操作
//展示上一次操作的结果
void DrawArea::cancleOperation() {
    if(currentShape!=NULL) {
        hideAuxilaryPoints();
    }
    currentShape = NULL;
    if(Pix_index>0&&Pix_index<PixQueue.size()) {
        qDebug()<<"Cancle operation:index= "<<Pix_index;
        Pix_index--;
        Pix = PixQueue[Pix_index];
        update();
    }

}

//恢复操作
//展示目前画面对应的后一次操作的结果
void DrawArea::restoreOperation() {
    if(currentShape!=NULL) {
        hideAuxilaryPoints();
    }
    currentShape = NULL;
    if(Pix_index<PixQueue.size()-1) {
        Pix_index++;
        Pix = PixQueue[Pix_index];
        qDebug()<<"Restore operation:index="<<Pix_index;
        update();
    }
}

//信号函数，提供和paintProject的实时交互
//发送鼠标坐标，触发坐标变动事件，使得paintproject的statusbar接受该坐标并显示。
void DrawArea::sendPos() {
    emit postionChange(currentPos);
}

//提示paintProject有未保存的改变发生了
void DrawArea::changeHappen() {
    qDebug() <<"Change Happen";
    emit unsavedChange(true);
}

//TODO message
void DrawArea::TODO() {
    QMessageBox::information(this,tr("Information"),tr("该功能待补充！"));
}

/*旋转指定度数*/
void DrawArea::rotateCurrentShape90(){
    rotateCurrentShape(90);
}

void DrawArea::rotateCurrentShape180() {
    rotateCurrentShape(180);
}

void DrawArea::rotateCurrentShape270() {
    rotateCurrentShape(270);
}

void DrawArea::rotateCurrentShapeInput() {
    bool ok =false;
    int res = QInputDialog::getInt(
                this,
                tr("输入角度"),
                tr("请输入图形旋转的角度"),
                0,0,360,1,&ok);
    if(ok) {
        rotateCurrentShape(res);
    }
    else{
        return;
    }
}

/*旋转指定度数*/
void DrawArea::rotateCurrentShape(int degrees) {
    qDebug()<<"Rotate current shape: "<< degrees;
    QPixmap oldPix = PixQueue[Pix_index-1];
    //在底图上绘制平移后的样子
    currentShape->addRotateDegrees(degrees);
    currentShape->draw(pen,oldPix);
    currentShape->showAuxilaryPoints(oldPix);
    //直接更新画板，并不需要保存
    PixQueue[Pix_index] = oldPix;
    Pix = oldPix;
    update();

}
//水平翻转
void DrawArea::horizontalFilp() {
    if(currentShape!=NULL) {
        QPixmap oldPix = PixQueue[Pix_index-1];
        currentShape->horizontalFilp();
        currentShape->draw(pen,oldPix);
        currentShape->showAuxilaryPoints(oldPix);
        PixQueue[Pix_index] = oldPix;
        Pix = oldPix;
        update();
    }
    if(cutArea!=NULL) {
        cutArea->horizontalFilp();
        update();
    }
}

//垂直翻转
void DrawArea::verticalFilp() {
    if(currentShape!=NULL) {
        QPixmap oldPix = PixQueue[Pix_index-1];
        currentShape->verticalFilp();
        currentShape->draw(pen,oldPix);
        currentShape->showAuxilaryPoints(oldPix);
        PixQueue[Pix_index] = oldPix;
        Pix = oldPix;
        update();
    }
    if(cutArea!=NULL) {
        cutArea->verticalFilp();
        update();
    }
}

/*图形缩放*/
void DrawArea::zoomCurrentShape() {
    bool ok = false;
    double res = QInputDialog::getDouble(this,tr("输入缩放比"),tr("输入缩放比"),1,0.01,1000,1,&ok);
    if(ok) {
        QPixmap oldPix = PixQueue[Pix_index-1];
        //在底图上绘制
        currentShape->zoom(res);
        currentShape->draw(pen,oldPix);
        currentShape->showAuxilaryPoints(oldPix);
        //直接更新画板，并不需要保存
        PixQueue[Pix_index] = oldPix;
        Pix = oldPix;
        update();
    }
    else {
        return;
    }
}

/*裁剪区域缩放*/
void DrawArea::zoomCutArea() {
    bool ok = false;
    double res = QInputDialog::getDouble(this,tr("输入缩放比"),tr("输入缩放比"),1,0.01,1000,1,&ok);
    if(ok) {
        cutArea->zoom(res);
        update();
    }
    else {
        return;
    }
}

//通过重绘隐藏辅助点
void DrawArea::hideAuxilaryPoints() {
    QPixmap oldPix = PixQueue[Pix_index-1];
    currentShape->draw(pen,oldPix);
    PixQueue[Pix_index] = oldPix;
    Pix = oldPix;
    update();
}

//固定裁剪区域
void DrawArea::fixCutArea() {
    if(cutArea!=NULL) {
        cutArea->fixImage(Pix);
        delete cutArea;
        cutArea = NULL;
        update();
        addToPixQueue();
    }
}
//删除所选区域
void DrawArea::deleteCutArea() {
    delete cutArea;
    cutArea = NULL;
    update();
    addToPixQueue();
}

//进入裁剪直线模式
void DrawArea::cut_Line() {
    qDebug()<<"is cutting line";
    isCuttingLine = true;
    setCursor(Qt::CrossCursor);
    emit quitFunctionSignal();
}
