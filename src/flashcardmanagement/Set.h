#include <iostream>
#include <vector>
#include <string>
#include "Flashcard.h"

class Set {
    private:
        std::vector<Flashcard*> flashcards_;
        std::string name_;
    public:
        Set(std::string name);
        ~Set();
        std::string getName();
        void addCard(Flashcard* card);
        Flashcard* giveRandomCard();
        
};