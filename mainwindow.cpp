#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AiTest");
    connect(ui->start, SIGNAL(triggered()), this, SLOT(showMessage()));
    connect(ui->exit, SIGNAL(triggered()), this, SLOT(close()));

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,782,533);
    ui->graphicsView->setScene(scene);
    widthPlane = 782;
    heightPlane = 533;

    generateField();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
    Коммент
*/
void MainWindow::showMessage()
{
    model = new Model();
    scene->addItem(model);
    model->setPos(ui->graphicsView->width()/2, ui->graphicsView->height()/2);
    ui->statusbar->showMessage("start");
    pos = model->pos();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Up:
            pos.y()>0 ? pos.setY(pos.y()-10) : pos.setY(pos.y());
            break;
        case Qt::Key_Down:
            pos.setY(pos.y()+10);
            break;
        case Qt::Key_Left:
            pos.x()>0 ? pos.setX(pos.x()-10) : pos.setX(pos.x());
            break;
        case Qt::Key_Right:
            pos.setX(pos.x()+10);
            break;
    }

    model->setPos(pos);
    scene->setSceneRect(pos.x()-widthPlane/2, pos.y()-heightPlane/2, widthPlane-5, heightPlane-7);

    //смещение области видимости
//    if(pos.x()>widthPlane/2 || pos.y()>heightPlane/2){
//        QRectF rect;
//        rect.setWidth(widthPlane-5);
//        rect.setHeight(heightPlane-7);
//        rect.setX(pos.x()>widthPlane/2 ? pos.x()-widthPlane/2 : widthPlane/2);
//        rect.setY(pos.y()>heightPlane/2 ? pos.y()-heightPlane/2 : heightPlane/2);
//        scene->setSceneRect(rect);
//        //scene->setSceneRect(pos.x()-widthPlane/2, pos.y()-heightPlane/2, widthPlane-5, heightPlane-7);
//    }
    qDebug()<<pos;
}

void MainWindow::generateField()
{
    for(int w=0; w<1501 ; w+=50){
        scene->addLine(w,0,w,1500);
    }
    for(int h=0; h<1501 ; h+=50){
        scene->addLine(0,h,1500,h);
    }
}
