#include "point.h"

Point::Point()
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

Point::~Point()
{

}

// Path
Point::Point(int color)
{
    this->color = color;
}

// Path that is an origin
Point::Point(int color, bool origin)
{
    this->origin = origin;
    this->color = color;
}

const int Point::getColor()
{
    return color;
}

bool Point::isOrigin()
{
    return origin;
}

bool Point::isCovered()
{
    return covered;
}

void Point::clear()
{
    this->color = -1;
    this->origin = false;
    clearOrigin();
}

void Point::clearOrigin()
{
    this->covered = false;
    this->previous[0] = 0;
    this->next[0] = 0;
    this->flag = false;
    this->pathComplete = false;
    this->firstOrigin = false;
    this->secondOrigin = false;
}
