#include "model.h"

Model::Model(QObject *parent) : QObject{parent}
{

}

QRectF Model::boundingRect() const
{
    return QRectF(0,0,10,10);
}

void Model::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::red);
    painter->drawEllipse(0,0,10,10);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Model::keyPress(QKeyEvent *event)
{
    qDebug()<<event;
}
