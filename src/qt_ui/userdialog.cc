#include "userdialog.h"

#include <iostream>

#include "./ui_userdialog.h"
#include "ui_userdialog.h"

UserDialog::UserDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::UserDialog) {
  ui->setupUi(this);
  connect(ui->createButton, &QDialogButtonBox::accepted, this,
          &UserDialog::createUser);
}

void UserDialog::createUser() {
  QString username = ui->usernameInput->text();

  if (!username.trimmed().isEmpty()) {
    emit userCreated(username);
    this->close();
  }
}

UserDialog::~UserDialog() { delete ui; }