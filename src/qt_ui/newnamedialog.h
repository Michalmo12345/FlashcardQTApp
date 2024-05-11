
#pragma once
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class NewNameDialog;
}
QT_END_NAMESPACE

class NewNameDialog : public QDialog {
  Q_OBJECT
 public:
  NewNameDialog(QWidget *parent = nullptr);
  ~NewNameDialog();
 signals:
  void userNameChanged(const QString &username);
 private slots:
  void changeUserName();

 private:
  Ui::NewNameDialog *ui;
};