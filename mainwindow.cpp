#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{  
    ui->setupUi(this);
    //installEventFilter(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent* e)
{
    //label->setText("X:"+QString::number(e->x())+"-- Y:"+QString::number(e->y()));

}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton){
        qDebug() << "Left Button";
    }
    mouseX = e->x();
    mouseY = e->y();

}

void MainWindow::mouseReleaseEvent(QMouseEvent* e)
{


}

/*
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    statusBar()->showMessage(QString("Mouse move (%1,%2)").arg(mouseEvent->pos().x()).arg(mouseEvent->pos().y()));
  }
  return false;
}*/
