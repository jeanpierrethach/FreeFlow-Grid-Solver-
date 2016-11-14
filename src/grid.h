#ifndef GRID_H
#define GRID_H

#include "path.h"
#include <string>
#include <QFileInfo>
#include <QByteArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

using std::string;

class Grid
{
public:
    Grid();
    Grid(const QString&);
    Grid(int, int);
    ~Grid();

    Path getElement();

    Path getPath(int, int);
    Path** getGrid(){ return gameGrid; }

    void setPosition(int, int);
    void setPath(int, int, Path);
    bool isCompleted();

    int getNbRow();
    int getNbColumn();


private:
    int row;
    int column;

    Path** gameGrid;


};

#endif // GRID_H
