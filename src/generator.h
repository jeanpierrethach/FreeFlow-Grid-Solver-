#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <QColor>
#include <QString>

class Generator
{
public:
    Generator();

    std::vector<QColor> getColorList() {
        return color;
    }

    std::vector<QColor> getBackgroundColorList() {
        return backgroundColor;
    }

    int generateNumber();

private:
    void generateColorPalette(int);
    int generateNumber(int, int);

    std::vector<QColor> color;
    std::vector<QColor> backgroundColor;
};

#endif // GENERATOR_H
