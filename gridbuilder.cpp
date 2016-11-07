#include "gridbuilder.h"

int** GridBuilder::adjacentPathMatrix;

GridBuilder::GridBuilder()
{

}

Grid GridBuilder::buildGrid(int row, int col)
{
    Grid* grid = new Grid(row, col);

    while(!grid->isCompleted())
    {
        QPoint pos = findLoneBlock(*grid);

        //If there is a standAlone free block, try to move an adjacent endPoint, otherwise impossible, restart from scratch
        if(pos.x() >= 0)
        {
            //Move adjacent endPoint
            //if success keep on
            //else restart
        }

        //If there is two adjacent free blocks that only connects to eachother do something
        pos = findPairOfLoneBlocks(*grid);
        if(pos.x >= 0)
        {
            //try to fill pair of lone blocks, if you cant restart
        }

        //If there is a dead end, make it the start of a path
        pos = findDeadEnd(*grid);
        if(pos.x() >= 0)
        {
            buildPathFromPoint(pos, *grid);
        }
    }

    return *grid;
}

//TODO build Path starting at startingPos
void GridBuilder::buildPathFromPoint(QPoint startingPos, Grid grid)
{
    Path p = grid.getPath(startingPos.x(), startingPos.y());
}

QPoint GridBuilder::findDeadEnd(Grid grid)
{
    return findXAdjacentFreeBlocks(grid, 1);
}

QPoint GridBuilder::findLoneBlock(Grid grid)
{
    return findXAdjacentFreeBlocks(grid, 0);
}

int GridBuilder::numberOfFreeAdjacentPositions(QPoint pos, Grid grid)
{
    int count = 0;

    //up
    if(pos.x() - 1 >= 0 && !grid.getPath(pos.x() - 1, pos.y()).isCovered()) count++;
    //down
    if(pos.x() + 1 <= grid.getNbRow() - 1 && !grid.getPath(pos.x() + 1, pos.y()).isCovered()) count++;
    //left
    if(pos.y() - 1 >= 0 && !grid.getPath(pos.x(), pos.y() - 1).isCovered()) count++;
    //right
    if(pos.y() + 1 <= grid.getNbColumn() - 1 && !grid.getPath(pos.x(), pos.y() + 1).isCovered()) count++;

    return count;
}

QPoint GridBuilder::findXAdjacentFreeBlocks(Grid grid, int x)
{
    fillAdjacentPathMatrix(grid);
    int row = grid.getNbRow();
    int col = grid.getNbColumn();
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(adjacentPathMatrix[i][j] == x)
            {
                return QPoint(i,j);
            }
        }
    }
    return QPoint(-1,-1);
}

void GridBuilder::fillAdjacentPathMatrix(Grid grid)
{
    for(int i = 0; i < grid.getNbRow(); i++)
    {
        for(int j = 0; j < grid.getNbColumn(); i++)
        {
            adjacentPathMatrix[i][j] = numberOfFreeAdjacentPositions(QPoint(i,j), grid);
        }
    }
}

QPoint GridBuilder::findPairOfLoneBlocks(Grid grid)
{
    for(int i = 0; i < grid.getNbRow(); i++)
    {
        for(int j = 0; j < grid.getNbColumn(); j++)
        {
            if(adjacentPathMatrix[i][j] == 1)
            {
                //if there is another single one to the right or under we have a pair
                if((i + 1 < grid.getNbRow() && adjacentPathMatrix[i+1][j] == 1)
                        || (j + 1 < grid.getNbColumn() && adjacentPathMatrix[i][j+1] == 1))
                {
                    return QPoint(i,j);
                }
            }
        }
    }
    return QPoint(-1,-1);
}
