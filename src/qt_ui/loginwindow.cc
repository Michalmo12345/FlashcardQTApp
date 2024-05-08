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
#include "db_connection/db_users.cc"


LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    auto userNames = getUsersFromDb();
    for (auto name : userNames) {
        ui->dbUsersWidget->addItem(QString::fromStdString(name));
    }
    // QListWidgetItem *selectedItem = ui->dbUsersWidget->currentItem();
    // if (selectedItem) {
    //     this->selectedUser_ = selectedItem->text();
    // }
    connect(ui->chooseButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(ui->createNewButton, &QPushButton::clicked, this, &LoginWindow::createNewUser);
    connect(ui->changeNameButton, &QPushButton::clicked, this, &LoginWindow::changeUserName);
    connect(ui->deleteButton, &QPushButton::clicked, this, &LoginWindow::deleteUser);
    connect(ui->exitButton, &QPushButton::clicked, this, &LoginWindow::quitApp);
}

void LoginWindow::login()
{
    emit loginSuccess();
    this->close();
}

void LoginWindow::createNewUser()
{
}

void LoginWindow::changeUserName()
{
}

void LoginWindow::deleteUser()
{
}

void LoginWindow::quitApp()
{
    QApplication::quit();
}


LoginWindow::~LoginWindow()
{
    delete ui;
}