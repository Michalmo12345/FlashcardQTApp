#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QStackedWidget>
#include <QMessageBox>
#include <memory>
#include <QFile>
#include "ui_loginwindow.h"


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->chooseButton, &QPushButton::clicked, this, &LoginWindow::login);
}

void LoginWindow::login()
{
    emit loginSuccess();
    this->close();
}
LoginWindow::~LoginWindow()
{
    delete ui;
}