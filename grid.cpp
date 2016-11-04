#include "grid.h"

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
