#include "showlevel.h"

ShowLevel::ShowLevel()
{

}

void ShowLevel::addBoutons(int x, int y, QWidget &parent)
{
    for(int i = 0; i < x; i++) {
        for(int j = 0; j < y; j++) {
            QPushButton *btn = new QPushButton ("test", &parent);
            btn->setGeometry(i*100 + 5, j*100 + 5, 95, 95);
        }
    }
}
