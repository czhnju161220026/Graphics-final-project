#ifndef INPUTRECTANGLE_H
#define INPUTRECTANGLE_H

#include <QDialog>
#include <QPoint>
namespace Ui {
class InputRectangle;
}

class InputRectangle : public QDialog
{
    Q_OBJECT

public:
    explicit InputRectangle(QWidget *parent = 0);
    void setRange(int rangeW,int rangeH);
    bool getRectangle(QPoint& point1,QPoint& point2);
    ~InputRectangle();

private:
    Ui::InputRectangle *ui;
    QPoint point1;
    QPoint point2;
    int rangeW;
    int rangeH;
    bool isAccepted;
    void getInput();
};

#endif // INPUTRECTANGLE_H
