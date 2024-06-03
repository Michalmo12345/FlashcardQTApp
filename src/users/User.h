#pragma once
#include <QDate>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
class User {
 private:
  std::string username_;
  std::chrono::time_point<std::chrono::steady_clock> sessionStartTime_;
  std::chrono::seconds totalLearningTimeToday_ = std::chrono::seconds(0);
  int flashcardsReviewedToday_ = 0;
  QDate currentLearningDate_;
  bool currentlyLearning_ = false;

 public:
  User(const std::string &username);
  ~User();
  std::string getUsername();
  void setUsername(std::string username);
  void saveToDb();
  void updateInDb(const std::string &oldUsername);
  void deleteFromDb();
  void startLearningSession();
  void endLearningSession();
  void incrementFlashcardsReviewed();

  int getFlashcardsReviewedToday() const;
  std::chrono::seconds getTotalLearningTimeToday() const;
};

bool checkUsernameInDb(const std::string &username);
int getUserId(const std::string &username);
