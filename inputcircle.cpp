#include "inputcircle.h"
#include "ui_inputcircle.h"
#include <QSpinBox>
InputCircle::InputCircle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputCircle)
{
    ui->setupUi(this);
    ui->label->setText("圆心坐标(x,y)");
    ui->label_2->setText("半径");
    rangeH = 0;
    rangeW = 0;
    isAccepted = false;
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&InputCircle::getInput);
    void (QSpinBox:: *spinBoxSignal)(int) =&QSpinBox::valueChanged;
    connect(ui->spinBox,spinBoxSignal,this,&InputCircle::setRadiusRange);
    connect(ui->spinBox_2,spinBoxSignal,this,&InputCircle::setRadiusRange);
    setWindowTitle("输入图形参数");
}

InputCircle::~InputCircle()
{
    delete ui;
}

void InputCircle::setRange(int rangeW, int rangeH) {
    this->rangeH = rangeH;
    this->rangeW = rangeW;
    ui->spinBox->setRange(0,rangeW);
    ui->spinBox_2->setRange(0,rangeH);
}

void InputCircle::getInput() {
    radius = ui->spinBox_3->value();
    center = QPoint(ui->spinBox->value(),ui->spinBox_2->value());
    isAccepted = true;
}

void InputCircle::setRadiusRange() {
    int rangeX = (rangeW - ui->spinBox->value() < ui->spinBox->value())?rangeW - ui->spinBox->value():ui->spinBox->value();
    int rangeY = (rangeH - ui->spinBox_2->value() < ui->spinBox_2->value())?rangeH - ui->spinBox_2->value():ui->spinBox_2->value();
    ui->spinBox_3->setRange(0,(rangeX<rangeY)?rangeX:rangeY);
}

bool InputCircle::getCircle(int &radius, QPoint &center) {
    radius = this->radius;
    center = this->center;
    return isAccepted;
}
