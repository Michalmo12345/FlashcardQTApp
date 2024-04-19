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


TEST(SetTest, GiveRandomCard) {
    Set set("TestSet");
    auto card1 = std::make_shared<Flashcard>("What is the capital of France?", "Paris");
    auto card2 = std::make_shared<Flashcard>("What is the capital of Germany?", "Berlin");
    set.addCard(card1);
    set.addCard(card2);
    auto randomCard = set.giveRandomCard();
    EXPECT_TRUE(randomCard == card1 || randomCard == card2);
}

TEST(SetTest, GetCard){
    Set set("TestSet");
    auto card1 = std::make_shared<Flashcard>("What is the capital of France?", "Paris");
    auto card2 = std::make_shared<Flashcard>("What is the capital of Germany?", "Berlin");
    set.addCard(card1);
    set.addCard(card2);
    EXPECT_EQ(set.getCard(0), card1);
    EXPECT_EQ(set.getCard(1), card2);
    EXPECT_EQ(set.getCard(2), nullptr);
}

