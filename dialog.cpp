#include "dialog.h"
#include "ui_dialog.h"
#include <QPixmap>
#include <QIcon>

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

    QPixmap title(":/Image/freeflow.png");
    ui->title->setPixmap(title);
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
