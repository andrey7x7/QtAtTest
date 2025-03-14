#ifndef PLANE_H
#define PLANE_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsScene>

namespace Ui {
class plane;
}

class Plane : public QWidget
{
    Q_OBJECT

public:
    explicit Plane(QWidget *parent = nullptr);
    ~Plane();

public slots:
    void planeMousePress(QMouseEvent *eventPress);
    void setScene(QGraphicsScene *scene);
private:
    Ui::plane *ui;

signals:
    void mousePressEvent(QMouseEvent *eventPress);
};

#endif // PLANE_H
