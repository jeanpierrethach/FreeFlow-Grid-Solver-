#include "levelmenu.h"
#include "ui_levelmenu.h"
#include <QDesktopWidget>

LevelMenu::LevelMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelMenu),
    map(this)
{
    ui->setupUi(this);
    setWindowTitle("Free Flow");
    this->setFixedSize(300, 400);
    QPalette pal;

    for (int i = 0; i < 6; ++i)
    {
        QString name = "Level " + QString::number(i+1);
        QPushButton* temp = new QPushButton(name, this);
        temp->setFlat(true);
        temp->setVisible(false);
        connect(temp, SIGNAL(clicked()), &map, SLOT(map()));
        map.setMapping(temp, i + 1);
        levelButton.push_back(temp);
        temp = 0;
    }

    connect(&map, SIGNAL(mapped(int)), this, SLOT(slotLevel(int)));
    connect(ui->generate, SIGNAL(clicked()), this, SLOT(startGeneratedLevel()));

    for (int i = 0; i < 6; ++i)
    {
        pal.setColor(QPalette::ButtonText, QColor(237, 28, 36));
        levelButton[i]->setPalette(pal);
        ui->chooseLevelPlatform->addWidget(levelButton[i], i/2, i % 2);
        levelButton[i]->setVisible(true);
    }

}

LevelMenu::~LevelMenu()
{
    delete ui;
    levelButton.clear();
}

void LevelMenu::slotLevel(int level)
{
    close();
    emit emitLevel(level);
}

void LevelMenu::start()
{
    exec();
}

void LevelMenu::startMode()
{
    start();
}

void LevelMenu::startGeneratedLevel()
{
    close();
    emit setGeneratedLevel();
}
