#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>

class Model : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

public slots:
    void keyPress(QKeyEvent *event);

private :
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

signals:

};

#endif // MODEL_H
