#pragma once
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
class Flashcard {
 private:
  std::string question;
  std::string answer;
  std::string questionFile;
  std::string answerFile;
  unsigned int repetitions;  // number of repetitions
  unsigned int interval;     // interval between repetitions
  double EFactor;            // easiness factor
  std::chrono::system_clock::time_point lastReview;

 public:
  Flashcard(std::string question, std::string answer);
  Flashcard(std::string question, std::string answer, std::string questionFile,
            std::string answerFile);
  double calculateEFactor(double EFactor, int quality);
  unsigned int calculateInterval(unsigned int repetitions, double EFactor);
  void update(unsigned int quality);
  double getEFactor() const;
  unsigned int getRepetitions() const;
  unsigned int getInterval() const;
  std::string getQuestion() const;
  std::string getAnswer() const;
  void setQuestionFile(const std::string& fileName);
  std::string getQuestionFile() const;
  void setAnswerFile(const std::string& fileName);
  std::string getAnswerFile() const;
  std::chrono::system_clock::time_point getLastReview() const;
  void setLastReview(std::chrono::system_clock::time_point time);
};