#include "mainwindow.h"
#include <QApplication>

#include "dialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    Dialog startMenu(&app);
    QObject::connect(&startMenu, SIGNAL(start()), &w, SLOT(start()));
    QObject::connect(&startMenu, SIGNAL(leave()), &w, SLOT(leave()));
    QObject::connect(&startMenu, SIGNAL(emitLevel(int)), &w, SLOT(setLevel(int)));
    QObject::connect(&startMenu, SIGNAL(setGeneratedLevel()), &w, SLOT(setGeneratedLevel()));
    QObject::connect(&w, SIGNAL(backToStart()), &startMenu, SLOT(open()));


    startMenu.show();

    return app.exec();
}
