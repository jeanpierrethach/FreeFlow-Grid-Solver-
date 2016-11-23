#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <QColor>
#include <QString>

class Generator
{
public:
    Generator();

    std::vector<QColor> getColorList(QString);
    int generateNumber();

private:
    void generateColorPalette(int);
    int generateNumber(int, int);

    std::vector<QColor> color;
    std::vector<QColor> currentColor;
};

#endif // GENERATOR_H
