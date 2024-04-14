#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Flashcard.h"
#include <memory>
class Set {
    private:
        std::vector<std::shared_ptr<Flashcard>> flashcards_;
        std::string name_;
    public:
        Set(std::string name);
        ~Set();
        std::string getName() const;
        void addCard(std::shared_ptr<Flashcard> card);
        std::shared_ptr<Flashcard> giveRandomCard();
        void saveToFile(const std::string& filename) const;
};

Set readFromFile(const std::string& filename, const std::string& setName);