#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include "paintproject.h"

/*应用主窗口类*/

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void newPainter(); //开启新项目
    void openPainter(); //打开已有的图片作为项目
    void openFile();    //打开文件
};

#endif // MAINWINDOW_H
