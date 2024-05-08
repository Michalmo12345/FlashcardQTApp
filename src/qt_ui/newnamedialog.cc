#include "newnamedialog.h"
#include "ui_newnamedialog.h"
#include "./ui_newnamedialog.h"
#include <iostream>


NewNameDialog::NewNameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewNameDialog)
{
    ui->setupUi(this);
    connect(ui->confirmBox, &QDialogButtonBox::accepted, this, &NewNameDialog::changeUserName);
}

void NewNameDialog::changeUserName()
{
    QString username = ui->newUsername->text();
    std::cout<<username.toStdString()<<std::endl;
    this->close();
    
    
}

NewNameDialog::~NewNameDialog()
{
    delete ui;
}