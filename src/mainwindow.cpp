#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grid.h"

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>
#include <QPainter>
#include <QPoint>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QRegExp>
#include <QDir>

#include <cmath>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Free Flow");

    this->setFixedWidth(width);
    this->setFixedHeight(height + bottomSpace);

    ui->back->setGeometry(30, height + height/24, 140, bottomSpace/3);
    ui->restart->setGeometry(width - 170, height + height/24, 140, bottomSpace/3);
    ui->save->setGeometry(width - 430, height + height/24, 140, bottomSpace/3);


    connect(ui->back, SIGNAL(clicked()), this, SLOT(back()));
    connect(ui->restart, SIGNAL(clicked()), this, SLOT(restart()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(saveGame()));

    color = gen.getColorList("color");
    currentColor = gen.getColorList("currentColor");

    resizeGrid();

    currentX = NOTACTIVE;
    currentY = NOTACTIVE;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete grid;
}

void MainWindow::setLevel(int level)
{

    QString fileName = ":/Other/levels/level" + QString::number(level) + ".json";

    grid = new Grid(fileName);

    setPositionStart();

    start();
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

void MainWindow::back()
{
    close();
    restart();
    emit(backToLevelMenu());
}

void MainWindow::restart()
{
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[j][i].isOrigin())
            {
                grid->getGrid()[j][i].clearOrigin();
            }
            else
            {
                grid->getGrid()[j][i].clear();
            }
        }
    }
    update();
}

void MainWindow::saveGame()
{
    bool result = false;

    QInputDialog* input = new QInputDialog();

    QString fileName = input->getText(this, "Free Flow", "Enter a name", QLineEdit::Normal, "", &result);

    storer = new GameStorer();
    storer->saveFile(grid, fileName, result);
    delete storer;
}

void MainWindow::loadGame()
{
    QString filter = "JSON Files (*.json)";
    QFileInfo selectedFile = QFileDialog::getOpenFileName(this, "Select a file to open", "/home/", filter);

    storer = new GameStorer();

    if(storer->loadFile(grid, selectedFile))
    {
        setPositionStart();
        emit closeWindow();
        start();
    }
    delete storer;
}

void MainWindow::setGeneratedLevel()
{
    int num = gen.generateNumber();

    grid = GridBuilder::buildGrid(num, num);

    qDebug() << num;

    setPositionStart();

    start();
}



void MainWindow::setPositionStart()
{
    pos.resize(grid->getNbRow());
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        pos[i].resize(grid->getNbColumn());
    }

    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            grid->setPosition(i, j);
        }
    }
}

void MainWindow::resizeGrid()
{
    // resize grid to a square
    if (width > height)
    {
        width = height;
    }
    if (height > width)
    {
        height = width;
    }
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
    intervalWidth = width / grid->getNbRow();
    intervalHeight = height / grid->getNbColumn();

    setPositionCase();

    drawRectangleGrid();

    drawRowColumnLines();

    drawOrigins();
}

void MainWindow::setPositionCase()
{
    // sets the center position (x,y) of each cases
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            pos[i][j].setX(leftside + j * intervalWidth + intervalWidth / 2);
            pos[i][j].setY(up + i * intervalHeight + intervalHeight / 2);
        }
    }
}

void MainWindow::drawRectangleGrid()
{
    // draw grid with 4 points (top left, top right, bottom right, bottom left)
    QPoint gridPoints[4] = {QPoint(leftside, up), QPoint(width + rightside, up),
                            QPoint(width + rightside, up + height), QPoint(leftside, up + height)};
    painter->drawPolygon(gridPoints, 4);
}

void MainWindow::drawRowColumnLines()
{
    // draw row lines
    for (int i = 1; i < grid->getNbRow(); ++i)
    {
        painter->drawLine(QPoint(leftside, up + i * intervalHeight),
                          QPoint(width + rightside, up + i * intervalHeight));
    }

    // draw column lines
    for (int i = 1; i < grid->getNbColumn(); ++i)
    {
        painter->drawLine(QPoint(leftside + i * intervalWidth, up),
                          QPoint(leftside + i * intervalWidth, up + height));
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
                painter->drawEllipse(pos[j][i],intervalWidth/3,intervalHeight/3);
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

                painter->drawLine(QPoint(intervalWidth/2 + offset + intervalWidth * grid->getGrid()[i][j].previous[0]->x,
                                  intervalHeight/2 + offset + intervalHeight * grid->getGrid()[i][j].previous[0]->y),
                        QPoint(intervalWidth/2 + offset  + intervalWidth * i, intervalHeight/2 + offset + intervalHeight * j));
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
                painter->drawRect(offset + intervalWidth * i, offset + intervalHeight * j, intervalWidth, intervalHeight);
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
            painter->drawEllipse(mousePosition, 300/grid->getNbRow(), 300/grid->getNbColumn());
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
            int x = ((e->x() - offset) / intervalWidth);
            int y = ((e->y() - offset) / intervalHeight);
            currentX = x;
            currentY = y;
            pathConnected = false;
            mousePosition.setX(e->x());
            mousePosition.setY(e->y());

            pressClearPathCase(x, y);

            pressClearPathOrigin(x, y);
        }
    }

    update();
}

void MainWindow::pressClearPathCase(int x, int y)
{
    // allow user to clear a path from a specific case
    if (grid->getGrid()[x][y].getColor() != -1 && grid->getGrid()[x][y].next[0] != 0
            && grid->getGrid()[x][y].next[0]->getColor() == grid->getGrid()[x][y].getColor())
    {
        Path* temp = grid->getGrid()[x][y].next[0];
        clearPathColorCase(temp);
    }
}

void MainWindow::pressClearPathOrigin(int x, int y)
{
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
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    // restrict mouse movement to only the grid
    if (e->x() > offset && e->y() > offset && e->x() < width - offset
            && e->y() < height - offset)
    {
        int x = ((e->x() - offset) / intervalWidth);
        int y = ((e->y() - offset) / intervalHeight);
        mousePosition.setX(e->x());
        mousePosition.setY(e->y());

        if (grid->getGrid()[x][y].isOrigin())
        {
            grid->getGrid()[x][y].setCovered(true);
        }

        // restrict movement on blank cases towards other blank cases
        if (grid->getGrid()[currentX][currentY].getColor() != -1)
        {
            // limits the pathing creation to adjacents cases only
            if (abs(currentX - x) + abs(currentY - y) == 1)
            {
                moveActions(x, y);
            }
        }
    }

    update();
}

void MainWindow::moveActions(int x, int y)
{
    // if the case is blank
    if (grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].getColor() == -1
            && !pathConnected)
    {
        moveToBlankCase(x, y);
    }
    // when the second origin is connected to the first origin
    else if (grid->getGrid()[x][y].isOrigin()
             && grid->getGrid()[currentX][currentY].getColor() == grid->getGrid()[x][y].getColor()
             && !pathConnected)
    {
        moveToConnectSecondOrigin(x, y);
    }
    // if the user wants to create a path on another colored path
    else if (grid->getGrid()[x][y].getColor() != grid->getGrid()[currentX][currentY].getColor()
             && grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].next[0] != 0 && !pathConnected)
    {
        moveToOverridePath(x, y);
    }
    // clear only the last case from another colored path
    else if (grid->getGrid()[x][y].getColor() != grid->getGrid()[currentX][currentY].getColor()
             && grid->getGrid()[x][y].isOrigin() == false && grid->getGrid()[x][y].next[0] == 0 && !pathConnected)
    {
        moveToClearLastCase(x, y);
    }
    // allow user to go back when creating a path
    else if (grid->getGrid()[x][y].getColor() == grid->getGrid()[currentX][currentY].getColor()
             && grid->getGrid()[x][y].hasFlag() && grid->getGrid()[x][y].isCovered()
             && grid->getGrid()[currentX][currentY].isOrigin() == false && !pathConnected)
    {
        moveBackwards(x, y);
    }
}

void MainWindow::moveToBlankCase(int x, int y)
{
    grid->getGrid()[currentX][currentY].next[0] = &grid->getGrid()[x][y];
    grid->getGrid()[x][y].previous[0] = &grid->getGrid()[currentX][currentY];
    grid->getGrid()[x][y].setColor(grid->getGrid()[currentX][currentY].getColor());
    grid->getGrid()[x][y].setCoveredFlag(true);
    currentX = x;
    currentY = y;
    pathConnected = false;
}

void MainWindow::moveToConnectSecondOrigin(int x, int y)
{
    if (grid->getGrid()[x][y].getFirstOrigin() == false)
    {
        grid->getGrid()[x][y].setSecondOrigin(true);
        grid->getGrid()[currentX][currentY].next[0] = &grid->getGrid()[x][y];
        grid->getGrid()[x][y].previous[0] = &grid->getGrid()[currentX][currentY];
        grid->getGrid()[x][y].setCoveredFlag(true);
        grid->getGrid()[x][y].setPathComplete(true);
        currentX = x;
        currentY = y;
        pathConnected = true;

        //Check if game is won
        if(grid->isCompleted())
        {
            currentX = NOTACTIVE;
            currentY = NOTACTIVE;

            gameIsWon();
        }
    }
}

void MainWindow::moveToOverridePath(int x, int y)
{
    Path* temp = grid->getGrid()[x][y].next[0];
    Path* temp2 = grid->getGrid()[x][y].previous[0];
    temp2->next[0] = 0;
    grid->getGrid()[x][y].clear();

    clearPathColorCase(temp);
    pathConnected = false;
}

void MainWindow::moveToClearLastCase(int x, int y)
{
    Path* temp = grid->getGrid()[x][y].previous[0];
    temp->next[0] = 0;
    grid->getGrid()[x][y].clear();
    pathConnected = false;
}

void MainWindow::moveBackwards(int x, int y)
{
    if (grid->getGrid()[x][y].next[0] == &grid->getGrid()[currentX][currentY])
    {
        grid->getGrid()[currentX][currentY].clear();
        currentX = x;
        currentY = y;
        pathConnected = false;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent*)
{
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
            temp->clearOrigin();

            break;
        }
        else
        {
            break;
        }
    }
}

void MainWindow::gameIsWon()
{
    QMessageBox msg;
    msg.setText("Congratulations!");
    msg.setBaseSize(QSize(400,200));
    msg.exec();
}
