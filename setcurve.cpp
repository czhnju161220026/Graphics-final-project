#include "setcurve.h"
#include "ui_setcurve.h"

SetCurve::SetCurve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetCurve)
{
    ui->setupUi(this);
    ui->radioButton->setChecked(true);
    curveType = 0;
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this,&SetCurve::setType);
}

SetCurve::~SetCurve()
{
    delete ui;
}

int SetCurve::getType() {
    return curveType;
}

void SetCurve::setType() {
    if(ui->radioButton->isChecked()) {
        curveType = 0;
    }
    else {
        curveType = 1;
    }
}
