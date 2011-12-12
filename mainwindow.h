#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>

#include <QGraphicsView>
#include <QGraphicsScene>

#define LEFT 0
#define RIGTH 1

namespace Ui {
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
//    void on_pushButton1_clicked();
//    void on_pushButton2_clicked();

private:
    void LoadImage(bool second);
    //Ui::MainWindow *ui;

    QFileDialog *imageDialog;

    QImage *imgs[2];

    QFrame *frame[2];
    QVBoxLayout *imageContainer[2];

    QGraphicsScene *scen[2];
    QGraphicsView *view[2];

    QPushButton *btnOpen[2];
};

#endif // MAINWINDOW_H
