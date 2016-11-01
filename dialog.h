#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QApplication *a, QWidget *parent = 0);
    ~Dialog();

public slots:
    void startSlot();
    void leaveSlot();

signals:
    void start();
    void leave();

private:
    Ui::Dialog *ui;
    QApplication *app;
};

#endif // DIALOG_H
