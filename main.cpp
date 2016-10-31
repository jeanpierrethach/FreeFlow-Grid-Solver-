#include "mainwindow.h"
#include <QApplication>

#include "showlevel.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    Dialog startMenu(&app);


    QObject::connect(&startMenu, SIGNAL(start()), &w, SLOT(start()));
    QObject::connect(&startMenu, SIGNAL(leave()), &w, SLOT(leave()));

    startMenu.show();

    //w.show();

    return app.exec();

    /*QWidget window;
    window.setFixedSize(780,780);

    ShowLevel *sl = new ShowLevel();

    //MainWindow *mw = new MainWindow();


    sl->addBoutons(12, 6, window);



    QPushButton *quitButton = new QPushButton("Quit", &window);
    quitButton->setGeometry(10,10,80,30);
    QObject::connect(quitButton, SIGNAL (clicked()),QApplication::instance(), SLOT (quit()));


    QPushButton *playButton = new QPushButton("Jouer", &window);
    playButton->setGeometry(100,10,80,30);

    // to do remove objects of window (playButton, quitButton)
    QObject::connect(playButton, SIGNAL (clicked()),playButton, SLOT (close()));
    QObject::connect(playButton, SIGNAL (clicked()),quitButton, SLOT (close()));
    window.show();
    */

}
