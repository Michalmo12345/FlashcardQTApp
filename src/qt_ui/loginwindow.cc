#include "loginwindow.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QStackedWidget>
#include <QWidget>
#include <memory>

#include "./ui_loginwindow.h"
#include "./ui_mainwindow.h"
#include "db_connection/db_users.cc"
#include "newnamedialog.h"
#include "ui_loginwindow.h"
#include "userdialog.h"
#include "users/User.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow) {
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
  connect(ui->createNewButton, &QPushButton::clicked, this,
          &LoginWindow::createNewUser);
  connect(ui->changeNameButton, &QPushButton::clicked, this,
          &LoginWindow::changeUserName);
  connect(ui->deleteButton, &QPushButton::clicked, this,
          &LoginWindow::deleteUser);
  connect(ui->exitButton, &QPushButton::clicked, this, &LoginWindow::quitApp);
}

void LoginWindow::login() {
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

void LoginWindow::createNewUser() {
  UserDialog dialog(this);
  connect(&dialog, &UserDialog::userCreated, this, &LoginWindow::onUserCreated);
  dialog.exec();
}

void LoginWindow::changeUserName() {
  NewNameDialog dialog(this);
  connect(&dialog, &NewNameDialog::userNameChanged, this,
          &LoginWindow::onUserNameChanged);
  dialog.exec();
}

void LoginWindow::onUserCreated(const QString &username) {
  qDebug() << "New user created with username:" << username;
  User newUser(username.toStdString());
  newUser.saveToDb();
  ui->dbUsersWidget->addItem(username);
}

void LoginWindow::onUserNameChanged(const QString &username) {
  qDebug() << "New username: " << username;
  QListWidgetItem *selectedItem = ui->dbUsersWidget->currentItem();
  if (selectedItem) {
    std::string oldUsername = selectedItem->text().toStdString();
    User user(oldUsername);
    user.updateInDb(username.toStdString());
    selectedItem->setText(username);
  }
}

void LoginWindow::deleteUser() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("Usuń użytkownika");
  msgBox.setText("Czy jesteś pewny, że chcesz usunąć tego użytkownika?");
  msgBox.setIcon(QMessageBox::Warning);
  QPushButton *buttonYes = msgBox.addButton("Tak", QMessageBox::YesRole);
  msgBox.addButton("Nie", QMessageBox::NoRole);

  msgBox.exec();
  if (msgBox.clickedButton() == buttonYes) {
    qDebug() << "User deletion confirmed.";
    QListWidgetItem *selectedItem = ui->dbUsersWidget->currentItem();
    if (selectedItem) {
      std::string username = selectedItem->text().toStdString();
      User user(username);
      user.deleteFromDb();
      delete selectedItem;
    }
    // Add deleteing the user from the database
    // Update any UI components such as clearing a lineEdit, refreshing a list,
    // etc.
  } else {
    qDebug() << "User deletion cancelled.";
  }
}

void LoginWindow::quitApp() { QApplication::quit(); }

LoginWindow::~LoginWindow() { delete ui; }