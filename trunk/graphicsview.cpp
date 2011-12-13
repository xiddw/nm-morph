#include "graphicsview.h"

QColor *GraphicsView::colorDrawing = new QColor(Qt::red);

GraphicsView::GraphicsView()  {
    enableDrawing = false;
    drawing = false;    
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    drawing = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if(!enableDrawing) return;

    if(!drawing && (event->buttons() & Qt::LeftButton)) {
        x1 = event->x() - 10;
        y1 = event->y() - 10;
    }

    drawing = (event->buttons() & Qt::LeftButton);
    setCursor(Qt::CrossCursor);
}


void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(!enableDrawing) return;

    setCursor(Qt::CrossCursor);

    QPen pen(Qt::red);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(2);


    if(!drawing) {
//        drawing = (event->buttons() & Qt::LeftButton);

//        if(drawing) {
//            x1 = event->x() - 10;
//            y1 = event->y() - 10;
//        }

        x2 = event->x() - 10;
        y2 = event->y() - 10;

        QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
        if(it != this->scene()->items().end()) {
            this->scene()->removeItem(*(it));
        }

        pen.setColor(GraphicsView::colorDrawing->rgb());
        this->scene()->addLine(x1, y1, x2, y2, pen);

    } else {
        x2 = event->x() - 10;
        y2 = event->y() - 10;

        if(event->buttons() & Qt::LeftButton) {
            drawing = false;
        } else {


        }

        this->scene()->addLine(x1, y1, x2, y2, pen);
    }
}

void GraphicsView::cleanLines() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    while(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it++));
    }
}

void GraphicsView::undoLastLine() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    if(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it));
    }
}
