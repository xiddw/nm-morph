#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QtGui>
#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
    Q_OBJECT

public:    
    GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    static void colorDrawing(QColor);
    static bool straightLine;

    void enableDrawing(bool);

    void cleanLines(void);
    void undoLastLine(void);


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

protected slots:
    void resizeImages();

private:
    bool drawing;
    bool able2Drawing;
    static QPen *pen;

    QTimer *timer;

    int x1, x2, y1, y2;
};

#endif // GRAPHICSVIEW_H
