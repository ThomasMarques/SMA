#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _planet = NULL;
    /// Initialisation du générateur aléatoire MersenneTwister
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);

    _zoom = 1;

    ui->setupUi(this);
    ui->button_pause->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->widgetFond->setGeometry(0,0,this->width(),this->height());
    ui->button_init->setGeometry(QRect(80, this->height()-30, 140, 23));
    ui->button_lancer->setGeometry(QRect(this->width()-250, this->height()-30, 140, 23));
    ui->button_pause->setGeometry(QRect(this->width()/2-40, this->height()-30, 140, 23));
    ui->labelgauche->setGeometry(0,0,405,402);
    ui->labeldroit->setGeometry(this->width()-378,0,378,384);
    if(ui->widget)
    {
        unsigned w,h;
        ui->widget->setGeometry(QRect(30, 10, this->width()-60, this->height()-40));
        w = LARGEUR * ((this->width()-60)/LARGEUR);
        h = HAUTEUR * ((this->height()-55)/HAUTEUR);
        ui->widgetTranspa->setGeometry(QRect(30, 25, w, h));
    }
    if(this->width() > 1350)
        ui->widgetFond->setStyleSheet("background: black url('fond.png') no-repeat top center;");
    else ui->widgetFond->setStyleSheet("background : black;");
}

void MainWindow::on_button_init_clicked()
{
    if(_planet)
    {
        _planet->stop();
        _planet->wait();
        delete ui->widget;
        delete _planet;
        _planet = NULL;
        ui->widget = NULL;
        ui->widgetTranspa->setStyleSheet("background-color: rgba( 0, 0, 0, 0% );");
    }
    ui->button_lancer->setDisabled(false);
    ui->button_pause->setDisabled(true);
    Resource::InitNbResource();
}

void MainWindow::on_button_lancer_clicked()
{
    unsigned w,h;
    ui->button_pause->setDisabled(false);
    _planet = new Planet();
    ui->widget = new WidgetView(_planet->getMap(),_planet->getT(),_planet->getMapClan(),_planet->getFightingMap(),_planet->getClans(),ui->centralWidget);
    ui->widget->setObjectName(QString::fromUtf8("widget"));
    ui->widget->setGeometry(QRect(30, 10, (this->width()-60)*_zoom, (this->height()-40)*_zoom));
    w = LARGEUR * ((this->width()-60)/LARGEUR);
    h = HAUTEUR * ((this->height()-55)/HAUTEUR);
    ui->widgetTranspa->setGeometry(QRect(30, 25, w, h));
    ui->widgetTranspa->setStyleSheet("background-color: rgba( 0, 0, 0, 80% );");
    ui->button_lancer->setDisabled(true);
    ui->widget->show();
    QObject::connect(_planet,SIGNAL(modelChanged()),ui->widget,SLOT(modelChanged()));
    _planet->start();
}

void MainWindow::on_button_pause_clicked()
{
    _planet->changedRun();
}
