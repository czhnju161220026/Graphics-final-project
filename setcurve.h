#ifndef SETCURVE_H
#define SETCURVE_H

#include <QDialog>

namespace Ui {
class SetCurve;
}

class SetCurve : public QDialog
{
    Q_OBJECT

public:
    explicit SetCurve(QWidget *parent = 0);
    ~SetCurve();
    int getType();
private:
    Ui::SetCurve *ui;
    int curveType;
    void setType();
};

#endif // SETCURVE_H
