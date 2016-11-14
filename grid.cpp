#include "grid.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>

Grid::Grid()
{
    this->row = 0;
    this->column = 0;
}

bool fileExists(QString path) {
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

            QFile file(filePath);
            file.open(QIODevice::ReadOnly);

            QByteArray rawData = file.readAll();

            // Parse document
            QJsonDocument doc(QJsonDocument::fromJson(rawData));

            // Get JSON object
            QJsonObject json = doc.object();

            // Access properties
            this->row = json["row"].toInt();
            this->column = json["column"].toInt();

            gameGrid = new Path*[row];

            for (int i = 0; i < row; ++i)
            {
                gameGrid[i] = new Path[column];
            }

            QJsonArray jsonArray = json["level"].toArray();

            std::vector<int> v, n, p;
            int color, x, y = 0;
            bool origin, firstOrigin, secondOrigin = false;
            int nextColor, nextX, nextY = 0;
            int previousColor, previousX, previousY = 0;
            bool nOk, pOk = false;

            foreach (const QJsonValue& item, jsonArray)
            {
                foreach (const QJsonValue& i, item.toArray())
                {
                    if (!i.isObject()){
                        v.push_back(i.toInt());
                    }
                    else {
                        QJsonArray settingsArray = i.toObject()["settings"].toArray();

                        foreach (const QJsonValue& settingsItem, settingsArray)
                        {
                            QString name = settingsItem.toObject().value("name").toString();

                            if (name == "origin")
                            {
                                qDebug() << name;
                                bool value = settingsItem.toObject().value("value").toBool();
                                origin = value;
                                qDebug() << value;
                            }
                            if (name == "first origin")
                            {
                                qDebug() << name;
                                bool value = settingsItem.toObject().value("value").toBool();
                                firstOrigin = value;
                                qDebug() << value;
                            }
                            else if (name == "second origin")
                            {
                                qDebug() << name;
                                bool value = settingsItem.toObject().value("value").toBool();
                                secondOrigin = value;
                                qDebug() << value;
                            }
                            else if (name == "next")
                            {
                                qDebug() << name;
                                QJsonArray value = settingsItem.toObject().value("value").toArray();
                                for (int i = 0; i < value.size(); ++i)
                                {
                                    n.push_back(value[i].toInt());
                                    qDebug() << value[i].toInt();
                                }
                                qDebug() << value;
                            }
                            else if (name == "previous")
                            {
                                qDebug() << name;
                                QJsonArray value = settingsItem.toObject().value("value").toArray();
                                for (int i = 0; i < value.size(); ++i)
                                {
                                    p.push_back(value[i].toInt());
                                    qDebug() << value[i].toInt();
                                }
                                qDebug() << value;
                            }
                        }

                    }
                }

                color = v.back();
                v.pop_back();
                y = v.back();
                v.pop_back();
                x = v.back();
                v.pop_back();

                if (!n.empty())
                {
                    nextColor = n.back();
                    n.pop_back();
                    nextY = n.back();
                    n.pop_back();
                    nextX = n.back();
                    n.pop_back();
                    nOk = true;
                }

                if (!p.empty())
                {
                    previousColor = p.back();
                    p.pop_back();
                    previousY = p.back();
                    p.pop_back();
                    previousX = p.back();
                    p.pop_back();
                    pOk = true;
                }

                if (firstOrigin)
                {
                    if (nOk && origin)
                    {
                        gameGrid[x][y].next[0] = &gameGrid[nextX][nextY];
                        gameGrid[x][y].setData(color);
                        gameGrid[x][y].setFirstOrigin(true);
                        gameGrid[x][y].setCoveredFlag(true);
                    }
                    else
                    {
                        gameGrid[x][y].setData(color);
                    }


                }
                else if (secondOrigin)
                {
                    if (pOk && origin)
                    {
                        gameGrid[x][y].previous[0] = &gameGrid[previousX][previousY];
                        gameGrid[x][y].setData(color);
                        gameGrid[x][y].setSecondOrigin(true);
                        gameGrid[x][y].setCoveredFlag(true);
                    }
                    else
                    {
                        gameGrid[x][y].setData(color);
                    }
                }
                else
                {
                    // it is a standard level
                    if (json["standard"].toBool())
                    {
                        qDebug() << "\n\n\nentered\n\n\n";
                        gameGrid[x][y].setData(color);
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
                nOk = false;
                pOk = false;
            }
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

Grid::Grid(int nbRow, int nbCol)
{
    this->row = nbRow;
    this->column = nbCol;

    gameGrid = new Path*[nbRow];

    for (int i = 0; i < nbRow; ++i)
    {
        gameGrid[i] = new Path[nbCol];
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

Path Grid::getPath(int i, int j)
{
    return gameGrid[i][j];
}

void Grid::setPath(int i, int j, Path path)
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
            Path path = gameGrid[i][j];
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
