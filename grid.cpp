#include "grid.h"

Grid::Grid(int height, int width)
{
    this->width = width;
    this->height = height;

    gameGrid = new Path*[width];

    for (int i = 0; i < width; ++i)
    {
        gameGrid[i] = new Path[height];
    }
}

Grid::~Grid()
{
    for (int i = 0; i < this->width; ++i)
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

int Grid::getWidth()
{
    return width;
}

int Grid::getHeight()
{
    return height;
}
