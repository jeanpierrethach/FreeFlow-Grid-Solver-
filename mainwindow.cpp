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

QColor color[9] = {QColor(237, 28, 36), QColor(0, 162, 232), QColor(102, 24, 126),
    QColor(244, 233, 11), QColor(255, 127, 39), QColor(144, 233, 50),
    QColor(0, 0, 0), QColor(185, 122, 87), QColor(254, 109, 221)};

QColor currentColor[9] = {QColor(237, 28, 36, 100), QColor(0, 162, 232, 100),
    QColor(102, 24, 126, 100), QColor(244, 233, 11, 100),
    QColor(255, 127, 39, 100), QColor(144, 233, 50, 100),
    QColor(0, 0, 0, 100), QColor(185, 122, 87, 100),
    QColor(254, 109, 221, 100)};

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
        ui->setupUi(this);

        this->setFixedWidth(width);
        this->setFixedHeight(height);

        setLevel();

        // set pos x,y (to change when reading data or in constructor)
        // or use method to set position
        for (int i = 0; i < grid->getNbRow(); ++i)
        {
            for (int j = 0; j < grid->getNbColumn(); ++j)
            {
                grid->getGrid()[j][i].x = j;
                grid->getGrid()[j][i].y = i;
            }
        }
        currentX = NOTACTIVE;
        currentY = NOTACTIVE;
    }

MainWindow::~MainWindow()
{
    delete ui;
    delete grid;
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

void MainWindow::setLevel()
{
    // proof of concept
    // refactor after in readData method
    grid->getGrid()[2][4].setData(0);
    grid->getGrid()[0][0].setData(0);
    grid->getGrid()[3][4].setData(5);
    grid->getGrid()[2][2].setData(5);
    grid->getGrid()[1][0].setData(1);
    grid->getGrid()[2][3].setData(1);
    grid->getGrid()[4][4].setData(4);
    grid->getGrid()[3][1].setData(4);

}

void MainWindow::paintEvent(QPaintEvent* )
{
    painter = new QPainter(this);
    drawGrid();
    drawColorLine();
    fillSquareColor();
    mouseRoundColor();
    delete painter;
}

void MainWindow::drawGrid()
{
    painter->setPen(Qt::black);
    interval = height / grid->getNbRow();

    setPositionCase();

    drawRectangleGrid();

    drawRowColumnLines();

    drawOrigins();
}

void MainWindow::setPositionCase()
{
    // sets position of x,y of each cases
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            pos[i][j].setX(leftside + j * interval + interval / 2);
            pos[i][j].setY(up + i * interval + interval / 2);
        }
    }
}

void MainWindow::drawRectangleGrid()
{
    // draw grid with 4 points (top left, top right, bottom right, bottom left)
    QPoint gridPoints[4] = {QPoint(leftside, up), QPoint(width - rightside, up),
        QPoint(width - rightside, up + height), QPoint(leftside, up + height)};
    painter->drawPolygon(gridPoints, 4);
}

void MainWindow::drawRowColumnLines()
{
    // draw row lines
    for (int i = 1; i < grid->getNbRow(); ++i)
    {
        painter->drawLine(QPoint(leftside, up + i * interval),
        QPoint(720 - rightside, up + i * interval));
    }

    // draw column lines
    for (int i = 1; i < grid->getNbColumn(); ++i)
    {
        painter->drawLine(QPoint(leftside + i * interval, up),
        QPoint(leftside + i * interval, up + 720));
    }
}

void MainWindow::drawOrigins()
{
    // draw origins from data
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].getColor() != -1 && grid->getGrid()[i][j].isOrigin()){
                painter->setPen(Qt::white);
                painter->setPen(color[grid->getGrid()[i][j].getColor()]);
                painter->setBrush(color[grid->getGrid()[i][j].getColor()]);
                painter->drawEllipse(pos[j][i],interval/3,interval/3);
            }
        }
    }
}

void MainWindow::drawColorLine()
{
    // paint line when connecting
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].previous[0] != 0)
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
    // fill the square with a color
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].getColor() != -1 && grid->getGrid()[i][j].hasFlag())
            {
                painter->setPen(currentColor[grid->getGrid()[i][j].getColor()]);
                painter->setBrush(currentColor[grid->getGrid()[i][j].getColor()]);
                painter->drawRect(offset + interval * i, offset + interval * j, interval, interval);
            }
        }
    }
}

void MainWindow::mouseRoundColor()
{
    // create visual effect on mouse pressed
    if (currentX != NOTACTIVE)
    {
        if (grid->getGrid()[currentX][currentY].getColor() != -1)
        {
            painter->setPen(currentColor[grid->getGrid()[currentX][currentY].getColor()]);
            painter->setBrush(currentColor[grid->getGrid()[currentX][currentY].getColor()]);
            painter->drawEllipse(mousePosition, 40, 40);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    // restrict mouse movement to only the grid
    if (e->x() > offset && e->y() > offset && e->x() < width - offset
            && e->y() < height - offset)
    {
        if (e->button() == Qt::LeftButton)
        {
            int x = ((e->x() - offset) / interval);
            int y = ((e->y() - offset) / interval);
            currentX = x;
            currentY = y;
            pathConnected = false;
            mousePosition.setX(e->x());
            mousePosition.setY(e->y());

            // allow user to clear a path from a specific case
            if (grid->getGrid()[x][y].getColor() != -1 && grid->getGrid()[x][y].next[0] != 0
                    && grid->getGrid()[x][y].next[0]->getColor() == grid->getGrid()[x][y].getColor())
            {
                Path* temp = grid->getGrid()[x][y].next[0];
                clearPathColorCase(temp);
            }

            // clear path from origin
            if (grid->getGrid()[x][y].isOrigin())
            {
                for (int i = 0; i < grid->getNbRow(); ++i)
                {
                    for (int j = 0; j < grid->getNbColumn(); ++j)
                    {
                        if (grid->getGrid()[i][j].getColor() == grid->getGrid()[x][y].getColor()
                                && grid->getGrid()[i][j].isOrigin() == false)
                        {
                            grid->getGrid()[i][j].clear();
                        }
                        if (grid->getGrid()[i][j].isOrigin()
                                && grid->getGrid()[i][j].getColor() == grid->getGrid()[x][y].getColor())
                        {
                            grid->getGrid()[i][j].clearOrigin();

                        }
                    }
                }
                grid->getGrid()[x][y].setFlag(true);
                grid->getGrid()[x][y].setFirstOrigin(true);
            }
            qDebug() << "Mouse pressed: Left button";
        }
    }

    update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    // restrict mouse movement to only the grid
    if (e->x() > offset && e->y() > offset && e->x() < width - offset
            && e->y() < height - offset)
    {
        int x = ((e->x() - offset) / interval);
        int y = ((e->y() - offset) / interval);
        mousePosition.setX(e->x());
        mousePosition.setY(e->y());

        // tracks and prints the mouse position
        positionCaseX = (e->x() - offset) / (height / grid->getNbColumn());
        positionCaseY = (e->y() - offset) / (width / grid->getNbRow());
        qDebug() << ("X:" + QString::number(e->x()) + ", Y:" + QString::number(e->y())
                     + " posCaseX: " + positionCaseX + "posCaseY: " + positionCaseY);

        // limits the pathing creation to adjacents cases only
        if (abs(currentX - x) + abs(currentY - y) == 1)
        {
            // if the case is blank
            if (grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].getColor() == -1
                    && !pathConnected)
            {
                qDebug() << "Entered blank case";
                grid->getGrid()[currentX][currentY].next[0] = &grid->getGrid()[x][y];
                grid->getGrid()[x][y].previous[0] = &grid->getGrid()[currentX][currentY];
                grid->getGrid()[x][y].setColor(grid->getGrid()[currentX][currentY].getColor());
                grid->getGrid()[x][y].setCoveredFlag(true);
                currentX = x;
                currentY = y;
                pathConnected = false;
            }
            // when the second origin is connected to the first origin
            else if (grid->getGrid()[x][y].isOrigin()
                    && grid->getGrid()[currentX][currentY].getColor() == grid->getGrid()[x][y].getColor()
                    && grid->getGrid()[x][y].isCovered() == false && !pathConnected)
            {
                if (grid->getGrid()[x][y].getFirstOrigin() == false)
                {
                    qDebug() << "Entered second origin case";
                    grid->getGrid()[x][y].setSecondOrigin(true);
                    grid->getGrid()[currentX][currentY].next[0] = &grid->getGrid()[x][y];
                    grid->getGrid()[x][y].previous[0] = &grid->getGrid()[currentX][currentY];
                    grid->getGrid()[x][y].setCoveredFlag(true);
                    grid->getGrid()[x][y].setPathComplete(true);
                    currentX = x;
                    currentY = y;
                    pathConnected = true;
                }
            }
            // if the user wants to create a path on another colored path
            else if (grid->getGrid()[x][y].getColor() != grid->getGrid()[currentX][currentY].getColor()
                    && grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].next[0] != 0 && !pathConnected)
            {
                Path* temp = grid->getGrid()[x][y].next[0];
                Path* temp2 = grid->getGrid()[x][y].previous[0];
                temp2->next[0] = 0;
                grid->getGrid()[x][y].clear();

                clearPathColorCase(temp);
                pathConnected = false;
            }
            // clear only the last case from another colored path
            else if (grid->getGrid()[x][y].getColor() != grid->getGrid()[currentX][currentY].getColor()
                    && grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].next[0] == 0 && !pathConnected)
            {
                Path* temp = grid->getGrid()[x][y].previous[0];
                temp->next[0] = 0;
                grid->getGrid()[x][y].clear();
                pathConnected = false;

            }
            // allow user to go back when creating a path
            else if (grid->getGrid()[x][y].getColor() == grid->getGrid()[currentX][currentY].getColor()
                    && grid->getGrid()[x][y].hasFlag() && grid->getGrid()[x][y].isCovered()
                    && grid->getGrid()[currentX][currentY].isOrigin() == false && !pathConnected)
            {
                if (grid->getGrid()[x][y].next[0] == &grid->getGrid()[currentX][currentY])
                {
                    grid->getGrid()[currentX][currentY].clear();
                    currentX = x;
                    currentY = y;
                    pathConnected = false;
                }
            }
            if (grid->getGrid()[x][y].hasFlag() == false && grid->getGrid()[x][y].isOrigin())
            {
                grid->getGrid()[x][y].setCovered(true);
            }
        }
    }

    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{
    qDebug() << "Mouse released at: " + QString::number(e->x()) + ","
                + QString::number(e->y());
    currentX = NOTACTIVE;
    currentY = NOTACTIVE;

    update();
}

void MainWindow::clearPathColorCase(Path* temp)
{
    int color = temp->getColor();

    while(temp)
    {
        if(temp->isOrigin() == false && temp->getColor() == color)
        {
            temp->previous[0] = 0;
            int tempX = temp->x;
            int tempY = temp->y;
            temp->setColor(-1);
            temp->setCoveredFlag(false);
            temp = temp->next[0];
            grid->getGrid()[tempX][tempY].next[0] = 0;
        }
        else if(temp->getColor() == color)
        {
            temp->previous[0] = 0;
            temp->setCoveredFlag(false);
            break;
        }
        else
        {
            break;
        }
    }
}
