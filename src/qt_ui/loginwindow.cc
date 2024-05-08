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
#include "userdialog.h"
#include "newnamedialog.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->dbUsersWidget->setSelectionMode(QAbstractItemView::SingleSelection);
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
    if (ui->dbUsersWidget->selectedItems().size() == 0) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Błąd logowania");
        msgBox.setText("Nie wybrano użytkownika.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    emit loginSuccess();
    this->close();
}

void LoginWindow::createNewUser()
{
    UserDialog dialog(this);
    dialog.exec();

}


void LoginWindow::changeUserName()
{

    NewNameDialog dialog(this);
    dialog.exec();

}

void LoginWindow::deleteUser()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Usuń użytkownika");
    msgBox.setText("Czy jesteś pewny, że chcesz usunąć tego użytkownika?");
    msgBox.setIcon(QMessageBox::Warning);
    QPushButton *buttonYes = msgBox.addButton("Tak", QMessageBox::YesRole);
    msgBox.addButton("Nie", QMessageBox::NoRole);

    msgBox.exec();
    if (msgBox.clickedButton() == buttonYes) {
        qDebug() << "User deletion confirmed.";
        // Add deleteing the user from the database
        // Update any UI components such as clearing a lineEdit, refreshing a list, etc.
    } else {
        qDebug() << "User deletion cancelled.";
    }

}

void LoginWindow::quitApp()
{
    QApplication::quit();
}


LoginWindow::~LoginWindow()
{

    delete ui;
}