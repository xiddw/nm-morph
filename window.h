#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>

#include <QGraphicsView>
#include <QGraphicsScene>

#include "graphicsview.h"

#define LEFT 0
#define RIGTH 1

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    void LoadImage(bool second);

    QFileDialog *imageDialog;

    QImage *imgs[2];

    QFrame *frame[2];
    QVBoxLayout *imageContainer[2];

    QGraphicsScene *scen[2];
    GraphicsView *view[2];

    QPushButton *btnOpen[2];
    QLabel *lblRuta[2];

};

#endif // WINDOW_H
