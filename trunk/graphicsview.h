#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QtGui>
#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
public:
    GraphicsView();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool drawing;

    int x1, x2, y1, y2;
};

#endif // GRAPHICSVIEW_H
