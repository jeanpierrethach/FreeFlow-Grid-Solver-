#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "levelmenu.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QApplication* a, QWidget* parent = 0);
    ~Dialog();

public slots:
    void leaveSlot();
    void slotGenerateLevel();
    void slotLevel(int);

private slots:
    void slotLoadLevel();
    void closeWindow();
    void openWindow();

signals:
    void start();
    void leave();
    void emitLevel(int);
    void setGeneratedLevel();
    void loadLevel();

private:
    Ui::Dialog* ui;
    LevelMenu levelMenu;
    QApplication* app;
};

#endif // DIALOG_H
