#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QDebug>
#include <QKeyEvent>
#include <QException>

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
    int widthPlane;
    int heightPlane;
    QPointF pos;

private slots:
    void showMessage();
    void generateField();

protected:
    void keyPressEvent(QKeyEvent *event);

};
#endif // MAINWINDOW_H
