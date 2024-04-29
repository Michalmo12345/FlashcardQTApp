#include "Flashcard.h"
#include <iostream>

Flashcard::Flashcard(std::string question, std::string answer):
    question(question), answer(answer), n(0), interval(0), EFactor(2.5) {}

void Flashcard::update() {
    
}

std::string Flashcard::getQuestion() {
    return question;
}

std::string Flashcard::getAnswer() {
    return answer;
}

