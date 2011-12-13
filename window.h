#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
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

protected:
    void keyPressEvent(QKeyEvent *event);
    //void closeEvent(QCloseEvent *event);

private slots:
    void on_btnOpen_clicked();
    void on_btnClear_clicked();
    void on_btnUndo_clicked();

    void on_btnColor_clicked();

private:
    void LoadImage(bool second);
    void CleanCanvas(bool second);
    void UndoLineCanvas(bool second);

    QFileDialog *diaImage;
    QColorDialog *diaColor;
    QColor *colorPen;

    QImage *imgs[2];

    QFrame *frame[2];
    QVBoxLayout *imageContainer[2];

    QGraphicsScene *scen[2];
    GraphicsView *view[2];

    QPushButton *btnOpen[2];
    QLineEdit *txtRuta[2];

    QLabel *lblColor;

    QRadioButton *radio[2];

    QPushButton *btnUndo[2], *btnClear[2];
    QPushButton *btnColor;
};

#endif // WINDOW_H
