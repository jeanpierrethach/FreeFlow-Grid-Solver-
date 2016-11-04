#ifndef GRID_H
#define GRID_H

#include "path.h"

class Grid
{
public:
    Grid();
    Grid(int, int);
    ~Grid();

    Path getElement();

    Path getPath(int, int);
    Path** getGrid(){ return gameGrid; }

    void setPath(int, int, Path);

    int getNbRow();
    int getNbColumn();


private:
    int row;
    int column;


    Path** gameGrid;


};

#endif // GRID_H
