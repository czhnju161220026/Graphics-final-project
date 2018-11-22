#include "inputdialog.h"
#include "ui_inputdialog.h"
#include "shape.h"
#include <QDebug>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    setWindowTitle("选择需要绘制的图形种类");
    ui->radioButton->setText("直线");
    ui->radioButton_2->setText("曲线");
    ui->radioButton_3->setText("椭圆");
    ui->radioButton_4->setText("圆");
    ui->radioButton_5->setText("矩形");
    ui->radioButton->setChecked(true);
    shapeCategory = SHAPE;
    connect(this,&InputDialog::accepted,this,&InputDialog::setShapeCategory);
}

InputDialog::~InputDialog()
{
    delete ui;
}

//根据用户的选择，设置下一步要输入的图形的种类
void InputDialog::setShapeCategory() {
    if(ui->radioButton->isChecked()) {
        shapeCategory = LINE;
    }
    if(ui->radioButton_4->isChecked()) {
        shapeCategory = CIRCLE;
    }
    if(ui->radioButton_5->isChecked()) {
        shapeCategory = RECTANGLE;
    }
    if(ui->radioButton_3->isChecked()) {
        shapeCategory = OVAL;
    }
}

//给出图形种类
int InputDialog::getShapeCategory() {
    return shapeCategory;
}




