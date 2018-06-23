#-------------------------------------------------
#
# Project created by QtCreator 2017-01-31T14:37:41
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Terraria
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    view.cpp \
    game.cpp \
    block.cpp \
    camera.cpp \
    world.cpp \
    player.cpp \
    entity.cpp \
    aabb.cpp \
    inventory.cpp \
    rabbit.cpp \
    worldgenerator.cpp \
    bee.cpp \
    shummi.cpp

HEADERS  += mainwindow.h \
    view.h \
    game.h \
    block.h \
    camera.h \
    world.h \
    player.h \
    entity.h \
    aabb.h \
    inventory.h \
    rabbit.h \
    worldgenerator.h \
    bee.h \
    shummi.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
