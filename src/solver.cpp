#include "solver.h"
#include <iostream>
#include "gridbuilder.h"

using namespace std;


void Solver::pressClearPathCase(int x, int y)
{
    // allow user to clear a path from a specific case
    if (grid->getGrid()[x][y].getColor() != -1 && grid->getGrid()[x][y].next[0] != 0
            && grid->getGrid()[x][y].next[0]->getColor() == grid->getGrid()[x][y].getColor())
    {
        Cell* temp = grid->getGrid()[x][y].next[0];
        clearPathColorCase(temp);
    }
}


void Solver::clearPathColorCase(Cell* temp)
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

void Solver::incrementNbOfExitsIfPossible(Cell* cell, int x, int y, int* numberOfExits)
{
    if (grid->getCell(x,y).getColor() != cell->getColor())
        return;

    if (grid->getCell(x,y).isOrigin() == false && grid->getCell(x,y).next[0] == 0)
    {
        (*numberOfExits)++;
    }
    if (grid->getCell(x,y).isOrigin() && (cell->isOrigin()
        || (cell->previous[0] != 0 && !cell->previous[0]->isOrigin())))
    {
        (*numberOfExits)++;
    }
    if (grid->getCell(x,y).isOrigin() && cell->previous[0] != 0 && cell->previous[0]->isOrigin())
    {
        if (!(grid->getCell(x,y).next[0] != 0 && cell->x == grid->getCell(x,y).next[0]->x
                && cell->y == grid->getCell(x,y).next[0]->y))
        {
            (*numberOfExits)++;
        }
    }
}

bool Solver::mandatoryMove()
{
    Cell* cell = new Cell();

    bool didAMandatoryMove = false;

    std::cout << "\n\nStart of mandatoryMove\n\n";
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].isOrigin() && !grid->getCellPtr(i,j)->isPartOfCompletedPath())
            {
                cell = grid->getCellPtr(i, j);

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

                // Checking left cell
                if (cell->x-1 >= 0)
                    incrementNbOfExitsIfPossible(cell, x-1, y, &numberOfExits);

                if (cell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == -1)
                {
                    caseChosen = 0;

                    numberOfExits++;
                }

                // Checking down cell
                if (cell->y+1 < grid->getNbRow())
                    incrementNbOfExitsIfPossible(cell, x, y+1, &numberOfExits);

                if (cell->y+1 < grid->getNbRow() && (grid->getCell(x,y+1).getColor() == -1))
                {
                    caseChosen = 1;

                    numberOfExits++;
                }

                // Checking right cell               
                if (cell->x+1 < grid->getNbColumn())
                    incrementNbOfExitsIfPossible(cell, x+1, y, &numberOfExits);

                if (cell->x+1 < grid->getNbColumn() && (grid->getCell(x+1,y).getColor() == -1)) // || grid->getCell(x+1,y).getColor() == cell->getColor()))
                {
                    caseChosen = 2;

                    numberOfExits++;
                }

                // Checking up cell
                if (cell->y-1 >= 0)
                    incrementNbOfExitsIfPossible(cell, x, y-1, &numberOfExits);

                if (cell->y-1 >= 0 && (grid->getCell(x,y-1).getColor() == -1)) // || grid->getCell(x,y-1).getColor() == cell->getColor()))
                {
                    caseChosen = 3;

                    numberOfExits++;
                }

                std::cout << "nbExit: " << numberOfExits << "\n";

                if(numberOfExits == 1) {
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

                        break;
                    case(1):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x,y+1);
                        grid->getGrid()[x][y+1].setCovered(true);
                        grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y+1].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];
                        std::cout << "nextCell case 1 GOING DOWN, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";

                        break;
                    case(2):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x+1,y);

                        grid->getGrid()[x+1][y].setCovered(true);
                        grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x+1][y].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];
                        std::cout << "nextCell case 2 GOING RIGHT, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";

                        break;
                    case(3):
                        didAMandatoryMove = true;
                        nextCell = grid->getCellPtr(x, y-1);

                        grid->getGrid()[x][y-1].setCovered(true);
                        grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
                        grid->getGrid()[x][y-1].previous[0] = &grid->getGrid()[x][y];
                        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];
                        std::cout << "nextCell case 3 GOING UP, x: " << nextCell->x << " ,y: " << nextCell->y << "\n\n";

                        break;
                    }

                    //repaintAndWait();
                }
            }
        }
    }

    return didAMandatoryMove;
}

void Solver::reversePath(Cell* cell) {
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

void Solver::reversePathFromSecondOrigin(int nbOfColors)
{
    //Reverse the path of the second origin
    std::vector<bool> reversableOrigins(nbOfColors);

    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            if (grid->getGrid()[i][j].isOrigin())
            {
                // if it's the first origin then false, else true
                if(reversableOrigins[grid->getGrid()[i][j].getColor()]) {
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
                    reversableOrigins[grid->getGrid()[i][j].getColor()] = true;
                }

            }
        }
    }
}

void Solver::solveFromEdgeOfOrigin(int nbOfColors)
{
    Cell* cell = new Cell();

    std::vector<bool> solveFromFirstOrigin(nbOfColors);
    for (int i = 0; i < grid->getNbRow(); ++i)
    {
        for (int j = 0; j < grid->getNbColumn(); ++j)
        {
            int color = grid->getGrid()[i][j].getColor();

            if (grid->getGrid()[i][j].isOrigin() && solveFromFirstOrigin[color] == false)
            {
                solveFromFirstOrigin[color] = true;

                nbOfColors++;
                cell = grid->getCellPtr(i,j);
                while(cell->next[0] != 0) {
                    cell = cell->next[0];
                }

                // Try Counterclock wise
                solveRec(cell, false, true); // p != 0
                if(!grid->getGrid()[i][j].isPartOfCompletedPath())
                {
                    pressClearPathCase(cell->x, cell->y);
                    cell->next[0] = 0;

                    // Retry Clockwise
                    solveRec(cell, false, false); // p != 0
                    if(!grid->getGrid()[i][j].isPartOfCompletedPath()) {
                        pressClearPathCase(cell->x, cell->y);
                        cell->next[0] = 0;
                    }
                }

                //while(mandatoryMove());
            }

        }
    }
}

void Solver::solveRandomly(const std::vector<bool>& pathCompleted)
{
    //Try to complete what is left to do randomly (hope for the best)
    std::vector<bool> pathCompletedColor(pathCompleted.size());

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
                    cell->next[0] = 0;

                    tries++;

                    solveRec(cell ,true, true);
                    isCompleted = grid->getCellPtr(i,j)->isPartOfCompletedPath();
                }
                if(!grid->getGrid()[i][j].isPartOfCompletedPath())
                {
                    pressClearPathCase(cell->x, cell->y);
                    cell->next[0] = 0;
                }
            }
        }
    }
}

void Solver::matchEmptyCaseWithPath(Cell** adjacentCell, Cell** adjacentCell2, bool* matching, Cell** bottomBlank, Cell** topBlank)
{

    if((*adjacentCell)->next[0] == *adjacentCell2) {
        *matching = true;
    }
    else if ((*adjacentCell2)->next[0] == *adjacentCell)
    {
        *matching = true;
        Cell* temp = new Cell();

        temp = *adjacentCell;
        *adjacentCell = *adjacentCell2;
        *adjacentCell2 = temp;

        temp = *bottomBlank;
        *bottomBlank = *topBlank;
        *topBlank = temp;
    }
}

void Solver::fillGrid() {
    for(int x = 0; x < grid->getNbRow(); ++x)
    {
        for(int y = 0; y < grid->getNbColumn(); ++y)
        {
            //Found empty spot
            if(grid->getCell(x,y).getColor() == -1) {
                //found a second adjacent empty cell
                Cell* adjacentCell = new Cell();
                Cell* adjacentCell2 = new Cell();
                bool matching = false;

                Cell* firstEmptyCell = new Cell();
                Cell* secondEmptyCell = new Cell();
                firstEmptyCell = grid->getCellPtr(x,y);

                if(y+1 < grid->getNbColumn() && grid->getCell(x,y+1).getColor() == -1)
                {
                    secondEmptyCell = grid->getCellPtr(x, y+1);

                    //check parralel on the left
                    if(x-1 >= 0)
                    {
                        adjacentCell = grid->getCellPtr(x-1,y);
                        adjacentCell2 = grid->getCellPtr(x-1,y+1);
                        matchEmptyCaseWithPath(&adjacentCell, &adjacentCell2, &matching, &secondEmptyCell, &firstEmptyCell);
                    }
                    //check parralel on the right
                    if(!matching && x+1 < grid->getNbRow())
                    {
                        adjacentCell = grid->getCellPtr(x+1,y);
                        adjacentCell2 = grid->getCellPtr(x+1,y+1);
                        matchEmptyCaseWithPath(&adjacentCell, &adjacentCell2, &matching, &secondEmptyCell, &firstEmptyCell);
                    }
                    if(matching)
                    {
                        firstEmptyCell->setColor(adjacentCell->getColor());
                        secondEmptyCell->setColor(adjacentCell->getColor());

                        firstEmptyCell->previous[0] = adjacentCell;
                        firstEmptyCell->next[0] = secondEmptyCell;

                        secondEmptyCell->previous[0] = firstEmptyCell;
                        secondEmptyCell->next[0] = adjacentCell2;

                        adjacentCell->next[0] = firstEmptyCell;
                        adjacentCell2->previous[0] = secondEmptyCell;
                    }
                }
                else if(x+1 < grid->getNbRow() && grid->getCell(x,y).getColor() == -1 && grid->getCell(x+1,y).getColor() == -1)
                {
                    secondEmptyCell = grid->getCellPtr(x+1, y);

                    //check parralel up
                    if(y-1 >= 0)
                    {
                        adjacentCell = grid->getCellPtr(x,y-1);
                        adjacentCell2 = grid->getCellPtr(x+1,y-1);
                        matchEmptyCaseWithPath(&adjacentCell, &adjacentCell2, &matching, &secondEmptyCell, &firstEmptyCell);
                    }
                    //check parralel down
                    if(!matching && y+1 < grid->getNbRow())
                    {
                        adjacentCell = grid->getCellPtr(x,y+1);
                        adjacentCell2 = grid->getCellPtr(x+1,y+1);
                        matchEmptyCaseWithPath(&adjacentCell, &adjacentCell2, &matching, &secondEmptyCell, &firstEmptyCell);
                    }
                    if(matching)
                    {
                        firstEmptyCell->setColor(adjacentCell->getColor());
                        secondEmptyCell->setColor(adjacentCell->getColor());

                        firstEmptyCell->previous[0] = adjacentCell;
                        firstEmptyCell->next[0] = secondEmptyCell;

                        secondEmptyCell->previous[0] = firstEmptyCell;
                        secondEmptyCell->next[0] = adjacentCell2;

                        adjacentCell->next[0] = firstEmptyCell;
                        adjacentCell2->previous[0] = secondEmptyCell;
                    }
                }

            }


        }
    }
}

void Solver::solve(Grid** g)
{
    this->grid = *g;

    int nbOfColors = 0;
    for(int i = 0; i < grid->getNbColumn(); i++) {
        for(int j = 0; j < grid->getNbRow(); j++) {
            if(nbOfColors < grid->getCell(i,j).getColor()) {
                nbOfColors = grid->getCell(i,j).getColor();
            }
        }
    }
    nbOfColors++;

    while(mandatoryMove());

    reversePathFromSecondOrigin(nbOfColors);

    //Try to recursively solve each path starting from point i,j
    solveFromEdgeOfOrigin(nbOfColors);

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

    solveRandomly(pathCompletedColor);

    bool hasEmptyCells = true;
    int maxTries = 20;
    while(hasEmptyCells && maxTries > 0)
    {
        maxTries--;
        hasEmptyCells = false;
        for(int i = 0; i < grid->getNbRow(); ++i)
        {
            for(int j = 0; j < grid->getNbColumn(); ++j)
            {
                if(grid->getCell(i,j).getColor() == -1)
                {
                    hasEmptyCells = true;
                }
            }
        }
        if(hasEmptyCells)
            fillGrid();
    }
}

bool Solver::adjacentCellIsCompatible(int x, int y, int color) {
    return  grid->getCell(x,y).isOrigin() &&
            grid->getCell(x,y).getColor() == color &&
            grid->getCell(x,y).next[0] == 0;
}

bool Solver::cellIsPartOfHimself(Cell* current, Cell* target) {
    Cell* it = new Cell();
    it = current;
    while(it->previous[0] != 0) {
        if(it->x == target->x && it->y == target->y)
            return true;
        it = it->previous[0];
    }
    return false;
}

void Solver::confirmSecondOriginFrom(int x, int y)
{
    Cell* it = grid->getCellPtr(x,y);
    while(it->next[0] != 0) {
        it = it->next[0];
    }
    if(it->isOrigin()) {
        it->setSecondOrigin(true);
        it->setPathComplete(true);
    }
}

void Solver::connectToLoneSecondOrigin(int x, int y, int color, Cell* nextCell)
{
    grid->getGrid()[x][y].setCovered(true);
    grid->getGrid()[x][y].setColor(color);
    grid->getGrid()[x][y].previous[0] = nextCell;
    grid->getGrid()[x][y].setSecondOrigin(true);
    grid->getGrid()[x][y].setPathComplete(true);
}

void Solver::solveRec(Cell* currentCell, bool isRandom, bool clockTurn)
{
    if(currentCell->getPathComplete() || currentCell->next[0] != 0) {
        return;
    }

    Cell* nextCell = new Cell();
    int x = currentCell->x;
    int y = currentCell->y;
    nextCell = grid->getCellPtr(x, y);

    currentCell->setCovered(true);

    // verify path free then connect
    if (nextCell->x-1 >= 0 && grid->getCell(x-1,y).getColor() == currentCell->getColor()
            && !grid->getCell(x-1,y).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x-1,y))
            && grid->getCellPtr(x-1,y)->previous[0] == 0)
    {
        grid->getGrid()[x-1][y].setCovered(true);
        grid->getGrid()[x-1][y].setColor(grid->getGrid()[x][y].getColor());
        grid->getGrid()[x-1][y].previous[0] = nextCell;
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];

        confirmSecondOriginFrom(x-1,y);

        return;
    }
    else if (nextCell->y+1 < grid->getNbRow() && grid->getCell(x,y+1).getColor() == currentCell->getColor()
             && !grid->getCell(x,y+1).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x,y+1))
             && grid->getCellPtr(x,y+1)->previous[0] == 0)
     {
         grid->getGrid()[x][y+1].setCovered(true);
         grid->getGrid()[x][y+1].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x][y+1].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];

         confirmSecondOriginFrom(x,y+1);

         return;
     }
    else if (nextCell->x+1 < grid->getNbColumn() && grid->getCell(x+1,y).getColor() == currentCell->getColor()
             && !grid->getCell(x+1,y).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x+1,y))
             && grid->getCellPtr(x+1,y)->previous[0] == 0)
     {
         grid->getGrid()[x+1][y].setCovered(true);
         grid->getGrid()[x+1][y].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x+1][y].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];

         confirmSecondOriginFrom(x+1,y);

         return;
     }
    else if (nextCell->y-1 >= 0  && grid->getCell(x,y-1).getColor() == currentCell->getColor()
             && !grid->getCell(x,y-1).isOrigin() && !cellIsPartOfHimself(nextCell, grid->getCellPtr(x,y-1))
             && grid->getCellPtr(x,y-1)->previous[0] == 0)
     {
         grid->getGrid()[x][y-1].setCovered(true);
         grid->getGrid()[x][y-1].setColor(grid->getGrid()[x][y].getColor());
         grid->getGrid()[x][y-1].previous[0] = nextCell;
         grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];

         confirmSecondOriginFrom(x,y-1);

         return;
     }

    // Verify origin then connect
    else if (nextCell->x-1 >= 0 && adjacentCellIsCompatible(x-1,y, currentCell->getColor()))
    {
        int color = grid->getGrid()[x][y].getColor();

        connectToLoneSecondOrigin(x-1,y,color,nextCell);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x-1][y];

        solveRec(grid->getCellPtr(x-1,y), isRandom, clockTurn);
    }
    else if (nextCell->y+1 < grid->getNbRow() && adjacentCellIsCompatible(x,y+1,currentCell->getColor()))
    {
        int color = grid->getGrid()[x][y].getColor();

        connectToLoneSecondOrigin(x,y+1,color,nextCell);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y+1];

        solveRec(grid->getCellPtr(x,y+1), isRandom,clockTurn);
    }
    else if (nextCell->x+1 < grid->getNbColumn() && adjacentCellIsCompatible(x+1,y,currentCell->getColor()))
    {
        int color = grid->getGrid()[x][y].getColor();

        connectToLoneSecondOrigin(x+1,y,color,nextCell);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x+1][y];

        solveRec(grid->getCellPtr(x+1,y), isRandom,clockTurn);
    }
    else if (nextCell->y-1 >= 0 && adjacentCellIsCompatible(x,y-1,currentCell->getColor()))
    {
        int color = grid->getGrid()[x][y].getColor();

        connectToLoneSecondOrigin(x,y-1,color,nextCell);
        grid->getGrid()[x][y].next[0] = &grid->getGrid()[x][y-1];

        solveRec(grid->getCellPtr(x,y-1), isRandom,clockTurn);
    }
    else {
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
