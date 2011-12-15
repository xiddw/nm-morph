#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    colorPen = new QColor(Qt::red);
    wimg = himg = 0;
    VARA = VARP = 0.0f;

    this->setMinimumSize(600, 440);
    this->resize(1000, 660);
    this->setStyleSheet("QAbstractButton, QLabel { padding: 10px; };");

    QHBoxLayout *superLayout = new QHBoxLayout();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QSplitter *mainSplit = new QSplitter(Qt::Horizontal);

    QHBoxLayout *headLayout = new QHBoxLayout();

    QGroupBox *opcGrp1 = new QGroupBox(tr("Tipo de linea que desea usar"));
    opcGrp1->setMinimumHeight(80);
    opcGrp1->setMaximumHeight(80);
    //opcGrp1->setMinimumWidth(300);

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
    //opcGrp2->setMinimumWidth(300);
    opcGrp2->setMinimumHeight(150);
    opcGrp2->setMaximumHeight(150);

    QFormLayout *opcForm2 = new QFormLayout();
    opcGrp2->setLayout(opcForm2);

    btnColor = new QPushButton("Color de pincel: ");
    lblColor = new QLabel(colorPen->name());
    lblColor->setPalette(QPalette(colorPen->rgb()));
    lblColor->setAutoFillBackground(true);

    opcForm2->addRow(lblColor, btnColor);
    connect(btnColor, SIGNAL(clicked()), this, SLOT(on_btnColor_clicked()));

    QGroupBox *opcGrp3 = new QGroupBox(tr("Parámetros"));
    opcGrp3->setStyleSheet("QLabel { padding: 1px; }");
    opcGrp3->setMaximumWidth(200);
    opcGrp3->setMaximumHeight(150);

    QFormLayout *opcForm3 = new QFormLayout();
    opcGrp3->setLayout(opcForm3);

    txtvalA = new QLineEdit();
    txtvalA->setAlignment(Qt::AlignRight);
    txtvalA->setText("0.001");

    txtvalB = new QLineEdit();
    txtvalB->setAlignment(Qt::AlignRight);
    txtvalB->setText("2");

    txtvalP = new QLineEdit();
    txtvalP->setAlignment(Qt::AlignRight);
    txtvalP->setText("0");

    opcForm3->addRow(tr("A:"), txtvalA);
    opcForm3->addRow(tr("B:"), txtvalB);
    opcForm3->addRow(tr("P:"), txtvalP);

    headLayout->addWidget(opcGrp3);
    headLayout->addWidget(opcGrp2);

    mainSplit->setChildrenCollapsible(false);

    imgs[4] = new QImage();
    view[4] = new GraphicsView();
    view[4]->enableDrawing(false);

    scen[4] = new QGraphicsScene();
    view[4]->setScene(scen[4]);

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

    btnProcess = new QPushButton("Realizar morphing");
    connect(btnProcess, SIGNAL(clicked()), this, SLOT(on_btnProcess_clicked()));
    opcForm2->addRow(btnProcess);

    btnSave = new QPushButton("Guardar imagen resultante");
    connect(btnSave, SIGNAL(clicked()), this, SLOT(on_btnSave_clicked()));
    opcForm2->addRow(btnSave);

    imageContainer[2]->addWidget(view[2]);
    imageContainer[2]->addWidget(view[3]);
    imageContainer[2]->addWidget(view[4]);

    frame[2] = new QFrame();
    frame[2]->setLayout(imageContainer[2]);
    frame[2]->setMaximumWidth(300);

    mainLayout->addWidget(mainSplit);

    superLayout->addLayout(mainLayout);
    superLayout->addWidget(frame[2]);

    this->setLayout(superLayout);

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

double dotProd(QPoint a, QPoint b) {
    return (a.x()*b.x() + a.y()*b.y());
}

double norma(QPoint a) {
    return sqrt(pow(a.x(), 2.0) + pow(a.y(), 2.0));
}

double norma2(QPoint a) {
    return (pow(a.x(), 2.0) + pow(a.y(), 2.0));
}

void MainWindow::on_btnProcess_clicked() {
    VARA = this->txtvalA->text().toDouble();
    VARB = this->txtvalB->text().toDouble();
    VARP = this->txtvalP->text().toDouble();

    printf("%.4f, %.4f, %.4f\n", VARA, VARB, VARP);

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
    imgs[4] = new QImage(wimg, himg, imgs[0]->format());

    QRgb white = qRgba(255, 255, 255, 100);
    for(int i=0; i<wimg; ++i) {
        for(int j=0; j<himg; ++j) {
            imgs[4]->setPixel(i, j, white);
        }
    }

    QRgb r = qRgb(255, 0, 0);
    //QRgb g = qRgb(0, 255, 0);
    QRgb b = qRgb(0, 0, 255);

    if(GraphicsView::straightLine) {


        int lenght = view[0]->listLine->size();

        for(int k=0; k<lenght; ++k) {
            double x3, x4, y3, y4;
            x3 = x4 = 0;
            y3 = y4 = 0;

            for(int h=0; h<2; ++h) {
                double x1 = view[h]->listLine->at(k).first.x();
                double y1 = view[h]->listLine->at(k).first.y();

                double x2 = view[h]->listLine->at(k).second.x();
                double y2 = view[h]->listLine->at(k).second.y();

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
                        imgs[4]->setPixel(x1, y1, h?r:b);
                        x1 += 0.01;
                    }
                } else {
                    if(y1 > y2) {
                        double x = x1; x1 = x2; x2 = x;
                        double y = y1; y1 = y2; y2 =y ;
                    }

                    while(y1 < y2) {
                        x1 = (y1 - y2)/m + x2;
                        imgs[4]->setPixel(x1, y1, h?r:b);
                        y1 += 0.01;
                    }
                }
            }

            double m = (y4 - y3) / (x4 - x3);

            pair<QPoint, QPoint> p = make_pair(QPoint(x3, y3), QPoint(x4, y4));
            view[0]->listAux->push_back(p);
            view[1]->listAux->push_back(p);

            //Si x esta mas disperso que y
            if(fabs(x4-x3) > fabs(y4-y3)) {
                // Si coordenadas estan al reves
                if(x3 > x4) {
                    double x = x3; x3 = x4; x4 = x;
                    double y = y3; y3 = y4; y4 = y;
                }

                while(x3 < x4) {
                    y3 = m*(x3 - x4) + y4;
                    imgs[4]->setPixel(x3, y3, qRgb(0, 0, 0));
                    x3 += 0.01;
                }
            } else {
                // Si coordenadas estan al reves
                if(y3 > y4) {
                    double x = x3; x3 = x4; x4 = x;
                    double y = y3; y3 = y4; y4 = y;
                }

                while(y3 < y4) {
                    x3 = (y3 - y4)/m + x4;
                    imgs[4]->setPixel(x3, y3, qRgb(0, 0, 0));
                    y3 += 0.01;
                }
            }
        }

        vector< pair<QPoint, double> >* posibles;
        posibles = new vector< pair<QPoint, double> >();


        for(int h=0; h<2; ++h) {
            int n = 0;

            for(int i=0; i<wimg; ++i) {
                for(int j=0; j<himg; ++j) {

                    QPoint X(i, j);
                    double u, v;

                    double ww[lenght];
                    QPoint pp[lenght];

                    // Para cada linea
                    for(int k=0; k<lenght; ++k) {

                        //Obtener puntos originales de linea de referencia
                        QPoint P = view[h]->listLine->at(k).first;
                        QPoint Q = view[h]->listLine->at(k).second;

                        QPoint XP = X - P;
                        QPoint QP = Q - P;

                        QPoint pQP;
                        pQP.setX(QP.y());
                        pQP.setY(-QP.x());

                        // Calcular u, v
                        u = dotProd(XP, QP) / norma2(QP);
                        v = dotProd(XP, pQP) / norma(QP);

                        // Obtener puntos interpolados de linea de referencia
                        QPoint P2 = view[h]->listAux->at(k).first;
                        QPoint Q2 = view[h]->listAux->at(k).second;

                        QPoint Q2P2 = Q2 - P2;
                        QPoint pQ2P2;
                        pQ2P2.setX(Q2P2.y());
                        pQ2P2.setY(-Q2P2.x());

                        QPoint X2 = P2 + u*Q2P2 + (v * pQ2P2) / norma(Q2P2);
                        QPoint p = X2 - X;

                        double dist = 0;
                        if(u > 0 && u < 1) dist = fabs(v);
                        else if(u < 0) dist = sqrt(pow(X.x() - P.x(), 2.0) + pow(X.y() - P.y(), 2.0));
                        else dist = sqrt(pow(X.x() - Q.x(), 2.0) + pow(X.y() - Q.y(), 2.0));

                        double w = 0;
                        w =  pow(norma(QP), VARP);
                        w /= (VARA + dist);
                        w = pow(w, VARB);

                        ww[k] = w;
                        pp[k] = p;
                        //posibles->push_back(make_pair(p, w));
                    }

                    QPoint sum(0.0, 0.0);
                    double wsum = 0;
                    for(int k=0; k<lenght; ++k) {
                        sum  += ww[k] * pp[k];
                        wsum += ww[k];
                    }

                    sum /= wsum;

                    QPoint X2 = X + sum;


                    double y0, x0;
                    if(X2.y() < himg && X2.y() > 0.5) {
                        y0 = ceil(X2.y());
                    } else if(X2.y() >= himg) {
                        y0 = himg - 1;
                    } else {
                        y0 = 0;
                    }

                    if(X2.x() < wimg && X2.x() > 0.5) {
                        x0 = ceil(X2.x());
                    } else if(X2.y() >= wimg) {
                        x0 = wimg - 1;
                    } else {
                        x0 = 0;
                    }

                    if(X2 == X)  n++;
                    imgs[h+2]->setPixel(X, imgs[h]->pixel(x0, y0));
                }
            }

            //printf("Imagen %02d: %d / %d ", h+1, n, (wimg * himg));
        }


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

    for(int h=2; h<5; ++h) {
        scen[h]->clear();
        scen[h]->setSceneRect(0, 0, wimg, himg);
        scen[h]->addPixmap(QPixmap::fromImage(*imgs[h]));

        view[h]->fitInView(*view[h]->scene()->items().begin(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_btnSave_clicked() {
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
