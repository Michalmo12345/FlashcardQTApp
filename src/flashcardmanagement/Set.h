#include <iostream>
#include <vector>
#include "Flashcard.h"

class Set {
    private:
        std::vector<Flashcard*> flashcards_;
    public:
        ~Set();
        void addCard(Flashcard* card);
        Flashcard* giveRandomCard();
        
};