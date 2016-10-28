#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>


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
    //bool eventFilter(QObject*, QEvent*);

private:
    Ui::MainWindow *ui;

    void drawGrid();
    void drawColorLine();

    QPoint pos[5][5];
    int mouseX;
    int mouseY;


};

#endif // MAINWINDOW_H
