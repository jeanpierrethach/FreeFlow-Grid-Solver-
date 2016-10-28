#include "path.h"

Path::Path()
{

}

// Path sans direction
Path::Path(int color)
{
    fixed = false;
    this->color = color;
}

// Path borne
Path::Path(int color, bool fixed)
{
    this->fixed = fixed;
    this->color = color;
}

// Path avec 2 directions
Path::Path(int color, Point edge1, Point edge2)
{
    Path(color, edge1);
    edgePoint2 = edge2;

}

// Path avec une direction
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

const bool Path::getFixed()
{
    return fixed;
}

const int Path::getColor()
{
    return color;
}
