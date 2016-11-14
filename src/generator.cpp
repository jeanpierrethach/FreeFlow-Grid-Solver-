#include "generator.h"

#include <random>

Generator::Generator()
{
    generateColorPalette(50);
}

std::vector<QColor> Generator::getColorList(QString listName)
{
    std::vector<QColor> list;
    if (listName == "color")
    {
        list = color;
    }
    else if (listName == "currentColor")
    {
        list = currentColor;
    }

    return list;
}

void Generator::generateColorPalette(int nbColors)
{
    // hue [0, 360), saturation [0, 255),
    // lightness [0, 255), alpha-transparency [0, 255]

    for(int i = 0; i < 360; i += 360 / nbColors)
    {
        int hue = i;
        int saturation = 90 + rand() % 100;
        int lightness = 60 + rand() % 100;

        QColor colorGen = QColor::fromHsl(hue, saturation, lightness, 255);
        QColor transColorGen = QColor::fromHsl(hue, saturation, lightness, 100);
        color.push_back(colorGen);
        currentColor.push_back(transColorGen);
    }
}

int Generator::generateNumber()
{
    return generateNumber(7,12);
}

int Generator::generateNumber(int min, int max)
{
    // random device that will seed the generator
    std::random_device seeder;

    // mersenne twister engine
    std::mt19937 engine(seeder());

    // uniform discrete distribution
    std::uniform_int_distribution<int> dist(min, max);

    // generate the integer
    return dist(engine);
}
