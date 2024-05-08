#include "userdialog.h"
#include "ui_userdialog.h"
#include "./ui_userdialog.h"
#include <iostream>


UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    connect(ui->createButton, &QDialogButtonBox::accepted, this, &UserDialog::createUser);
}

void UserDialog::createUser()
{
    QString username = ui->usernameInput->text();
    std::cout<<username.toStdString()<<std::endl;
    this->close();
}

UserDialog::~UserDialog()
{
    delete ui;
}