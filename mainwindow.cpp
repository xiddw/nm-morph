#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    imageDialog = new QFileDialog(this);
    imageDialog->setFileMode(QFileDialog::ExistingFile);
    imageDialog->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    imageDialog->setViewMode(QFileDialog::Detail);

    img1 = new QImage();
    img2 = new QImage();

    scen1 = new QGraphicsScene();
    scen2 = new QGraphicsScene();
}

MainWindow::~MainWindow() {
    delete ui;
    delete imageDialog;
}

void MainWindow::on_pushButton1_clicked() {
    QStringList fileNames;
    if (!imageDialog->exec()) return;

    fileNames = imageDialog->selectedFiles();

    QString imagePath = fileNames.first();

    QSize scont = ui->graphicsView1->size();
    scont -= QSize(5, 5);

    img1->load(imagePath);

    scen1->clear();
    scen1->addPixmap(QPixmap::fromImage(img1->scaled(scont, Qt::KeepAspectRatio)));

    ui->graphicsView1->setScene(scen1);
}

void MainWindow::on_pushButton2_clicked() {
    QStringList fileNames;
    if (!imageDialog->exec()) return;

    fileNames = imageDialog->selectedFiles();

    QString imagePath = fileNames.first();

    QSize scont = ui->graphicsView1->size();
    scont -= QSize(5, 5);

    img2->load(imagePath);

    scen2->clear();
    scen2->addPixmap(QPixmap::fromImage(img2->scaled(scont, Qt::KeepAspectRatio)));

    ui->graphicsView2->clearMask();
    ui->graphicsView2->setScene(scen2);
}
