#include "gridbuilder.h"

int GridBuilder::color = -1;

GridBuilder::GridBuilder()
{

}

Grid* GridBuilder::buildGrid(Grid* grid, int row, int col)
{
    int** adjacentPathMatrix = new int*[row];
    for(int i = 0; i < row; i++)
    {
        adjacentPathMatrix[i] = new int[col];
    }
    fillAdjacentPathMatrix(grid, adjacentPathMatrix);

    while(!grid->isCompleted())
    {
        QPoint* pos;

        //If there is two adjacent free blocks that only connects to eachother do something
        pos = findPairOfLoneBlocks(grid, adjacentPathMatrix);
        if(pos->x() >= 0)
        {
            //TODO TRY TO FILL PAIR
            //try to fill pair of lone blocks, if you cant, restart
            destroyAdjacentPathMatrix(adjacentPathMatrix, row);
            return grid;
        }
        else
        {
            //If there is a standAlone free block, try to move an adjacent endPoint, otherwise impossible, restart from scratch
            pos = findLoneBlock(grid, adjacentPathMatrix);
            if(pos->x() >= 0)
            {
                //Move adjacent endPoint
                //if success keep on
                //else restart
                destroyAdjacentPathMatrix(adjacentPathMatrix, row);
                return grid;
            }
            else
            {
                //If there is a dead end, make it the start of a path
                pos = findDeadEnd(grid, adjacentPathMatrix);
                if(pos->x() >= 0)
                {
                    buildPathStartingPoint(pos, grid);
                }
                else
                {
                    //Start random path from here
                    QPoint* point = getRandomFreeBlock(grid);
                    if(point->x() < 0) return grid;
                    buildPathStartingPoint(point, grid);
                }
            }
        }
    }

    destroyAdjacentPathMatrix(adjacentPathMatrix, row);

    return grid;
}

Grid* GridBuilder::buildGrid(int row, int col)
{
    std::srand(std::time(0));
    Grid* grid = new Grid(row, col);

    while(!grid->isCompleted())
    {
        color = -1;
        delete grid;
        grid = new Grid(row, col);
        buildGrid(grid, row, col);
    }

    clearPaths(grid);

    return grid;
}

void GridBuilder::clearPaths(Grid* grid)
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
}

void GridBuilder::buildPathStartingPoint(QPoint* startingPos, Grid* grid)
{
    color++;
    //Path path = grid->getPath(startingPos->x(), startingPos->y());

    grid->getGrid()[startingPos->x()][startingPos->y()].setCovered(true);
    //path.setCovered(true);

    grid->getGrid()[startingPos->x()][startingPos->y()].setColor(color);

    grid->getGrid()[startingPos->x()][startingPos->y()].setOrigin(true);
    //path.setOrigin(true);


    grid->getGrid()[startingPos->x()][startingPos->y()].setFirstOrigin(true);
    //path.setFirstOrigin(true);

    buildPath(startingPos, grid);
}

//TODO build Path starting at startingPos
void GridBuilder::buildPath(QPoint* startingPos, Grid* grid)
{
    int possibleMoves = numberOfFreeAdjacentPositions(startingPos, grid);
    //Path path = grid->getPath(startingPos->x(), startingPos->y());

    //path.setCovered(true);
    grid->getGrid()[startingPos->x()][startingPos->y()].setCovered(true);

    //setting color
    grid->getGrid()[startingPos->x()][startingPos->y()].setColor(color);

    if(possibleMoves == 0)
    {
        //path.setOrigin(true);
        grid->getGrid()[startingPos->x()][startingPos->y()].setOrigin(true);
        //path.setPathComplete(true);
        grid->getGrid()[startingPos->x()][startingPos->y()].setPathComplete(true);
    }
    else if(possibleMoves == 1)
    {
        QPoint* nextPathPos = getRandomAdjacentFreeBlock(startingPos, grid);
        Cell nextPath = grid->getCell(nextPathPos->x(), nextPathPos->y());
        //nextPath.setCovered(true);
        grid->getGrid()[nextPathPos->x()][nextPathPos->y()].setCovered(true);
        //nextPath.previous[0] = &path;
        grid->getGrid()[nextPathPos->x()][nextPathPos->y()].previous[0] = &(grid->getGrid()[startingPos->x()][startingPos->y()]);

        grid->getGrid()[nextPathPos->x()][nextPathPos->y()].setColor(true);

        //grid->getPath(startingPos->x(), startingPos->y()).next[0] = &nextPath;
        grid->getGrid()[startingPos->x()][startingPos->y()].next[0] = &(grid->getGrid()[nextPathPos->x()][nextPathPos->y()]);

        buildPath(nextPathPos, grid);
    }
    else
    {

        if(grid->getGrid()[startingPos->x()][startingPos->y()].isOrigin() || getRandomNumberFrom0To(grid->getNbColumn() / 2) != 1)
        {

            QPoint* nextPathPos = getRandomAdjacentFreeBlock(startingPos, grid);
            //Path nextPath = grid->getPath(nextPathPos->x(), nextPathPos->y());

            //nextPath.setCovered(true);
            grid->getGrid()[nextPathPos->x()][nextPathPos->y()].setCovered(true);

            //nextPath.previous[0] = &path;
            grid->getGrid()[nextPathPos->x()][nextPathPos->y()].previous[0] = &(grid->getGrid()[startingPos->x()][startingPos->y()]);

            grid->getGrid()[startingPos->x()][startingPos->y()].next[0] = &(grid->getGrid()[nextPathPos->x()][nextPathPos->y()]);
            //grid->getPath(startingPos->x(), startingPos->y()).next[0] = &nextPath;

            grid->getGrid()[nextPathPos->x()][nextPathPos->y()].setColor(color);

            buildPath(nextPathPos, grid);
        }
        else
        {
            grid->getGrid()[startingPos->x()][startingPos->y()].setOrigin(true);
            grid->getGrid()[startingPos->x()][startingPos->y()].setPathComplete(true);
            grid->getGrid()[startingPos->x()][startingPos->y()].setColor(color);
            //path.setOrigin(true);
            //path.setPathComplete(true);
        }
    }

}

QPoint* GridBuilder::findDeadEnd(Grid* grid, int** adjacentPathMatrix)
{
    return findXAdjacentFreeBlocks(grid, 1, adjacentPathMatrix);
}

QPoint*  GridBuilder::findLoneBlock(Grid* grid, int** adjacentPathMatrix)
{
    return findXAdjacentFreeBlocks(grid, 0, adjacentPathMatrix);
}

int GridBuilder::numberOfFreeAdjacentPositions(QPoint* pos, Grid* grid)
{
    int count = 0;

    //up
    if(pos->x() - 1 >= 0 && !grid->getCell(pos->x() - 1, pos->y()).isCovered()) count++;
    //down
    if(pos->x() + 1 <= grid->getNbRow() - 1 && !grid->getCell(pos->x() + 1, pos->y()).isCovered()) count++;
    //left
    if(pos->y() - 1 >= 0 && !grid->getCell(pos->x(), pos->y() - 1).isCovered()) count++;
    //right
    if(pos->y() + 1 <= grid->getNbColumn() - 1 && !grid->getCell(pos->x(), pos->y() + 1).isCovered()) count++;

    return count;
}

QPoint*  GridBuilder::findXAdjacentFreeBlocks(Grid* grid, int x, int** adjacentPathMatrix)
{
    fillAdjacentPathMatrix(grid, adjacentPathMatrix);
    int row = grid->getNbRow();
    int col = grid->getNbColumn();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(adjacentPathMatrix[i][j] == x && !grid->getCell(i,j).isCovered())
            {
                return new QPoint(i,j);
            }
        }
    }
    return new QPoint(-1,-1);
}

void GridBuilder::fillAdjacentPathMatrix(Grid* grid, int** adjacentPathMatrix)
{
    for(int i = 0; i < grid->getNbRow(); i++)
    {
        for(int j = 0; j < grid->getNbColumn(); j++)
        {
            QPoint* point = new QPoint(i,j);
            adjacentPathMatrix[i][j] = numberOfFreeAdjacentPositions(point, grid);

        }
    }
}

QPoint* GridBuilder::findPairOfLoneBlocks(Grid* grid, int** adjacentPathMatrix)
{
    for(int i = 0; i < grid->getNbRow(); i++)
    {
        for(int j = 0; j < grid->getNbColumn(); j++)
        {
            if(adjacentPathMatrix[i][j] == 1 && !grid->getCell(i,j).isCovered())
            {
                //if there is another single one to the right or under we have a pair
                if((i + 1 < grid->getNbRow() && adjacentPathMatrix[i+1][j] == 1 && !grid->getCell(i+1,j).isCovered())
                        || (j + 1 < grid->getNbColumn() && adjacentPathMatrix[i][j+1] == 1 && !grid->getCell(i,j+1).isCovered()))
                {
                    return new QPoint(i,j);
                }
            }
        }
    }
    return new QPoint(-1,-1);
}

void GridBuilder::destroyAdjacentPathMatrix(int **adjacentPathMatrix, int row)
{
    for(int i = 0; i < row; ++i){
        delete[] adjacentPathMatrix[i];
    }

    delete[] adjacentPathMatrix;
}

QPoint* GridBuilder::getRandomFreeBlock(Grid* grid)
{
    int nbOfFreeBlocks = 0;
    for(int i = 0; i < grid->getNbRow(); i++)
    {
        for(int j = 0; j < grid->getNbColumn(); j++)
        {
            if(!grid->getCell(i,j).isCovered()) nbOfFreeBlocks++;
        }
    }

    if(nbOfFreeBlocks == 0) nbOfFreeBlocks++;

    int pos = getRandomNumberFrom0To(nbOfFreeBlocks);

    for(int i = 0; i < grid->getNbRow(); i++)
    {
        for(int j = 0; j < grid->getNbColumn(); j++)
        {
            if(!grid->getCell(i,j).isCovered())
            {
                pos--;
                if(pos == 0)
                {
                    return new QPoint(i,j);
                }
            }
        }
    }

    return new QPoint(-1,-1);
}

int GridBuilder::getRandomNumberFrom0To(int max)
{
     // use current time as seed for random generator
    return std::rand() % max;
}

QPoint* GridBuilder::getRandomAdjacentFreeBlock(QPoint* pos, Grid* grid)
{
    QPoint qp = QPoint(pos->x(), pos->y());
    int num = numberOfFreeAdjacentPositions(&qp, grid);
    int direction = getRandomNumberFrom0To(num);

    //up
    if(pos->x() - 1 >= 0 && !grid->getCell(pos->x() - 1, pos->y()).isCovered())
    {
        if(direction == 0) return new QPoint(pos->x() - 1, pos->y());
        --direction;
    }

    //down
    if(pos->x() + 1 <= grid->getNbRow() - 1 && !grid->getCell(pos->x() + 1, pos->y()).isCovered())
    {
        if(direction == 0) return new QPoint(pos->x() + 1, pos->y());
        --direction;
    }
    //left
    if(pos->y() - 1 >= 0 && !grid->getCell(pos->x(), pos->y() - 1).isCovered())
    {
        if(direction == 0) return new QPoint(pos->x(), pos->y() - 1);
        --direction;
    }
    //right
    if(pos->y() + 1 <= grid->getNbColumn() - 1 && !grid->getCell(pos->x(), pos->y() + 1).isCovered())
    {
        if(direction == 0) return new QPoint(pos->x(), pos->y() + 1);
        --direction;
    }

    //SHOULD NEVER HIT THIS
    qDebug() << "ERROR";
    return new QPoint(-1,-1);
}

bool GridBuilder::moveAdjacentEndPoint(QPoint* pos, Grid* grid)
{
    Cell adjacentOrigin;
    //up
    if(pos->x() - 1 >= 0 && grid->getCell(pos->x() - 1, pos->y()).isOrigin())
    {
        adjacentOrigin = grid->getCell(pos->x()-1, pos->y());
    }
    //down
    else if(pos->x() + 1 <= grid->getNbRow() - 1 && grid->getCell(pos->x() + 1, pos->y()).isOrigin())
    {
        adjacentOrigin = grid->getCell(pos->x() + 1, pos->y());
    }
    //left
    else if(pos->y() - 1 >= 0 && grid->getCell(pos->x(), pos->y() - 1).isOrigin())
    {
        adjacentOrigin = grid->getCell(pos->x(), pos->y() - 1);
    }
    //right
    else if(pos->y() + 1 <= grid->getNbColumn() - 1 && grid->getCell(pos->x(), pos->y() + 1).isOrigin())
    {
        adjacentOrigin = grid->getCell(pos->x(), pos->y() + 1);
    }
    else
    {
        return false;
    }

    Cell point = grid->getCell(pos->x(), pos->y());
    point.setOrigin(true);
    adjacentOrigin.setOrigin(false);
    adjacentOrigin.setPathComplete(false);
    if(adjacentOrigin.next == NULL)
    {
        point.setPathComplete(true);
        point.previous[0] = &adjacentOrigin;
        adjacentOrigin.next[0] = &point;
    }
    else
    {
        adjacentOrigin.previous[0] = &point;
        point.next[0] = &adjacentOrigin;
    }

    return true;
}
