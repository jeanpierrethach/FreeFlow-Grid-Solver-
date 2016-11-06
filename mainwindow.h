#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"

#include <QMessageBox>
#include <QMainWindow>
#include <QPainter>
#include <vector>
#include "grid.h"
#include <QLabel>


#define NOTACTIVE -1


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setLevel();

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

    void clearPathColorCase(Path*);


public slots:
    void start();
    void leave();

private:
    Ui::MainWindow* ui;

    void pressClearPathCase(int, int);
    void pressClearPathOrigin(int, int);

    void moveActions(int, int);
    void moveToBlankCase(int, int);
    void moveToConnectSecondOrigin(int, int);
    void moveToOverridePath(int, int);
    void moveToClearLastCase(int, int);
    void moveBackwards(int, int);


    void drawGrid();
    void setPositionCase();
    void drawRectangleGrid();
    void drawRowColumnLines();
    void drawOrigins();
    void drawColorLine();
    void fillSquareColor();
    void mouseRoundColor();
    void gameIsWon();

    int intervalWidth; // width length of one case in the grid
    int intervalHeight; // height length of one case in the grid
    const int height = 720;
    const int width = 720;
    const int leftside = 5;
    const int rightside = 5;
    const int up = 5;
    //const int bottom = 5;
    const int offset = 5; // separator between the frame and the grid

    bool pathConnected;

    QPainter* painter;
    int positionCaseX;
    int positionCaseY;

    int currentX;
    int currentY;
    QPoint mousePosition; // track mouse position

    QPoint pos[5][5]; // static level

    Grid* grid;
    QLabel* label;

};

#endif // MAINWINDOW_H
