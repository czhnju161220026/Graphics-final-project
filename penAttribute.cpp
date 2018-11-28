#include "penAttribute.h"
#include "ui_penAttribute.h"
#include <QDebug>

penAttribute::penAttribute(QWidget *parent,int Width,int Style,int Size) :
    QDialog(parent),
    ui(new Ui::penAttribute)
{
    ui->setupUi(this);
    setWindowTitle("属性设置");

    ui->radioButton->setText("实线");
    ui->radioButton_2->setText("虚线");
    ui->radioButton_3->setText("点");
    ui->horizontalSlider->setRange(1,10);
    ui->spinBox->setRange(2,10);
    ui->spinBox_2->setRange(1,50);
    ui->horizontalSlider_2->setRange(1,50);
    connect(ui->horizontalSlider,&QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);
    connect(ui->horizontalSlider_2,&QSlider::valueChanged,ui->spinBox_2,&QSpinBox::setValue);
    void (QSpinBox:: *spinBoxSignal)(int) =&QSpinBox::valueChanged;
    connect(ui->spinBox,spinBoxSignal,ui->horizontalSlider,&QSlider::setValue);
    connect(ui->spinBox_2,spinBoxSignal,ui->horizontalSlider_2,&QSlider::setValue);

    this->penWidth=Width;
    this->edgeStyle=Style;
    this->rubberSize=Size;

    ui->spinBox->setValue(penWidth);
    ui->spinBox_2->setValue(rubberSize);
    switch (edgeStyle) {
    case 0:
        ui->radioButton->setChecked(true);
        break;
    case 1:
        ui->radioButton_2->setChecked(true);
        break;
    default:
        ui->radioButton_3->setChecked(true);
    }
    connect(this,&penAttribute::accepted,this,&penAttribute::setPenAttribute);
}

penAttribute::~penAttribute()
{
    delete ui;
}

void penAttribute::setPenAttribute() {
    penWidth = ui->spinBox->value();
    rubberSize = ui->spinBox_2->value();
   // qDebug()<<penWidth;

    if(ui->radioButton->isChecked()) {
        edgeStyle = 0;
    }
    else if(ui->radioButton_2->isChecked()) {
        edgeStyle = 1;
    }

    else {
        edgeStyle = 2;
    }
    //qDebug() <<edgeStyle;
}

void penAttribute::getPenAttribute(int &penWidth, int &edgeStyle, int &rubberSize) {
    penWidth = this->penWidth;
    edgeStyle = this->edgeStyle;
    rubberSize = this->rubberSize;
}
