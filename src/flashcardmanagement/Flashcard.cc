#include "Flashcard.h"
#include <iostream>

Flashcard::Flashcard(std::string question, std::string answer):
    question(question), answer(answer), n(0), I(1), EF(2.5) {}

void Flashcard::update(int q) {
    //implement supermemo algorithm
}

void Flashcard::showQuestion() {
    std::cout << question << std::endl;
}

void Flashcard::showAnswer() {
    std::cout << answer << std::endl;
}

std::string Flashcard::getQuestion() {
    return question;
}

std::string Flashcard::getAnswer() {
    return answer;
}

