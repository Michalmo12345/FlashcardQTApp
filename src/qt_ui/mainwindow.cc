#include "mainwindow.h"

#include <QButtonGroup>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

#include "./ui_mainwindow.h"
#include "db_connection/db_sets.cc"
#include "flashcardmanagement/Set.h"
#include "flashcardmanagement/Users_sets.cc"
#include "ui_mainwindow.h"
#include "users/User.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      set_(std::make_unique<Set>("Zestaw")),
      currentCard_(nullptr) {
  ui->setupUi(this);
  ui->baseStack->setCurrentIndex(0);
  ffmpegProcess = new QProcess(this);

  connect(ui->findSetsButton, SIGNAL(clicked()), this, SLOT(findSets()));
  connect(ui->pushContinueButton, SIGNAL(clicked()), this,
          SLOT(pushContinue()));
  connect(ui->usersFavButton, SIGNAL(clicked()), this, SLOT(goToSMSets()));
  connect(ui->dbCardButton, SIGNAL(clicked()), this, SLOT(readSetFromDB()));
  connect(ui->fileCardButton, SIGNAL(clicked()), this, SLOT(readSetFromFile()));
  connect(ui->pushBeginLearning, SIGNAL(clicked()), this,
          SLOT(beginLearning()));
  connect(ui->pushAddFlashcard, SIGNAL(clicked()), this, SLOT(addFlashcard()));
  connect(ui->nextFlashcardButton, SIGNAL(clicked()), this,
          SLOT(goToNextFlashcard()));
  connect(ui->showAnswerButton, &QPushButton::clicked, this,
          [this]() { showAnswer(); });
  connect(ui->returnButton, SIGNAL(clicked()), this, SLOT(pushContinue()));
  connect(ui->returnButton_2, &QPushButton::clicked, this,
          [this]() { navigateToPage(HomePage); });
  connect(ui->saveToDbButton, SIGNAL(clicked()), this, SLOT(saveToDB()));
  connect(ui->saveToFileButton, SIGNAL(clicked()), this, SLOT(saveToFile()));
  connect(ui->repeatButton, &QPushButton::clicked, this, [this]() {
    ui->repeatButton->setStyleSheet("QPushButton { background-color: grey; }");
    currentSuperMemoIndex_ = 0;
    updateFlashcard(0);
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->repeatButton;
  });
  connect(ui->hardButton, &QPushButton::clicked, this, [this]() {
    ui->hardButton->setStyleSheet("QPushButton { background-color: red; }");
    updateFlashcard(1);
    currentSuperMemoIndex_ = 1;
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->hardButton;
  });
  connect(ui->problematicButton, &QPushButton::clicked, this, [this]() {
    ui->problematicButton->setStyleSheet(
        "QPushButton { background-color: orange; }");
    currentSuperMemoIndex_ = 2;
    updateFlashcard(2);
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->problematicButton;
  });
  connect(ui->mediumButton, &QPushButton::clicked, this, [this]() {
    ui->mediumButton->setStyleSheet(
        "QPushButton { background-color: yellow; }");
    currentSuperMemoIndex_ = 3;
    updateFlashcard(3);
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->mediumButton;
  });
  connect(ui->easyButton, &QPushButton::clicked, this, [this]() {
    ui->easyButton->setStyleSheet("QPushButton { background-color: green; }");
    currentSuperMemoIndex_ = 4;
    updateFlashcard(4);
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->easyButton;
  });
  connect(ui->perfectButton, &QPushButton::clicked, this, [this]() {
    ui->perfectButton->setStyleSheet("QPushButton { background-color: blue; }");
    currentSuperMemoIndex_ = 5;
    updateFlashcard(5);
    if (lastClickedButton_ != nullptr) {
      lastClickedButton_->setStyleSheet("");
    }
    lastClickedButton_ = ui->perfectButton;
  });

  connect(ui->actionProfile, &QAction::triggered, this, &MainWindow::goToStats);
  connect(ui->actionFullScreen, &QAction::triggered, this,
          &MainWindow::toggleFullScreen);
  connect(ui->actionInfo, &QAction::triggered, this, &MainWindow::showInfo);
  connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);
  connect(ui->actionSwitchUser, &QAction::triggered, this,
          &MainWindow::swichUser);
  connect(ui->actionAllSets, &QAction::triggered, this,
          &MainWindow::seeAllSets);
  connect(ui->questionFileButton, &QPushButton::clicked, this, [this]() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "Wybierz plik", "",
        "Wszystkie pliki (*.*);;Obrazy (*.png *.jpg *.bmp);;Filmy (*.mp4 *.avi "
        "*.mkv);;Audio (*.mp3 *.wav *.ogg)");
    if (!filePath.isEmpty()) {
      ui->questionFilePath->setText(filePath);
    }
  });
  connect(ui->answerFileButton, &QPushButton::clicked, this, [this]() {
    QString filePath = QFileDialog::getOpenFileName(
        this, "Wybierz plik", "",
        "Wszystkie pliki (*.*);;Obrazy (*.png *.jpg *.bmp);;Filmy (*.mp4 *.avi "
        "*.mkv);;Audio (*.mp3 *.wav *.ogg)");
    if (!filePath.isEmpty()) {
      ui->answerFilePath->setText(filePath);
    }
  });
  connect(ui->questionFileShowButton, SIGNAL(clicked()), this,
          SLOT(showQuestionFile()));
  connect(ui->answerFileShowButton, SIGNAL(clicked()), this,
          SLOT(showAnswerFile()));
  connect(ui->dbSetsList, &QListWidget::itemClicked, this,
          &MainWindow::showItemInfo);
  connect(ui->dbSetsList2, &QListWidget::itemClicked, this,
          &MainWindow::showItemInfoAllSets);
  connect(ui->subscribeSetButton, SIGNAL(clicked()), this,
          SLOT(subscribeSet()));
  connect(ui->loadSetButton, SIGNAL(clicked()), this, SLOT(learnFromAllSets()));
  connect(ui->tableWidget, SIGNAL(cellClicked(int, int)), this,
          SLOT(onTableItemClicked(int, int)));
}

void MainWindow::navigateToPage(Page page) {
  ui->baseStack->setCurrentIndex(static_cast<int>(page));
}
void MainWindow::findSets() {
  navigateToPage(ChooseSetPage);
  auto db_names = getSubscribedSetNamesFromDb(getUserId(user->getUsername()));
  ui->dbSetsList->clear();
  for (auto name : db_names) {
    ui->dbSetsList->addItem(QString::fromStdString(name));
  }
  auto file_names = getSetNamesFromFiles();
  ui->fileSetsList->clear();
  for (auto name : file_names) {
    ui->fileSetsList->addItem(QString::fromStdString(name));
  }
}

void MainWindow::seeAllSets() {
  navigateToPage(ObserveSetPage);
  auto db_names = getSetNamesFromDb();
  ui->dbSetsList2->clear();
  for (auto name : db_names) {
    ui->dbSetsList2->addItem(QString::fromStdString(name));
  }
}

void MainWindow::goToSMSets() {
  navigateToPage(UserPage);
  updateStatsWidget();
}

void MainWindow::showItemInfoAllSets() {
  QString setName = ui->dbSetsList2->currentItem()->text();
  auto set = getSetInfo(setName.toStdString());
  ui->infoSetText2->setPlainText(QString::fromStdString(
      "Nick autora zestawu: " + set->getCreatorUsername()));
  ui->infoSetText2->insertPlainText(
      QString::fromStdString("Nazwa zestawu: " + set->getName() + "\n"));
  ui->infoSetText2->insertPlainText(QString::fromStdString(
      "Data utworzenia zestawu: " + set->getCreationDate() + "\n"));
}

void MainWindow::showItemInfo() {
  QString setName = ui->dbSetsList->currentItem()->text();
  auto set = getSetInfo(setName.toStdString());
  ui->infoSetText->setPlainText(QString::fromStdString(
      "Nick autora zestawu: " + set->getCreatorUsername()));
  ui->infoSetText->insertPlainText(
      QString::fromStdString("Nazwa zestawu: " + set->getName() + "\n"));
  ui->infoSetText->insertPlainText(QString::fromStdString(
      "Data utworzenia zestawu: " + set->getCreationDate() + "\n"));
}

void MainWindow::subscribeSet() {
  QString set_name = ui->dbSetsList2->currentItem()->text();
  int set_id = getSetId(set_name.toStdString());
  int user_id = getUserId(user->getUsername());
  if (checkIsSetSubscribed(set_id, user_id)) {
    saveUsersSetToDb(set_id, user_id);
  } else {
    QMessageBox::warning(this, tr("Zestaw już zaobserwowany"),
                         tr("Nie możesz już tego zestawu zaobserwować"));
  }
}

void MainWindow::pushContinue() {
  set_ = std::make_unique<Set>("Zestaw");
  ui->setNameTextEdit->clear();
  navigateToPage(CreateSetPage);
}

void MainWindow::readSetFromDB() {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  QListWidgetItem *selectedItem = ui->dbSetsList->currentItem();
  if (selectedItem) {
    QString selectedText = selectedItem->text();
    set_ = getSetByName(selectedText.toStdString());
    ui->baseStack->setCurrentIndex(3);
    currentCard_ = set_->giveRandomCard();
    updateFileShowButtons();
    ui->questionBrowser->setText(
        QString::fromStdString(currentCard_->getQuestion()));
  }
}

void MainWindow::learnFromAllSets() {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  QListWidgetItem *selectedItem = ui->dbSetsList2->currentItem();
  if (selectedItem) {
    QString selectedText = selectedItem->text();
    set_ = getSetByName(selectedText.toStdString());
    navigateToPage(LearningPage);
    currentCard_ = set_->giveRandomCard();
    updateFileShowButtons();
    ui->questionBrowser->setText(
        QString::fromStdString(currentCard_->getQuestion()));
  }
}

void MainWindow::readSetFromFile() {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  QListWidgetItem *selectedItem = ui->fileSetsList->currentItem();
  if (selectedItem) {
    QString selectedText = selectedItem->text();
    set_ = readFromFile(selectedText.toStdString() + ".txt",
                        selectedText.toStdString());
    navigateToPage(LearningPage);
    currentCard_ = set_->giveRandomCard();
    ui->questionBrowser->setText(
        QString::fromStdString(currentCard_->getQuestion()));
  }
}

void MainWindow::beginLearning() {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  navigateToPage(LearningPage);
  isSuperMemoLearning_ = false;
  currentCard_ = set_->giveRandomCard();
  updateFileShowButtons();
  ui->questionBrowser->setText(
      QString::fromStdString(currentCard_->getQuestion()));
}

void MainWindow::beginSuperMemoLearning(const QString &setName) {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  navigateToPage(LearningPage);
  set_ = getUserSetByName(setName.toStdString(), user->getUsername());
  isSuperMemoLearning_ = true;
  std::vector<std::shared_ptr<Flashcard>> pendingFlashcards;
  for (const auto &flashcard : set_->getFlashcards()) {
    if (flashcard->isPending() || flashcard->isNew()) {
      pendingFlashcards.push_back(flashcard);
    }
  }
  if (pendingFlashcards.empty()) {
    QMessageBox::information(this, "Brak Fiszek",
                             "Nie masz obecnie fiszek wymagających nauki");
    navigateToPage(UserPage);
    return;
  }
  currentSessionFlashcards_ = pendingFlashcards;
  currentCard_ = set_->giveRandomCard();
  updateFileShowButtons();
  ui->questionBrowser->setText(
      QString::fromStdString(currentCard_->getQuestion()));
}

void MainWindow::goToNextFlashcard() {
  ui->questionBrowser->clear();
  ui->answerBrowser->clear();
  if (lastClickedButton_ != nullptr) {
    lastClickedButton_->setStyleSheet("");
    lastClickedButton_ = nullptr;
  }
  if (isSuperMemoLearning_) {
    goToNextSuperMemoFlashcard();
    return;
  }
  // auto card = set_.giveRandomCard();
  currentCard_ = set_->giveRandomCard();
  updateFileShowButtons();
  ui->questionBrowser->setText(
      QString::fromStdString(currentCard_->getQuestion()));
}

void MainWindow::showAnswer() {
  if (currentCard_ == nullptr) {
    return;
  }
  ui->answerBrowser->setText(QString::fromStdString(currentCard_->getAnswer()));
}

void MainWindow::addFlashcard() {
  std::string question = ui->questionTextEdit->toPlainText().toStdString();
  std::string answer = ui->answerTextEdit->toPlainText().toStdString();
  std::string questionFile = ui->questionFilePath->text().toStdString();
  std::string answerFile = ui->answerFilePath->text().toStdString();
  std::shared_ptr<Flashcard> card =
      std::make_shared<Flashcard>(question, answer, questionFile, answerFile);
  set_->addCard(card);
  ui->questionTextEdit->clear();
  ui->answerTextEdit->clear();
  ui->questionFilePath->clear();
  ui->answerFilePath->clear();
}

void MainWindow::saveToDB() {
  set_->setName(ui->setNameTextEdit->toPlainText().toStdString());
  set_->saveToDB(user->getUsername());
  QMessageBox::information(this, "Zapisano",
                           "Zestaw został zapisany do bazy danych.");
}

void MainWindow::saveToFile() {
  set_->setName(ui->setNameTextEdit->toPlainText().toStdString());
  set_->saveToFile();
  QMessageBox::information(this, "Zapisano",
                           "Zestaw został zapisany do pliku.");
}

void MainWindow::updateFlashcard(unsigned int quality) {
  currentCard_->update(quality);
}

void MainWindow::goToStats() {
  navigateToPage(UserPage);
  updateStatsWidget();
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

void MainWindow::swichUser() { emit switchUserSuccess(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::showQuestionFile() {
  if (currentCard_->getQuestionFile() != "" &&
      getFileType(currentCard_->getQuestionFile()) == "video") {
    playVideo(currentCard_->getQuestionFile());
  } else if (currentCard_->getQuestionFile() != "" &&
             getFileType(currentCard_->getQuestionFile()) == "audio") {
    playAudio(currentCard_->getQuestionFile());
  } else if (currentCard_->getQuestionFile() != "" &&
             getFileType(currentCard_->getQuestionFile()) == "img") {
    showPhoto(currentCard_->getQuestionFile());
  }
}

void MainWindow::showAnswerFile() {
  if (currentCard_->getAnswerFile() != "" &&
      getFileType(currentCard_->getAnswerFile()) == "video") {
    playVideo(currentCard_->getAnswerFile());
  } else if (currentCard_->getAnswerFile() != "" &&
             getFileType(currentCard_->getAnswerFile()) == "audio") {
    playAudio(currentCard_->getAnswerFile());
  } else if (currentCard_->getAnswerFile() != "" &&
             getFileType(currentCard_->getAnswerFile()) == "img") {
    showPhoto(currentCard_->getAnswerFile());
  }
}

void MainWindow::playVideo(const std::string &videoPath) {
  QString program = "ffmpeg";
  QStringList arguments = {"-re", "-i",   QString::fromStdString(videoPath),
                           "-f",  "sdl",  "Video Output",
                           "-f",  "alsa", "default"};
  ffmpegProcess->start(program, arguments);
  if (!ffmpegProcess->waitForStarted()) {
    QMessageBox::critical(this, "Error", "Failed to start ffmpeg process.");
  }
}

void MainWindow::showPhoto(const std::string &photoPath) {
  QPixmap image(QString::fromStdString(photoPath));

  QDialog *imageDialog = new QDialog(this);
  imageDialog->setWindowTitle("Wyświetlanie obrazu");

  QLabel *imageLabel = new QLabel(imageDialog);
  imageLabel->setPixmap(image);
  imageLabel->setAlignment(Qt::AlignCenter);

  QVBoxLayout *layout = new QVBoxLayout(imageDialog);
  layout->addWidget(imageLabel);

  imageDialog->setLayout(layout);
  imageDialog->exec();
}

void MainWindow::playAudio(const std::string &audioPath) {
  QString program = "ffmpeg";
  QStringList arguments = {"-i", QString::fromStdString(audioPath), "-f",
                           "alsa", "default"};
  ffmpegProcess->start(program, arguments);
  if (!ffmpegProcess->waitForStarted()) {
    QMessageBox::critical(this, "Error", "Failed to start ffmpeg process.");
  }
  QDialog *audioDialog = new QDialog(this);
  audioDialog->setWindowTitle("Audio Control");

  QPushButton *stopButton = new QPushButton("Stop Audio", audioDialog);
  connect(stopButton, &QPushButton::clicked, [this, audioDialog]() {
    if (ffmpegProcess->state() == QProcess::Running) {
      ffmpegProcess->kill();
      ffmpegProcess->waitForFinished();
    }
    audioDialog->close();
  });
  QVBoxLayout *layout = new QVBoxLayout(audioDialog);
  layout->addWidget(stopButton);

  audioDialog->setLayout(layout);
  audioDialog->exec();
}

void MainWindow::setUser(const std::string &username) {
  user = std::make_unique<User>(username);
  ui->usernameLabel->setText(QString::fromStdString(username));
}

void MainWindow::updateStatsWidget() {
  auto db_names = getSubscribedSetNamesFromDb(getUserId(user->getUsername()));
  ui->tableWidget->clearContents();
  ui->tableWidget->setRowCount(db_names.size());
  for (std::vector<std::string>::size_type i = 0; i < db_names.size(); ++i) {
    QTableWidgetItem *nameItem =
        new QTableWidgetItem(QString::fromStdString(db_names[i]));
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(i, 0, nameItem);
    set_ = getUserSetByName(db_names[i], user->getUsername());
    int newCount = 0, learningCount = 0, pendingCount = 0;
    for (const auto &flashcard : set_->getFlashcards()) {
      if (flashcard->isNew()) {
        ++newCount;
      } else if (flashcard->isLearned()) {
        ++learningCount;
      } else if (flashcard->isPending()) {
        ++pendingCount;
      }
    }
    QTableWidgetItem *newCountItem =
        new QTableWidgetItem(QString::number(newCount));
    newCountItem->setFlags(newCountItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(i, 1, newCountItem);

    QTableWidgetItem *learningCountItem =
        new QTableWidgetItem(QString::number(learningCount));
    learningCountItem->setFlags(learningCountItem->flags() &
                                ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(i, 2, learningCountItem);

    QTableWidgetItem *pendingCountItem =
        new QTableWidgetItem(QString::number(pendingCount));
    pendingCountItem->setFlags(pendingCountItem->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->setItem(i, 3, pendingCountItem);
  }
}

void MainWindow::onTableItemClicked(int row, int column) {
  if (column == 0) {
    QString setName = ui->tableWidget->item(row, column)->text();
    beginSuperMemoLearning(setName);
  }
}

void MainWindow::goToNextSuperMemoFlashcard() {
  if (currentSessionFlashcards_.empty()) {
    QMessageBox::information(this, "Zakończono sesję nauki",
                             "Wszystkie fiszki zostały powtórzone.");

    set_->updateAllUserFlashcardInDB();
    goToSMSets();
    return;
  }
  currentCard_ = currentSessionFlashcards_.back();
  if (currentSuperMemoIndex_ >= 4) {
    currentSessionFlashcards_.pop_back();
  } else {
    currentSessionFlashcards_.insert(currentSessionFlashcards_.begin(),
                                     currentCard_);
  }
  updateFileShowButtons();
  ui->questionBrowser->setText(
      QString::fromStdString(currentCard_->getQuestion()));
}

void MainWindow::updateFileShowButtons() {
  if (currentCard_->getQuestionFile() == "") {
    ui->questionFileShowButton->setVisible(false);
  } else {
    ui->questionFileShowButton->setVisible(true);
  }
  if (currentCard_->getAnswerFile() == "") {
    ui->answerFileShowButton->setVisible(false);
  } else {
    ui->answerFileShowButton->setVisible(true);
  }
}
