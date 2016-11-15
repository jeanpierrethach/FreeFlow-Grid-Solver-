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
#include <vector>

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

    int color = 0, x = 0, y = 0;
    int nextColor = 0, nextX = 0, nextY = 0;
    int previousColor = 0, previousX = 0, previousY = 0;

    bool nOk = false, pOk = false, pathComplete = false;
    bool origin = false, firstOrigin = false, secondOrigin = false;

    bool fileExists(const QString&);

    QJsonObject initializeFileReader(const QString&);
    void readGridFormat(const QJsonObject&);
    void initializeGameGrid();
    void analyzeFile(const QJsonObject&);

    void readPointValues(std::vector<int>&);
    bool readNextValues(std::vector<int>&);
    bool readPreviousValues(std::vector<int>&);

    void setValues();

    Path** gameGrid;


};

#endif // GRID_H
