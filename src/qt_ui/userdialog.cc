#include "userdialog.h"

#include <iostream>

#include "users/User.h"
#include "./ui_userdialog.h"
#include "ui_userdialog.h"
#include <QMessageBox>

UserDialog::UserDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::UserDialog) {
  ui->setupUi(this);
  connect(ui->createButton, &QDialogButtonBox::accepted, this,
          &UserDialog::createUser);
}

void UserDialog::createUser() {
  QString username = ui->usernameInput->text();

  if (!username.trimmed().isEmpty()) {
    if (checkUsernameInDb(username.toStdString())) {
      emit userCreated(username);
      this->close();
    } else {
      QMessageBox::warning(this, tr("Nazwa użytkownika zajęta"),
                          tr("Wpisz nazwę użytkownika, której jeszcze nie ma"));
      ui->usernameInput->clear();
      ui->usernameInput->setFocus();
    }
  }
}

UserDialog::~UserDialog() { delete ui; }