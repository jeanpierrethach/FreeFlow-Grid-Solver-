#ifndef GRIDBUILDER_H
#define GRIDBUILDER_H

#include "grid.h"

#include <QPoint>

class GridBuilder
{
public:
    static Grid buildGrid(int row, int col);
    GridBuilder();

private:
    static int** adjacentPathMatrix;


    static QPoint findXAdjacentFreeBlocks(Grid, int x);
    static QPoint findLoneBlock(Grid);
    static QPoint findDeadEnd(Grid);
    static void buildPathFromPoint(QPoint, Grid);
    static int numberOfFreeAdjacentPositions(QPoint, Grid);

    static QPoint findPairOfLoneBlocks(Grid);

    static void fillAdjacentPathMatrix(Grid);
};

#endif // GRIDBUILDER_H
