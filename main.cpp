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
    QObject::connect(&startMenu, SIGNAL(loadLevel()), &w, SLOT(loadGame()));
    QObject::connect(&w, SIGNAL(backToStart()), &startMenu, SLOT(openWindow()));
    QObject::connect(&w, SIGNAL(closeWindow()), &startMenu, SLOT(closeWindow()));

    startMenu.show();

    return app.exec();
}
