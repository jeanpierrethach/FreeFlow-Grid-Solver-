#include "path.h"

Path::Path()
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

Path::~Path()
{
    delete next[0];
    delete previous[0];
}

// Path without direction
Path::Path(int color)
{
    this->color = color;
}

// Path that is an edge
Path::Path(int color, bool origin)
{
    this->origin = origin;
    this->color = color;
}

// Path with two directions
Path::Path(int color, Point edge1, Point edge2)
{
    Path(color, edge1);
    edgePoint2 = edge2;

}

// Path with one direction
Path::Path(int color, Point edge1)
{
    this->color = color;
    edgePoint1 = edge1;
}

const Point Path::getEdgePoint1()
{
    return edgePoint1;
}

const Point Path::getEdgePoint2()
{
    return edgePoint2;
}

const int Path::getColor()
{
    return color;
}

bool Path::isOrigin()
{
    return origin;
}

bool Path::isCovered()
{
    return covered;
}

void Path::clear()
{
    this->color = -1;
    this->origin = false;
    clearOrigin();
}

void Path::clearOrigin()
{
    this->covered = false;
    this->previous[0] = 0;
    this->next[0] = 0;
    this->flag = false;
    this->pathComplete = false;
    this->firstOrigin = false;
    this->secondOrigin = false;
}
