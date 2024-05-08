#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>


QT_BEGIN_NAMESPACE
namespace Ui { class UserDialog; }
QT_END_NAMESPACE


class UserDialog : public QDialog
{
    Q_OBJECT
    public:
        UserDialog(QWidget *parent = nullptr);
        ~UserDialog();
    private slots:
        void createUser();
    private:
        Ui::UserDialog *ui;
};