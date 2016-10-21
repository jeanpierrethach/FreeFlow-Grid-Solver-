#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include "string.h"
#include "showlevel.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget window;
    window.setFixedSize(780,780);

    ShowLevel *sl = new ShowLevel();

    sl->addBoutons(12, 6, window);

    window.show();

    return a.exec();
}
