#include "graphicsview.h"

QPen *GraphicsView::pen = NULL;

GraphicsView::GraphicsView()  {
    enableDrawing(false);

    drawing = false;    
    straightLine = true;

    pen = new QPen(Qt::red);
    pen->setStyle(Qt::SolidLine);
    pen->setCapStyle(Qt::RoundCap);
    pen->setWidth(2);
    pen->setColor(Qt::red);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    drawing = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if(!able2Drawing) return;

    if(!straightLine) {
        if(!drawing && (event->buttons() & Qt::LeftButton)) {
            x1 = event->x();
            y1 = event->y();
        }

        drawing = (event->buttons() & Qt::LeftButton);
    } else {
        if(!drawing && (event->buttons() & Qt::LeftButton)) {
            x1 = event->x();
            y1 = event->y();
        }

        drawing = (event->buttons() & Qt::LeftButton);
    }
}


void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(!able2Drawing) return;

    if(!straightLine) {
        if(!drawing) {
            x2 = event->x();
            y2 = event->y();

            QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
            if(it != this->scene()->items().end()) {
                this->scene()->removeItem(*(it));
            }
            this->scene()->addLine(x1, y1, x2, y2, *pen);

        } else {
            x2 = event->x();
            y2 = event->y();

            if(event->buttons() & Qt::LeftButton) {
                drawing = false;
            }

            this->scene()->addLine(x1, y1, x2, y2, *pen);
        }
    } else {
        if(!drawing) {
            x2 = event->x();
            y2 = event->y();

            QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
            if(it != this->scene()->items().end()) {
                this->scene()->removeItem(*(it));
            }
            this->scene()->addLine(x1, y1, x2, y2, *pen);

        } else {
            x2 = event->x();
            y2 = event->y();

            if(event->buttons() & Qt::LeftButton) {
                drawing = false;
            }

            this->scene()->addLine(x1, y1, x2, y2, *pen);
        }
    }
}

void GraphicsView::cleanLines() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    while(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it));
        it = this->scene()->items().begin();
    }

    enableDrawing(false);
}

void GraphicsView::undoLastLine() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    if(it != this->scene()->items().end()) {
        this->scene()->removeItem(*(it));
    }

    if(this->scene()->items().count() == 0) enableDrawing(false);
}

void GraphicsView::enableDrawing(bool v) {
    able2Drawing = v;
    setCursor(able2Drawing ? Qt::CrossCursor : Qt::ForbiddenCursor);
}

void GraphicsView::colorDrawing(QColor c) {
    GraphicsView::pen->setColor(c);
}
