#include "inputrectangle.h"
#include "ui_inputrectangle.h"

InputRectangle::InputRectangle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputRectangle)
{
    ui->setupUi(this);
    setWindowTitle("输入图形参数");
    ui->label->setText("输入矩形一条对角线上的两个顶点");
    ui->label_2->setText("顶点1");
    ui->label_3->setText("顶点2");
    isAccepted = false;
    point1 = QPoint(0,0);
    point2 = QPoint(0,0);
    connect(this,&InputRectangle::accepted,this,&InputRectangle::getInput);
}

void InputRectangle::setRange(int rangeW, int rangeH) {
    this->rangeW = rangeW;
    this->rangeH = rangeH;
    ui->spinBox->setRange(0,rangeW);
    ui->spinBox_2->setRange(0,rangeH);
    ui->spinBox_3->setRange(0,rangeW);
    ui->spinBox_4->setRange(0,rangeH);
}

void InputRectangle::getInput() {
    isAccepted = true;
    point1 = QPoint(ui->spinBox->value(),ui->spinBox_2->value());
    point2 = QPoint(ui->spinBox_3->value(),ui->spinBox_4->value());
}

bool InputRectangle::getRectangle(QPoint &point1, QPoint &point2) {
    point1 = this->point1;
    point2 = this->point2;
    return isAccepted;
}

InputRectangle::~InputRectangle()
{
    delete ui;
}
