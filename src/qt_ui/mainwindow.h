#pragma once
#include <QAction>
#include <QLabel>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QTableWidget>
#include <list>
#include <memory>

#include "enumpage.h"
#include "flashcardmanagement/Set.h"
#include "users/User.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  void setUser(const std::string& username);

 signals:
  void switchUserSuccess();
 private slots:
  void navigateToPage(Page page);
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
  void playVideo(const std::string& videoPath);
  void showPhoto(const std::string& photoPath);
  void playAudio(const std::string& audioPath);
  void showQuestionFile();
  void showAnswerFile();
  void showItemInfo();
  void showItemInfoAllSets();
  void seeAllSets();
  void subscribeSet();
  void learnFromAllSets();
  void beginSuperMemoLearning(const QString& setName);
  void onTableItemClicked(int row, int column);
  void goToSMSets();
  void deleteSelectedSet();

 private:
  void updateStatsWidget();
  void goToNextSuperMemoFlashcard();
  void updateFileShowButtons();
  QString formatTime(std::chrono::seconds time);
  void updateUserStats();
  Ui::MainWindow* ui;
  std::unique_ptr<Set> set_;
  std::vector<std::shared_ptr<Flashcard>> currentSessionFlashcards_;
  std::shared_ptr<Flashcard> currentCard_;
  std::string questionFilePath_;
  std::string answerFilePath_;
  QPushButton* lastClickedButton_ = nullptr;
  QProcess* ffmpegProcess = nullptr;
  // std::string currentUserName_;
  std::unique_ptr<User> user;
  bool isSuperMemoLearning_ = false;
  bool isFlashcardValidated_ = false;
  int currentSuperMemoIndex_ = 5;
};
