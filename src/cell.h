#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    Cell();
    ~Cell();
    Cell(int);
    Cell(int, bool);

    int getColor();
    bool isOrigin();
    bool isCovered();
    bool hasFlag() { return this->flag; }
    bool getPathComplete() { return this->pathComplete; }
    bool getFirstOrigin() { return this->firstOrigin; }
    bool getSecondOrigin() { return this->secondOrigin; }

    void setColor(int color) { this->color = color; }
    void setData(int color) { this->color = color; this->origin = true; }
    void setOrigin(bool b) { this->origin = b; }
    void setFlag(bool f) { this->flag = f; }
    void setCovered(bool c) { this->covered = c; }
    void setCoveredFlag(bool b) { this->covered = b; this->flag = b; }

    void setPathComplete(bool p) { this->pathComplete = p; }
    void setFirstOrigin(bool f) { this->firstOrigin = f; }
    void setSecondOrigin(bool f) { this->secondOrigin = f; }

    void clear();
    void clearOrigin();

    int x,y;

    Cell* next[1];
    Cell* previous[1];

private:

    bool firstOrigin;
    bool secondOrigin;
    bool pathComplete;

    bool origin;
    bool covered;
    bool flag;

    int color;

};

#endif // PATH_H
