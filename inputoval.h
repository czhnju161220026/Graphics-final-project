#ifndef INPUTOVAL_H
#define INPUTOVAL_H

#include <QDialog>
#include <QPoint>

namespace Ui {
class InputOval;
}

class InputOval : public QDialog
{
    Q_OBJECT

public:
    explicit InputOval(QWidget *parent = 0);
    ~InputOval();
    void setRange(int rangeW,int rangeH);
    bool getOval(QPoint& center,int& a,int &b, bool& in_x);

private:
    Ui::InputOval *ui;
    int rangeH;
    int rangeW;
    int double_a; //长轴长
    int double_b; //短轴长
    QPoint center;
    bool in_x;
    bool isAccepted;
    void getInput();
    void updateRange();
};

#endif // INPUTOVAL_H
