#include "dialog.h"
#include "ui_dialog.h"
#include <QPixmap>
#include <QIcon>

Dialog::Dialog(QApplication* a, QWidget* parent) :
    app(a),
    QDialog(parent),
    ui(new Ui::Dialog),
    levelMenu(this)
{
    ui->setupUi(this);
    setWindowTitle("Free Flow");
    setFixedSize(400,300);

    connect(ui->leave, SIGNAL(clicked()), this, SLOT(leaveSlot()));
    connect(ui->start, SIGNAL(clicked()), &levelMenu, SLOT(startMode()));
    connect(&levelMenu, SIGNAL(emitLevel(int)), this, SLOT(slotLevel(int)));
    connect(&levelMenu, SIGNAL(setGeneratedLevel()), this, SLOT(slotGenerateLevel()));


    QPixmap title(":/Image/assets/freeflow.png");
    ui->title->setPixmap(title);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::leaveSlot()
{
    qApp->quit();
}

void Dialog::slotLevel(int level)
{
    close();
    emit emitLevel(level);
}

void Dialog::slotGenerateLevel()
{
    close();
    emit setGeneratedLevel();
}
