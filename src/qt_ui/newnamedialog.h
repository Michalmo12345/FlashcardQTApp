
#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>


QT_BEGIN_NAMESPACE
namespace Ui { class NewNameDialog; }
QT_END_NAMESPACE


class NewNameDialog : public QDialog
{
    Q_OBJECT
    public:
        NewNameDialog(QWidget *parent = nullptr);
        ~NewNameDialog();
    private slots:
        void changeUserName();
    private:
        Ui::NewNameDialog *ui;
};