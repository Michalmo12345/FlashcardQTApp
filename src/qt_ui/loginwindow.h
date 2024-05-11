#pragma once
#include <QDialog>
#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow {
  Q_OBJECT
 public:
  LoginWindow(QWidget *parent = nullptr);
  ~LoginWindow();

 signals:
  void loginSuccess();

 private slots:
  void login();
  void createNewUser();
  void onUserCreated(const QString &username);
  void onUserNameChanged(const QString &username);
  void changeUserName();
  void deleteUser();
  void quitApp();

 private:
  Ui::LoginWindow *ui;
  std::string selectedUser_;
  // QDialog* userDialog_;
};