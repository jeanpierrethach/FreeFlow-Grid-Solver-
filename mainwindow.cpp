#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grid.h"

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <QPainter>
#include <QColor>
#include <QPoint>


enum {blank, red, blue, purple, yellow, orange, green, black, brown};

QColor color[9] = {QColor(237, 28, 36), QColor(0, 162, 232), QColor(102, 24, 126),
                   QColor(244, 233, 11), QColor(255, 127, 39), QColor(144, 233, 50),
                   QColor(0, 0, 0), QColor(185, 122, 87), QColor(254, 109, 221)};

QColor activeColor[9] = {QColor(237, 28, 36, 100), QColor(0, 162, 232, 100),
                         QColor(102, 24, 126, 100), QColor(244, 233, 11, 100),
                         QColor(255, 127, 39, 100), QColor(144, 233, 50, 100),
                         QColor(0, 0, 0, 100), QColor(185, 122, 87, 100),
                         QColor(254, 109, 221, 100)};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //installEventFilter(this);
    this->setFixedWidth(width);
    this->setFixedHeight(height);
    //setMouseTracking(true);

    // proof of concept
    grid->getGrid()[2][4].setColor(0);
    grid->getGrid()[0][0].setColor(0);
    grid->getGrid()[3][4].setColor(5);
    grid->getGrid()[2][2].setColor(5);
    grid->getGrid()[1][0].setColor(1);
    grid->getGrid()[2][3].setColor(1);
    grid->getGrid()[4][4].setColor(4);
    grid->getGrid()[3][1].setColor(4);


    activeX = NOTACTIVE;
    activeY = NOTACTIVE;

}




MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent* e)
{

    painter = new QPainter(this);
    drawGrid();
    //drawColorLine();

    // create visual effect on mouse pressed
    if(activeX != NOTACTIVE)
    {
        if(grid->getGrid()[activeX][activeY].getColor() != -1)
        {
            painter->setPen(activeColor[grid->getGrid()[activeX][activeY].getColor()]);
            painter->setBrush(activeColor[grid->getGrid()[activeX][activeY].getColor()]);
            painter->drawEllipse(mousePosition, 40, 40);
        }
    }

}

void MainWindow::drawGrid()
{

    //painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    interval = height / grid->getWidth();

    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            pos[i][j].setX(leftside + j * interval + interval / 2);
            pos[i][j].setY(up + i * interval + interval / 2);
        }
    }

    // draw rectangle with 4 points
    QPoint pointList[4] = {QPoint(leftside,up),
                             QPoint(720 - rightside,up),
                            QPoint(720 - rightside,up + 720),
                            QPoint(leftside,up + 720)};
    painter->drawPolygon(pointList,4);

    // draw lines
    for(int i = 1; i < grid->getWidth(); i++)
    {
        painter->drawLine(QPoint(leftside,up+i*interval),
                          QPoint(leftside+720,up+i*interval));
    }
    // can put this in the for above if same size
    for(int i = 1; i < grid->getHeight(); i++)
    {
        painter->drawLine(QPoint(leftside+i*interval,up),
                          QPoint(leftside+i*interval,up+720));
    }


    // draw grid from data input
    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            painter->setPen(Qt::white);
            //painter->setBrush(Qt::green);
            //if(data[i][j] == 0) continue;
            //painter->setBrush(transColor[data[i][j]]);
            painter->setPen(color[grid->getGrid()[i][j].getColor()]);
            painter->setBrush(color[grid->getGrid()[i][j].getColor()]);
            painter->drawEllipse(pos[j][i],interval/3,interval/3);
            update();
        }
    }

    // update();
}

void MainWindow::drawColorLine()
{


       //qDebug() << "mouseXinit: " + mouseXinit << ", mouseYinit: " + mouseYinit;

       painter->setPen(QPen(Qt::SolidPattern,20,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

       // if (mousePressed != false)
       painter->drawLine(startPos, endPos);

       /*for (int i = 0; i < grid->getWidth(); ++i)
       {

           for (int j = 0; j < grid->getHeight(); ++j)
           {
               painter->drawLine(startPos,endPos);
           }
       }*/


}


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    //if (e->type() == QEvent::MouseMove){

    int x = ((e->x() - 5) / interval);
    int y = ((e->y() - 5) / interval);
    mousePosition.setX(e->x());
    mousePosition.setY(e->y());


        caseX = (e->x() - 5) / (height / grid->getHeight());
        caseY = (e->y() - 5) / (width / grid->getWidth());
        qDebug() << ("X:"+QString::number(e->x())+"-- Y:"+QString::number(e->y()) +" posCaseX: " + caseX + "posCaseY: " +caseY);

        //endPos = e->pos();

        //mouseXfinal = (e->x() - 5) / (height / grid->getHeight());
        //mouseYfinal = (e->y() - 5) / (width / grid->getWidth());
        /*mouseXfinal = (caseX + (interval/3 * e->x()));
        mouseYfinal = (caseY + (interval/3 * e->y()));
        endPos.setX(mouseXfinal);
        endPos.setY(mouseYfinal);*/

        //mouseXfinal = e->pos().x();
        //mouseYfinal = e->pos().y();

        update();
    //}

}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton){
        mousePressed = true;
        if(mousePressed)
        {
            //startPos = e->pos();

            //mouseXinit = (e->x() - 5) / (height / grid->getHeight());
            //mouseYinit = (e->y() - 5) / (width / grid->getWidth());

            int x = ((e->x() - 5) / interval);
            int y = ((e->y() - 5) / interval);
            activeX = x;
            activeY = y;
            mousePosition.setX(e->x());
            mousePosition.setY(e->y());



            /*mouseXinit = ((e->x() - 5) / (height / grid->getHeight()) + (interval/3 * e->x()));
            mouseYinit = ((e->y() - 5) / (width / grid->getWidth()) + (interval/3 * e->y()));
            startPos.setX(mouseXinit);
            startPos.setY(mouseYinit);*/

            //mouseXinit = e->pos().x();
            //mouseYinit = e->pos().y();
            qDebug() << "Mouse pressed";
            qDebug() << "Left Button";
            //update();
        }




    }





}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    qDebug() << "Mouse released at: " +QString::number(e->x()) + "," + QString::number(e->y());
    mousePressed = false;
    activeX = NOTACTIVE;
    activeY = NOTACTIVE;
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
