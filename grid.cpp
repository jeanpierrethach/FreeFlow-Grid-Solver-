#include "grid.h"

Grid::Grid(int height, int width)
{
    this->height = height;
    this->width = width;

    gameGrid = new Path*[width];

    for (int i = 0; i < width; ++i)
    {
        gameGrid[i] = new Path[height];
    }
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
