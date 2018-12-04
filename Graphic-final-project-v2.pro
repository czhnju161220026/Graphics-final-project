#-------------------------------------------------
#
# Project created by QtCreator 2018-11-22T19:06:25
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
LIBS     += -lOpengl32 \
            -lglu32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Graphic-final-project-v2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    circle.cpp \
    CutArea.cpp \
    drawarea.cpp \
    inputcircle.cpp \
    inputdialog.cpp \
    inputline.cpp \
    inputoval.cpp \
    inputrectangle.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    oval.cpp \
    paintproject.cpp \
    penAttribute.cpp \
    Polygon.cpp \
    rectangle.cpp \
    shape.cpp \
    Curve.cpp \
    Myglwidget.cpp

HEADERS += \
    circle.h \
    CutArea.h \
    drawarea.h \
    filling.h \
    inputcircle.h \
    inputdialog.h \
    inputline.h \
    inputoval.h \
    inputrectangle.h \
    line.h \
    mainwindow.h \
    oval.h \
    paintproject.h \
    penAttribute.h \
    Polygon.h \
    rectangle.h \
    shape.h \
    Curve.h \
    Myglwidget.h

FORMS += \
    drawarea.ui \
    inputcircle.ui \
    inputdialog.ui \
    inputline.ui \
    inputoval.ui \
    inputrectangle.ui \
    mainwindow.ui \
    paintproject.ui \
    penAttribute.ui

RESOURCES += \
    qrf1.qrc
