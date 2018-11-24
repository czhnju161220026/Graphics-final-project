#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    /*设置UI*/
    /*为各个按钮添加功能*/
    ui->setupUi(this);
    setWindowTitle("My Painter");
    //push_button 1
    ui->pushButton->setText(tr("新建项目(&N)"));
    ui->pushButton->setStatusTip(tr("建立一个空白的画板...(Ctrl + N)"));
    ui->pushButton->setShortcut(QKeySequence::New);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::newPainter);

    //push_button 2
    ui->pushButton_2->setText(tr("打开项目(&O)"));
    ui->pushButton_2->setStatusTip(tr("选择一个已有的画板并打开...(Ctrl + O)"));
    ui->pushButton_2->setShortcut(QKeySequence::Open);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::openFile);
    //push_button 3
    ui->pushButton_3->setText("退出(&Q)");
    ui->pushButton_3->setStatusTip(tr("退出软件(Ctrl + Q)"));
    ui->pushButton_3->setShortcut(QKeySequence::Quit);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newPainter() {
    qDebug() << "New project" ;
    PaintProject* p= new PaintProject();//新项目
    p->setWindowModality(Qt::ApplicationModal);//设置为顶层窗口
    p->showMaximized();
}

void MainWindow::openFile() {
    QString filePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "D:\\",
                tr("Image (*.jpg *.png *.jpeg)")
               );

    if(!filePath.isEmpty()) {
        qDebug() <<filePath;
        PaintProject* p = new PaintProject();
        p->setWindowModality(Qt::ApplicationModal);//设置为顶层窗口
        p->showMaximized();
        p->loadImage(filePath);  //加载已有的图片
    }
}

