#ifndef LEVELMENU_H
#define LEVELMENU_H

#include <QDialog>
#include <QString>
#include <QVector>
#include <QPushButton>
#include <QSignalMapper>

namespace Ui {
class LevelMenu;
}

class LevelMenu : public QDialog
{
    Q_OBJECT

public:
    explicit LevelMenu(QWidget *parent = 0);
    ~LevelMenu();

public slots:
    void slotLevel(int);
    void start();
    void startMode();

private slots:
    void startGeneratedLevel();
    void slotLoadLevel();

signals:
    void emitLevel(int);
    void setGeneratedLevel();
    void loadLevel();


private:
    Ui::LevelMenu *ui;

    QVector<QPushButton*> levelButton;
    QSignalMapper map;
};

#endif // LEVELMENU_H
