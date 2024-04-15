#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QListWidgetItem>
#include <list>
#include "flashcardmanagement/Set.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void findSets();
        void pushContinue();
        void readSetFromDB();
        void beginLearning();
        void addFlashcard();
        void goToNextFlashcard();
        void showAnswer();
        void saveToDB();
    private:
        Ui::MainWindow *ui;
        Set set_;
        std::shared_ptr<Flashcard> currentCard_;
};

