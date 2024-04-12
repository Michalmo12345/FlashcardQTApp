#include "qt_ui/mainwindow.h"
#include "db_connection/connect_db.cpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // int d = base();
    return a.exec();
}