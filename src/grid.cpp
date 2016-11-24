#include "grid.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

Grid::Grid()
{
    this->row = 0;
    this->column = 0;
}

bool Grid::fileExists(const QString& path) {

    QFileInfo checkFile(path);

    // check if file exists. If yes then is it really a file and not a directory
    return (checkFile.exists() && checkFile.isFile());
}

Grid::Grid(const QString& filePath)
{
    try
    {
        if (fileExists(filePath))
        {
            QJsonObject json = initializeJsonObject(filePath);

            readGridFormat(json);

            initializeGameGrid();

            analyzeFile(json);
        }
        else
        {
            std::cout << "The file doesn't exist\n";
        }

    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }

}

QJsonObject Grid::initializeJsonObject(const QString& filePath)
{
    QFile file(filePath);
    file.open(QIODevice::ReadOnly);

    QByteArray rawData = file.readAll();

    // Parse document
    QJsonDocument doc(QJsonDocument::fromJson(rawData));

    // Get JSON object
    QJsonObject json = doc.object();

    return json;
}

void Grid::readGridFormat(const QJsonObject& json)
{
    // Access properties
    this->row = json["row"].toInt();
    this->column = json["column"].toInt();
}

void Grid::initializeGameGrid()
{
    gameGrid = new Point*[row];

    for (int i = 0; i < row; ++i)
    {
        gameGrid[i] = new Point[column];
    }
}

void Grid::analyzeFile(const QJsonObject& json)
{
    QJsonArray jsonArray = json["level"].toArray();

    std::vector<int> pointList, nextList, prevList;

    foreach (const QJsonValue& item, jsonArray)
    {
        foreach (const QJsonValue& i, item.toArray())
        {
            // values of the point (x,y,color)
            if (!i.isObject()){
                pointList.push_back(i.toInt());
            }
            // settings of the point
            else
            {
                QJsonArray settingsArray = i.toObject()["settings"].toArray();

                foreach (const QJsonValue& settingsItem, settingsArray)
                {
                    readJsonValues(settingsItem, nextList, prevList);
                }

            }
        }

        readPointValues(pointList);

        // it is a standard level
        if (json["standard"].toBool())
        {
            gameGrid[x][y].setData(color);
        }
        else
        {
            nOk = hasNextValues(nextList);
            pOk = hasPreviousValues(prevList);

            setValues();
        }
    }
}

void Grid::readJsonValues(const QJsonValue& settingsItem,
                          std::vector<int>& nextList, std::vector<int>& prevList)
{
    QString name = settingsItem.toObject().value("name").toString();

    if (name == "origin")
    {
        origin = settingsItem.toObject().value("value").toBool();
    }
    if (name == "first origin")
    {
        firstOrigin = settingsItem.toObject().value("value").toBool();
    }
    else if (name == "second origin")
    {
        secondOrigin = settingsItem.toObject().value("value").toBool();
    }
    else if (name == "path complete")
    {
        pathComplete = settingsItem.toObject().value("value").toBool();
    }
    else if (name == "next")
    {
        addValues(nextList, settingsItem);
    }
    else if (name == "previous")
    {
        addValues(prevList, settingsItem);
    }
}

void Grid::addValues(std::vector<int>& list, const QJsonValue& settingsItem)
{
    QJsonArray value = settingsItem.toObject().value("value").toArray();
    for (int i = 0; i < value.size(); ++i)
    {
        list.push_back(value[i].toInt());
    }
}

void Grid::readPointValues(std::vector<int>& list)
{
    color = list.back();
    list.pop_back();
    y = list.back();
    list.pop_back();
    x = list.back();
    list.pop_back();
}

bool Grid::hasNextValues(std::vector<int>& list)
{
    if (!list.empty())
    {
        nextColor = list.back();
        list.pop_back();
        nextY = list.back();
        list.pop_back();
        nextX = list.back();
        list.pop_back();
        return true;
    }
    return false;
}

bool Grid::hasPreviousValues(std::vector<int>& list)
{
    if (!list.empty())
    {
        previousColor = list.back();
        list.pop_back();
        previousY = list.back();
        list.pop_back();
        previousX = list.back();
        list.pop_back();
        return true;
    }
    return false;
}

void Grid::setValues()
{
    if (firstOrigin)
    {
        if (nOk || origin)
        {
            gameGrid[x][y].next[0] = &gameGrid[nextX][nextY];
            gameGrid[x][y].setData(color);
            gameGrid[x][y].setFirstOrigin(true);
            gameGrid[x][y].setCoveredFlag(true);
        }

    }
    else if (secondOrigin)
    {
        if (pOk || origin)
        {
            gameGrid[x][y].previous[0] = &gameGrid[previousX][previousY];
            gameGrid[x][y].setData(color);
            gameGrid[x][y].setSecondOrigin(true);
            gameGrid[x][y].setCoveredFlag(true);
            gameGrid[x][y].setPathComplete(true);
        }
    }
    // it is a point
    else
    {
        // there is a next point to the current grid point
        if (nOk)
        {
            gameGrid[x][y].next[0] = &gameGrid[nextX][nextY];
            gameGrid[x][y].setColor(color);
            gameGrid[x][y].setCoveredFlag(true);
        }
        // there is a previous point to the current grid point
        if (pOk)
        {
            gameGrid[x][y].previous[0] = &gameGrid[previousX][previousY];
            gameGrid[x][y].setColor(color);
            gameGrid[x][y].setCoveredFlag(true);
        }
        // if the point is an origin
        if (origin)
        {
            gameGrid[x][y].setData(color);
        }
    }
}

Grid::Grid(int nbRow, int nbCol)
{
    this->row = nbRow;
    this->column = nbCol;

    gameGrid = new Point*[nbRow];

    for (int i = 0; i < nbRow; ++i)
    {
        gameGrid[i] = new Point[nbCol];
    }
}

Grid::~Grid()
{
    for (int i = 0; i < this->row; ++i)
    {
        delete [] gameGrid[i];
    }
    delete [] gameGrid;
}

Point Grid::getPath(int i, int j)
{
    return gameGrid[i][j];
}

void Grid::setPath(int i, int j, Point path)
{
    gameGrid[i][j] = path;
}

void Grid::setPosition(int i, int j)
{
    gameGrid[i][j].x = i;
    gameGrid[i][j].y = j;
}

int Grid::getNbRow()
{
    return row;
}

int Grid::getNbColumn()
{
    return column;
}

//Finds if the game is won
bool Grid::isCompleted()
{
    int completedPaths = 0, totalPaths = 0;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            Point path = gameGrid[i][j];
            if(!path.isCovered() && !path.isOrigin())
            {
                return false;
            }
            else if(path.isOrigin())
            {
                totalPaths++;
                completedPaths += path.getPathComplete() ? 1 : 0;
            }
        }
    }

    return completedPaths == totalPaths/2;
}
