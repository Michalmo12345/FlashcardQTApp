/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QMenuBar *menubar;
    QMenu *menuPlik;
    QMenu *menuEdytuj;
    QMenu *menuWidok;
    QMenu *menuNarz_dzia;
    QMenu *menuPomoc;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(230, 210, 161, 81));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(280, 330, 104, 70));
        textEdit->setReadOnly(true);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        menuPlik = new QMenu(menubar);
        menuPlik->setObjectName(QString::fromUtf8("menuPlik"));
        menuEdytuj = new QMenu(menubar);
        menuEdytuj->setObjectName(QString::fromUtf8("menuEdytuj"));
        menuWidok = new QMenu(menubar);
        menuWidok->setObjectName(QString::fromUtf8("menuWidok"));
        menuNarz_dzia = new QMenu(menubar);
        menuNarz_dzia->setObjectName(QString::fromUtf8("menuNarz_dzia"));
        menuPomoc = new QMenu(menubar);
        menuPomoc->setObjectName(QString::fromUtf8("menuPomoc"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menuPlik->menuAction());
        menubar->addAction(menuEdytuj->menuAction());
        menubar->addAction(menuWidok->menuAction());
        menubar->addAction(menuNarz_dzia->menuAction());
        menubar->addAction(menuPomoc->menuAction());
        toolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "PushButton", nullptr));
        menuPlik->setTitle(QCoreApplication::translate("MainWindow", "Plik", nullptr));
        menuEdytuj->setTitle(QCoreApplication::translate("MainWindow", "Edytuj", nullptr));
        menuWidok->setTitle(QCoreApplication::translate("MainWindow", "Widok", nullptr));
        menuNarz_dzia->setTitle(QCoreApplication::translate("MainWindow", "Narz\304\231dzia", nullptr));
        menuPomoc->setTitle(QCoreApplication::translate("MainWindow", "Pomoc", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
