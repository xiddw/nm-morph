#include "window.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    //colorPen = new QColor(Qt::red);
    wimg = himg = 0;
    VARA = VARP = 0.0f;

    this->setMinimumSize(600, 440);
    this->resize(1000, 660);
    this->setStyleSheet("QAbstractButton, QLabel { padding: 6px 10px; };");

    // Layout para toda la ventana
    QHBoxLayout *superLayout = new QHBoxLayout();

    // Layout para componentes principales
    // (cargar imagenes y controles)
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Layout para cargar imagenes
    QHBoxLayout *imgLayout = new QHBoxLayout();

    // Layout para controles
    QHBoxLayout *headLayout = new QHBoxLayout();

    // Layout para Tipo de linea a usar
    QGroupBox *opcGrp1 = new QGroupBox(tr("Tipo de linea que desea usar"));
    opcGrp1->setMinimumHeight(80);
    opcGrp1->setMaximumHeight(80);
    QFormLayout *opcForm1 = new QFormLayout();
    opcGrp1->setLayout(opcForm1);
    radio[0] = new QRadioButton(tr("Segmentos de lineas rectas"));
    radio[0]->setStyleSheet("padding: 1px");    
    radio[0]->setChecked(true);    
    radio[1] = new QRadioButton(tr("Linea continua a mano"));
    radio[1]->setStyleSheet("padding: 1px");
    connect(radio[0], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));
    connect(radio[1], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));
    opcForm1->addRow(radio[0]);
    opcForm1->addRow(radio[1]);

    // Layout para Controles basicos
    QGroupBox *opcGrp2 = new QGroupBox(tr("Controles"));
    opcGrp2->setMinimumHeight(150);
    opcGrp2->setMaximumHeight(150);
    QFormLayout *opcForm2 = new QFormLayout();
    opcGrp2->setLayout(opcForm2);
    btnColor = new QPushButton("Color de pincel: ");
    btnProcess = new QPushButton("Realizar morphing");
    btnSave = new QPushButton("Guardar imagen resultante");
    lblColor = new QLabel();
    lblColor->setAutoFillBackground(true);        
    connect(btnColor,   SIGNAL(clicked()), this, SLOT(on_btnColor_clicked()));
    connect(btnProcess, SIGNAL(clicked()), this, SLOT(on_btnProcess_clicked()));
    connect(btnSave,    SIGNAL(clicked()), this, SLOT(on_btnSave_clicked()));
    opcForm2->addRow(lblColor, btnColor);
    opcForm2->addRow(btnProcess);
    opcForm2->addRow(btnSave);

    // Layout para Parametros de morphing
    QGroupBox *opcGrp3 = new QGroupBox(tr("Parámetros"));
    opcGrp3->setStyleSheet("QLabel { padding: 1px; }");
    opcGrp3->setMaximumWidth(200);
    opcGrp3->setMaximumHeight(150);
    QFormLayout *opcForm3 = new QFormLayout();
    opcGrp3->setLayout(opcForm3);
    txtvalA = new QLineEdit();
    txtvalA->setAlignment(Qt::AlignRight);
    txtvalA->setText("0.001");
    connect(txtvalA, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    txtvalB = new QLineEdit();
    txtvalB->setAlignment(Qt::AlignRight);
    txtvalB->setText("2");
    connect(txtvalB, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    txtvalP = new QLineEdit();
    txtvalP->setAlignment(Qt::AlignRight);
    txtvalP->setText("0");
    connect(txtvalP, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    opcForm3->addRow(tr("A:"), txtvalA);
    opcForm3->addRow(tr("B:"), txtvalB);
    opcForm3->addRow(tr("P:"), txtvalP);

    // Agregar layouts de controles a cabecera
    headLayout->addWidget(opcGrp1);
    headLayout->addWidget(opcGrp3);
    headLayout->addWidget(opcGrp2);
    mainLayout->addLayout(headLayout);


    // Inicializar controles para imagenes
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
        btnClear[i] = new QPushButton("Limpiar lienzo");
        btnOpen[i] = new QPushButton(tr("Cargar imagen"));

        connect(btnUndo[i], SIGNAL(clicked()), this, SLOT(on_btnUndo_clicked()));
        connect(btnClear[i],SIGNAL(clicked()),this,SLOT(on_btnClear_clicked()));
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));

        imageContainer[i] = new QVBoxLayout();

        imageContainer[i]->addWidget(btnUndo[i]);
        imageContainer[i]->addWidget(btnClear[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        imgLayout->addWidget(frame[i]);

        view[i]->setScene(scen[i]);
        view[i+2]->setScene(scen[i+2]);
    }

    // Para las imagenes a la derecha
    imageContainer[2] = new QVBoxLayout();
    imageContainer[2]->addWidget(view[2]);
    imageContainer[2]->addWidget(view[3]);

    slider = new QSlider(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setValue(50);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_change(int)));

    imageContainer[2]->addWidget(slider);

    imageContainer[2]->addWidget(view[4]);
    frame[2] = new QFrame();
    frame[2]->setLayout(imageContainer[2]);
    frame[2]->setMaximumWidth(300);

    // Layout de ventana
    mainLayout->addLayout(imgLayout);
    superLayout->addLayout(mainLayout);
    superLayout->addWidget(frame[2]);
    this->setLayout(superLayout);

    // Configuracion del dialogo para cargar imágenes
    diaImage = new QFileDialog(this);
    diaImage->setFileMode(QFileDialog::ExistingFile);
    diaImage->setNameFilter(tr("Images (*.png *.jpg *bmp)"));
    diaImage->setViewMode(QFileDialog::Detail);

    diaColor = new QColorDialog(this);
    //diaColor->setCurrentColor(colorPen->rgb());
    this->ChangeColorPen(QColor(Qt::red));
}

MainWindow::~MainWindow() {

}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(e);
    }
}    

void MainWindow::on_txtValEnter() {
    this->ArreglameLaVida();
}

void MainWindow::on_radioLinea_toogled(bool a) {
    GraphicsView::straightLine = (sender() == radio[!a] && a);
}

void MainWindow::on_btnOpen_clicked() {
    bool pos = (sender() == btnOpen[RIGTH]);
    LoadImage(pos);
}

// Limpia el widget contenedor de la imagen
void MainWindow::on_btnClear_clicked() {
    bool pos = (sender() == btnClear[RIGTH]);
    CleanCanvas(pos);
}

// Deshace último trazo realizado
void MainWindow::on_btnUndo_clicked() {
    bool pos = (sender() == btnUndo[RIGTH]);
    UndoLineCanvas(pos);
}

// Cambia el color del pincel con el que se dibuja
void MainWindow::on_btnColor_clicked() {
    if(!diaColor->exec()) return;
    this->ChangeColorPen(diaColor->selectedColor());
}

void MainWindow::ChangeColorPen(QColor colorPen) {
    diaColor->setCurrentColor(colorPen);
    lblColor->setText(colorPen.name());
    lblColor->setPalette(QPalette(colorPen.rgb()));
    GraphicsView::colorDrawing(colorPen);
}

//double dotProd(QPoint a, QPoint b) {
//    return (a.x()*b.x() + a.y()*b.y());
//}

//double norma(QPoint a) {
//    return sqrt(pow(a.x(), 2.0) + pow(a.y(), 2.0));
//}

//double norma2(QPoint a) {
//    return (pow(a.x(), 2.0) + pow(a.y(), 2.0));
//}


// Realizar el morphing entre las imagenes, de acuerdo a los trazos
void MainWindow::on_btnProcess_clicked() {
    this->ArreglameLaVida();
}

void MainWindow::ArreglameLaVida() {
    // Obtener parametros para el morph
    VARA = this->txtvalA->text().toDouble();
    VARB = this->txtvalB->text().toDouble();
    VARP = this->txtvalP->text().toDouble();

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

                //Si x esta mas disperso que y
                if(fabs(x1-x2) > fabs(y1-y2)) {
                    // Cambiar si coordenadas estan al reves
                    if(x1 > x2) { swap(x1, x2); swap(y1, y2); }

                    // Ir trazando linea de acuerdo a la pendiente obtenida
                    while(x1 < x2) {
                        y1 = m*(x1 - x2) + y2;
                        imgs[4]->setPixel(x1, y1, h?r:b);
                        x1 += 0.01;
                    }
                } else {
                    // Cambiar si coordenadas estan al reves
                    if(y1 > y2) { swap(x1, x2); swap(y1, y2); }

                    // Ir trazando linea de acuerdo a la pendiente obtenida
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
                // Cambiar si coordenadas estan al reves
                if(x3 > x4) { swap(x3, x4); swap(y3, y4); }

                // Ir trazando linea de acuerdo a la pendiente obtenida
                while(x3 < x4) {
                    y3 = m*(x3 - x4) + y4;
                    imgs[4]->setPixel(x3, y3, qRgb(0, 0, 0));
                    x3 += 0.01;
                }
            } else {
                // Cambiar si coordenadas estan al reves
                if(y3 > y4) { swap(x3, x4); swap(y3, y4); }

                // Ir trazando linea de acuerdo a la pendiente obtenida
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

                        QVector2D XP(X - P);
                        QVector2D QP(Q - P);

                        QVector2D pQP(QP.y(), -QP.x());

                        // Calcular u, v
                        u = QVector2D::dotProduct(XP, QP) /  QP.lengthSquared();
                        v = QVector2D::dotProduct(XP, pQP) / QP.length();

                        // Obtener puntos interpolados de linea de referencia
                        QPoint P2 = view[h]->listAux->at(k).first;
                        QPoint Q2 = view[h]->listAux->at(k).second;

                        QVector2D Q2P2(Q2 - P2);
                        QVector2D pQ2P2(Q2P2.y(), -Q2P2.x());

                        QVector2D X2 = QVector2D(P2) + u * Q2P2 + (v * pQ2P2) / Q2P2.length();

                        QPoint p = X2.toPoint() - X;

                        double dist = 0;
                        if(u > 0 && u < 1) dist = fabs(v);
                        else if(u <= 0) dist = sqrt(pow(X.x() - P.x(), 2.0) + pow(X.y() - P.y(), 2.0));
                        else dist = sqrt(pow(X.x() - Q.x(), 2.0) + pow(X.y() - Q.y(), 2.0));

                        double w = 0;
                        w =  pow(QP.length(), VARP);
                        w /= (VARA + dist);
                        w = pow(w, VARB);

                        ww[k] = w;
                        pp[k] = p;
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
                    x0 = ceil(X2.x());
                    if(x0 < 0) x0 = 0;
                    if(x0 >= wimg) x0 = wimg-1;

                    y0 = ceil(X2.y());
                    if(y0 < 0) y0 = 0;
                    if(y0 >= himg) y0 = himg-1;

                    X2 = QPoint(x0, y0);

                    if(X2 == X) n++;
                    imgs[h+2]->setPixel(X, imgs[h]->pixel(X2));
                }
            }
        }

        this->CrossDisolve(this->slider->value());
    } else {
//        for(int k=0; k<2; ++k) {
//            for(int i = 0, j = view[k]->listPoint->size(); i<j; ++i) {

//                int x = view[k]->listPoint->at(i).x();
//                int y = view[k]->listPoint->at(i).y();

//                if(x >= 0 && x < wimg && y >= 0 && y < himg)
//                    imgs[2]->setPixel(x, y, k?r:b);
//            }
//        }
    }

    for(int h=2; h<5; ++h) {
        scen[h]->clear();
        scen[h]->setSceneRect(0, 0, wimg, himg);
        scen[h]->addPixmap(QPixmap::fromImage(*imgs[h]));

        view[h]->fitInView(*view[h]->scene()->items().begin(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_slider_change(int a) {
    CrossDisolve(a);
}

void MainWindow::CrossDisolve(int a) {
    if(imgs[2] == NULL || imgs[3] == NULL || imgs[4] == NULL) return;

    float f = a / 100.0;
    float g = 1 - f;
    QRgb c, d;
    QPoint X;

    for(int i=0; i<wimg; ++i) {
        for(int j=0; j<himg; ++j) {
            X = QPoint(i, j);

            c = imgs[2]->pixel(X);
            d = imgs[3]->pixel(X);

            QRgb e = qRgb(f*qRed(c) + g*qRed(d),
                          f*qGreen(c) + g*qGreen(d),
                          f*qBlue(c) + g*qBlue(d)) ;
            imgs[4]->setPixel(X, qRgba(qRed(e), qGreen(e), qBlue(e), 255));
        }
    }

    scen[4]->addPixmap(QPixmap::fromImage(*imgs[4]));
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
