#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    colorPen = new QColor(Qt::red);

    this->setMinimumSize(600, 420);
    this->resize(800, 600);
    this->setStyleSheet("QPushButton, QLabel { padding: 10px; } ");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    QHBoxLayout *headLayout = new QHBoxLayout();

    QGroupBox *opcGrp1 = new QGroupBox(tr("Tipo de linea que desea usar"));
    opcGrp1->setMinimumHeight(80);
    opcGrp1->setMaximumHeight(80);

    QFormLayout *opcForm1 = new QFormLayout();
    opcGrp1->setLayout(opcForm1);

    radio[0] = new QRadioButton(tr("Segmentos de lineas rectas"));
    connect(radio[0], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));

    radio[0]->setChecked(true);
    opcForm1->addRow(radio[0]);

    radio[1] = new QRadioButton(tr("Linea continua a mano"));
    connect(radio[1], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));
    opcForm1->addRow(radio[1]);

    headLayout->addWidget(opcGrp1);
    mainLayout->addLayout(headLayout);

    QGroupBox *opcGrp2 = new QGroupBox(tr("Controles"));
    opcGrp2->setMinimumHeight(80);
    opcGrp2->setMaximumHeight(80);

    QFormLayout *opcForm2 = new QFormLayout();
    opcGrp2->setLayout(opcForm2);

    btnColor = new QPushButton("Color de pincel: ");
    lblColor = new QLabel(colorPen->name());
    lblColor->setPalette(QPalette(colorPen->rgb()));
    lblColor->setAutoFillBackground(true);

    opcForm2->addRow(lblColor, btnColor);
    connect(btnColor, SIGNAL(clicked()), this, SLOT(on_btnColor_clicked()));
    headLayout->addWidget(opcGrp2);

    mainSplit->setChildrenCollapsible(false);

    for(int i=0; i<2; ++i) {
        imgs[i] = new QImage();
        //pix[i] = new QPixmap();

        view[i] = new GraphicsView();
        view[i]->enableDrawing(false);

        scen[i] = new QGraphicsScene();

        btnUndo[i] = new QPushButton("Deshacer ultimo trazo");
        connect(btnUndo[i], SIGNAL(clicked()), this, SLOT(on_btnUndo_clicked()));

        btnClear[i] = new QPushButton("Limpiar lienzo");
        connect(btnClear[i], SIGNAL(clicked()), this, SLOT(on_btnClear_clicked()));

        btnOpen[i] = new QPushButton(tr("Cargar imagen"));
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));

        imageContainer[i] = new QVBoxLayout();

        //txtRuta[i] = new QLineEdit(tr("hola"));
        //txtRuta[i]->setDisabled(true);
        //imageContainer[i]->addWidget(txtRuta[i]);

        imageContainer[i]->addWidget(btnUndo[i]);
        imageContainer[i]->addWidget(btnClear[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        mainSplit->addWidget(frame[i]);
        view[i]->setScene(scen[i]);
    }

    mainLayout->addWidget(mainSplit);
    this->setLayout(mainLayout);

    diaImage = new QFileDialog(this);
    diaImage->setFileMode(QFileDialog::ExistingFile);
    diaImage->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    diaImage->setViewMode(QFileDialog::Detail);

    diaColor = new QColorDialog(this);
    diaColor->setCurrentColor(colorPen->rgb());
}

MainWindow::~MainWindow() {
//    delete imageDialog;

//    delete[] imgs;
//    delete[] scen;
//    delete[] view;
//    delete[] btnOpen;

//    delete[] frame;
//    delete[] imageContainer;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void MainWindow::on_radioLinea_toogled(bool a) {
    GraphicsView::straightLine = (sender() == radio[!a] && a);
}

void MainWindow::on_btnOpen_clicked() {
    bool pos = (sender() == btnOpen[RIGTH]);
    LoadImage(pos);
}


void MainWindow::on_btnClear_clicked() {
    bool pos = (sender() == btnClear[RIGTH]);
    CleanCanvas(pos);
}

void MainWindow::on_btnUndo_clicked() {
    bool pos = (sender() == btnUndo[RIGTH]);
    UndoLineCanvas(pos);
}

void MainWindow::on_btnColor_clicked() {
    if(!diaColor->exec()) return;

    *colorPen = diaColor->selectedColor();
    lblColor->setText(colorPen->name());
    lblColor->setPalette(QPalette(colorPen->rgb()));
    GraphicsView::colorDrawing(*colorPen);
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!diaImage->exec()) return;

    fileNames = diaImage->selectedFiles();

    QString imagePath = fileNames.first();

    QSize sizecont = view[pos]->size();
    sizecont -= QSize(5, 5);

    imgs[pos]->load(imagePath);

    //QPixmap pix = QPixmap::fromImage(imgs[pos]->scaled(imgs[pos]->size()));
    scen[pos]->clear();
    scen[pos]->setSceneRect(0, 0, sizecont.width(), sizecont.height());
    //scen[pos]->addPixmap(QPixmap::fromImage(imgs[pos]->scaled(sizecont, Qt::KeepAspectRatio)));
    scen[pos]->addPixmap(QPixmap::fromImage(*imgs[pos]));

    view[pos]->enableDrawing(true);
    //view[pos]->fitInView(view[pos]->scene()->items().at(0), Qt::KeepAspectRatio);
    //view[pos]->fitInView(0, 0, sizecont.width(), sizecont.height(), Qt::KeepAspectRatio) ;
    view[pos]->resize(sizecont);
    //view[pos]->scale(2, 2);
}

void MainWindow::CleanCanvas(bool pos) {
    view[pos]->cleanLines();
}

void MainWindow::UndoLineCanvas(bool pos) {
    view[pos]->undoLastLine();
}
