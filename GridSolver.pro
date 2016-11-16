#-------------------------------------------------
#
# Project created by QtCreator 2016-10-21T15:15:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridSolver
TEMPLATE = app

INCLUDEPATH += $$PWD/src \

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/grid.cpp \
    src/path.cpp \
    src/dialog.cpp \
    src/levelmenu.cpp \
    src/gridbuilder.cpp \
    src/generator.cpp \
    src/gamestorer.cpp


HEADERS  += src/mainwindow.h \
    src/grid.h \
    src/path.h \
    src/dialog.h \
    src/levelmenu.h \
    src/gridbuilder.h \
    src/generator.h \
    src/gamestorer.h

FORMS    += src/mainwindow.ui \
    src/dialog.ui \
    src/levelmenu.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    assets/freeflow.png \
    levels/*.json
