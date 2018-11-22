#ifndef PENATTRIBUTE_H
#define PENATTRIBUTE_H

#include <QDialog>

/*画刷属性设置界面类*/
namespace Ui {
class penAttribute;
}

class penAttribute : public QDialog
{
    Q_OBJECT

public:
    explicit penAttribute(QWidget *parent = 0,int Width = 2,int Style = 0,int Size=8); //构造函数
    ~penAttribute();
    void getPenAttribute(int& penWidth, int&edgeStyle, int& rubberSize); //使用引用，返回用户设置的属性
private:
    Ui::penAttribute *ui;
    int penWidth;
    int edgeStyle;
    int rubberSize;
    void setPenAttribute();
};

#endif // PENATTRIBUTE_H
