#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {

    this->setMinimumWidth(600);
    this->setMinimumHeight(400);

    imageDialog = new QFileDialog(this);
    imageDialog->setFileMode(QFileDialog::ExistingFile);
    imageDialog->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    imageDialog->setViewMode(QFileDialog::Detail);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    QGroupBox *opcGrp = new QGroupBox();
    QFormLayout *opcForm = new QFormLayout();
    opcGrp->setLayout(opcForm);

    QLabel *lbl1 = new QLabel(tr("Seleccione el tipo de linea que desea usar"));
    opcForm->addRow(lbl1);

    QRadioButton *radio1 = new QRadioButton(tr("Segmentos de lineas rectas"));
    opcForm->addRow(radio1);

    QRadioButton *radio2 = new QRadioButton(tr("Linea continua a pulso"));
    opcForm->addRow(radio2);

    mainLayout->addWidget(opcGrp);

    mainSplit->setChildrenCollapsible(false);

    for(int i=0; i<2; ++i) {
        imgs[i] = new QImage();
        view[i] = new GraphicsView();
        scen[i] = new QGraphicsScene();

//        lblRuta[i] = new QLabel(tr("as"));
        btnOpen[i] = new QPushButton(tr("Cargar imagen"));
        btnOpen[i]->setStyleSheet("padding: 10px; ");
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));

        imageContainer[i] = new QVBoxLayout();
        //imageContainer[i]->addWidget(lblRuta[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        mainSplit->addWidget(frame[i]);
        view[i]->setScene(scen[i]);
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

void MainWindow::on_pushButton_clicked() {
    bool pos = (sender() == btnOpen[RIGTH]);
    LoadImage(pos);
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!imageDialog->exec()) return;

    fileNames = imageDialog->selectedFiles();

    QString imagePath = fileNames.first();

    //lblRuta[pos]->setText(imagePath);

    QSize sizecont = view[pos]->size();
    sizecont -= QSize(5, 5);

    imgs[pos]->load(imagePath);

    scen[pos]->clear();
    scen[pos]->addPixmap(QPixmap::fromImage(imgs[pos]->scaled(sizecont, Qt::KeepAspectRatio)));
    scen[pos]->setSceneRect(0, 0, sizecont.width(), sizecont.height());

}
