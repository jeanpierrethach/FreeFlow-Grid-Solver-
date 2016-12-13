#ifndef GRID_H
#define GRID_H

#include "cell.h"
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

    Cell getElement();

    Cell getCell(int, int);
    Cell* getCellPtr(int, int);
    Cell** getGrid(){ return gameGrid; }

    void setPosition(int, int);
    void setCell(int, int, Cell);
    bool isCompleted();

    int getNbRow();
    int getNbColumn();


private:
    int nbRow;
    int nbColumn;

    int color = 0, x = 0, y = 0;
    int nextColor = 0, nextX = 0, nextY = 0;
    int previousColor = 0, previousX = 0, previousY = 0;

    bool nOk = false, pOk = false, pathComplete = false;
    bool origin = false, firstOrigin = false, secondOrigin = false;

    bool fileExists(const QString&);

    QJsonObject initializeJsonObject(const QString&);
    void readGridFormat(const QJsonObject&);
    void initializeGameGrid();
    void analyzeFile(const QJsonObject&);

    void readJsonValues(const QJsonValue&, std::vector<int>&, std::vector<int>&);

    void addValues(std::vector<int>&, const QJsonValue&);

    void readCellValues(std::vector<int>&);
    bool hasNextValues(std::vector<int>&);
    bool hasPreviousValues(std::vector<int>&);

    void setValues();

    Cell** gameGrid;


};

#endif // GRID_H
