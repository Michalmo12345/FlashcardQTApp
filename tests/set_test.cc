#include "../src/flashcardmanagement/Set.h"
#include <iostream>
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

TEST(SetTest, ToString){

    std::tm tm = {};
    tm.tm_year = 2024 - 1900;
    tm.tm_mon = 4;
    tm.tm_mday = 30;
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;

    std::chrono::system_clock::time_point time_point = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    EXPECT_EQ(toString(time_point), "2024-05-30 13:00:00");
}

