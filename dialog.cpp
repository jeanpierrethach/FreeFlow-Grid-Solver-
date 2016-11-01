#include "dialog.h"
#include "ui_dialog.h"
#include <QPixmap>
#include <QIcon>

Dialog::Dialog(QApplication *a, QWidget *parent) :
    app(a),
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Free Flow");
    setFixedSize(400,300);

    connect(ui->leave, SIGNAL(clicked()), this, SLOT(leaveSlot()));
    connect(ui->start, SIGNAL(clicked()), this, SLOT(startSlot()));

    QPixmap title(":/Image/freeflow.png");
    ui->title->setPixmap(title);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::startSlot()
{
    emit start();
    close();
}

void Dialog::leaveSlot()
{
    qApp->quit();
}
