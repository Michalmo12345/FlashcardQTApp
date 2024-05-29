#include "Flashcard.h"

#include <iostream>

Flashcard::Flashcard(std::string question, std::string answer)
    : question(question),
      answer(answer),
      questionFile(""),
      answerFile(""),
      repetitions(0),
      interval(0),
      EFactor(2.5),
      lastReview(std::chrono::system_clock::now()),
      userFlashcardId(0) {}

Flashcard::Flashcard(std::string question, std::string answer,
                     std::string questionFile, std::string answerFile)
    : question(question),
      answer(answer),
      questionFile(questionFile),
      answerFile(answerFile),
      repetitions(0),
      interval(0),
      EFactor(2.5),
      lastReview(std::chrono::system_clock::now()),
      userFlashcardId(0) {}

Flashcard::Flashcard(std::string question, std::string answer,
                     std::string questionFile, std::string answerFile,
                     double EFactor, unsigned int interval,
                     unsigned int repetitions,
                     std::chrono::system_clock::time_point lastReview,
                     std::chrono::system_clock::time_point next_review_date,
                     int userFlashcardId)
    : question(question),
      answer(answer),
      questionFile(questionFile),
      answerFile(answerFile),
      repetitions(repetitions),
      interval(interval),
      EFactor(EFactor),
      lastReview(lastReview),
      next_review_date(next_review_date),
      userFlashcardId(userFlashcardId) {}

double Flashcard::calculateEFactor(double EFactor, int quality) {
  return std::max(
      1.3, EFactor + 0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
}

unsigned int Flashcard::calculateInterval(unsigned int repetitions,
                                          double EFactor) {
  if (repetitions == 0) {
    return 1;
  }
  if (repetitions == 1) {
    return 6;
  }
  return (unsigned int)std::round(interval * EFactor);
}

void Flashcard::update(unsigned int quality) {
  EFactor = calculateEFactor(EFactor, quality);
  interval = calculateInterval(repetitions, EFactor);
  repetitions++;
  lastReview = std::chrono::system_clock::now();
  next_review_date = lastReview + std::chrono::hours(24 * interval);
}

double Flashcard::getEFactor() const { return EFactor; }

unsigned int Flashcard::getRepetitions() const { return repetitions; }
unsigned int Flashcard::getInterval() const { return interval; }

std::string Flashcard::getQuestion() const { return question; }

std::string Flashcard::getAnswer() const { return answer; }

std::string Flashcard::getQuestionFile() const { return questionFile; }

std::string Flashcard::getAnswerFile() const { return answerFile; }

void Flashcard::setUserFlashcardId(int id) { userFlashcardId = id; }

int Flashcard::getUserFlashcardId() const { return userFlashcardId; }

void Flashcard::setQuestionFile(const std::string& fileName) {
  this->questionFile = fileName;
}

void Flashcard::setAnswerFile(const std::string& fileName) {
  this->answerFile = fileName;
}

std::chrono::system_clock::time_point Flashcard::getLastReview() const {
  return lastReview;
}

void Flashcard::setLastReview(std::chrono::system_clock::time_point time) {
  lastReview = time;
}

std::chrono::system_clock::time_point Flashcard::getNextReviewDate() const {
  return next_review_date;
}

bool Flashcard::isNew() const { return repetitions == 0; }

bool Flashcard::isLearned() const {
  return repetitions > 0 && std::chrono::system_clock::now() < next_review_date;
}

bool Flashcard::isPending() const {
  return repetitions > 0 &&
         std::chrono::system_clock::now() >= next_review_date;
}
