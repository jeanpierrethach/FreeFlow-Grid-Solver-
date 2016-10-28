#ifndef PATH_H
#define PATH_H

struct Point {
    int x;
    int y;
};

class Path
{
public:
    Path();
    Path(int);
    Path(int, bool);
    Path(int, Point, Point);
    Path(int, Point);

    const Point getEdgePoint1();
    const Point getEdgePoint2();
    const bool getFixed();
    const int getColor();

private:
    Point edgePoint1;
    Point edgePoint2;

    bool fixed;
    int color;
    //Path p;

};

#endif // PATH_H
