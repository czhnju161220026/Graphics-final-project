#ifndef INPUTLINE_H
#define INPUTLINE_H

#include <QDialog>
#include <QPoint>

namespace Ui {
class inputLine;
}

//直线参数输入对话框类
class inputLine : public QDialog
{
    Q_OBJECT

public:
    explicit inputLine(QWidget *parent = 0);
    ~inputLine();
    void setRange(int w, int h);//根据目前画布大小，设置参数范围
    bool getPoint(QPoint& startPoint, QPoint& endPoint); //通过引用返回用户输入

private:
    Ui::inputLine *ui;
    int rangeW;
    int rangeH;
    QPoint startPoint;
    QPoint endPoint;
    bool isAccepted;
    void getInput();
};

#endif // INPUTLINE_H
