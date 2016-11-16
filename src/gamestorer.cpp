#include "gamestorer.h"


#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QString>


GameStorer::GameStorer()
{

}

void GameStorer::saveFile(Grid*& grid, const QString& fileName, const bool& result)
{
    QDir().mkdir("save");

    QJsonObject jsonObject;

    insertValues(grid, jsonObject);

    QJsonDocument doc(jsonObject);

    QString jsonString = doc.toJson(QJsonDocument::Indented);

    QMessageBox msg;

    if (fileName.contains(QRegExp("[^a-zA-Z\\d\\s]")))
    {
        qDebug() << "The file contains special characters.";
        msg.setText("Invalid name. No special characters allowed.");
    }
    else if (result && !fileName.isEmpty())
    {
        QFile saveFile("save/" + fileName.trimmed() + ".json");
        if(!saveFile.open(QIODevice::WriteOnly)){
            qDebug() << "Failed to open save file";
            msg.setText("Your game hasn't been saved.");
            exit(-1);
        }
        else
        {
            saveFile.write(jsonString.toLocal8Bit());

            msg.setText("Your game has been successfully saved.");
        }
    }
    else
    {
        msg.setText("Your game hasn't been saved.");
    }
    msg.exec();
}

void GameStorer::insertValues(Grid*& grid, QJsonObject& jsonObject)
{
    jsonObject["row"] = grid->getNbRow();
    jsonObject["column"] = grid->getNbColumn();

    QJsonArray array;
    QJsonArray nestedArray;

    for (int i = 0; i < grid->getNbRow(); ++i)
    {
      for (int j = 0; j < grid->getNbColumn(); ++j)
      {

          QJsonObject settings;

          QJsonArray settingsArray;

          addSettings("origin", grid->getGrid()[i][j].isOrigin(), settingsArray);
          addSettings("first origin", grid->getGrid()[i][j].getFirstOrigin(), settingsArray);
          addSettings("second origin", grid->getGrid()[i][j].getSecondOrigin(), settingsArray);
          addSettings("path complete", grid->getGrid()[i][j].getPathComplete(), settingsArray);
          addNextSettings("next", grid, i, j, settingsArray);
          addPreviousSettings("previous", grid, i, j, settingsArray);

          settings["settings"] = settingsArray;

          array.insert(0, grid->getGrid()[i][j].x);
          array.insert(1, grid->getGrid()[i][j].y);
          array.insert(2, grid->getGrid()[i][j].getColor());
          array.insert(3, settings);

          nestedArray.append(array);

          array.removeLast();
          array.removeLast();
          array.removeLast();
          array.removeLast();
      }

    }
    jsonObject["level"] = nestedArray;
}

void GameStorer::addSettings(const char* nameValue, bool func, QJsonArray& settingsArray)
{
    QJsonObject setting;
    setting["name"] = nameValue;
    setting["value"] = func;
    settingsArray.append(setting);
}

void GameStorer::addNextSettings(const char* nameValue, Grid*& grid, int i, int j, QJsonArray& settingsArray)
{
    QJsonObject settingsNext;
    QJsonArray nextValues;

    settingsNext["name"] = nameValue;

    if (grid->getGrid()[i][j].next[0] != 0)
    {
        nextValues.append(grid->getGrid()[i][j].next[0]->x);
        nextValues.append(grid->getGrid()[i][j].next[0]->y);
        nextValues.append(grid->getGrid()[i][j].next[0]->getColor());
    }

    settingsNext["value"] = nextValues;
    settingsArray.append(settingsNext);
}

void GameStorer::addPreviousSettings(const char* nameValue, Grid*& grid, int i, int j, QJsonArray& settingsArray)
{
    QJsonObject settingsPrevious;
    QJsonArray previousValues;

    settingsPrevious["name"] = nameValue;

    if (grid->getGrid()[i][j].previous[0] != 0)
    {
        previousValues.append(grid->getGrid()[i][j].previous[0]->x);
        previousValues.append(grid->getGrid()[i][j].previous[0]->y);
        previousValues.append(grid->getGrid()[i][j].previous[0]->getColor());
    }

    settingsPrevious["value"] = previousValues;
    settingsArray.append(settingsPrevious);
}

bool GameStorer::loadFile(Grid*& grid, const QFileInfo& selectedFile)
{
    QMessageBox msg;

    QDir().mkdir("save");

    QString fileName = selectedFile.baseName();

    QString filePath = ("save/" + fileName + ".json");

    if (!fileName.isEmpty())
    {
        QFile loadFile(filePath);
        if(!loadFile.exists())
        {
            qDebug() << "File doesn't exist";
            msg.setText("The file isn't in the correct repository.");
        }
        else if(!loadFile.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to load file";
            msg.setText("Your game hasn't been loaded.");
        }
        else
        {
            grid = new Grid(filePath);
            msg.setText("Your game has been successfully loaded.");
            msg.exec();
            return true;
        }
    }
    else
    {
        msg.setText("Your game hasn't been loaded.");
    }
    msg.exec();
    return false;
}
