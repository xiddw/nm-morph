#include "graphicsview.h"

QPen *GraphicsView::pen = NULL;

bool GraphicsView::straightLine = true;

//list<pair <QPoint, QPoint> >* GraphicsView::listLine = NULL;
//list<QPoint>* GraphicsView::listPoint = NULL;

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)  {
    enableDrawing(false);

    drawing = false;    
    straightLine = true;

    pen = new QPen(Qt::red);
    pen->setStyle(Qt::SolidLine);
    pen->setCapStyle(Qt::RoundCap);
    pen->setWidth(2);
    pen->setColor(Qt::red);

    listLine = new vector<pair <QPoint, QPoint> >();
    listPoint = new vector<QPoint>();

    timer = new QTimer();
    timer->setInterval(100);
    timer->setSingleShot(true);
    //connect(timer, SIGNAL(timeout()), this, SLOT(resizeImages()));
}

GraphicsView::~GraphicsView() {}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    drawing = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if(!able2Drawing) return;

    if(straightLine) {
        if(!drawing && (event->buttons() & Qt::LeftButton)) {
            x1 = event->x();
            y1 = event->y();
        }

        drawing = (event->buttons() & Qt::LeftButton);
    } else {
        x1 = event->x();
        y1 = event->y();
    }
}


void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(!able2Drawing) return;

    x2 = event->x();
    y2 = event->y();

    if(straightLine) {
        if(!drawing) {
            this->undoLastLine();
        } else {
            if(event->buttons() & Qt::LeftButton) {
                drawing = false;

                x1 = event->x();
                y1 = event->y();
            }
        }
        listLine->push_back( make_pair(QPoint(x1, y1), QPoint(x2, y2) ) );
    } else {
        listPoint->push_back( QPoint(x1, y1) );
        listPoint->push_back( QPoint(x2, y2) );

        this->scene()->addLine(x1, y1, x2, y2, *pen);
        x1 = x2; event->x();
        y1 = y2; event->y();
    }

    this->scene()->addLine(x1, y1, x2, y2, *pen);
}

void GraphicsView::resizeEvent(QResizeEvent *event) {
    timer->start();
}

void GraphicsView::resizeImages(void) {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    QList<QGraphicsItem *>::Iterator end = this->scene()->items().end();

    while(it != end) {
        this->fitInView(*it, Qt::KeepAspectRatio);
        it++;
    }
}

void GraphicsView::cleanLines() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    while(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it));
        it = this->scene()->items().begin();
    }

    if(straightLine)
        listLine->clear();
    else
        listPoint->clear();

    enableDrawing(false);
}

void GraphicsView::undoLastLine() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    if(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it));
    }

    if(straightLine)
        listLine->pop_back();
    else
        listPoint->pop_back();

    if(this->scene()->items().count() == 0) enableDrawing(false);
}

void GraphicsView::enableDrawing(bool v) {
    able2Drawing = v;
    setCursor(able2Drawing ? Qt::CrossCursor : Qt::ForbiddenCursor);
}

void GraphicsView::colorDrawing(QColor c) {
    GraphicsView::pen->setColor(c);
}
