#include "Flashcard.h"
#include <iostream>

Flashcard::Flashcard(std::string question, std::string answer):
    question(question), answer(answer), n(0), interval(0), EFactor(2.5) {}


double Flashcard::calculateEFactor(double EFactor, int quality) {
    double newEFactor = std::max(1.3, EFactor + 0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
    return newEFactor;
}


void Flashcard::update() {
    
}


double Flashcard::getEFactor() {
    return EFactor;
}

unsigned int Flashcard::getN() {
    return n;
}
unsigned int Flashcard::getInterval() {
    return interval;
}

std::string Flashcard::getQuestion() {
    return question;
}

std::string Flashcard::getAnswer() {
    return answer;
}

