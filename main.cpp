#include "mainwindow.h"
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile qss(":/style.qss");
    if(qss.open(QFile::ReadOnly)) {
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
    }
    else {
        qDebug() << "open failed"<<endl;
    }


    w.show(); //开启应用
    return a.exec();
}
