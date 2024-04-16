#include "../src/flashcardmanagement/Set.h"
#include <gtest/gtest.h>

TEST(SetTest, BasicAssertions) {
    Set set("TestSet");
    EXPECT_EQ(set.getName(), "TestSet");
    set.setName("NewName");
    EXPECT_EQ(set.getName(), "NewName");
}

TEST(SetTest, AddCard) {
    Set set("TestSet");
    auto card = std::make_shared<Flashcard>("What is the capital of France?", "Paris");
    set.addCard(card);
    EXPECT_EQ(set.getCard(0), card);
}
