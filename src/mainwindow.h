#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "gridbuilder.h"
#include "gamestorer.h"
#include <unistd.h>

#include <QMessageBox>
#include <QMainWindow>
#include <QPainter>
#include <vector>
#include "grid.h"
#include "generator.h"
#include <QLabel>
#include <QColor>

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

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

    void paintEvent(QPaintEvent*);

    void clearPathColorCase(Cell*);

private slots:
    void back();
    void restart();
    void saveGame();
    void loadGame();
    void solve();

public slots:
    void setLevel(int);
    void leave();
    void start();
    void setGeneratedLevel();

signals:
    void backToLevelMenu();
    void closeWindow();

private:
    Ui::MainWindow* ui;

    void solveRec(Cell*);

    void pressClearPathCase(int, int);
    void pressClearPathOrigin(int, int);

    void moveActions(int, int);
    void moveToBlankCase(int, int);
    void moveToConnectSecondOrigin(int, int);
    void moveToOverridePath(int, int);
    void moveToClearLastCase(int, int);
    void moveBackwards(int, int);


    void setPositionStart();
    void setPositionCase();
    void resizeGrid();

    void drawGrid();
    void drawRectangleGrid();
    void drawRowColumnLines();
    void drawOrigins();
    void drawColorLine();
    void fillSquareColor();
    void mouseRoundColor();
    void gameIsWon();

    int intervalWidth; // width length of one case in the grid
    int intervalHeight; // height length of one case in the grid

    const int leftside = 5;
    const int rightside = 5;
    const int up = 5;
    //const int bottom = 5;
    const int offset = 5; // separator between the frame and the grid

    int height = 600;
    int width = 720 + leftside + rightside;
    const int bottomSpace = height/5;

    bool pathConnected;

    QPainter* painter;
    std::vector<QColor> color;
    std::vector<QColor> backgroundColor;

    int currentX;
    int currentY;
    QPoint mousePosition; // track mouse position

    std::vector<std::vector<QPoint> > pos;

    Grid* grid;
    QLabel* label;

    GameStorer* storer;
    Generator gen;

};

#endif // MAINWINDOW_H
