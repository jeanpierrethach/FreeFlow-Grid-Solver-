#ifndef GRIDBUILDER_H
#define GRIDBUILDER_H

#include "grid.h"

//Used for random number generation
#include <cstdlib>
#include <iostream>
#include <ctime>

#include <QPoint>
#include <QDebug>

class GridBuilder
{
public:
    static Grid* buildGrid(int row, int col);
    GridBuilder();

private:

    static Grid* buildGrid(Grid*, int, int);

    static void destroyAdjacentPathMatrix(int**, int);

    static QPoint* getRandomFreeBlock(Grid*);

    static int getRandomNumberFrom0To(int max);

    static QPoint* findXAdjacentFreeBlocks(Grid*, int x, int** adjacentPathMatrix);
    static QPoint* findLoneBlock(Grid*, int** adjacentPathMatrix);
    static QPoint* findDeadEnd(Grid*, int** adjacentPathMatrix);
    static void buildPathStartingPoint(QPoint*, Grid*);
    static void buildPath(QPoint*, Grid*);
    static int numberOfFreeAdjacentPositions(QPoint*, Grid*);

    static QPoint* getRandomAdjacentFreeBlock(QPoint* pos, Grid* grid);

    static bool moveAdjacentEndPoint(QPoint* pos, Grid* grid);

    static QPoint* findPairOfLoneBlocks(Grid*, int** adjacentPathMatrix);

    static void fillAdjacentPathMatrix(Grid*, int** adjacentPathMatrix);
};

#endif // GRIDBUILDER_H
