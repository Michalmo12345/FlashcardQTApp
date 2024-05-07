#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QListWidgetItem>
#include <QDialog>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT
    public:
        LoginWindow(QWidget *parent = nullptr);
        ~LoginWindow();

    signals:
        void loginSuccess();

    private slots:
        void login();
        void createNewUser();
        void changeUserName();
        void deleteUser();
        void quitApp();
    private:
        Ui::LoginWindow *ui;
};