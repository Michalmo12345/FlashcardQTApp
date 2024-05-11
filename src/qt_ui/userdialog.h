#pragma once
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class UserDialog;
}
QT_END_NAMESPACE

class UserDialog : public QDialog {
  Q_OBJECT
 public:
  UserDialog(QWidget *parent = nullptr);
  ~UserDialog();
 signals:
  void userCreated(const QString &username);
 private slots:
  void createUser();

 private:
  Ui::UserDialog *ui;
};