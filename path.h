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
    ~Path();
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
    bool getPathComplete() { return this->pathComplete; }
    bool getFirstOrigin() { return this->firstOrigin; }
    bool getSecondOrigin() { return this->secondOrigin; }

    void setColor(int color) { this->color = color; }
    void setData(int color) { this->color = color; this->origin = true; }
    void setOrigin(bool b) { this->origin = b; }
    void setFlag(bool f) { this->flag = f; }
    void setCovered(bool c) { this->covered = c; }
    void setPathComplete(bool p) { this->pathComplete = p; }
    void setFirstOrigin(bool f) { this->firstOrigin = f; }
    void setSecondOrigin(bool f) { this->secondOrigin = f; }

    void clear();

    // to be moved to private
    int x,y;
    Path* next[1];
    Path* previous[1];

private:
    Point edgePoint1;
    Point edgePoint2;


    bool firstOrigin;
    bool secondOrigin;
    bool pathComplete;

    bool origin;
    bool covered;
    bool flag;

    bool fixed;
    int color;

};

#endif // PATH_H
