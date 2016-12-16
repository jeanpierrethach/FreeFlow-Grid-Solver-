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

#include <algorithm>
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
    ui->save->setGeometry(width - 340, height + height/24, 140, bottomSpace/3);
    ui->solve->setGeometry(200, height + height/24, 140, bottomSpace/3);

    connect(ui->back, SIGNAL(clicked()), this, SLOT(back()));
    connect(ui->restart, SIGNAL(clicked()), this, SLOT(restart()));
    connect(ui->save, SIGNAL(clicked()), this, SLOT(saveGame()));
    connect(ui->solve, SIGNAL(clicked()), this, SLOT(solve()));

    color = gen.getColorList();
    backgroundColor = gen.getBackgroundColorList();

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

bool MainWindow::mandatoryMove()
{
    Cell* cell = new Cell();

    bool didAMandatoryMove = false;

    std::cout << "\n\nStart of mandatoryMove\n\n";
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            //int color = grid->getGrid()[i][j].getColor();

            if (grid->getGrid()[i][j].isOrigin() && !grid->getCellPtr(i,j)->isPartOfCompletedPath()) //&& std::find(listOfOriginsColor.begin(), listOfOriginsColor.end(), color) == listOfOriginsColor.end())
            {
                cell = grid->getCellPtr(i, j);//&grid->getGrid()[i][j];
                //std::cout << "found origin\n";
                if(cell->next[0] != 0) {
                    while(cell->next[0] != 0) {
                        cell = cell->next[0];
                    }
                } else if(cell->previous[0] != 0) {
                    while(cell->previous[0] != 0) {
                        cell = cell->previous[0];
                    }
                }

                int x = cell->x;
                int y = cell->y;

                std::cout << "Position of the current cell: x = " << x << ",y = " << y << "\n";

                int numberOfExits = 0;
                int caseChosen = -1;


                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == cell->getColor() && grid->getCell(x-1,y).isOrigin()
                        && cell->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == cell->getColor() && !grid->getCell(x-1,y).isOrigin()
                        && grid->getCell(x-1,y).next[0] == 0)
                {
                    numberOfExits++;
                }
                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == cell->getColor() && grid->getCell(x-1,y).isOrigin()
                        && cell->previous[0] != 0 && !cell->previous[0]->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == cell->getColor() && grid->getCell(x-1,y).isOrigin()
                        && cell->previous[0] != 0 && cell->previous[0]->isOrigin())
                {
                    if (grid->getCell(x-1,y).next[0] != 0 && cell->x == grid->getCell(x-1,y).next[0]->x
                            && cell->y == grid->getCell(x-1,y).next[0]->y)
                    {

                    }
                    else
                    {
                        numberOfExits++;
                    }

                }
                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == -1) //|| grid->getCell(x-1,y).getColor() == cell->getColor()))
                {
                    caseChosen = 0;
                    /*Cell* iterator = new Cell();
                    iterator = cell;
                    while(iterator->previous[0] != 0) {
                        iterator = iterator->previous[0];
                        if(iterator->x == x && iterator->y == y){
                            numberOfExits--;
                            caseChosen = -1;
                        }
                    }*/
                    //std::cout << "LEFT\n";

                    numberOfExits++;



                }


                if (cell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == cell->getColor() && grid->getCell(x,y+1).isOrigin()
                        && cell->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == cell->getColor() && !grid->getCell(x,y+1).isOrigin()
                        && grid->getCell(x,y+1).next[0] == 0)
                {
                    numberOfExits++;
                }
                if (cell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == cell->getColor() && grid->getCell(x,y+1).isOrigin()
                        && cell->previous[0] != 0 && !cell->previous[0]->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == cell->getColor() && grid->getCell(x,y+1).isOrigin()
                        && cell->previous[0] != 0 && cell->previous[0]->isOrigin())
                {
                    if (grid->getCell(x,y+1).next[0] != 0 && cell->x == grid->getCell(x,y+1).next[0]->x
                            && cell->y == grid->getCell(x,y+1).next[0]->y)
                    {

                    }
                    else
                    {
                        numberOfExits++;
                    }

                }
                if (cell->y+1 < grid->getNbRow() && (grid->getCell(x,y+1).getColor() == -1)) // || grid->getCell(x,y+1).getColor() == cell->getColor()))
                {
                    caseChosen = 1;
                    /*Cell* iterator = new Cell();
                    iterator = cell;
                    while(iterator->previous[0] != 0) {
                        iterator = iterator->previous[0];
                        if(iterator->x == x && iterator->y == y) {
                            numberOfExits--;
                            caseChosen = -1;
                        }
                    }*/
                    //std::cout << "DOWN\n";

                    numberOfExits++;


                }


                if (cell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == cell->getColor() && grid->getCell(x+1,y).isOrigin()
                        && cell->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == cell->getColor() && !grid->getCell(x+1,y).isOrigin()
                        && grid->getCell(x+1,y).next[0] == 0)
                {
                    numberOfExits++;
                }
                if (cell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == cell->getColor() && grid->getCell(x+1,y).isOrigin()
                        && cell->previous[0] != 0 && !cell->previous[0]->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == cell->getColor() && grid->getCell(x+1,y).isOrigin()
                        && cell->previous[0] != 0 && cell->previous[0]->isOrigin())
                {
                    if (grid->getCell(x+1,y).next[0] != 0 && cell->x == grid->getCell(x+1,y).next[0]->x
                            && cell->y == grid->getCell(x+1,y).next[0]->y)
                    {

                    }
                    else
                    {
                        numberOfExits++;
                    }

                }
                if (cell->x+1 < grid->getNbColumn() && (grid->getCell(x+1,y).getColor() == -1)) // || grid->getCell(x+1,y).getColor() == cell->getColor()))
                {
                    caseChosen = 2;
                    /*Cell* iterator = new Cell();
                    iterator = cell;
                    while(iterator->previous[0] != 0) {
                        iterator = iterator->previous[0];
                        if(iterator->x == x && iterator->y == y){
                            numberOfExits--;
                            caseChosen = -1;
                        }
                    }*/
                    //std::cout << "RIGHT\n";

                    numberOfExits++;


                }

                if (cell->y-1 >= 0 && grid->getCell(x,y-1).getColor() == cell->getColor() && grid->getCell(x,y-1).isOrigin()
                        && cell->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->y-1 >= 0 && grid->getCell(x,y-1).getColor() == cell->getColor() && !grid->getCell(x,y-1).isOrigin()
                        && grid->getCell(x,y-1).next[0] == 0)
                {
                    numberOfExits++;
                }
                if (cell->y-1 >= 0 && grid->getCell(x,y-1).getColor() == cell->getColor() && grid->getCell(x,y-1).isOrigin()
                        && cell->previous[0] != 0 && !cell->previous[0]->isOrigin())
                {
                    numberOfExits++;
                }
                if (cell->y-1 >= 0 && grid->getCell(x,y-1).getColor() == cell->getColor() && grid->getCell(x,y-1).isOrigin()
                        && cell->previous[0] != 0 && cell->previous[0]->isOrigin())
                {
                    if (grid->getCell(x,y-1).next[0] != 0 && cell->x == grid->getCell(x,y-1).next[0]->x
                            && cell->y == grid->getCell(x,y-1).next[0]->y)
                    {

                    }
                    else
                    {
                        numberOfExits++;
                    }

                }
                if (cell->y-1 >= 0 && (grid->getCell(x,y-1).getColor() == -1)) // || grid->getCell(x,y-1).getColor() == cell->getColor()))
                {
                    caseChosen = 3;
                    /*Cell* iterator = new Cell();
                    iterator = cell;
                    while(iterator->previous[0] != 0) {
                        iterator = iterator->previous[0];
                        if(iterator->x == x && iterator->y == y){
                            numberOfExits--;
                            caseChosen = -1;
                        }
                    }*/
                    //std::cout << "UP\n";

                    numberOfExits++;


                }

                std::cout << "\nnbExit: " << numberOfExits << "\n";
                if(numberOfExits == 1) { //  && notValidExit == 1
                    std::cout << "Before entering; caseChosen: " << caseChosen << "\n";
                    Cell* nextCell = new Cell();
                    switch(caseChosen) {
                        case(0):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x-1, y);
                        grid->getGrid()[x-1][y].setCovered(true);
                        grid->getGrid()[x-1][y].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x-1][y].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];
                        std::cout << "nextCell case 0 GOING LEFT, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";
                        repaint();
                        usleep(300000);

                        break;
                    case(1):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x,y+1);
                        grid->getGrid()[x][y+1].setCovered(true);
                        grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y+1].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];
                        std::cout << "nextCell case 1 GOING DOWN, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";
                        repaint();
                        usleep(300000);

                        break;
                    case(2):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x+1,y);

                        grid->getGrid()[x+1][y].setCovered(true);
                        grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x+1][y].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];
                        std::cout << "nextCell case 2 GOING RIGHT, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";
                        repaint();
                        usleep(300000);

                        break;
                    case(3):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x, y-1);

                        grid->getGrid()[x][y-1].setCovered(true);
                        grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y-1].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];
                        std::cout << "nextCell case 3 GOING UP, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";
                        repaint();
                        usleep(300000);

                        break;
                    }





                }

            }

        }
    }



    //TODO remove ?
    //repaint();
    //usleep(30000);
    //update();

    return didAMandatoryMove;

}

void MainWindow::reversePath(Cell* cell) {
    Cell* current = new Cell();
    current = cell;
    if(current->next[0] != 0) {
        Cell* next = new Cell();
        next = current->next[0];

        Cell* previous = new Cell();
        if (current->previous[0] != 0){
            previous = current->previous[0];
        }
        else
        {
            previous = 0;
        }

        while(current->next[0] != 0)
        {
            current->previous[0] = next;
            current->next[0] = previous;

            current = next;
            previous = current->previous[0];
            next = current->next[0];
        }
        if(current->previous[0] != 0)
        {
            current->next[0] = current->previous[0];
            current->previous[0] = 0;
        }

    }
}

void MainWindow::solve()
{
    Cell* temp2 = new Cell();

    int nbOfColors = 0;
    std::vector<int> listOfOriginsColor;

    // Do the mandatory moves
    /*for (int i = 0; i < 50; i++)
    {
        mandatoryMove();
        //
    }*/
    while(mandatoryMove());

    //Reverse the path of the second origin
    std::vector<bool> reversePath(nbOfColors);
    for(bool b : reversePath) {
        b = false;
    }
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].isOrigin())
            {
                if(reversePath[grid->getGrid()[i][j].getColor()]) {
                    Cell* current = new Cell();
                    current = grid->getCellPtr(i,j);
                    if(current->next[0] != 0) {
                        Cell* next = new Cell();
                        next = current->next[0];

                        Cell* previous = new Cell();
                        if (current->previous[0] != 0){
                            previous = current->previous[0];
                        }
                        else
                        {
                            previous = 0;
                        }

                        while(current->next[0] != 0)
                        {
                            current->previous[0] = next;
                            current->next[0] = previous;

                            current = next;
                            previous = current->previous[0];
                            next = current->next[0];
                        }
                        if(current->previous[0] != 0)
                        {
                            current->next[0] = current->previous[0];
                            current->previous[0] = 0;
                        }

                    }
                } else {
                    reversePath[grid->getGrid()[i][j].getColor()] = true;
                }

            }
        }
    }



    // for .. solveRec(temp2, false);


    //Try to recursively solve each path starting from point i,j
    //for(int p = 0; p < 10; p++) {
    std::vector<bool> solveFromFirstOrigin(nbOfColors);
        for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            int color = grid->getGrid()[i][j].getColor();

            if (grid->getGrid()[i][j].isOrigin() && solveFromFirstOrigin[color] == false) //&& std::find(listOfOriginsColor.begin(), listOfOriginsColor.end(), color) == listOfOriginsColor.end())
            {
                listOfOriginsColor.push_back(color);
                solveFromFirstOrigin[color] = true;

                nbOfColors++;
                temp2 = grid->getCellPtr(i,j);
                while(temp2->next != 0 && temp2->next[0] != 0) {
                    temp2 = temp2->next[0];
                }

                solveRec(temp2, false, true); // p != 0
                if(!grid->getGrid()[i][j].isPartOfCompletedPath()) {
                    //pressClearPathOrigin(i,j);
                    pressClearPathCase(temp2->x, temp2->y);
                    //TEST
                                            temp2->next[0] = 0;

                    if(temp2->isOrigin()) {
                        temp2->next[0] = 0;
                    }
                    /*
                    if (temp2->isOrigin()){
                        pressClearPathOrigin(temp2->x,temp2->y);
                    }
                    else
                    {
                        pressClearPathCase(temp2->x, temp2->y);
                    }
                    */
                    //retry counterCW
                    solveRec(temp2, false, false); // p != 0
                    if(!grid->getGrid()[i][j].isPartOfCompletedPath()) {
                        pressClearPathCase(temp2->x, temp2->y);
//TEST
                        temp2->next[0] = 0;

                        if(temp2->isOrigin()) {
                            temp2->next[0] = 0;
                        }
                        /*
                        if (temp2->isOrigin()){
                            pressClearPathOrigin(temp2->x,temp2->y);
                        }
                        else
                        {
                            pressClearPathCase(temp2->x, temp2->y);
                        }
                        */
                        //pressClearPathOrigin(i,j);
                    }
                }
                //while(mandatoryMove());
            }

        }
    }
//}


    std::cout << "TEST";
    //Check which color is completed

    std::vector<bool> pathCompletedColor(nbOfColors);
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].getPathComplete())
            {
                pathCompletedColor[grid->getGrid()[i][j].getColor()] = true;
            }
        }
    }
    repaint();
    usleep(30000);

    //if(false) {

    //Clear the paths that arent completed
/*
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].isOrigin() && pathCompletedColor[grid->getGrid()[i][j].getColor()] == false)
            {
                pressClearPathOrigin(i, j);
            }
        }
    }*/

    //Try to complete what is left to do randomly (hope for the best)
    for(bool b : pathCompletedColor) {
        std::cout << "Path complete =" << b << "\n";
    }

    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].isOrigin() && pathCompletedColor[grid->getGrid()[i][j].getColor()] == false)
            {
                int tries = 0;
                pathCompletedColor[grid->getGrid()[i][j].getColor()] = true;
                bool isCompleted = false;

                Cell* cell = new Cell();
                cell = grid->getCellPtr(i, j);
                while(cell->next[0] != 0) {
                    cell = cell->next[0];
                }

                while (!isCompleted && tries < 50) {

                    pressClearPathCase(cell->x, cell->y);
                    if(cell->isOrigin()) {
                        cell->next[0] = 0;
                    }
/*
                    if (cell->isOrigin()){
                        pressClearPathOrigin(cell->x,cell->y);
                    }
                    else
                    {
                        pressClearPathCase(cell->x, cell->y);
                    }
  */
                    tries++;

                    //Cell* cellRec = new Cell();
                    //cellRec = grid->getCellPtr(cell->x, cell->y);

                    solveRec(cell ,true, true);
                    isCompleted = grid->getCellPtr(i,j)->isPartOfCompletedPath();
                    //std::cout << isCompleted;

                }
                if(!grid->getGrid()[i][j].isPartOfCompletedPath())
                {
                    //clearPathColorCase(initialCell);
                    pressClearPathCase(cell->x, cell->y);
                    if(cell->isOrigin()) {
                        cell->next[0] = 0;
                    }
/*
                    if (cell->isOrigin()){
                        pressClearPathOrigin(cell->x,cell->y);
                    }
                    else
                    {
                        pressClearPathCase(cell->x, cell->y);
                    }
  */
                }
            }
        }

    //}

}

    update();

}

bool MainWindow::adjacentCellIsCompatible(int x, int y, int color) {
    return  grid->getCell(x,y).isOrigin() &&
            grid->getCell(x,y).getColor() == color &&
            grid->getCell(x,y).next[0] == 0;
}

bool MainWindow::cellIsPartOfHimself(Cell* current, Cell* target) {
    Cell* it = new Cell();
    it = current;
    while(it->previous[0] != 0) {
        if(it->x == target->x && it->y == target->y)
            return true;
        it = it->previous[0];
    }
    return false;
}

void MainWindow::verifySecondOrigin(int x, int y)
{
    Cell* it = grid->getCellPtr(x-1,y);
    while(it->next[0] != 0) {
        it = it->next[0];
    }
    if(it->isOrigin()) {
        it->setSecondOrigin(true);
        it->setPathComplete(true);
    }
}

void MainWindow::solveRec(Cell* currentCell, bool isRandom, bool clockTurn)
{
    repaint();
    usleep(30000);

    if(currentCell->getPathComplete() || currentCell->next[0] != 0) {
        return;
    }

    Cell* nextCell = new Cell();
    int x = currentCell->x;
    int y = currentCell->y;
    nextCell = grid->getCellPtr(x, y);

    currentCell->setCovered(true);

    // TODO check if can connect to path with
    //check connect with free path
    //if

    // verify path free then connect
    if (nextCell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == currentCell->getColor()
            && !grid->getCell(x-1,y).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x-1,y)) &&
            //(grid->getCellPtr(x-1,y)->next[0] == 0 ||
             grid->getCellPtr(x-1,y)->previous[0] == 0)
    {
        /*if(grid->getGrid()[x-1][y].next[0] == 0)
        {
            reversePath(grid->getCellPtr(x-1,y));
        }*/

        grid->getGrid()[x-1][y].setCovered(true);
        grid->getGrid()[x-1][y].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x-1][y].previous[0] = nextCell;
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];

        verifySecondOrigin(x-1,y);

        return;
    }
    else if (nextCell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == currentCell->getColor()
             && !grid->getCell(x,y+1).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x,y+1)) &&
             //(grid->getCellPtr(x,y+1)->next[0] == 0 ||
                 grid->getCellPtr(x,y+1)->previous[0] == 0)
     {
        /*if(grid->getGrid()[x][y+1].next[0] == 0)
        {
            reversePath(grid->getCellPtr(x,y+1));
        }*/
         grid->getGrid()[x][y+1].setCovered(true);
         grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x][y+1].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];

         verifySecondOrigin(x,y+1);

         return;
     }
    else if (nextCell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == currentCell->getColor()
             && !grid->getCell(x+1,y).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x+1,y))
             &&
             //(grid->getCellPtr(x+1,y)->next[0] == 0 ||
             grid->getCellPtr(x+1,y)->previous[0] == 0)
     {
        /*
        if(grid->getGrid()[x+1][y].next[0] == 0)
        {
            reversePath(grid->getCellPtr(x+1,y));
        }
        */
         grid->getGrid()[x+1][y].setCovered(true);
         grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x+1][y].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];

         verifySecondOrigin(x+1,y);

         return;
     }
    else if (nextCell->y-1 >= 0  && grid->getCell(x,y-1).getColor() == currentCell->getColor()
             && !grid->getCell(x,y-1).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x,y-1))
             &&
             //(grid->getCellPtr(x,y-1)->next[0] == 0 ||
             grid->getCellPtr(x,y-1)->previous[0] == 0)
     {
        /*
        if(grid->getGrid()[x][y-1].next[0] == 0)
        {
            reversePath(grid->getCellPtr(x,y-1));
        }
        */
         grid->getGrid()[x][y-1].setCovered(true);
         grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x][y-1].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];

         verifySecondOrigin(x,y-1);

         return;
     }

    // Verify origin then connect
    else if (nextCell->x-1 >= 0 && adjacentCellIsCompatible(x-1,y, currentCell->getColor()))
    {
        grid->getGrid()[x-1][y].setCovered(true);
        grid->getGrid()[x-1][y].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x-1][y].previous[0] = nextCell;
        grid->getGrid()[x-1][y].setSecondOrigin(true);
        grid->getGrid()[x-1][y].setPathComplete(true);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];

        solveRec(grid->getCellPtr(x-1,y), isRandom, clockTurn);
    }
    else if (nextCell->y+1 < grid->getNbRow() && adjacentCellIsCompatible(x,y+1,currentCell->getColor()))
    {
        grid->getGrid()[x][y+1].setCovered(true);
        grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x][y+1].previous[0] = nextCell;
        grid->getGrid()[x][y+1].setSecondOrigin(true);
        grid->getGrid()[x][y+1].setPathComplete(true);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];

        solveRec(grid->getCellPtr(x,y+1), isRandom,clockTurn);
    }
    else if (nextCell->x+1 < grid->getNbColumn() && adjacentCellIsCompatible(x+1,y,currentCell->getColor()))
    {
        grid->getGrid()[x+1][y].setCovered(true);
        grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x+1][y].previous[0] = nextCell;
        grid->getGrid()[x+1][y].setSecondOrigin(true);
        grid->getGrid()[x+1][y].setPathComplete(true);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];

        solveRec(grid->getCellPtr(x+1,y), isRandom,clockTurn);
    }
    else if (nextCell->y-1 >= 0 && adjacentCellIsCompatible(x,y-1,currentCell->getColor()))
    {
        grid->getGrid()[x][y-1].setCovered(true);
        grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x][y-1].previous[0] = nextCell;
        grid->getGrid()[x][y-1].setSecondOrigin(true);
        grid->getGrid()[x][y-1].setPathComplete(true);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];

        solveRec(grid->getCellPtr(x,y-1), isRandom,clockTurn);
    }
    else {
        //int random = !isRandom ? 0 : GridBuilder::getRandomNumberFrom0To(4);

        std::vector<int> order(4);
        for(int i = 0; i < 4; i++) {
            int val = !isRandom ? i : GridBuilder::getRandomNumberFrom0To(100);
            if(!isRandom && !clockTurn)
                val = 10 - val;
            order[i] = val;
        }

        bool enteredARecursion = false;
        for(int i = 0; i < 4; i++) {
            int min = 0;
            for(int i = 1; i < 4; i++){
                if(order[i] < order[min])
                    min = i;
            }
            order[min] += 100;

            //random++;
            //random %= 4;
            switch(min) {
            case(0):
                if (!enteredARecursion && nextCell->x-1 >= 0 && grid->getCell(x-1,y).isOrigin() == false && grid->getCell(x-1,y).isCovered() == false)
                    {
                        enteredARecursion = true;

                        grid->getGrid()[x-1][y].setCovered(true);
                        grid->getGrid()[x-1][y].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x-1][y].previous[0] = nextCell;
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];

                        solveRec(grid->getCellPtr(x-1,y), isRandom,clockTurn);
                    }
                break;
            case(1):
                if (!enteredARecursion && nextCell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).isOrigin() == false && grid->getCell(x,y+1).isCovered() == false)
                    {
                        enteredARecursion = true;

                        grid->getGrid()[x][y+1].setCovered(true);
                        grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y+1].previous[0] = nextCell;
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];

                        solveRec(grid->getCellPtr(x,y+1), isRandom,clockTurn);
                    }
                break;
            case(2):
                if (!enteredARecursion && nextCell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).isOrigin() == false && grid->getCell(x+1,y).isCovered() == false)
                    {
                        enteredARecursion = true;

                        grid->getGrid()[x+1][y].setCovered(true);
                        grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x+1][y].previous[0] = nextCell;
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];

                        solveRec(grid->getCellPtr(x+1,y), isRandom,clockTurn);
                    }
                break;
            case(3):
                if (!enteredARecursion && nextCell->y-1 >= 0 && grid->getCell(x,y-1).isOrigin() == false && grid->getCell(x,y-1).isCovered() == false)
                    {
                        enteredARecursion = true;

                        grid->getGrid()[x][y-1].setCovered(true);
                        grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y-1].previous[0] = nextCell;
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];

                        solveRec(grid->getCellPtr(x,y-1), isRandom,clockTurn);
                    }
                break;
            }
        }
    }

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
                painter->setPen(backgroundColor[grid->getGrid()[i][j].getColor()]);
                painter->setBrush(backgroundColor[grid->getGrid()[i][j].getColor()]);
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
            painter->setPen(backgroundColor[grid->getGrid()[currentX][currentY].getColor()]);
            painter->setBrush(backgroundColor[grid->getGrid()[currentX][currentY].getColor()]);
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
        Cell* temp = grid->getGrid()[x][y].next[0];
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
    Cell* temp = grid->getGrid()[x][y].next[0];
    Cell* temp2 = grid->getGrid()[x][y].previous[0];
    temp2->next[0] = 0;
    grid->getGrid()[x][y].clear();

    clearPathColorCase(temp);
    pathConnected = false;
}

void MainWindow::moveToClearLastCase(int x, int y)
{
    Cell* temp = grid->getGrid()[x][y].previous[0];
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

void MainWindow::clearPathColorCase(Cell* temp)
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
