#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QPointer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;

    QWidget window;
    window.setFixedSize(400,500);
    QPushButton *quitButton = new QPushButton("Quit", &window);
    quitButton->setGeometry(10,10,80,30);
    QObject::connect(quitButton, SIGNAL (clicked()),QApplication::instance(), SLOT (quit()));


    QPushButton *playButton = new QPushButton("Jouer", &window);
    playButton->setGeometry(100,10,80,30);

    // to do remove objects of window (playButton, quitButton)
    QObject::connect(playButton, SIGNAL (clicked()),QApplication::instance(), SLOT (playButton->deleteLater();));

    window.show();

    return a.exec();
}
