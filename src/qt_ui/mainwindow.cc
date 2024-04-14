#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QStackedWidget>
#include "flashcardmanagement/Set.h"
#include <memory>

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    set_("Zestaw1"),
    currentCard_(nullptr)
{
    ui->setupUi(this);
    ui->baseStack->setCurrentIndex(0);
    // connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->pushContinueButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
    connect(ui->pushBeginLearning, SIGNAL(clicked()), this, SLOT(beginLearning()));
    connect(ui->pushAddFlashcard, SIGNAL(clicked()), this, SLOT(addFlashcard()));
    connect(ui->nextFlashcardButton, SIGNAL(clicked()), this, SLOT(goToNextFlashcard()));
    connect(ui->showAnswerButton, SIGNAL(clicked()), this, SLOT(showAnswer()));
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
}

void MainWindow::pushContinue() {
    ui->baseStack->setCurrentIndex(1);
}

void MainWindow::beginLearning() {
    ui->baseStack->setCurrentIndex(2);
    // auto card = set_.giveRandomCard();
    currentCard_ = set_.giveRandomCard();
    ui->questionBrowser->setText(QString::fromStdString(currentCard_->getQuestion()));
}

void MainWindow::goToNextFlashcard() {
    ui->answerBrowser->clear();
    // auto card = set_.giveRandomCard();
    currentCard_ = set_.giveRandomCard();
    ui->questionBrowser->setText(QString::fromStdString(currentCard_->getQuestion()));
    
}

void MainWindow::showAnswer() {
    if(currentCard_ == nullptr) {
        return;
    }
    ui->answerBrowser->setText(QString::fromStdString(currentCard_->getAnswer()));
}

void MainWindow::addFlashcard() {
    std::string question = ui->questionTextEdit->toPlainText().toStdString();
    std::string answer = ui->answerTextEdit->toPlainText().toStdString();
    std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(question, answer);
    set_.addCard(card);
    ui->questionTextEdit->clear();
    ui->answerTextEdit->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}