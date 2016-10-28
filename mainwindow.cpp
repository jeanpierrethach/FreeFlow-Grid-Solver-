#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grid.h"
#include "grid.cpp"

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <QPainter>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //installEventFilter(this);
    this->setFixedWidth(width);
    this->setFixedHeight(height);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* e)
{

    painter = new QPainter(this);
    drawGrid();

}

void MainWindow::drawGrid()
{

    Grid* grid = new Grid(5,5);
    painter->setBrush(Qt::black);
    painter->setPen(Qt::white);
    interval = height / grid->getWidth();

    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            pos[i][j].setX(leftside + j * interval + interval / 2);
            pos[i][j].setY(up + i * interval + interval / 2);
        }
    }

    QPoint pointList[4] = {QPoint(leftside,up),
                             QPoint(720 - rightside,up),
                            QPoint(720 - rightside,up + 720),
                            QPoint(leftside,up + 720)};
    painter->drawPolygon(pointList,4);

    for(int i = 1; i < grid->getWidth(); i++)
    {
        painter->drawLine(QPoint(leftside,up+i*interval),
                          QPoint(leftside+720,up+i*interval));
    }
    for(int i = 1; i < grid->getHeight(); i++)
    {
        painter->drawLine(QPoint(leftside+i*interval,up),
                          QPoint(leftside+i*interval,up+720));
    }
/*
    for(int i = 0;i < n;i++)
    {
        for(int j = 0;j < n;j++)
        {
            painter->setPen(Qt::black);
            //if(data[i][j] == 0) continue;
            //painter->setBrush(transColor[data[i][j]]);
            painter->drawEllipse(pos[i][j],interval/3,interval/3);
            update();
        }
    }*/
    /*QPoint p1 = QPoint(5,5);
    QPoint p2 = QPoint(5,715);


    for (int i = 0; i < 6; ++i){
        painter->drawLine(p1, p2);
        p1.setX(p1.x() + i*(720-10)/6);
    }*/
    // update();
}


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    caseX = (e->x() - 5) / 144;
    caseY = (e->y() - 5) / 144;
    qDebug() << ("X:"+QString::number(e->x())+"-- Y:"+QString::number(e->y()) +" posCaseX: " + caseX + "posCaseY: " +caseY);

}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton){
        qDebug() << "Left Button";
    }
    mouseX = e->x();
    mouseY = e->y();




}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{


}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
  }
  return false;
}*/
