#include "Set.h"
#include <iostream>

Set::~Set() {
    for (auto card : flashcards_) {
        delete card;
    }
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
