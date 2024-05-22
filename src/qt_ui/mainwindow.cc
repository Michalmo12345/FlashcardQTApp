#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include "flashcardmanagement/Set.h"
#include "db_connection/db_sets.cc"
#include <memory>
#include <QFile>
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
    set_("Zestaw"),
    currentCard_(nullptr)
{
    ui->setupUi(this);
    ui->baseStack->setCurrentIndex(0);
    ffmpegProcess = new QProcess(this);

    connect(ui->findSetsButton, SIGNAL(clicked()), this, SLOT(findSets()));
    connect(ui->pushContinueButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
    connect(ui->dbCardButton, SIGNAL(clicked()), this, SLOT(readSetFromDB()));
    connect(ui->fileCardButton, SIGNAL(clicked()), this, SLOT(readSetFromFile()));
    connect(ui->pushBeginLearning, SIGNAL(clicked()), this, SLOT(beginLearning()));
    connect(ui->pushAddFlashcard, SIGNAL(clicked()), this, SLOT(addFlashcard()));
    connect(ui->nextFlashcardButton, SIGNAL(clicked()), this, SLOT(goToNextFlashcard()));
    connect(ui->showAnswerButton, &QPushButton::clicked, this, [this]() {
        showAnswer();
        
    });
    connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
    connect(ui->returnButton_2, SIGNAL(clicked()), this, SLOT(returnToMainPage()));
    connect(ui->saveToDbButton, SIGNAL(clicked()), this, SLOT(saveToDB()));
    connect(ui->saveToFileButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
    connect(ui->repeatButton, &QPushButton::clicked, this, [this]() {
        ui->repeatButton->setStyleSheet("QPushButton { background-color: black; }");
        updateFlashcard(0);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->repeatButton;
        });
    connect(ui->hardButton, &QPushButton::clicked, this, [this]() {
        ui->hardButton->setStyleSheet("QPushButton { background-color: red; }");
        updateFlashcard(1);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->hardButton;
        });
    connect(ui->problematicButton, &QPushButton::clicked, this, [this]() {
        ui->problematicButton->setStyleSheet("QPushButton { background-color: orange; }");
        updateFlashcard(2);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->problematicButton;
        });
    connect(ui->mediumButton, &QPushButton::clicked, this, [this]() {
        ui->mediumButton->setStyleSheet("QPushButton { background-color: yellow; }");
        updateFlashcard(3);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->mediumButton;
        });
    connect(ui->easyButton, &QPushButton::clicked, this, [this]() {
        ui->easyButton->setStyleSheet("QPushButton { background-color: green; }");
        updateFlashcard(4);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->easyButton;
        });
    connect(ui->perfectButton, &QPushButton::clicked, this, [this]() {
        ui->perfectButton->setStyleSheet("QPushButton { background-color: blue; }");
        updateFlashcard(5);
        if (lastClickedButton_ != nullptr){
        lastClickedButton_->setStyleSheet("");
        }
        lastClickedButton_ = ui->perfectButton;
        });


    connect(ui->actionProfile, &QAction::triggered, this, &MainWindow::goToStats);
    connect(ui->actionFullScreen, &QAction::triggered, this, &MainWindow::toggleFullScreen);
    connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::showInfo);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionSwitchUser, &QAction::triggered, this, &MainWindow::swichUser);
    connect(ui->questionFileButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik", "", "Wszystkie pliki (*.*);;Obrazy (*.png *.jpg *.bmp);;Filmy (*.mp4 *.avi *.mkv);;Audio (*.mp3 *.wav *.ogg)");
        if (!filePath.isEmpty()) {
            ui->questionFilePath->setText(filePath);
        }
        });
    connect(ui->answerFileButton, &QPushButton::clicked, this, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Wybierz plik", "", "Wszystkie pliki (*.*);;Obrazy (*.png *.jpg *.bmp);;Filmy (*.mp4 *.avi *.mkv);;Audio (*.mp3 *.wav *.ogg)");
        if (!filePath.isEmpty()) {
            ui->answerFilePath->setText(filePath);
        }
        });
    connect(ui->questionFileShowButton, SIGNAL(clicked()), this, SLOT(showQuestionFile()));
    connect(ui->answerFileShowButton, SIGNAL(clicked()), this, SLOT(showAnswerFile()));
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


void MainWindow::returnToMainPage() {
    ui->baseStack->setCurrentIndex(0);
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
        if (currentCard_->getQuestionFile() == "") {
            ui->questionFileShowButton->setVisible(false);
        }
        if (currentCard_->getAnswerFile() == "") {
            ui->answerFileShowButton->setVisible(false);
        }
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
    if (lastClickedButton_ != nullptr) {
        lastClickedButton_->setStyleSheet("");
        lastClickedButton_ = nullptr;
    }
    // auto card = set_.giveRandomCard();
    currentCard_ = set_.giveRandomCard();
    if (currentCard_->getQuestionFile() == "") {
        ui->questionFileShowButton->setVisible(false);
    }
    else {
        ui->questionFileShowButton->setVisible(true);
    }
    if (currentCard_->getAnswerFile() == "") {
        ui->answerFileShowButton->setVisible(false);
    }
    else {
        ui->answerFileShowButton->setVisible(true);
    }
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
    std::string questionFile = ui->questionFilePath->text().toStdString();
    std::string answerFile = ui->answerFilePath->text().toStdString();
    std::shared_ptr<Flashcard> card = std::make_shared<Flashcard>(question, answer, questionFile, answerFile);
    set_.addCard(card);
    ui->questionTextEdit->clear();
    ui->answerTextEdit->clear();
    ui->questionFilePath->clear();
    ui->answerFilePath->clear();
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
    currentCard_->update(quality);
}


void MainWindow::goToStats() {
    ui->baseStack->setCurrentIndex(4);
}

void MainWindow::toggleFullScreen() {
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}

void MainWindow::showInfo() {
    QString filename = "Requirements.md";
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Readme", "Cannot open file: " + filename);
        return;
    }

    QTextStream in(&file);
    QString contents = in.readAll();
    file.close();


    QMessageBox::information(this, "README", contents);
}


void MainWindow::swichUser() {
    emit switchUserSuccess();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showQuestionFile() {
    if (currentCard_->getQuestionFile() != "" && getFileType(currentCard_->getQuestionFile()) == "video") {
        playVideo("flashcardFiles/" + set_.getName() + "/" + currentCard_->getQuestionFile());
    }
}

void MainWindow::showAnswerFile() {
    if (currentCard_->getAnswerFile() != "" && getFileType(currentCard_->getAnswerFile()) == "video") {
        playVideo("flashcardFiles/" + set_.getName() + "/" + currentCard_->getAnswerFile());
    }
}

void MainWindow::playVideo(const std::string& videoPath)
{
    QString program = "ffmpeg";
    QStringList arguments = {"-re", "-i", QString::fromStdString(videoPath), "-f", "sdl", "Video Output", "-f", "alsa", "default"};
    ffmpegProcess->start(program, arguments);
    if (!ffmpegProcess->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start ffmpeg process.");
    }
}
