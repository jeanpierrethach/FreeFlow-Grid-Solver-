#include "generator.h"

#include <random>

Generator::Generator()
{
    generateColorPalette(50);
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
        backgroundColor.push_back(transColorGen);
    }
}

int Generator::generateNumber()
{
    return generateNumber(7,12);
}

int Generator::generateNumber(int min, int max)
{
    // random number generator from Stroustrup:
    // http://www.stroustrup.com/C++11FAQ.html#std-random
    static std::default_random_engine re {};

    using Dist = std::uniform_int_distribution<int>;

    static Dist uid {};

    return uid(re, Dist::param_type{min,max});

}
