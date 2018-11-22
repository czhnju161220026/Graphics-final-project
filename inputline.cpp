#include "inputline.h"
#include "ui_inputline.h"

inputLine::inputLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputLine)
{
    ui->setupUi(this);
    setWindowTitle("输入图形参数");
    ui->label->setText("起点(x,y)");
    ui->label_2->setText("终点(x,y)");
    isAccepted = false;
    connect(this,&inputLine::accepted,this,&inputLine::getInput);
}

inputLine::~inputLine()
{
    delete ui;
}

//根据目前画布大小，设置参数范围
void inputLine::setRange(int w, int h) {
    rangeH = h;
    rangeW = w;
    ui->spinBox->setRange(0,w);
    ui->spinBox_2->setRange(0,h);
    ui->spinBox_3->setRange(0,w);
    ui->spinBox_4->setRange(0,h);
}
//获得用户输入
void inputLine::getInput() {
    startPoint = QPoint(ui->spinBox->value(),ui->spinBox_2->value());
    endPoint = QPoint(ui->spinBox_3->value(),ui->spinBox_4->value());
    isAccepted = true;
}

//通过引用返回用户输入
bool inputLine::getPoint(QPoint &startPoint, QPoint &endPoint) {
    startPoint = this->startPoint;
    endPoint = this->endPoint;
    return this->isAccepted;
}


