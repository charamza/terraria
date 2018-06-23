#-------------------------------------------------
#
# Project created by QtCreator 2017-01-31T14:37:41
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Terraria
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/view.cpp \
    src/game.cpp \
    src/block.cpp \
    src/camera.cpp \
    src/world.cpp \
    src/player.cpp \
    src/entity.cpp \
    src/aabb.cpp \
    src/inventory.cpp \
    src/rabbit.cpp \
    src/worldgenerator.cpp \
    src/bee.cpp \
    src/shummi.cpp

HEADERS  += src/mainwindow.h \
    src/view.h \
    src/game.h \
    src/block.h \
    src/camera.h \
    src/world.h \
    src/player.h \
    src/entity.h \
    src/aabb.h \
    src/inventory.h \
    src/rabbit.h \
    src/worldgenerator.h \
    src/bee.h \
    src/shummi.h

FORMS    += src/mainwindow.ui

RESOURCES += \
    res.qrc
