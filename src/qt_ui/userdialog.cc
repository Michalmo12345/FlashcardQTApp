#include "userdialog.h"
#include "ui_userdialog.h"
#include "./ui_userdialog.h"



UserDialog::UserDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    connect(ui->createButton, &QDialogButtonBox::accepted, this, &UserDialog::createUser);
}

void UserDialog::createUser()
{
}
 
UserDialog::~UserDialog()
{
    delete ui;
}