#ifndef GRID_H
#define GRID_H

#include "path.h"

class Grid
{
public:
    Grid(int, int);
    ~Grid();

    Path getElement();

    Path getPath(int, int);
    Path** getGrid(){ return gameGrid; }

    void setPath(int, int, Path);

    int getWidth();
    int getHeight();


private:
    int width;
    int height;


    Path** gameGrid;


};

#endif // GRID_H
