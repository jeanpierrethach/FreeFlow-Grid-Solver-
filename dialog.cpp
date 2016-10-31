#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QApplication *a, QWidget *parent) :
    app(a),
    QDialog(parent),
    ui(new Ui::Dialog)
    //mainWindow(this)
{
    ui->setupUi(this);
    setWindowTitle("Free Flow");

    connect(ui->leave, SIGNAL(clicked()), this, SLOT(leaveReceivers()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startReceivers()));

}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::startReceivers()
{
    emit start();
    close();
}

void Dialog::leaveReceivers()
{
    qApp->quit();
}
