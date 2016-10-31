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
    
private:
    Ui::MainWindow *ui;

    void drawGrid();
    void drawColorLine();


    QPoint pos[5][5];
    QPoint mousePosition;

    int activeX;
    int activeY;


    QPoint startPos;
    QPoint endPos;

    bool mousePressed = false;
    int interval;
    const int height = 720;
    const int width = 720;
    const int leftside = 5;
    const int rightside = 5;
    const int up = 5;
    QPainter* painter;
    int caseX;
    int caseY;



    //int offset = 5; // separator between case

    Grid* grid = new Grid(5,5);

};

#endif // MAINWINDOW_H
