#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QtGui>
#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
public:    
    GraphicsView();

    static void colorDrawing(QColor);
    static bool straightLine;

    void enableDrawing(bool);

    void cleanLines(void);
    void undoLastLine(void);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    bool drawing;
    bool able2Drawing;
    static QPen *pen;

    int x1, x2, y1, y2;
};

#endif // GRAPHICSVIEW_H
