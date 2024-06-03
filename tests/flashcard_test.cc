#include "../src/flashcardmanagement/Flashcard.h"

#include <gtest/gtest.h>

TEST(FlashcardTest, BasicAssertions) {
  Flashcard flashcard("What is the capital of France?", "Paris");
  EXPECT_EQ(flashcard.getQuestion(), "What is the capital of France?");
  EXPECT_EQ(flashcard.getAnswer(), "Paris");
}

TEST(FlashcardTest, Update) {
  Flashcard flashcard("What is the capital of France?", "Paris");
  EXPECT_EQ(flashcard.getRepetitions(), 0);
  EXPECT_EQ(flashcard.getEFactor(), 2.5);
  EXPECT_EQ(flashcard.getInterval(), 0);
  EXPECT_LE(flashcard.getNextReviewDate(), std::chrono::system_clock::now());
  flashcard.update(5);
  EXPECT_EQ(flashcard.getRepetitions(), 1);
  EXPECT_EQ(flashcard.getEFactor(), 2.6);
  EXPECT_EQ(flashcard.getInterval(), 1);
  EXPECT_GE(flashcard.getNextReviewDate(), std::chrono::system_clock::now());
}

TEST(FlashcardTest, Filenames) {
  Flashcard flashcard("What is the capital of France?", "Paris", "question.txt",
                      "answer.txt");
  EXPECT_EQ(flashcard.getQuestionFile(), "question.txt");
  EXPECT_EQ(flashcard.getAnswerFile(), "answer.txt");
  flashcard.setQuestionFile("new_question.txt");
  flashcard.setAnswerFile("new_answer.txt");
  EXPECT_EQ(flashcard.getQuestionFile(), "new_question.txt");
  EXPECT_EQ(flashcard.getAnswerFile(), "new_answer.txt");
}

TEST(FlashcardTest, FlashcardStatus) {
  Flashcard flashcard("What is the capital of France?", "Paris", "question.txt",
                      "answer.txt");
  EXPECT_EQ(flashcard.isNew(), true);
  EXPECT_EQ(flashcard.isLearned(), false);
  EXPECT_EQ(flashcard.isPending(), false);

  flashcard.update(5);

  EXPECT_EQ(flashcard.getRepetitions(), 1);
  EXPECT_EQ(flashcard.isNew(), false);
  EXPECT_EQ(flashcard.isLearned(), true);
  EXPECT_EQ(flashcard.isPending(), false);
}

TEST(FlashcardTest, FullConstructor) {
  Flashcard flashcard("What is the capital of France?", "Paris", "question.txt",
                      "answer.txt", 1.1, 2, 3, std::chrono::system_clock::now(),
                      std::chrono::system_clock::now(), 3);
  EXPECT_EQ(flashcard.getEFactor(), 1.1);
  EXPECT_EQ(flashcard.getRepetitions(), 3);
  EXPECT_EQ(flashcard.getInterval(), 2);
  EXPECT_EQ(flashcard.getQuestion(), "What is the capital of France?");
  EXPECT_EQ(flashcard.getAnswer(), "Paris");
  EXPECT_EQ(flashcard.getQuestionFile(), "question.txt");
  EXPECT_EQ(flashcard.getAnswerFile(), "answer.txt");
  EXPECT_EQ(flashcard.getUserFlashcardId(), 3);
  EXPECT_LE(flashcard.getLastReview(), std::chrono::system_clock::now());
  EXPECT_LE(flashcard.getNextReviewDate(), std::chrono::system_clock::now());
}
