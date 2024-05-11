#include "newnamedialog.h"

#include <iostream>

#include "./ui_newnamedialog.h"
#include "ui_newnamedialog.h"

NewNameDialog::NewNameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewNameDialog) {
  ui->setupUi(this);
  connect(ui->confirmBox, &QDialogButtonBox::accepted, this,
          &NewNameDialog::changeUserName);
}

void NewNameDialog::changeUserName() {
  QString username = ui->newUsername->text();
  if (!username.trimmed().isEmpty()) {
    emit userNameChanged(username);
  }
  this->close();
}

NewNameDialog::~NewNameDialog() { delete ui; }