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

}

// Path
Path::Path(int color)
{
    this->color = color;
}

// Path that is an origin
Path::Path(int color, bool origin)
{
    this->origin = origin;
    this->color = color;
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
