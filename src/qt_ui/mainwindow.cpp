#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}