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
  flashcard.update(5);
  EXPECT_EQ(flashcard.getRepetitions(), 1);
  EXPECT_EQ(flashcard.getEFactor(), 2.6);
  EXPECT_EQ(flashcard.getInterval(), 1);
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