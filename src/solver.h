#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"

class Solver
{
public:

    void solve(Grid** grid);

private:
    Grid* grid;

    void pressClearPathCase(int, int);

    void clearPathColorCase(Cell*);
    void reversePath(Cell* cell);
    void solveRec(Cell*, bool, bool);
    bool cellIsPartOfHimself(Cell* current, Cell* target);

    bool adjacentCellIsCompatible(int, int, int);
    bool mandatoryMove();

    void confirmSecondOriginFrom(int, int);
    void connectToLoneSecondOrigin(int x, int y, int color, Cell* nextCell);
    void reversePathFromSecondOrigin(int);
    void solveFromEdgeOfOrigin(int);
    void solveRandomly(const std::vector<bool>&);
    void fillGrid();
    void matchEmptyCaseWithPath(Cell** adjacentCell, Cell** adjacentCell2, bool* matching, Cell** bottomBlank, Cell** topBlank);


};

#endif // SOLVER_H
