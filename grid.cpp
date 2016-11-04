#include "grid.h"
#include <QDebug>

Grid::Grid()
{
    this->row = 0;
    this->column = 0;
}

Grid::Grid(int nbRow, int nbCol)
{
    this->row = nbRow;
    this->column = nbCol;

    gameGrid = new Path*[nbRow];
    
    for (int i = 0; i < nbRow; ++i)
    {
        gameGrid[i] = new Path[nbCol];
    }
}

Grid::~Grid()
{
    for (int i = 0; i < this->row; ++i)
    {
        delete [] gameGrid[i];
    }
    delete [] gameGrid;
}

Path Grid::getPath(int i, int j)
{
    return gameGrid[i][j];
}

void Grid::setPath(int i, int j, Path path)
{
    gameGrid[i][j] = path;
}

int Grid::getNbRow()
{
    return row;
}

int Grid::getNbColumn()
{
    return column;
}

//Finds if the game is won
bool Grid::isCompleted()
{
    int completedPaths = 0;
    int totalPaths = 0;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            if(!gameGrid[i][j].isCovered())
            {
                qDebug() << "Position " << i << " " << j;
            }
        }
    }

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            Path path = gameGrid[i][j];
            if(!path.isCovered() && !path.isOrigin())
            {
                return false;
            }
            else if(path.isOrigin())
            {
                totalPaths++;
                completedPaths += path.getPathComplete() ? 1 : 0;
            }
        }
    }
    qDebug() << "CompletedPaths = " << completedPaths << "\nTotalPaths = " << totalPaths;
    return completedPaths == totalPaths/2;
}
