#include "../src/flashcardmanagement/Flashcard.h"
#include <gtest/gtest.h>

TEST(FlashcardTest, BasicAssertions) {
    Flashcard flashcard("What is the capital of France?", "Paris");
    EXPECT_EQ(flashcard.getQuestion(), "What is the capital of France?");
    EXPECT_EQ(flashcard.getAnswer(), "Paris");
}