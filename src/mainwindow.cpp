#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "game.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QRect resolution = QApplication::desktop()->screenGeometry();
    scene.setSceneRect(0, 0, resolution.width(), resolution.height());
    View *view = new View(&scene);

    showFullScreen();
    QTimer::singleShot(1000, this, SLOT(showFullScreen()));

    this->setCentralWidget(view);
}

MainWindow::~MainWindow()
{
    delete ui;
}
