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
    const bool isFixed();
    const int getColor();
    bool isOrigin();
    bool isCovered();
    bool hasFlag() { return flag; }

    void setColor(int color) { this->color = color; }
    void setOrigin(bool b) { this->origin = b; }
    void setFlag(bool f) { this->flag = f; }
    void setCovered(bool c) { this->covered = c; }

    void clear();

    int x,y;
    Path* next[2];
    Path* previous[2];

private:
    Point edgePoint1;
    Point edgePoint2;


    bool origin;
    bool covered;
    bool flag;

    bool fixed;
    int color;

};

#endif // PATH_H
