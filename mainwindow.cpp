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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMessage()
{
    ui->statusbar->showMessage("start");
}

