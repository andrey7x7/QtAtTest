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

    widthPlane = 782;
    heightPlane = 533;

    ui->graphicsView->setScene(scene);
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
    widthPlane=width();
    heightPlane=height();
    scene->setSceneRect(0,0,widthPlane,heightPlane);

    generateField(50, widthPlane, heightPlane);
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
    //смещение области видимости
    QRectF rect;
    rect.setWidth(widthPlane);
    rect.setHeight(heightPlane);
    rect.setX(pos.x()>widthPlane/2 ?  pos.x()-widthPlane/2 : 0);
    rect.setY(pos.y()>heightPlane/2 ? pos.y()-heightPlane/2 : 0);
    scene->setSceneRect(rect);
    //scene->setSceneRect(pos.x()-widthPlane/2, pos.y()-heightPlane/2, widthPlane-5, heightPlane-7);


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

// Определение функции qHash для использования QPoint в QMap
uint qHash(const QPoint &point, uint seed = 0) {
     return qHash(point.x(), seed) ^ qHash(point.y(), seed); // Используем хеширование координат
}

void MainWindow::generateField(int size, int width, int height)
{
    //генерация сетки
//    for(int w=0; w<width+1 ; w+=size){
//        scene->addLine(w,0,w,height);
//    }
//    for(int h=0; h<height+1 ; h+=50){
//        scene->addLine(0,h,width,h);
//    }

    //заполнение
    QHash<QPoint, bool> mapa;
    for(int w=0; w<width/size; w++){
        for(int h=0; h<height/size; h++){
            bool isWhite = (QRandomGenerator::global()->generate() % 3 == 0);
            mapa.insert(QPoint(w*size,h*size),isWhite);
        }
    }

    for(QPoint pos : mapa.keys()){
        scene->addRect(pos.x(),pos.y(),50,50, QPen(Qt::black), mapa.value(pos)? QBrush(Qt::gray) : QBrush(Qt::yellow));
    }
}
