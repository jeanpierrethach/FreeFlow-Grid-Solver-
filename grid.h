#ifndef GRID_H
#define GRID_H

#include "path.h"
#include <string>


using std::string;

class Grid
{
public:
    Grid();
    Grid(string);
    Grid(int, int);
    ~Grid();

    Path getElement();

    Path getPath(int, int);
    Path** getGrid(){ return gameGrid; }

    void setPath(int, int, Path);
    bool isCompleted();

    int getNbRow();
    int getNbColumn();


private:
    int row;
    int column;

    Path** gameGrid;


};

#endif // GRID_H
