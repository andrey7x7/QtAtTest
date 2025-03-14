#include "plane.h"
#include "ui_plane.h"

Plane::Plane(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plane)
{
    ui->setupUi(this);
}

Plane::~Plane()
{
    delete ui;
}


void Plane::planeMousePress(QMouseEvent *eventPress)
{
    emit mousePressEvent(eventPress);
}

void Plane::setScene(QGraphicsScene *scene)
{
    ui->graphicsView->setScene(scene);
}
