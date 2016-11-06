#include "grid.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <exception>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include <QFileInfo>

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

Grid::Grid(string filePath)
{
    try
    {
        //std::ifstream jsonFile(filePath);

        std::stringstream ss;
        ss << "{\"row\": 5,\"column\": 5,\"level\": [[2, 4, 0],[0, 0, 0],[3, 4, 5],[2, 2, 5],[1, 0, 1],[2, 3, 1],[4, 4, 4],[3, 1, 4]]}";

        std::cout << filePath;
        std::string str = filePath;

        QString qstr = QString::fromStdString(str);
        if (fileExists(qstr))
        {
            std::cout << "\nThe file exists";
        }
        else
        {
            std::cout << "\nThe file doesn't exists";
        }

        ptree root;
        read_json(ss, root);

        this->row = root.get<int>("row");
        this->column = root.get<int>("column");

        gameGrid = new Path*[row];

        for (int i = 0; i < row; ++i)
        {
            gameGrid[i] = new Path[column];
        }

        std::vector<int> v;
        int value, x, y = 0;

        for (ptree::value_type &row : root.get_child("level"))
        {
            for (ptree::value_type &cell : row.second)
            {
                v.push_back(cell.second.get_value<int>());
            }
            value = v.back();
            v.pop_back();
            y = v.back();
            v.pop_back();
            x = v.back();
            v.pop_back();

            gameGrid[x][y].setData(value);
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
    int completedPaths = 0;
    int totalPaths = 0;

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j++)
        {
            if(!gameGrid[i][j].isCovered())
            {
                qDebug() << "Position " << i << " " << j;
            }
        }
    }

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
    qDebug() << "CompletedPaths = " << completedPaths << "\nTotalPaths = " << totalPaths;
    return completedPaths == totalPaths/2;
}
