#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T15:15:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridSolver
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp \
    path.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    grid.h \
    path.h \
    dialog.h

FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    assets/freeflow.png \
    levels/level1.json
