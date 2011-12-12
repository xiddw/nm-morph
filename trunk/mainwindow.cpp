#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    this->setMinimumWidth(600);
    this->setMinimumHeight(400);

    imageDialog = new QFileDialog(this);
    imageDialog->setFileMode(QFileDialog::ExistingFile);
    imageDialog->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    imageDialog->setViewMode(QFileDialog::Detail);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    mainSplit->setChildrenCollapsible(false);

    for(int i=0; i<2; ++i) {
        imgs[i] = new QImage();
        view[i] = new QGraphicsView();
        scen[i] = new QGraphicsScene();
        btnOpen[i] = new QPushButton(tr("Cargar imagen"));

        frame[i] = new QFrame();
        imageContainer[i] = new QVBoxLayout();

        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i]->setLayout(imageContainer[i]);
        mainSplit->addWidget(frame[i]);

        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    }

    mainLayout->addWidget(mainSplit);
    this->setLayout(mainLayout);
}

MainWindow::~MainWindow() {
    delete imageDialog;

    delete[] imgs;
    delete[] scen;
    delete[] view;
    delete[] btnOpen;

    delete[] frame;
    delete[] imageContainer;
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!imageDialog->exec()) return;

    fileNames = imageDialog->selectedFiles();

    QString imagePath = fileNames.first();

    QSize scont = view[pos]->size();
    scont -= QSize(5, 5);

    imgs[pos]->load(imagePath);

    scen[pos]->clear();
    scen[pos]->addPixmap(QPixmap::fromImage(imgs[pos]->scaled(scont, Qt::KeepAspectRatio)));

    view[pos]->setScene(scen[pos]);
}

void MainWindow::on_pushButton_clicked() {
    if(sender() == btnOpen[LEFT]) {
        LoadImage(LEFT);
    } else {
        LoadImage(RIGTH);
    }
}
