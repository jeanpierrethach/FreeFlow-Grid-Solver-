#ifndef GRID_H
#define GRID_H

#include "path.h"

class Grid
{
public:
    Grid(int, int);


    Path getElement();

    Path getPath(int, int);

    void setPath(int, int, Path);

    int getHeight();
    int getWidth();

private:

    int height;
    int width;

    Path** gameGrid;


};

#endif // GRID_H
