#ifndef GAMESTORER_H
#define GAMESTORER_H

#include <QString>
#include "grid.h"

class GameStorer
{
public:
    GameStorer();

    void saveFile(Grid*&, const QString&, const bool&);
    bool loadFile(Grid*&, const QFileInfo&);

private:

    void insertValues(Grid*&, QJsonObject&);
    void addSettings(const char*, bool, QJsonArray&);
    void addNextSettings(const char*, Grid*&, int i, int j, QJsonArray&);
    void addPreviousSettings(const char*, Grid*&, int i, int j, QJsonArray&);

};

#endif // GAMESTORER_H
