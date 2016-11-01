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
#include <cmath>

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

        // refactor after in readData method
        grid->getGrid()[2][4].setOrigin(true);
        grid->getGrid()[0][0].setOrigin(true);
        grid->getGrid()[3][4].setOrigin(true);
        grid->getGrid()[2][2].setOrigin(true);
        grid->getGrid()[1][0].setOrigin(true);
        grid->getGrid()[2][3].setOrigin(true);
        grid->getGrid()[4][4].setOrigin(true);
        grid->getGrid()[3][1].setOrigin(true);

        // set pos x,y (to change when reading data or in constructor)

        for (int i = 0; i < grid->getWidth(); ++i)
        {
            for (int j = 0; j < grid->getHeight(); ++j)
            {
                grid->getGrid()[j][i].x = j;
                grid->getGrid()[j][i].y = i;
            }
        }
        activeX = NOTACTIVE;
        activeY = NOTACTIVE;
    }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    setEnabled(true);
    show();
}

void MainWindow::leave()
{
    close();
}


void MainWindow::paintEvent(QPaintEvent* e)
{

    painter = new QPainter(this);
    drawGrid();
    drawColorLine();
    fillSquareColor();
    mouseRoundColor();

}

void MainWindow::drawGrid()
{
    //painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    interval = height / grid->getWidth();

    // sets position of x,y of each cases
    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            pos[i][j].setX(leftside + j * interval + interval / 2);
            pos[i][j].setY(up + i * interval + interval / 2);
        }
    }

    // draw grid with 4 points (top left, top right, bottom right, bottom left)
    QPoint pointList[4] = {QPoint(leftside,up), QPoint(720 - rightside,up),
        QPoint(720 - rightside,up + 720), QPoint(leftside,up + 720)};
    painter->drawPolygon(pointList,4);

    // draw lines top left
    for(int i = 1; i < grid->getWidth(); i++)
    {
        painter->drawLine(QPoint(leftside,up+i*interval),
        QPoint(720 - rightside,up+i*interval));
    }

    // draw lines
    for(int i = 1; i < grid->getHeight(); i++)
    {
        painter->drawLine(QPoint(leftside+i*interval,up),
        QPoint(leftside+i*interval,up+720));
    }

    // draw origins from data input from data
    for(int i = 0; i < grid->getWidth(); i++)
    {
        for(int j = 0; j < grid->getHeight(); j++)
        {
            if (grid->getGrid()[i][j].getColor() != -1 && grid->getGrid()[i][j].isOrigin()){

                painter->setPen(Qt::white);
                //painter->setBrush(Qt::green);
                //if(data[i][j] == 0) continue;
                //painter->setBrush(transColor[data[i][j]]);
                painter->setPen(color[grid->getGrid()[i][j].getColor()]);
                painter->setBrush(color[grid->getGrid()[i][j].getColor()]);
                painter->drawEllipse(pos[j][i],interval/3,interval/3);
                //update();
            }
        }
    }
}

void MainWindow::drawColorLine()
{
    // paint line when connecting
    // to change interval/2 + offset -> center of the square
    for (int i = 0; i < grid->getWidth(); ++i)
    {
        for (int j = 0; j < grid->getHeight(); ++j)
        {

            //qDebug() << grid->getGrid()[i][j].isCovered();
            if(grid->getGrid()[i][j].previous[0] != 0) // && grid->getGrid()[i][j].isCovered() == false
            {
                painter->setPen(QPen(QBrush(color[grid->getGrid()[i][j].getColor()]),
                    15, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
                    painter->setBrush(color[grid->getGrid()[i][j].getColor()]);

                painter->drawLine(QPoint(interval/2 + offset + interval * grid->getGrid()[i][j].previous[0]->x,
                    interval/2 + offset + interval * grid->getGrid()[i][j].previous[0]->y),
                    QPoint(interval/2 + offset  + interval * i, interval/2 + offset + interval * j));
            }

        }
    }
}

void MainWindow::fillSquareColor()
{
    // fill the square when there are paths created

    for (int i = 0; i < grid->getWidth(); ++i)
    {
        for (int j = 0; j < grid->getHeight(); ++j)
        {
            if(grid->getGrid()[i][j].getColor() != -1 && grid->getGrid()[i][j].hasFlag())
            {
                painter->setPen(activeColor[grid->getGrid()[i][j].getColor()]);
                painter->setBrush(activeColor[grid->getGrid()[i][j].getColor()]);
                painter->drawRect(offset + interval * i, offset + interval * j, interval, interval);
            }
        }
    }
}

void MainWindow::mouseRoundColor()
{
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


void MainWindow::mouseMoveEvent(QMouseEvent* e)
{

    if (e->x() > offset && e->y() > offset && e->x() < width - offset  && e->y() < height - offset)
    {
        int x = ((e->x() - offset) / interval);
        int y = ((e->y() - offset) / interval);
        mousePosition.setX(e->x());
        mousePosition.setY(e->y());

        caseX = (e->x() - offset) / (height / grid->getHeight());
        caseY = (e->y() - offset) / (width / grid->getWidth());
        qDebug() << ("X:"+QString::number(e->x())+"-- Y:"+QString::number(e->y()) +" posCaseX: " + caseX + "posCaseY: " +caseY);



        if (abs(activeX - x) + abs(activeY - y) == 1){
        /*if (grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].getColor() == -1
                && grid->getGrid()[x][y].previous != 0 && grid->getGrid()[x][y].previous[0]->isOrigin()
                && grid->getGrid()[x][y].previous[0]->noMore == true)
        {
            qDebug() << "noMore == true";
        }
        else*/
        if (grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].getColor() == -1)
        {
            qDebug() << "entered blank case";
            grid->getGrid()[x][y].previous[0] = &grid->getGrid()[activeX][activeY];
            grid->getGrid()[x][y].setColor(grid->getGrid()[activeX][activeY].getColor());
            grid->getGrid()[x][y].setFlag(true);
            grid->getGrid()[x][y].setCovered(true);
            activeX = x;
            activeY = y;
        }
        else if(grid->getGrid()[x][y].isOrigin() && grid->getGrid()[activeX][activeY].getColor() == grid->getGrid()[x][y].getColor()
                && grid->getGrid()[x][y].isCovered() == false)
        {
            qDebug() << "entered origin case";
            grid->getGrid()[x][y].previous[0] = &grid->getGrid()[activeX][activeY];
            grid->getGrid()[x][y].setFlag(true);
            grid->getGrid()[x][y].setCovered(true);
            //grid->getGrid()[x][y].noMore = true;
            activeX = x;
            activeY = y;
        }
        /*else if(grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].getColor() != grid->getGrid()[activeX][activeY].getColor())
        {

        }*/
        }
        if (grid->getGrid()[x][y].hasFlag() == false && grid->getGrid()[x][y].isOrigin())
        {
            grid->getGrid()[x][y].setFlag(true);
            grid->getGrid()[x][y].setCovered(true);

        }
    }

    update();
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->x() > offset && e->y() > offset && e->x() < width - offset  && e->y() < height - offset)
    {
        if (e->button() == Qt::LeftButton){

            mousePressed = true;
            if(mousePressed)
            {
                int x = ((e->x() - offset) / interval);
                int y = ((e->y() - offset) / interval);
                activeX = x;
                activeY = y;
                mousePosition.setX(e->x());
                mousePosition.setY(e->y());

                if (grid->getGrid()[x][y].isOrigin())
                {
                    //grid->getGrid()[x][y].setFlag(true);

                    for (int i = 0; i < grid->getWidth(); ++i)
                    {
                        for (int j = 0; j < grid->getHeight(); ++j)
                        {
                            if(grid->getGrid()[i][j].getColor() == grid->getGrid()[x][y].getColor() && grid->getGrid()[i][j].isOrigin() == false)
                            {
                                grid->getGrid()[i][j].clear();
                            }
                            if(grid->getGrid()[i][j].isOrigin() && grid->getGrid()[i][j].getColor() == grid->getGrid()[x][y].getColor())
                            {
                                grid->getGrid()[i][j].next[0] = 0;
                                grid->getGrid()[i][j].previous[0] = 0;
                                grid->getGrid()[i][j].setFlag(false);
                                grid->getGrid()[i][j].setCovered(false);
                                //grid->getGrid()[i][j].noMore = false;
                            }
                        }
                    }
                    grid->getGrid()[x][y].setFlag(false);
                }
                qDebug() << "Mouse pressed";
                qDebug() << "Left Button";
            }
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    qDebug() << "Mouse released at: " +QString::number(e->x()) + "," + QString::number(e->y());
    mousePressed = false;
    activeX = NOTACTIVE;
    activeY = NOTACTIVE;
    update();
}
