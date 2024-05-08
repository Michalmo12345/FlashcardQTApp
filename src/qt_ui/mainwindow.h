#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QListWidgetItem>
#include <list>
#include "flashcardmanagement/Set.h"
#include <QPushButton>
#include <QAction>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
    signals:
        void switchUserSuccess();
    private slots:
        void returnToMainPage();
        void findSets();
        void pushContinue();
        void readSetFromDB();
        void readSetFromFile();
        void beginLearning();
        void addFlashcard();
        void goToNextFlashcard();
        void showAnswer();
        void saveToDB();
        void saveToFile();
        void updateFlashcard(unsigned int quality);
        void goToStats();
        void toggleFullScreen();
        void showInfo();
        void swichUser();
    private:
        Ui::MainWindow *ui;
        Set set_;
        std::shared_ptr<Flashcard> currentCard_;
        QPushButton* lastClickedButton_ = nullptr;

};

