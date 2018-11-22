#ifndef INPUTCIRCLE_H
#define INPUTCIRCLE_H

#include <QDialog>
#include <QPoint>

namespace Ui {
class InputCircle;
}

//圆的参数输入对话框类
class InputCircle : public QDialog
{
    Q_OBJECT

public:
    explicit InputCircle(QWidget *parent = 0);
    ~InputCircle();
    void setRange(int rangeW,int rangeH);//根据目前画布大小，设置参数范围
    bool getCircle(int& radius, QPoint& center);////通过引用返回用户输入

private:
    Ui::InputCircle *ui;
    int rangeW;
    int rangeH;
    int radius;
    QPoint center;
    bool isAccepted;
    void setRadiusRange();
    void getInput();
};

#endif // INPUTCIRCLE_H
