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

    plane1 = new Plane();
    ui->gridLayout->addWidget(plane1);
    plane1->setScene(scene);
    connect(plane1, &Plane::mousePressEvent, this, &MainWindow::planeMousePress);
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
    start = QPoint(-1,-1);
    finish = QPoint(-1,-1);
    widthPlane=width();
    heightPlane=height();
    scene->setSceneRect(0,0,widthPlane,heightPlane);

    generateField(50, widthPlane, heightPlane);
    model = new Model();
    scene->addItem(model);
    model->setPos(plane1->width()/2, plane1->height()/2);
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
    //scene->setSceneRect(rect);
    scene->setSceneRect(pos.x()-widthPlane/2, pos.y()-heightPlane/2, widthPlane-5, heightPlane-7);


//    if(pos.x()>widthPlane/2 || pos.y()>heightPlane/2){
//        QRectF rect;
//        rect.setWidth(widthPlane-5);
//        rect.setHeight(heightPlane-7);
//        rect.setX(pos.x()>widthPlane/2 ? pos.x()-widthPlane/2 : widthPlane/2);
//        rect.setY(pos.y()>heightPlane/2 ? pos.y()-heightPlane/2 : heightPlane/2);
//        scene->setSceneRect(rect);
//        //scene->setSceneRect(pos.x()-widthPlane/2, pos.y()-heightPlane/2, widthPlane-5, heightPlane-7);
//    }
   // qDebug()<<pos << scene->sceneRect();
}

void MainWindow::planeMousePress(QMouseEvent *eventPress)
{
    //qDebug()<<QPoint(eventPress->pos().x()/50, eventPress->pos().y()/50);
    setStartFinish(QPoint(eventPress->pos().x()/50*50,
                          eventPress->pos().y()/50*50));
}

void MainWindow::setStartFinish(QPoint point){
    //qDebug()<<mapa.value(point);
    if(start!=QPoint(-1,-1) && finish!=QPoint(-1,-1)){
        paintMap(mapa);
        start = QPoint(-1,-1);
        finish = QPoint(-1,-1);
    }

    if(start==QPoint(-1,-1)){
        start = point;
        scene->addRect(QRectF(point, QSize(50,50)), QPen(Qt::black), QBrush(Qt::red));
    } else if(finish==QPoint(-1,-1)){
        finish = point;
        scene->addRect(QRectF(point, QSize(50,50)), QPen(Qt::black), QBrush(Qt::green));
        paintPath();
    }
}

// Определение функции qHash для использования QPoint в QMap
uint qHash(const QPoint &point, uint seed = 0) {
     return qHash(point.x(), seed) ^ qHash(point.y(), seed); // Используем хеширование координат
}

void MainWindow::generateField(int size, int width, int height)
{    
    for(int w=0; w<width/size; w++){
        for(int h=0; h<height/size; h++){
            bool isWhite = (QRandomGenerator::global()->generate() % 3 == 0);
            mapa.insert(QPoint(w*size,h*size),isWhite);
        }
    }
    paintMap(mapa);
}

//заполнение
void MainWindow::paintMap(QHash<QPoint, bool> map)
{
    for(QPoint pos : map.keys()){
        scene->addRect(pos.x(),pos.y(),50,50, QPen(Qt::black), map.value(pos)? QBrush(Qt::gray) : QBrush(Qt::yellow));
    }
}


/**
 * Функция для поиска пути на карте
 */
void MainWindow::paintPath()
{
    qDebug()<< "Find path";

//    if (startRow == -1 || startCol == -1 || endRow == -1 || endCol == -1) {
//        return; // Начальная или конечная точка не установлена
//    }

//    int width = ui->tableWidget->columnCount();
//    int height = ui->tableWidget->rowCount();

    // Проверка валидности координат
    if (start.isNull() || finish.isNull() || start==QPoint(-1,-1) || finish==QPoint(-1,-1)) {
        return;
    }

    if(mapa.value(start)){
         QMessageBox::warning(this, "Ошибка", "Начальная точка находится на черной клетке.");
         return;
    }

    if(mapa.value(finish)){
         QMessageBox::warning(this, "Ошибка", "Конечная точка находится на черной клетке.");
         return;
    }

    QHash<QPoint,bool> map;
    for(const QPoint& key : mapa.keys()){
        QPoint nKey = QPoint(key.x()/50, key.y()/50);
        map.insert(nKey, mapa.value(key));
    }

    QVector<QPoint> path = findPath(map, QPoint(start.x()/50, start.y()/50), QPoint(finish.x()/50, finish.y()/50));
    qDebug()<<"path  "<<path;
    if(path.size()>0){
        ui->statusbar->showMessage("Маршрут построен", 5000);
    }
    else{
        ui->statusbar->clearMessage();
         QMessageBox::warning(this, "Ошибка", "Построение маршрута невозможно.");
         return;
    }

    // Отображение пути на лабиринте
    for (QPoint point : path) {
        scene->addRect(QRectF(QPoint(point.x()*50,point.y()*50), QSize(10,10)), QPen(Qt::black), QBrush(Qt::blue));
    }

}

/**
 *  Алгоритм поиска пути
 *  return QVector<QPoint> map
 */
QVector<QPoint> MainWindow::findPath(const QHash<QPoint, bool>& map, const QPoint& start, const QPoint& end) {
    QVector<QPoint> path;
    // Check if start or end points are blocked
    if (map.value(start, false) || map.value(end, false)) {
        return path; // Return empty path if either point is blocked
    }

    // Определение направлений для перемещения
    QVector<QPoint> directions = {
        QPoint(1, 0),   // Вправо
        QPoint(-1, 0),  // Влево
        QPoint(0, 1),   // Вниз
        QPoint(0, -1)   // Вверх
    };

    QQueue<QPoint> queue;               // Очередь для BFS
    queue.enqueue(start);               // Добавляем стартовую точку в очередь

    QHash<QPoint, QPoint> cameFrom;     // Используем QHash для хранения предшественников
    cameFrom[start] = QPoint(-1, -1);   // Начальная точка не имеет предшественника

    while (!queue.isEmpty()) {
        QPoint current = queue.dequeue(); // Извлечение текущей точки из очереди

        // Check if we reached the end point
        if (current == end) {
            // Reconstruct the path
            QPoint p = end;
            while (p != QPoint(-1, -1)) {
                path.prepend(p);
                p = cameFrom[p];
            }

            return path;
        }

        // Explore neighbors
        for (const auto& direction : directions) {
            QPoint neighbor = current + direction;

            // Ensure neighbor is within bounds and not blocked
            if (!map.value(neighbor, true) && !cameFrom.contains(neighbor)) {
                queue.enqueue(neighbor);
                cameFrom[neighbor] = current; // Record where we came from
            }
        }
    }

    return path; // Return empty path if no path found
}
