#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDebug>
#include <QKeyEvent>
#include <QException>
#include <QRandomGenerator>
#include <QPoint>
#include <QMap>
#include <QList>
#include <QHash>
#include <QQueue>
#include <QMessageBox>
#include "plane.h"

#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Model *model;
    int squareSize;
    int widthPlane;
    int heightPlane;
    QPointF pos;
    QHash<QPoint, bool> mapa;
    Plane *plane1;
    QPoint start;
    QPoint finish;

private slots:
    void resizeWindow();
    void startAndGenerate();
    void generateField(int size, int width, int height);
    void setStartFinish(QPoint point);
    void paintPath();
    QVector<QPoint> findPath(const QHash<QPoint, bool>& map, const QPoint& start, const QPoint& end);
    void paintMap(QHash<QPoint, bool> map);
protected:
    void keyPressEvent(QKeyEvent *event);
    void planeMousePress(QMouseEvent *eventPress);

};
#endif // MAINWINDOW_H
