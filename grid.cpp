#include "grid.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>

#include <QByteArray>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>

using boost::property_tree::ptree;


Grid::Grid()
{
    this->row = 0;
    this->column = 0;
}

bool fileExists(QString path) {
    QFileInfo check_file(path);

    // check if file exists. If yes then is it really a file and not a directory
    return (check_file.exists() && check_file.isFile());
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

            std::vector<int> v;
            int color, x, y = 0;

            foreach (const QJsonValue& item, jsonArray)
            {
                foreach (const QJsonValue& i, item.toArray())
                {
                    v.push_back(i.toInt());
                }

                color = v.back();
                v.pop_back();
                y = v.back();
                v.pop_back();
                x = v.back();
                v.pop_back();

                gameGrid[x][y].setData(color);
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
