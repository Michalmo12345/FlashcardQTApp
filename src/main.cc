#include "qt_ui/mainwindow.h"
#include "qt_ui/loginwindow.h"
#include "db_connection/db_sets.h"
#include "flashcardmanagement/Set.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginWindow loginWindow;
    MainWindow mainWindow;

    QObject::connect(&loginWindow, &LoginWindow::loginSuccess, [&](){
        mainWindow.show();
        loginWindow.close();
    });
    loginWindow.show();
    
    return app.exec();
}