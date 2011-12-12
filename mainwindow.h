#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <QGraphicsView>
#include <QGraphicsScene>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton1_clicked();

    void on_pushButton2_clicked();

private:
    Ui::MainWindow *ui;

    QFileDialog *imageDialog;

    QImage *img1, *img2;

    QGraphicsScene *scen1, *scen2;
};

#endif // MAINWINDOW_H
