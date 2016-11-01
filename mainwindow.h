#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>
#include "grid.h"


#define NOTACTIVE -1


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

    void clearPath(Path*);
    void clearPathCase(Path*);


public slots:
    void start();
    void leave();
    
private:
    Ui::MainWindow *ui;

    void drawGrid();
    void drawColorLine();
    void fillSquareColor();
    void mouseRoundColor();

    int interval;
    const int height = 720;
    const int width = 720;
    const int leftside = 5;
    const int rightside = 5;
    const int up = 5;
    //const int bottom = 5;
    QPainter* painter;
    int caseX;
    int caseY;



    int offset = 5; // separator between case

    int activeX;
    int activeY;
    bool mousePressed = false;

    // static level
    QPoint pos[8][8];
    QPoint mousePosition;

    // static level
    Grid* grid = new Grid(8,8);

};

#endif // MAINWINDOW_H
