#include "cell.h"

Cell::Cell()
{
    this->color = -1;
    this->origin = false;
    this->covered = false;
    this->previous[0] = 0;
    this->next[0] = 0;
    this->flag = false;
    this->pathComplete = false;
    this->firstOrigin = false;
    this->secondOrigin = false;
}

Cell::~Cell()
{

}

Cell::Cell(int color)
{
    this->color = color;
}

Cell::Cell(int color, bool origin)
{
    this->origin = origin;
    this->color = color;
}

int Cell::getColor()
{
    return color;
}

bool Cell::isOrigin()
{
    return origin;
}

bool Cell::isCovered()
{
    return covered;
}

void Cell::clear()
{
    this->color = -1;
    this->origin = false;
    clearOrigin();
}

void Cell::clearOrigin()
{
    this->covered = false;
    this->previous[0] = 0;
    this->next[0] = 0;
    this->flag = false;
    this->pathComplete = false;
    this->firstOrigin = false;
    this->secondOrigin = false;
}

void Cell::setCellInformation(int color, bool origin, bool covered, Cell* previous, Cell* next, bool flag, bool pathComplete, bool firstOrigin, bool secondOrigin) {
    this->color = color;
    this->origin = origin;
    this->covered = covered;
    this->previous[0] = previous;
    this->next[0] = next;
    this->flag = flag;
    this->pathComplete = pathComplete;
    this->firstOrigin = firstOrigin;
    this->secondOrigin = secondOrigin;
}
