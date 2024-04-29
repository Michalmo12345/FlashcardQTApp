#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QStackedWidget>
#include <QMessageBox>
#include "flashcardmanagement/Set.h"
#include "db_connection/db_sets.cc"
#include <memory>

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    set_("Zestaw"),
    currentCard_(nullptr)
{
    ui->setupUi(this);
    ui->baseStack->setCurrentIndex(0);
    // connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::buttonClicked);
    connect(ui->findSetsButton, SIGNAL(clicked()), this, SLOT(findSets()));
    connect(ui->pushContinueButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
    connect(ui->dbCardButton, SIGNAL(clicked()), this, SLOT(readSetFromDB()));
    connect(ui->fileCardButton, SIGNAL(clicked()), this, SLOT(readSetFromFile()));
    connect(ui->pushBeginLearning, SIGNAL(clicked()), this, SLOT(beginLearning()));
    connect(ui->pushAddFlashcard, SIGNAL(clicked()), this, SLOT(addFlashcard()));
    connect(ui->nextFlashcardButton, SIGNAL(clicked()), this, SLOT(goToNextFlashcard()));
    connect(ui->showAnswerButton, SIGNAL(clicked()), this, SLOT(showAnswer()));
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
    connect(ui->saveToDbButton, SIGNAL(clicked()), this, SLOT(saveToDB()));
    connect(ui->saveToFileButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(ui->repeatButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(0)));
    connect(ui->hardButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(1)));
    connect(ui->problematicButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(2)));
    connect(ui->mediumButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(3)));
    connect(ui->easyButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(4)));
    connect(ui->perfectButton, SIGNAL(clicked()), this, SLOT(updateFlashcard(5)));
}

void MainWindow::findSets() {
    ui->baseStack->setCurrentIndex(1);
    auto db_names = getSetNamesFromDb();
    for (auto name : db_names) {
        ui->dbSetsList->addItem(QString::fromStdString(name));
    }
    auto file_names = getSetNamesFromFiles();
    for (auto name : file_names) {
        ui->fileSetsList->addItem(QString::fromStdString(name));
    }
}

void MainWindow::pushContinue() {
    ui->baseStack->setCurrentIndex(2);
}

void MainWindow::readSetFromDB() {
    QListWidgetItem *selectedItem = ui->dbSetsList->currentItem();
    if (selectedItem) {
        QString selectedText = selectedItem->text();
        set_ = getSetByName(selectedText.toStdString());
        ui->baseStack->setCurrentIndex(3);
        currentCard_ = set_.giveRandomCard();
        ui->questionBrowser->setText(QString::fromStdString(currentCard_->getQuestion()));
    }
}

void MainWindow::readSetFromFile() {
    QListWidgetItem *selectedItem = ui->fileSetsList->currentItem();
    if (selectedItem) {
        QString selectedText = selectedItem->text();
        set_ = readFromFile(selectedText.toStdString() + ".txt", selectedText.toStdString());
        ui->baseStack->setCurrentIndex(3);
        currentCard_ = set_.giveRandomCard();
        ui->questionBrowser->setText(QString::fromStdString(currentCard_->getQuestion()));
    }
}

void MainWindow::beginLearning() {
    ui->baseStack->setCurrentIndex(3);
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

void MainWindow::saveToDB() {
    set_.setName(ui->setNameTextEdit->toPlainText().toStdString());
    set_.saveToDB();
    QMessageBox::information(this, "Zapisano", "Zestaw został zapisany do bazy danych.");
}
void MainWindow::saveToFile() {
    set_.setName(ui->setNameTextEdit->toPlainText().toStdString());
    set_.saveToFile();
    QMessageBox::information(this, "Zapisano", "Zestaw został zapisany do pliku.");
}

void MainWindow::updateFlashcard(unsigned int quality) {
    currentCard_->calculateEFactor(currentCard_->getEFactor(), quality);
}

MainWindow::~MainWindow()
{
    delete ui;
}