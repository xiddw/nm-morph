#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    colorPen = new QColor(Qt::red);
    wimg = himg = 0;

    this->setMinimumSize(600, 440);
    this->resize(900, 660);
    this->setStyleSheet("QAbstractButton, QLabel { padding: 10px; };");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    QHBoxLayout *headLayout = new QHBoxLayout();

    QGroupBox *opcGrp1 = new QGroupBox(tr("Tipo de linea que desea usar"));
    opcGrp1->setMinimumHeight(80);
    opcGrp1->setMaximumHeight(80);

    QFormLayout *opcForm1 = new QFormLayout();
    opcGrp1->setLayout(opcForm1);

    radio[0] = new QRadioButton(tr("Segmentos de lineas rectas"));
    radio[0]->setStyleSheet("padding: 1px");
    connect(radio[0], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));

    radio[0]->setChecked(true);
    opcForm1->addRow(radio[0]);

    radio[1] = new QRadioButton(tr("Linea continua a mano"));
    radio[1]->setStyleSheet("padding: 1px");
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

        view[i] = new GraphicsView();
        view[i]->enableDrawing(false);
        view[i+2] = new GraphicsView();
        view[i+2]->enableDrawing(false);

        scen[i] = new QGraphicsScene();
        scen[i+2] = new QGraphicsScene();

        btnUndo[i] = new QPushButton("Deshacer ultimo trazo");
        connect(btnUndo[i], SIGNAL(clicked()), this, SLOT(on_btnUndo_clicked()));

        btnClear[i] = new QPushButton("Limpiar lienzo");
        connect(btnClear[i],SIGNAL(clicked()),this,SLOT(on_btnClear_clicked()));

        btnOpen[i] = new QPushButton(tr("Cargar imagen"));
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));

        imageContainer[i] = new QVBoxLayout();

        imageContainer[i]->addWidget(btnUndo[i]);
        imageContainer[i]->addWidget(btnClear[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        mainSplit->addWidget(frame[i]);

        view[i]->setScene(scen[i]);
        view[i+2]->setScene(scen[i+2]);
    }

    imageContainer[2] = new QVBoxLayout();

    btnProcess = new QPushButton("¡Procesar!");
    connect(btnProcess, SIGNAL(clicked()), this, SLOT(on_btnProcess_clicked()));

    btnSave = new QPushButton("Guardar imagen");
    connect(btnSave, SIGNAL(clicked()), this, SLOT(on_btnSave_clicked()));

    imageContainer[2]->addWidget(btnProcess);
    imageContainer[2]->addWidget(btnSave);
    imageContainer[2]->addWidget(view[2]);
    imageContainer[2]->addWidget(view[3]);

    frame[2] = new QFrame();
    frame[2]->setLayout(imageContainer[2]);
    mainSplit->addWidget(frame[2]);

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
    if (e->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(e);
    }
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

void MainWindow::on_btnProcess_clicked() {
    // Revisar que ambas imagenes esten cargadas
    if(loadimg[0] && loadimg[1] == 0) {
        QMessageBox::critical(this,
            tr("Proyecto Final - ALN - Morphing - RJRJ"),
            tr("Faltan imágenes por cargar para poder procesarlas."),
            QMessageBox::Ok, QMessageBox::Ok
        );

        return;
    }

    // Revisar que cada imagen tenga el mismo numero de lineas
    if(!(GraphicsView::straightLine &&
            view[0]->listLine->size() == view[1]->listLine->size())) {
        QMessageBox::critical(this,
            tr("Proyecto Final - ALN - Morphing - RJRJ"),
            tr("Las imagenes no tienen el mismo numero de lineas."),
            QMessageBox::Ok, QMessageBox::Ok
        );

        return;
    }


    imgs[2] = new QImage(wimg, himg, imgs[0]->format());
    imgs[3] = new QImage(wimg, himg, imgs[0]->format());


    QRgb white = qRgba(255, 255, 255, 100);
    for(int i=0; i<wimg; ++i) {
        for(int j=0; j<himg; ++j) {
            QRgb a = imgs[0]->pixel(i, j);
            QRgb b = imgs[1]->pixel(i, j);

            float f = 0.5;
            float g = 1 - f;

            QRgb c = qRgb(f*qRed(a) + g*qRed(b),
                          f*qGreen(a) + g*qGreen(b),
                          f*qBlue(a) + g*qBlue(b)) ;

            imgs[2]->setPixel(i, j, white);
            imgs[3]->setPixel(i, j, c);
        }
    }

    QRgb r = qRgb(255, 0, 0);
    QRgb g = qRgb(0, 255, 0);
    QRgb b = qRgb(0, 0, 255);
    QRgb k = qRgb(0, 0, 0);

    if(GraphicsView::straightLine) {


        int lenght = view[0]->listLine->size();

        for(int i=0; i<lenght; ++i) {
            double x3, x4, y3, y4;
            x3 = x4 = 0;
            y3 = y4 = 0;

            for(int h=0; h<2; ++h) {
                double x1 = view[h]->listLine->at(i).first.x();
                double y1 = view[h]->listLine->at(i).first.y();

                double x2 = view[h]->listLine->at(i).second.x();
                double y2 = view[h]->listLine->at(i).second.y();

                double m = (y2 - y1) / (x2 - x1);

                x3 += x1 / 2;
                x4 += x2 / 2;

                y3 += y1 / 2;
                y4 += y2 / 2;

                if(fabs(x1-x2) > fabs(y1-y2)) {
                    if(x1 > x2) {
                        double x = x1; x1 = x2; x2 = x;
                        double y = y1; y1 = y2; y2 =y ;
                    }

                    while(x1 < x2) {
                        y1 = m*(x1 - x2) + y2;
                        imgs[2]->setPixel(x1, y1, h?r:b);
                        x1 += 0.01;
                    }
                } else {
                    if(y1 > y2) {
                        double x = x1; x1 = x2; x2 = x;
                        double y = y1; y1 = y2; y2 =y ;
                    }

                    while(y1 < y2) {
                        x1 = (y1 - y2)/m + x2;
                        imgs[2]->setPixel(x1, y1, h?r:b);
                        y1 += 0.01;
                    }
                }
            }

            double m = (y4 - y3) / (x4 - x3);
            if(fabs(x4-x3) > fabs(y4-y3)) {
                if(x3 > x4) {
                    double x = x3; x3 = x4; x4 = x;
                    double y = y3; y3 = y4; y4 = y;
                }

                while(x3 < x4) {
                    y3 = m*(x3 - x4) + y4;
                    imgs[2]->setPixel(x3, y3, k);
                    x3 += 0.01;
                }
            } else {
                if(y3 > y4) {
                    double x = x3; x3 = x4; x4 = x;
                    double y = y3; y3 = y4; y4 = y;
                }

                while(y3 < y4) {
                    x3 = (y3 - y4)/m + x4;
                    imgs[2]->setPixel(x3, y3, k);
                    y3 += 0.01;
                }
            }





        }

//        for(int i=0; i<lenght; ++i) {
//            for(int h=0; h<2; ++h) {
//            }
//        }

    } else {
        for(int k=0; k<2; ++k) {
            for(int i = 0, j = view[k]->listPoint->size(); i<j; ++i) {

                int x = view[k]->listPoint->at(i).x();
                int y = view[k]->listPoint->at(i).y();

                if(x >= 0 && x < wimg && y >= 0 && y < himg)
                    imgs[2]->setPixel(x, y, k?r:b);
            }
        }
    }

    QSize s = view[2]->size();
    scen[2]->clear();
    scen[2]->setSceneRect(0, 0, wimg, himg);
    scen[2]->addPixmap(QPixmap::fromImage(*imgs[2])); //->scaled(s, Qt::KeepAspectRatio)));

    view[2]->fitInView(*view[2]->scene()->items().begin(), Qt::KeepAspectRatio);

    scen[3]->clear();
    scen[3]->setSceneRect(0, 0, wimg, himg);
    scen[3]->addPixmap(QPixmap::fromImage(*imgs[3])); //->scaled(s, Qt::KeepAspectRatio)));

    view[3]->fitInView(*view[3]->scene()->items().begin(), Qt::KeepAspectRatio);
    //view[2]->resize(wimg, himg);
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!diaImage->exec()) return;

    fileNames = diaImage->selectedFiles();

    QString imagePath = fileNames.first();

    QSize sizecont = view[pos]->size();
    sizecont -= QSize(5, 5);

    imgs[pos]->load(imagePath);

    // Primer imagen cargada
    if(wimg == 0 && himg == 0) {
        wimg = imgs[pos]->width();
        himg = imgs[pos]->height();
    } else {
    // Segunda imagen cargada
        if( (wimg != imgs[pos]->width()) ||
            (himg = imgs[pos]->height())) {

//            QMessageBox::critical(this,
//                tr("Proyecto Final - ALN - Morphing - RJRJ"),
//                tr("La imagen cargada no coincide en dimensiones con la otra imagen"),
//                QMessageBox::Ok, QMessageBox::Ok
//            );

//            imgs[pos] = NULL;
//            return;
        }
    }

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

    loadimg[pos] = true;
}

void MainWindow::CleanCanvas(bool pos) {
    view[pos]->cleanLines();
    loadimg[pos] = false;

    if(loadimg[0] && loadimg[1] == 0) {
        wimg = himg = 0;
    }
}

void MainWindow::UndoLineCanvas(bool pos) {
    view[pos]->undoLastLine();
}
