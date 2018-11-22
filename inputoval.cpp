#include "inputoval.h"
#include "ui_inputoval.h"
int min(int a, int b) {
    return (a<b)?a:b;
}

int max(int a,int b) {
    return (a>b)?a:b;
}

InputOval::InputOval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputOval)
{
    ui->setupUi(this);
    setWindowTitle("输入图形参数");
    ui->label->setText("椭圆中心坐标");
    ui->label_2->setText("长轴长，短轴长");
    ui->label_3->setText("沿坐标轴方向");
    ui->radioButton->setText("x轴");
    ui->radioButton->setChecked(true);
    ui->radioButton_2->setText("y轴");
    isAccepted = false;
    in_x = true;

     void (QSpinBox:: *spinBoxSignal)(int) =&QSpinBox::valueChanged;
     connect(ui->spinBox,spinBoxSignal,this,&InputOval::updateRange);
     connect(ui->spinBox_2,spinBoxSignal,this,&InputOval::updateRange);

     connect(ui->radioButton,&QRadioButton::clicked,this,&InputOval::updateRange);
     connect(ui->radioButton_2,&QRadioButton::clicked,this,&InputOval::updateRange);

     connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&InputOval::getInput);
}

void InputOval::setRange(int rangeW, int rangeH) {
    this->rangeW = rangeW;
    this->rangeH = rangeH;
    ui->spinBox->setRange(0,rangeW);
    ui->spinBox_2->setRange(0,rangeH);
}

bool InputOval::getOval(QPoint &center, int &double_a, int &double_b, bool &in_x) {
    if(!isAccepted) {
        return false;
    }
    center = this->center;
    double_a = this->double_a;
    double_b = this->double_b;
    in_x = this->in_x;
    return true;
}

void InputOval::getInput() {
    center.setX(ui->spinBox->value());
    center.setY(ui->spinBox_2->value());
    double_a = ui->spinBox_3->value();
    double_b = ui->spinBox_4->value();
    in_x = ui->radioButton->isChecked();
    isAccepted = true;
}

void InputOval::updateRange() {
    in_x = ui->radioButton->isChecked();
    if(in_x) {
        int center_x = ui->spinBox->value();
        int center_y = ui->spinBox_2->value();
        int rangeA = min(center_x,rangeW-center_x);
        int rangeB = min(center_y,rangeH-center_y);
        rangeB = min(rangeB,rangeA);
        ui->spinBox_3->setRange(0,2*rangeA);
        ui->spinBox_4->setRange(0,2*rangeB);
    }
    else {
        int center_x = ui->spinBox->value();
        int center_y = ui->spinBox_2->value();
        int rangeA = min(center_y,rangeH-center_y);
        int rangeB = min(center_x,rangeW-center_x);
        rangeB = min(rangeB,rangeA);
        ui->spinBox_3->setRange(0,2*rangeA);
        ui->spinBox_4->setRange(0,2*rangeB);
    }
}

InputOval::~InputOval()
{
    delete ui;
}
