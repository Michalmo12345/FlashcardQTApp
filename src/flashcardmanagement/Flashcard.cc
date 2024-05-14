#include "Flashcard.h"
#include <iostream>

Flashcard::Flashcard(std::string question, std::string answer):
    question(question), answer(answer), questionFile(""), answerFile(""), repetitions(0), interval(0), EFactor(2.5) {}

Flashcard::Flashcard(std::string question, std::string answer, std::string questionFile, std::string answerFile):
    question(question), answer(answer), questionFile(questionFile), answerFile(answerFile), repetitions(0), interval(0), EFactor(2.5) {}


double Flashcard::calculateEFactor(double EFactor, int quality) {
    return std::max(1.3, EFactor + 0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
}

unsigned int Flashcard::calculateInterval(unsigned int repetitions, double EFactor) {
    if (repetitions == 0) {
        return 1;
    }
    if (repetitions == 1) {
        return 6;
    }
    return (unsigned int)std::round(interval * EFactor);
}

void Flashcard::update(unsigned int quality) {
    EFactor = calculateEFactor(EFactor,quality);
    interval = calculateInterval(repetitions, EFactor);
    repetitions++;
}


double Flashcard::getEFactor() {
    return EFactor;
}

unsigned int Flashcard::getRepetitions() {
    return repetitions;
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

std::string Flashcard::getQuestionFile() {
    return questionFile;
}
        
std::string Flashcard::getAnswerFile() {
    return answerFile;
}

