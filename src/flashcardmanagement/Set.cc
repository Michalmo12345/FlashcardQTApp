#include "Set.h"
#include <iostream>
#include <string>

Set::Set(std::string name): name_(name) {}

Set::~Set() {
    for (auto card : flashcards_) {
        delete card;
    }
}

std::string Set::getName() {
    return name_;
}

void Set::addCard(Flashcard* card) {
    flashcards_.push_back(card);
}

void Set::giveRandomCard() {
    if (flashcards_.empty()) {
        return nullptr;
    }

    srand(time(nullptr));
    return flashcards_[rand() % flashcards_.size()];
}
