#include "newnamedialog.h"

#include <QMessageBox>
#include <iostream>

#include "./ui_newnamedialog.h"
#include "ui_newnamedialog.h"
#include "users/User.h"

NewNameDialog::NewNameDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::NewNameDialog) {
  ui->setupUi(this);
  connect(ui->confirmBox, &QDialogButtonBox::accepted, this,
          &NewNameDialog::changeUserName);
}

void NewNameDialog::changeUserName() {
  QString username = ui->newUsername->text();
  if (!username.trimmed().isEmpty()) {
    if (checkUsernameInDb(username.toStdString())) {
      emit userNameChanged(username);
      this->close();
    } else {
      QMessageBox::warning(
          this, tr("Nazwa użytkownika zajęta"),
          tr("Wpisz nazwę użytkownika, której jeszcze nie ma"));
      ui->newUsername->clear();
      ui->newUsername->setFocus();
    }
  }
}

NewNameDialog::~NewNameDialog() { delete ui; }