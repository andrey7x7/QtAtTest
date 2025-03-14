#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("AiTest");
    connect(ui->start, SIGNAL(triggered()), this, SLOT(startAndGenerate()));
    connect(ui->exit, SIGNAL(triggered()), this, SLOT(close()));

    ui->action800x600->setProperty("index",QSize(800,600));
    connect(ui->action800x600,  SIGNAL(triggered()), this, SLOT(resizeWindow()));

    ui->action1024x768->setProperty("index",QSize(1024,768));
    connect(ui->action1024x768,  SIGNAL(triggered()), this, SLOT(resizeWindow()));

    ui->action1920x1080->setProperty("index",QSize(1920,1080));
    connect(ui->action1920x1080,  SIGNAL(triggered()), this, SLOT(resizeWindow()));

    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,782,533);

    squareSize = 26;
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

void MainWindow::resizeWindow()
{
    QAction* action = qobject_cast<QAction*>(QObject::sender());
    QSize size = action->property("index").toSize();
    resize(size);
    setMinimumSize(size);
    setMaximumSize(size);
    setGeometry(geometry().x(), geometry().y(), size.width(), size.height());
}

/**
    Коммент
*/
void MainWindow::startAndGenerate()
{
    start = QPoint(-1,-1);
    finish = QPoint(-1,-1);
    widthPlane=width();
    heightPlane=height();
    scene->setSceneRect(0,0,widthPlane-18,heightPlane-70);

    generateField(squareSize, widthPlane-30, heightPlane-70);
    model = new Model();
    scene->addItem(model);
    model->setPos(plane1->width()/2, plane1->height()/2);
    ui->statusbar->showMessage("start");
    pos = model->pos();
}

void MainWindow::generateField(int size, int width, int height)
{
    mapa.clear();
    scene->clear();
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
        scene->addRect(pos.x(),pos.y(),squareSize,squareSize, QPen(Qt::black), map.value(pos)? QBrush(Qt::gray) : QBrush(Qt::yellow));
    }
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
    //нормализация точки по левому верхнему углу квадрата
    setStartFinish(QPoint(eventPress->pos().x()/squareSize*squareSize,
                          eventPress->pos().y()/squareSize*squareSize));
}

void MainWindow::setStartFinish(QPoint point){

    if(mapa.size()>0){
        if(start!=QPoint(-1,-1) && finish!=QPoint(-1,-1)){
            paintMap(mapa);
            start = QPoint(-1,-1);
            finish = QPoint(-1,-1);
        }

        if(start==QPoint(-1,-1)){
            start = point;
            scene->addRect(QRectF(point, QSize(squareSize,squareSize)), QPen(Qt::black), QBrush(Qt::red));
        } else if(finish==QPoint(-1,-1)){
            finish = point;
            scene->addRect(QRectF(point, QSize(squareSize,squareSize)), QPen(Qt::black), QBrush(Qt::green));
            paintPath();
        }
    }
    else{
        ui->statusbar->showMessage("Карта не создана, построение маршрута невозможно", 5000);
    }
}

// Определение функции qHash для использования QPoint в QMap
uint qHash(const QPoint &point, uint seed = 0) {
     return qHash(point.x(), seed) ^ qHash(point.y(), seed); // Используем хеширование координат
}

/**
 * Функция для поиска пути на карте
 */
void MainWindow::paintPath()
{
    // Проверка валидности координат
    if (start==QPoint(-1,-1) || finish==QPoint(-1,-1)) {
        ui->statusbar->showMessage("Координаты не валидны", 5000);
        qDebug()<<"Координаты "<<start<<finish<<" не валидны";
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
        QPoint nKey = QPoint(key.x()/squareSize, key.y()/squareSize);
        map.insert(nKey, mapa.value(key));
    }

    QVector<QPoint> path = findPath(mapa, QPoint(start.x(), start.y()), QPoint(finish.x(), finish.y()));
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
        scene->addEllipse(QRectF(QPoint(point.x()+(squareSize/3),point.y()+(squareSize/3)), QSize(squareSize/3,squareSize/3)), QPen(Qt::black), QBrush(Qt::blue));
    }

}

/**
 *  Алгоритм поиска пути
 *  return QVector<QPoint> map
 */
QVector<QPoint> MainWindow::findPath(const QHash<QPoint, bool>& map, const QPoint& start, const QPoint& end) {
        qDebug()<< "Find path";
    QVector<QPoint> path;
    // Проверка что тарт и финиш не блокированы
    if (map.value(start, false) || map.value(end, false)) {
        return path; // Возвращаем пусто если блокированы
    }

    QVector<QPoint> directions = {
        QPoint(squareSize, 0),   // Вправо
        QPoint(-squareSize, 0),  // Влево
        QPoint(0, squareSize),   // Вниз
        QPoint(0, -squareSize)   // Вверх
    };

    QQueue<QPoint> queue;               // Очередь для BFS
    queue.enqueue(start);               // Добавляем стартовую точку в очередь

    QHash<QPoint, QPoint> cameFrom;     // Используем QHash для хранения предшественников
    cameFrom[start] = QPoint(-1, -1);   // Начальная точка не имеет предшественника

    while (!queue.isEmpty()) {
        QPoint current = queue.dequeue(); // Извлечение текущей точки из очереди

        // Проверка конечной точки
        if (current == end) {
            // Восстановление пути
            QPoint p = end;
            while (p != QPoint(-1, -1)) {
                path.prepend(p);
                p = cameFrom[p];
            }

            return path;
        }

        // Приверка соседа
        for (const auto& direction : directions) {
            QPoint neighbor = current + direction;

            // Проверка что сосед не блокирован
            if (!map.value(neighbor, true) && !cameFrom.contains(neighbor)) {
                queue.enqueue(neighbor);
                cameFrom[neighbor] = current;
            }
        }
    }

    return path; // Возвращаем пусто если путь не найден
}
