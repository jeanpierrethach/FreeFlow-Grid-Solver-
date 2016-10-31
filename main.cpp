#include "mainwindow.h"
#include <QApplication>

#include "showlevel.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;

    // can encapsulate into another class
    Dialog startMenu(&app);
    QObject::connect(&startMenu, SIGNAL(start()), &w, SLOT(start()));
    QObject::connect(&startMenu, SIGNAL(leave()), &w, SLOT(leave()));
    startMenu.show();

    return app.exec();
}
